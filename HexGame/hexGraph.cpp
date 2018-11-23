/*
Implementation of the hex game:

*/
#include"hexGraph.h"
#include"c:\Users\Mustafa\source\repos\Dijkstra7\Dijkstra7\ShortestPathAlgo.h"

#include<iostream>
#include<ctime>
#include<random>

hexGraph::hexGraph(unsigned n) {
	nNode = n;
	edgelist.resize(nNode*nNode);
	hexplace.resize(nNode*nNode);
	for (size_t i = 0; i < nNode; i++)
		for (size_t j = 0; j < nNode; j++) {
			if (i + 1 < nNode)
				edgelist[i*nNode + j].push_back((i + 1)*nNode + j);
			if (j + 1 < nNode)
				edgelist[i*nNode + j].push_back(i*nNode + (j + 1));
			if (i - 1 < i)
				edgelist[i*nNode + j].push_back((i - 1)*nNode + j);
			if (j - 1 < j)
				edgelist[i*nNode + j].push_back(i*nNode + (j - 1));
			if (i + 1 < nNode && j + 1 < nNode)
				edgelist[i*nNode + j].push_back((i + 1)*nNode + (j + 1));
			if (i - 1 < i && j - 1 < j)
				edgelist[i*nNode + j].push_back((i - 1)*nNode + (j - 1));
			hexplace[i*nNode + j] = EMPTY;
		}
}

void hexGraph::print() {
	for (size_t i = 0; i < nNode; i++) {
		for (size_t k = 0; k < nNode - i; k++)
			cout << "  ";
		for (size_t j = 0; j < nNode; j++)
			if (j == 0)
				if (hexplace[i*nNode + j] == XPLR)
					cout << " X ";
				else if (hexplace[i*nNode + j] == OPLR)
					cout << " O ";
				else
					cout << " . ";
			else
				if (hexplace[i*nNode + j] == XPLR)
					cout << "- X ";
				else if (hexplace[i*nNode + j] == OPLR)
					cout << "- O ";
				else
					cout << "- . ";
		cout << endl;
		if (i < nNode - 1) {
			for (size_t k = 0; k < nNode - i; k++)
				cout << "  ";
			for (size_t j = 0; j < nNode; j++)
				if (j == 0)
					cout << "/ ";
				else
					cout << "\\ / ";
			cout << endl;
		}
	}
}

bool hexGraph::game_over(char player) {
	Graph g(50, 1.0);
	return true;
}

void hexGraph::hex_game() {
	cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
	cout << "Instructions:" << endl;
	cout << "The hex board numbered from 0 to " << nNode*nNode - 1 << " starting from the upper left corner," << endl;
	cout << "There are two types of stones: X and O; X is horizontally and O is vertically," << endl;
	cout << "Good luck!" << endl;
	cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
	char flag, human, machine;
	unsigned move;
	srand(static_cast<unsigned>(time(0)));
	cout << "which stone does you want? X or O" << endl;
	cin >> human;
	if (human == 'X')
		machine = 'O';
	else
		machine = 'X';
	cout << "which player does start? X or O" << endl;
	cin >> flag;
	while (true) {
		if (flag == human) {
			cout << "enter your move:" << endl;
			while (true) {
				cin >> move;
				if (hexplace[move] == EMPTY) {
					if (human == 'X')
						hexplace[move] = XPLR;
					else
						hexplace[move] = OPLR;
					break;
				}
				else
					cout << "your move is invalid, choose another hexagon:" << endl;
			}
			print();
			flag = machine;
		}
		else {//machine round
			while (true) {
				move = rand() % (nNode*nNode - 1);
				if (hexplace[move] == EMPTY) {
					if (machine == 'X')
						hexplace[move] = XPLR;
					else
						hexplace[move] = OPLR;
					break;
				}
			}
			print();
			flag = human;
		}
	}
}

hexGraph::~hexGraph()
{
}