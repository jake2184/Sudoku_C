#include "puzzle.h"


#include <iostream>
#include <fstream>
#include <math.h>


Puzzle::Puzzle()
{
	headNode = new ColumnNode();
}

Puzzle::~Puzzle()
{
	ColumnNode* nextNode = headNode->right;
	ColumnNode* currentNode;

	while (nextNode != headNode) {
		// Delete each column
		currentNode = nextNode;
		nextNode = currentNode->right;
		// Go down 
		Node* nextRow = currentNode->down;
		Node* currentRow = currentNode->down;
		while (nextRow != currentNode) {
			nextRow = currentRow->down;
			delete currentRow;
			currentRow = nextRow;
		}
		
		delete currentNode;
	}
	delete headNode;
}


void Puzzle::printGrid() {
	for (int i = 0; i < grid.size(); i++) {
		if (i % BASE_SIZE == 0) {
			cout << "-";
			for (int w = 0; w <= grid.size() + BASE_SIZE - 1; w++) {
				cout << "--";
			}
			cout << endl;
		}
		for (int j = 0; j < grid[i].size(); j++) {
			if (j % BASE_SIZE == 0) {
				cout << "| ";
			}
			cout << grid[i][j] << " ";
		}
		cout << "|" << endl;
	}
	cout << "-";
	for (int w = 0; w <= grid.size() + BASE_SIZE - 1; w++) {
		cout << "--";
	}
	cout << endl;
}

void Puzzle::loadFromFile(string filename) {
	ifstream fileIn;
	string lineIn;

	fileIn.open(filename);

	if (fileIn.is_open()) {

		// Determine block size
		fileIn >> lineIn;
		int size = int(sqrt(lineIn.length()));
		if (size*size == lineIn.length()) {
			BASE_SIZE = size;
		}
		else {
			cerr << "Input text file incorrect dimensions" << endl;
			exit(1);
		}

		// Read in data
		fileIn.seekg(0, ios::beg);
		int lineNumber = 0;
		do {
			vector<int> line;
			fileIn >> lineIn;
			for (int i = 0; i < BASE_SIZE*BASE_SIZE; i++) {
				line.push_back(stoi(string(1, lineIn[i])));
			}
			grid.push_back(line);			
		} while (!fileIn.eof());

		if (!checkGrid()) {
			cerr << "Input file contains errors - cannot solve" << endl;
			exit(1);
		};
	}
	else {
		cerr << "Cannot open input file " + filename << endl;
		exit(1);
	}
}


bool Puzzle::checkCellRow(int row, int index) {
	for (unsigned int i = 0; i < BASE_SIZE*BASE_SIZE; i++) {
		if (i != index) {
			if (grid[row][i] == grid[row][index]) {
				return false;
			}
		}
	}
	return true;
}

bool Puzzle::checkCellColumn(int column, int index) {
	for (unsigned int i = 0; i < BASE_SIZE*BASE_SIZE; i++) {
		if (i != index) {
			if (grid[i][column] == grid[index][column]) {
				return false;
			}
		}
	}
	return true;
}

bool Puzzle::checkCellBlock(int row, int column) {
	int rowBlock = row / BASE_SIZE;
	int columnBlock = column / BASE_SIZE;

	for (unsigned int i = (rowBlock*BASE_SIZE); i < ((rowBlock+1)*BASE_SIZE); i++) {
		for (unsigned int j = (columnBlock*BASE_SIZE); j < ((columnBlock + 1)*BASE_SIZE); j++) {
			if (i != row && j != column) {
				if (grid.at(i).at(j) == grid[row][column]) {
					return false;
				}
			}
		}
	}
	return true;
}

bool Puzzle::checkCell(int row, int column) {
	return checkCellRow(row, column) && checkCellColumn(column, row) && checkCellBlock(row, column);
}

bool Puzzle::checkGrid(){
	for (int i = 0; i < BASE_SIZE*BASE_SIZE; i++) {
		for (int j = 0; j < BASE_SIZE*BASE_SIZE; j++) {
			if (grid[i][j] != 0) {
				if (!checkCell(i, j)) {
					return false;
				}
			}
		}
	}
	return true;
}

Cell nextCell(Cell cell, int size) {
	cell.column++;
	if (cell.column == size*size) {
		cell.row++;
		cell.column = 0;
	}
	return cell;
}

bool Puzzle::recursiveBacktracking(Cell cell) {
	int row = cell.row;
	int column = cell.column;


	// Have reached the end
	if (row == BASE_SIZE*BASE_SIZE) {
		return checkGrid();
	}

	if ( grid[row][column] != 0 ) {
		// Already solved cell
		return recursiveBacktracking(nextCell(cell, BASE_SIZE));
	}
	else {
		for (int i = 1; i <= BASE_SIZE*BASE_SIZE; i++) {
			grid[row][column] = i;
			if (checkCell(row, column)) {
				if (recursiveBacktracking(nextCell(cell, BASE_SIZE))) {
					return true;
				};
			}
		}
		grid[row][column] = 0;
		return false;
	}
}


void Puzzle::buildMatrix() {
	// Create head nodes
	ColumnNode* currentNode = headNode;
	ColumnNode* previousNode = headNode;
	for (int i = 0; i < pow(BASE_SIZE, 6); i++) {
		currentNode->id = i-1;
		currentNode->right = new ColumnNode();
		currentNode->left = previousNode;
		previousNode = currentNode;
		currentNode = currentNode->right;
	}
	currentNode->right = headNode;
	headNode->left = currentNode;

	// Create columns
	currentNode = headNode->right;
	while (currentNode != headNode) {
		// Create BASE_SIZE*BASE_SIZE children
		Node* previousChild = currentNode;
		Node* currentChild = new Node();
		currentNode->down = currentChild;
		for (int i = 0; i < BASE_SIZE*BASE_SIZE; i++) {
			currentChild->up = previousChild;
			currentChild->down = new Node();
			previousChild = currentChild;
			currentChild = currentChild->down;
		}
		currentChild->up = previousChild;
		currentChild->down = currentNode;
		currentNode->up = currentChild;

		currentNode = currentNode->right;
	}

}


bool Puzzle::solve() {
	// Verify grid is established
	if (!checkGrid()) {
		return 1;
	};
	// Alternative solve
	buildMatrix();

	// Solve
	Cell startCell = Cell(0,0);
	return recursiveBacktracking(startCell);


}