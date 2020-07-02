/*
Implementation of the hex game:
*/
#include"hexGraph.h"
#include"c:\Users\Mustafa\source\repos\Dijkstra7\Dijkstra7\ShortestPathAlgo.h"

#include<iostream>
#include<ctime>
#include<random>
#include <unordered_set>

hexGraph::hexGraph(unsigned n) {//construct the hex board with EMPTY squares
	nNode = n;
	edgelist.resize(nNode*nNode);
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
		}
}

void hexGraph::print(vector<hexState>& hexplace) {
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

bool hexGraph::game_over(char stone, vector<hexState>& hexplace) {
	hexState int_stone;
	vector<vector<unsigned>> graph(nNode*nNode);
	vector<unsigned> nodes(nNode*nNode);
	unsigned k = 0;//number of nodes of the Dijksrta graph
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
			if (nlist.size() != 0)//is there any path from north (west) to south (east)
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
	vector<hexState> hexplace(nNode*nNode,EMPTY);
	char flag, human, machine;
	unsigned move, human_count = 0, machine_count = 0;//Dijkstra algorithm is not invoked untill either count >= nNode
	srand(static_cast<unsigned>(time(0)));//seed the PRNG
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
			print(hexplace);
			if (human_count >= nNode && game_over(human, hexplace)){
				cout << "Congratulations! You win the game" << endl;
				break;
			}
			flag = machine;//machine round now!
		}
		else {//machine round
			while (true) {
				move = rand() % (nNode*nNode - 1);//randomly select a move
				//move = plausibile_move(machine, nNode*nNode - human_count - machine_count, 1000, hexplace);
				if (hexplace[move] == EMPTY) {
					if (machine == 'X')
						hexplace[move] = XPLR;
					else
						hexplace[move] = OPLR;
					machine_count++;
					break;
				}
			}
			print(hexplace);
			if (machine_count >= nNode && game_over(machine, hexplace)) {
				cout << "Unfortunately! The computer win the game" << endl;
				break;
			}
			flag = human;//human round now!
		}
	}
}

unsigned hexGraph::plausibile_move(char stone, unsigned no_empty_squares, unsigned no_sim, vector<hexState>& hexplace) {
	srand(static_cast<unsigned>(time(0)));//seed the PRNG
	unsigned p_move, tmp_move;
	double p_win = 0.0;
	vector<hexState> hplace = hexplace, empty_sqrs;
	vector<int> empty_indices;//a vector of indices
	//find the indices of the empty squares
	for (size_t i = 0; i < nNode*nNode; i++)
		if (hplace[i] == EMPTY) {
			empty_indices.push_back(i);
		}
	//prepare a vector that contains the right numbers of Xs and Os
	if (stone == 'X') {
		empty_sqrs.insert(empty_sqrs.begin(), ceil(1.0*(no_empty_squares-1) / 2), OPLR);
		empty_sqrs.insert(empty_sqrs.end() - 1, (no_empty_squares-1) / 2, XPLR);
	}
	else {
		empty_sqrs.insert(empty_sqrs.begin(), ceil(1.0*(no_empty_squares-1) / 2), XPLR);
		empty_sqrs.insert(empty_sqrs.end(), (no_empty_squares-1) / 2, OPLR);
	}
	//select a plausible move
	for (auto ind:empty_indices){
		unsigned X_count = 0, O_count = 0;
		tmp_move = ind;
		if (stone == 'X') 
			hplace[ind] = XPLR;
		else 
			hplace[ind] = OPLR;
		//Now only use shuffle to simulate random moves, only the empty squares should be shuffled
		for (size_t i = 0; i < no_sim; i++) {
			random_shuffle(empty_sqrs.begin(), empty_sqrs.end());
			//filling the hex board using the shuffled contents
			vector<hexState> t = empty_sqrs;//temporary vector
			for (auto ind : empty_indices) {
				if (ind == tmp_move) continue;
				hplace[ind] = t.back();
				t.pop_back();
			}
			//print(hplace);
			if (game_over('X', hplace))//should pass the board as well
				X_count++;
			else
				O_count++;
		}
		if (stone == 'X') {
			if (1.0*X_count / no_sim >= p_win) {
				p_move = tmp_move;
				p_win = 1.0*X_count / no_sim;
			}
		}
		else {
			if (1.0*O_count / no_sim >= p_win) {
				p_move = tmp_move;
				p_win = 1.0*O_count / no_sim;
			}
		}
	}
	return p_move;
}

hexGraph::~hexGraph(){
}
