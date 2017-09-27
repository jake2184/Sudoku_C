
#include <string>
#include <vector>
#include "node.h"
using namespace std;

struct Cell {
	Cell(int _row, int _column) {
		row = _row;
		column = _column;
	}
	int row;
	int column;
};

class Puzzle {
	public:
		Puzzle();
		~Puzzle();
		void loadFromFile(string filename);
		void printGrid();
		bool solve();

	private:
		int BASE_SIZE = 0;
		vector<vector<int>> grid;
		ColumnNode* headNode;


		bool checkCell(int row, int column);
		bool checkCellRow(int row, int index);
		bool checkCellColumn(int column, int index);
		bool checkCellBlock(int row, int column);
		bool checkGrid();

		void buildMatrix();

		bool recursiveBacktracking(Cell cell);
};