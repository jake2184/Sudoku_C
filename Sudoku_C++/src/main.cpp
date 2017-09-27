#include "puzzle.h"
#include "main.h"
#include "node.h"
#include <iostream>

using namespace std;


int main() {


	Puzzle puzzle = Puzzle();


	puzzle.loadFromFile("C:/Users/Jake/source/repos/Sudoku_C++/Sudoku_C++/src/puzzles/4x4");
	
	cout << "\n\n";
	puzzle.printGrid();
	cout << "\n\n";


	if (puzzle.solve()) {
		cout << "Solved! \n" << endl;
	}

	//puzzle.printGrid();
	
	return 0;
}
