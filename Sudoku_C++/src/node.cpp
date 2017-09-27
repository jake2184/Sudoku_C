#include "node.h"
#include <iostream>

using namespace std;

void Node::print() {
	cout << "UP: " << up << endl;
	cout << "DOWN: " << down << endl;
	cout << "LEFT: " << left << endl;
	cout << "RIGHT: " << right << endl;
}