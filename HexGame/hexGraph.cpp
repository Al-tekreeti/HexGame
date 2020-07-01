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
	bool f_start = false, f_end = false;//flags
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
	vector<unsigned> i_start(k, 0), i_end(k, 0);//indices of start and end nodes
	for (size_t i = 0; i < k; i++){
		//double check whether any of the start and end nodes are among the moves 
				if (int_stone == XPLR) {//horizontally
					if (nodes[i] % nNode == 0){
						if(f_start == false)
							f_start = true;//a horizontal start node exist
						i_start[i] = 1;//kth node is a start node
					}
					if ((nodes[i] + 1) % nNode == 0) {
						if(f_end == false)
							f_end = true;//a horizantal end node exist
						i_end[i] = 1;//kth node is an end node
					}
				}
				else {//vertically
					if (nodes[i] < nNode) {
						if(f_start == false)
							f_start = true;//a vertical start node exist
						i_start[i] = 1;//kth node is a start node
					}
					if (nodes[i] > nNode*(nNode - 1)) {
						if(f_end == false)
							f_end = true;//a vertical end node exist
						i_end[i] = 1;//kth node is an end node
					}
				}
	}
	if (f_start == false || f_end == false)
		return false;//the game is for sure not over
	////The following part can be added to the Graph class as another constructor
	////Graph(unsigned k, vector<unsigned> nodes, vector<vector<unsigned>> graph)
	//double** ec;//new W
	//unsigned temp;
	//ec = new double*[k];
	//for (size_t i = 0; i < k; i++){
	//	ec[i] = new double[k];
	//	for (size_t j = 0; j < k; j++){
	//		if (i == j) {
	//			*(ec[i] + j) = 0;
	//			continue;
	//		}
	//		else if (j > i) {
	//			temp = nodes[j];
	//			if (any_of(graph[i].cbegin(), graph[i].cend(), [temp](auto& ele) {return ele == temp; }))
	//				*(ec[i] + j) = 1;
	//			else
	//				*(ec[i] + j) = 0;
	//		}
	//		else
	//			*(ec[i] + j) = *(ec[j] + i);
	//	}
	//}
	////This is the end of the constructor
	//Graph g(k, ec);
	Graph g(k, nodes, graph);
	ShortestPathAlgo spa(g);
	for (size_t i = 0; i < k; i++){
		if (i_start[i] != 1)
			continue;
		for (size_t j = 0; j < k; j++){
			if (i_end[j] != 1)
				continue;
			list<int> nlist = spa.path(i,j);
			if (nlist.size() != 0)
				return true;//game over, one of the players win the game
		}
	}				
	//cout << "g graph" << endl;
	//cout << g << endl;
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