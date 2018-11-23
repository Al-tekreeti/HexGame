#pragma once
#include<vector>

using namespace std;

class hexGraph
{
public:
	enum hexState { EMPTY, XPLR, OPLR };
	hexGraph(unsigned n = 0);
	void print();
	void hex_game();
	bool game_over(char player);
	~hexGraph();

private:
	unsigned nNode;
	vector<vector<unsigned>> edgelist;
	vector<hexState> hexplace;
};