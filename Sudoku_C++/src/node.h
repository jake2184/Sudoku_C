#pragma once
#include <iostream>


class Node {
	public:
		Node* up;
		Node* down;
		Node* left;
		Node* right;
		void print();
};

class ColumnNode: public Node {
	public:
		int id; 
		ColumnNode* left;
		ColumnNode* right;
};

