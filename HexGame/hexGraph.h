#pragma once
#include<vector>

using namespace std;

class hexGraph
{
public:
	enum hexState { EMPTY, XPLR, OPLR };
	hexGraph(unsigned n = 0);
	void print(vector<hexState>&);
	void hex_game();
	bool game_over(char, vector<hexState>&);
	bool is_connected(vector<unsigned> s, vector<unsigned> e, char, vector<hexState>&);// to check whether there is a path from s to e
	unsigned plausibile_move(char, unsigned, unsigned, vector<hexState>&);//generate legal moves using monte carlo
	~hexGraph();

private:
	unsigned nNode;
	vector<vector<unsigned>> edgelist;
};
