#include "universe.h"
#include "cinder/Rand.h"

using namespace ci;
using namespace ci;

Universe::Universe(int rows, int columns) : columns_m(columns), rows_m(rows)
{
	grid_m.resize(columns_m*rows_m);
}

Universe::~Universe()
{
}

void Universe::clear()
{
	grid_m.clear();
	grid_m.resize(columns_m*rows_m);
}

const std::vector<int>& Universe::getGrid()
{
	return grid_m;
}
void Universe::tick()
{
	std::vector<int> nextGrid = grid_m;

	for (int i = 0; i < columns_m; i++)
	{
		for (int j = 0; j < rows_m; j++)
		{
			int state = grid_m[i + j * columns_m];
			int sum = 0;
			int neighbors = countNeighbors(i, j);

			if (state == 0 && neighbors == 3)
			{
				nextGrid[i + j * columns_m] = 1;
			}
			else if (state == 1 && (neighbors < 2 || neighbors > 3))
			{
				nextGrid[i + j * columns_m] = 0;
			}
		}
	}

	grid_m = nextGrid;
}

void Universe::flipCellState(int x, int y)
{
	grid_m[x + y * columns_m] = !(grid_m[x + y * columns_m]);
}

void Universe::randomize()
{
	for (int i = 0; i < columns_m; i++)
	{
		for (int j = 0; j < rows_m; j++)
		{
			grid_m[i + j * columns_m] = Rand::randInt(2);
		}
	}
}

int Universe::countNeighbors(int x, int y)
{
	int sum = 0;
	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			int col = (x + i + columns_m) % columns_m;
			int row = (y + j + rows_m) % rows_m;
			sum += grid_m[col + row * columns_m];
		}
	}
	sum -= grid_m[x + y * columns_m];
	return sum;
}