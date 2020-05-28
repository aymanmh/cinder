#pragma once
#include<vector>


class Universe
{
public:
	Universe(int rows, int columns);
	~Universe();
	void clear();
	void tick();
	void flipCellState(int x, int y);
	void randomize();
	const std::vector<int>& getGrid();
private:
	int countNeighbors(int x, int y);

	std::vector<int> grid_m;
	int columns_m;
	int rows_m;
};