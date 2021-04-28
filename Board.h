#pragma once

#include <string>
#include <vector>
using namespace std;
class Board
{
private:
	char board[16][16];
	int boardSize;


	int find(char input, vector<char> temp);
	int find(char input, vector<char> temp, int start);
	int findDirection(vector<char> input);

	int getR(int encoded);
	int getC(int encoded);


	vector<char> getSurroundingSpaces(int column, int row);
	vector<char> getSurroundingSpaces(int encoded);

	char getValue(int column, int row);
	char getValue(int encoded);
	char Invert(char input);
public:
	Board(int size);


	const int POSREF[8] = { -101,-100,-99,1,101,100,99,-1 };
	const char WHITE = 'o';
	const char BLACK = 'x';
	const char EMPTY = ' ';
	const char INVALID = '#';

	vector<int> getEmptySpaces();

	vector<int> getMoves(char player);

	vector<int> isValidMove(int encoded,char player);
	//bool isValidBlackMove(int encoded);

	vector<int> doMove(int encoded,char player);

	int getScore(char player);

	vector<int> getWeightings(char player, vector<int> moves);

	string convertOutputFormat(int input);


	string toString();
	string toStringRaw();
	string toMoveString(vector<int> input, int algor);
	
};

