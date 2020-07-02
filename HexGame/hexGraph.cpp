/*
Implementation of the hex game:

*/
#include"hexGraph.h"

#include<iostream>
#include<ctime>
#include<random>
#include<unordered_set>

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

bool hexGraph::is_connected(vector<unsigned> s, vector<unsigned> e, char stone, vector<hexState>& hexplace) {
	//implement Breadth First Search algorithm
	unordered_set<unsigned> done_set, doing_set;
	unsigned node;
	hexState int_stone;
	if (stone == 'X')
		int_stone = XPLR;
	else
		int_stone = OPLR;
	doing_set.insert(s.begin(),s.end());
	while (doing_set.empty() != true) {
		node = *doing_set.begin();
		doing_set.erase(node);
		for (auto v : edgelist[node])
			if (hexplace[v] == int_stone && done_set.find(v) == done_set.end())
				if (find(e.begin(),e.end(),v) != e.end())
					return true;
				else
					doing_set.insert(v);
		done_set.insert(node);
	}
	return false;
}

bool hexGraph::game_over(char stone, vector<hexState>& hexplace) {
	hexState int_stone;
	if (stone == 'X')
		int_stone = XPLR;
	else
		int_stone = OPLR;
	vector<unsigned> n_start, n_end;//start and end nodes
	n_start.reserve(nNode);
	n_end.reserve(nNode);
	for (size_t i = 0; i < nNode; i++){
		//double check whether any of the start and end nodes are among the moves 
				if (int_stone == XPLR) {//horizontally
					if (hexplace[i*nNode] == int_stone)
						n_start.push_back(i*nNode);//ith node is a horizantal start node
					if (hexplace[(nNode-1)+i*nNode] == int_stone)
						n_end.push_back((nNode - 1) + i * nNode);//ith node is a horizantal end node				
				}
				else {//vertically
					if (hexplace[i] == int_stone)
						n_start.push_back(i);//ith node is a vertical start node					
					if (hexplace[nNode*(nNode - 1)+i] == int_stone) 					
						n_end.push_back(nNode*(nNode - 1) + i);//ith node is a vertical end node					
				}
	}
	if (n_start.size() == 0 || n_end.size() == 0)
		return false;//the game is for sure not over
	if (is_connected(n_start, n_end, stone, hexplace))
		return true; 
	/*for (auto x:n_start)
		for (auto y:n_end)
			if (is_connected(x, y, stone, hexplace))
				return true;*/
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
				//move = rand() % (nNode*nNode - 1);//randomly select a move
				move = plausibile_move(machine, nNode*nNode - human_count - machine_count, 1000, hexplace);
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
