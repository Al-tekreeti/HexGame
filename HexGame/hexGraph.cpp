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

bool hexGraph::game_over(char stone) {
	hexState int_stone;
	vector<vector<unsigned>> graph(nNode*nNode);
	vector<unsigned> nodes(nNode*nNode);
	unsigned k = 0;
	if (stone == 'X')
		int_stone = XPLR;
	else
		int_stone = OPLR;
	//In this part, we extract the sub-graph that corresponds to either human or machine moves
	for (size_t i = 0; i < nNode; i++)
		for (size_t j = 0; j < nNode; j++) {
			if (hexplace[i*nNode + j] == int_stone) {
				graph[k] = edgelist[i*nNode + j];
				nodes[k] = i * nNode + j;
				k = k + 1;
			}
		}
	graph.resize(k);
	nodes.resize(k);
	graph.shrink_to_fit();
	nodes.shrink_to_fit();
	vector<unsigned> i_start, i_end;//indices of start and end nodes
	for (size_t i = 0; i < k; i++){
		//double check whether any of the start and end nodes are among the moves 
				if (int_stone == XPLR) {//horizontally
					if (nodes[i] % nNode == 0)
						i_start.push_back(nodes[i]);//ith node is a horizantal start node
					if ((nodes[i] + 1) % nNode == 0) 						
						i_end.push_back(nodes[i]);//ith node is a horizantal end node				
				}
				else {//vertically
					if (nodes[i] < nNode) 						
						i_start.push_back(nodes[i]);//ith node is a vertical start node					
					if (nodes[i] > nNode*(nNode - 1)) 					
						i_end.push_back(nodes[i]);//ith node is a vertical end node					
				}
	}
	if (i_start.size() == 0 || i_end.size() == 0)
		return false;//the game is for sure not over
	//evaluate the graph using Dijkstra
	Graph g(k, nodes, graph);
	ShortestPathAlgo spa(g);
	for (auto x:i_start) {
		auto it_s = find(nodes.cbegin(), nodes.cend(), x);
		auto index_s = distance(nodes.cbegin(), it_s);//index of the start node
		for (auto y:i_end){
			auto it_e = find(nodes.cbegin(), nodes.cend(), y);
			auto index_e = distance(nodes.cbegin(), it_e);//index of the end node
			list<int> nlist = spa.path(index_s,index_e);
			if (nlist.size() != 0)
				return true;//game over, one of the players win the game
		}
	}				
	return false;//nobody win the game
}

void hexGraph::hex_game() {
	cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
	cout << "Instructions:" << endl;
	cout << "The hex board numbered from 0 to " << nNode*nNode - 1 << " starting from the upper left corner," << endl;
	cout << "There are two types of stones: X and O; X is horizontally and O is vertically," << endl;
	cout << "Good luck!" << endl;
	cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
	char flag, human, machine;
	unsigned move, human_count = 0, machine_count = 0;
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
					human_count++;
					break;
				}
				else
					cout << "your move is invalid, choose another hexagon:" << endl;
			}
			print();
			if (human_count >= nNode && game_over(human)){
				cout << "Congratulations! You win the game" << endl;
				break;
			}
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
					machine_count++;
					break;
				}
			}
			print();
			if (machine_count >= nNode && game_over(machine)) {
				cout << "Unfortunately! The computer win the game" << endl;
				break;
			}
			flag = human;
		}
	}
}

hexGraph::~hexGraph()
{
}
