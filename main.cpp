#include "Board.h"
#include <vector>
#include <fstream>
#include <chrono>

using namespace std;

int alg1(vector<int> moves, vector<int> weighting) {

	int output = 0;
	int highestWeighting = 0;
	for (int i = 0; i < weighting.size(); i++)
	{
		//choose the highest move in the parallel move array and do the corresponding move
		if (weighting.at(i) > highestWeighting) {
			highestWeighting = weighting.at(i);
			output = i;
		}

	}

	return moves.at(output);
}

int alg2(vector<int> moves) {

	//chose a random move from the list given
	std::srand(rand());
	int random_integer = std::rand() % moves.size();

	return moves.at(random_integer);
}

int main()
{
	//Seeding the random number generator
	std::srand(time(NULL));

	//Creating and reading files
	ifstream inFile = ifstream("input.txt");
	string inText;
	ofstream outFile = ofstream("output.txt");

	bool firstGame = true;

	int size;

	//Runs a game for each "line" in the file
	//redefines the ',' as the enter character
	while (getline(inFile, inText, ',')) {

		//This is to stop there being 2 extra lines above the first games output
		if (firstGame) {
			firstGame = false;
		}
		else {
			outFile << endl << endl;
		}

		//Get the value of size from the string
		size = stoi(inText);
		outFile << "size = " << size << endl;

		//Instantiate new board object
		Board board = Board(size);

		//Create exit conditions
		bool exit1 = false, exit2 = false;

		//while both players can do moves
		while ((!exit1) && (!exit2))
		{
			//Get moves that alg1 can do
			vector<int> moves = board.getMoves(board.BLACK);

			//Get the weightings for the moves alg1 can do
			vector<int> weighting = board.getWeightings(board.BLACK, moves);

			//Vector that will store the tiles changed when the move is performed

			if (moves.size() == 0) {
				//if alg1 cannot do any moves
				exit1 = true;
			}
			else {
				//Get the move that alg1 wants to do
				int blackMove = alg1(moves, weighting);

				//do the move
				vector<int> tilesChanged = board.doMove(blackMove, board.BLACK);

				//add the tiles changed to the output file
				outFile << board.toMoveString(tilesChanged, 1) << endl;
				exit1 = false;
			}

			//Gets the moves for alg 2;
			moves = board.getMoves(board.WHITE);
			if (moves.size() == 0) {
				//if alg2 can't do any moves
				exit2 = true;
			}
			else {
				//Get alg2's move choice
				int whiteMove = alg2(moves);

				//Do alg2's move
				vector<int> tilesChanged = board.doMove(whiteMove, board.WHITE);

				//add tiles changed to output file
				outFile << board.toMoveString(tilesChanged, 2) << endl;
			}

		}

		//Calculate final scores
		int blackScore = board.getScore(board.BLACK);
		int whiteScore = board.getScore(board.WHITE);

		//Print final scorest to output file
		outFile << "alg1 = " << blackScore << endl;
		outFile << "alg2 = " << whiteScore << endl;

		//Check who is winner and print to file
		if (blackScore > whiteScore) {
			outFile << "win = alg1";
		}
		else if (blackScore < whiteScore) {
			outFile << "win = alg2";
		}
		else {
			outFile << "win = draw";
		}


	}

	//closing files
	inFile.close();
	outFile.close();



	return 0;
}
