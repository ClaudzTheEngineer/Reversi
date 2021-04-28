#include "Board.h"


using namespace std;
Board::Board(int size)
{

	//data correction of board size to limit it to being between 4 and 16 and being even
	size = size / 2 * 2;//makes input even

	//clamping values
	if (size > 16) { size = 16; }
	if (size < 4) { size = 4; }
	boardSize = size;


	//initialise the 2D board array
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			if (i < size&&j<size) {
				board[i][j] = EMPTY;
			}
			else {
				board[i][j] = INVALID;
			}
			
		}
	}

	//Placing the centre tokens
	board[size / 2 - 1][(size / 2)-1] = WHITE;
	board[size / 2][size / 2] = WHITE;

	board[size / 2 - 1][size / 2] = BLACK;
	board[size / 2][(size / 2) - 1] = BLACK;

}

//Returns the row of an encoded coordinate
int Board::getR(int encoded)
{
	return (encoded/100)-1;
}

//Returns the Column of an encoded coordinate
int Board::getC(int encoded)
{
	return (encoded%100)-1;
}

//Returns the character at a defined Coordinate
char Board::getValue(int colomn, int row)
{
	return board[row][colomn];
}

char Board::getValue(int encoded)
{
	return getValue(getC(encoded),getR(encoded));
}

//Returns a vector containing the coordinates of the boards empty spaces
vector<int> Board::getEmptySpaces() {
	vector<int> output;
	for (int row = 0; row < boardSize; row++)
	{
		for (int column = 0; column < boardSize; column++)
		{
			if (board[row][column] == EMPTY) {
				output.push_back((row + 1) * 100 + column + 1);//Adding the encoded coordinate
			}
		}
	}
	return output;
}

int Board::findDirection(vector<char> input)
{
	return 0;
}


//returns vector of size 8 going clockwise from top-left listing the contents of surrounding blocks from column & row
vector<char> Board::getSurroundingSpaces(int column, int row)
{
	vector<char> output;
	output.resize(8);

	for (int i = 0; i < 8; i++)
	{
		output.at(i) = INVALID;
	}

	if (column < boardSize - 1) {
		output.at(3) = board[row][column+1];
		if (row < boardSize - 1) {
			output.at(4) = board[row + 1][column + 1];
		}
	}

	if (row < boardSize - 1) {
		output.at(5) = board[row + 1][column];
		if (column > 0) {
			output.at(6) = board[row + 1][column-1];
		}
	}

	if (column > 0) {
		output.at(7) = board[row][column - 1];
		if (row > 0) {
			output.at(0) = board[row - 1][column - 1];
		}
	}

	if (row > 0) {
		output.at(1) = board[row - 1][column];
		if (column < boardSize - 1) {
			output.at(2) = board[row - 1][column + 1];
		}
	}

	return output;
}

//same as before but using single input
vector<char> Board::getSurroundingSpaces(int encoded)
{
	return getSurroundingSpaces(getC(encoded),getR(encoded));
}

char Board::Invert(char input)
{
	if (input == BLACK)
		return WHITE;
	if (input == WHITE)
		return BLACK;

	return INVALID;
}

vector<int> Board::getMoves(char player)
{
	vector<int> emptySpaces = getEmptySpaces();
	vector<int> output;

	for (int i = 0; i < (int)emptySpaces.size(); i++)
	{
		if (isValidMove(emptySpaces.at(i),player).size() != 0) {
			output.push_back(emptySpaces.at(i));
		}
	}

	return output;
}

vector<int> Board::isValidMove(int encoded,char player)
{
	vector<int> output;

	//Makes sure tile in question is empty
	if (getValue(encoded) != EMPTY) {
		return output;
	}
	//Get the tiles around the point in question
	vector<char> temp = getSurroundingSpaces(encoded);


	//checks if there are any tiles around the point of the opposite color
	int dir = find(Invert(player), temp);

	
	while (dir != -1) {
		//changes the reference point to the found piece of opposite color
		int currPos = encoded+POSREF[dir];
		bool exit = false;

		//temporary vector that stores the positions of tiles that need to be flipped if the move is valid
		vector<int> tempAdd;

		//Checks along a straight line until it reaches the end
		while (!exit) {
			char check = getSurroundingSpaces(currPos).at(dir);
			if (check == Invert(player)) {
				tempAdd.push_back(currPos);
				currPos += POSREF[dir];
			}
			else if (check == player) {
				
				tempAdd.push_back(currPos);
				exit = true;
			}
			else {
				tempAdd.clear();
				exit = true;
			}
		}

		//checks if there are more pieces of the opposit color around it
		dir = find(Invert(player), temp, dir);

		//appends the temporary vector to the output
		output.insert(output.end(), tempAdd.begin(), tempAdd.end());
	}
	
	//if positions have been added to the vector, add the original one at the end
	if (output.size() > 0) {
		output.push_back(encoded);
	}

	return output;
}

//Does the move and all subsequent tile flips for the given position
vector<int> Board::doMove(int encoded,char player)
{
	//if a player tries to do a move that is invalid the posToChange vector will have 0 length and no tiles will be changed
	vector<int> posToChange = isValidMove(encoded,player);

	//steps through the vector and changes all positions marked off to be changed
	for (int i = 0; i < posToChange.size(); i++)
	{
		board[getR(posToChange.at(i))][getC(posToChange.at(i))] = player;
		
	}
	return posToChange;
}

//Returns the score for a given color
int Board::getScore(char player)
{
	int output = 0;
	for (int row = 0; row < boardSize; row++)
	{
		for (int column = 0; column < boardSize; column++)
		{
			if (board[row][column] == player) {
				output++;
			}
		}
	}
	return output;
}

//returns a parallel vector containing the amount of tiles each move will possibly change
vector<int> Board::getWeightings(char player, vector<int> moves)
{
	vector<int> output;
	

	for (int i = 0; i < moves.size(); i++)
	{
		//isValidMove returns a vector thats size is the weighting, this number gets added to the output vector
		output.push_back((int)isValidMove(moves.at(i), player).size());
	}

	return output;
}

string Board::convertOutputFormat(int input)
{
	string output = "r" + to_string(input / 100 - 1);
	output = output + "c" + to_string(input % 100 - 1);
	

	return output;
}

//returns a string containing the board and all tiles
string Board::toString()
{
	string output = "";

	for (int i = 0; i < boardSize; i++)
	{
		output = output + "\033[4m";
		for (int j = 0; j < boardSize; j++)
		{
			output = output + board[i][j] + "|";
		}
		output += "\n\033[0m";
	}

	return output;
}

//returns a string containing the board, tiles and invalid spaces
string Board::toStringRaw()
{
	string output = "";


	for (int i = 0; i < 16; i++)
	{
		output = output + "\033[4m";
		for (int j = 0; j < 16; j++)
		{
			output = output + board[i][j] + "|";
			//output += "] ";
		}
		output += "\n\033[0m";
	}

	return output;
}

//returns a string formated for the output of the program
string Board::toMoveString(vector<int> input, int algor)
{
	string output= "";
	if (input.size() == 0) {
		return "";
	}
	output.append(convertOutputFormat(input.at(input.size() - 1)));
	output.append(" alg"); 
	output+=to_string(algor);
	output += ',';

	for (int i = 0; i < input.size()-1; i++)
	{
		output += " " + convertOutputFormat(input.at(i));
	}

	return output;
}

//Finds the first matching character in a vector
int Board::find(char input,vector<char> temp) {
	for (int i = 0; i < temp.size(); i++)
	{
		if (temp.at(i) == input) {return i;}
	}
	return -1;
}
int Board::find(char input, vector<char> temp,int start) {
	for (int i = start+1; i < temp.size(); i++)
	{
		if (temp.at(i) == input) { return i; }
	}
	return -1;
}