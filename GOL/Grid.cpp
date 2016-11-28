#include "stdafx.h"
#include "Grid.h"
#include <vector>
#include <algorithm>

using namespace std;

// Constructor
Grid::Grid(int dim) : gridDim(dim), generation(1)
{
	currGen.assign(gridDim*gridDim, 0);
	nextGen.assign(gridDim*gridDim, 0);
}
Grid::~Grid()
{
}

int Grid::Length()
{
	return (this->currGen.size());
}
int Grid::Size()
{
	return (this->gridDim);
}

void Grid::Resize(int dim)
{
	gridDim = dim;
	currGen.resize(gridDim*gridDim);
	nextGen.resize(gridDim*gridDim);
}
void Grid::Clear()
{
	currGen.assign(gridDim*gridDim, 0);
	nextGen.assign(gridDim*gridDim, 0);
}

void Grid::CountNeighbours()
{
	// Go through each cell in current generation and if it's alive save 
	// count to it's neighbours in next generation

	// Corner cells
	// Top Left p(0,0)=p(0)
	if (currGen.at(0) == 1)
	{
		++nextGen.at(1);
		++nextGen.at(gridDim);
		++nextGen.at(gridDim + 1);
	}
	// Top Right p(gridDim-1,0)=p(gridDim-1)
	if (currGen.at(gridDim - 1) == 1)
	{
		++nextGen.at(gridDim - 2);
		++nextGen.at(2 * (gridDim - 1));
		++nextGen.at(2 * gridDim - 1);
	}
	// Bottom Left p(0,gridDim-1)=p(gridDim*(gridDim-1))
	if (currGen.at(gridDim * (gridDim - 1)) == 1)
	{
		++nextGen.at(gridDim * (gridDim - 2));
		++nextGen.at(gridDim * (gridDim - 2) + 1);
		++nextGen.at(gridDim * (gridDim - 1) - 1);
	}
	// Bottom Right p(gridDim-1,gridDim-1)=p(gridDim*gridDim-1)
	if (currGen.at(gridDim*gridDim - 1) == 1)
	{
		++nextGen.at(gridDim * (gridDim - 1) - 2);
		++nextGen.at(gridDim * (gridDim - 1) - 1);
		++nextGen.at(gridDim * gridDim - 2);
	}

	const int bRowConst = gridDim * (gridDim - 1);
	for (int i = 1; i < gridDim - 1; ++i)
	{
		// Top and Bottom row cells without first and last cells
		int bRowIdx = bRowConst + i;
		// Top row [p(1),p(gridDim-2)]
		if (currGen.at(i) == 1)
		{
			++nextGen.at(i - 1);
			++nextGen.at(i + 1);
			++nextGen.at(i + gridDim - 1);
			++nextGen.at(i + gridDim);
			++nextGen.at(i + gridDim + 1);
		}
		// Bottom row [p(gridDim*gridDim-gridDim+1),p(gridDim*gridDim-2)]
		if (currGen.at(bRowIdx) == 1)
		{
			++nextGen.at(bRowIdx - gridDim - 1);
			++nextGen.at(bRowIdx - gridDim);
			++nextGen.at(bRowIdx - gridDim + 1);
			++nextGen.at(bRowIdx - 1);
			++nextGen.at(bRowIdx + 1);
		}
		// Left and Right columns without top and bottom cells
		int colIdx = i * gridDim;
		// Left Column [p(gridDim),p(gridDim*(gridDim-2))]
		if (currGen.at(colIdx) == 1)
		{
			++nextGen.at(colIdx - gridDim);
			++nextGen.at(colIdx - gridDim + 1);
			++nextGen.at(colIdx + 1);
			++nextGen.at(colIdx + gridDim);
			++nextGen.at(colIdx + gridDim + 1);
		}
		// Right column [p(2*gridDim-1),p(gridDim*gridDim-gridDim-1)]
		int rColIdx = colIdx + gridDim - 1;
		if (currGen.at(rColIdx) == 1)
		{
			++nextGen.at(rColIdx - gridDim - 1);
			++nextGen.at(rColIdx - gridDim);
			++nextGen.at(rColIdx - 1);
			++nextGen.at(rColIdx + gridDim - 1);
			++nextGen.at(rColIdx + gridDim);
		}
	}
	// Center region cells
	for (int i = 1; i < gridDim - 1; ++i)
	{
		for (int j = 1; j < gridDim - 1; ++j)
		{
			int idx = i + gridDim * j;
			if (currGen.at(idx) == 1)
			{
				++nextGen.at(idx - gridDim - 1);
				++nextGen.at(idx - gridDim);
				++nextGen.at(idx - gridDim + 1);
				++nextGen.at(idx - 1);
				++nextGen.at(idx + 1);
				++nextGen.at(idx + gridDim - 1);
				++nextGen.at(idx + gridDim);
				++nextGen.at(idx + gridDim + 1);
			}
		}
	}
}

void Grid::SurviveOrDie()
{
	// For every cell apply rules of Game of Life:
	// 1) If a cell is alive (currGen=1) and has 2 live neighbours (nextGen=2) it lives in the next generation (nextGen=1)
	// 2) if a cell has 3 live neighbours, it will become or stay alive regardless of it's current state

	transform(nextGen.begin(), nextGen.end(), currGen.begin(), nextGen.begin(), [](int n, int c) {return ((n == 3 || (c == 1 && n == 2)) ? 1 : 0); });

	//for (unsigned int i = 0; i < nextGen.size(); ++i)
	//{
	//	nextGen.at(i) = (nextGen.at(i) == 3 || (currGen.at(i) == 1 && nextGen.at(i) == 2)) ? 1:0;
	//}
}

void Grid::NewWave()
{
	currGen = nextGen;
	nextGen.assign(gridDim*gridDim, 0);
	++generation;
}

void Grid::StepGeneration()
{
	CountNeighbours();
	SurviveOrDie();
	NewWave();
}

bool Grid::CellState(int cellIndex)
{
	return (currGen.at(cellIndex) > 0);
}
bool Grid::CellState(int x, int y)
{
	return CellState(x + gridDim * y);
}

void Grid::AlterCell(int cellIndex, int cellState)
{
	currGen.at(cellIndex) = cellState;
}
void Grid::AlterCell(int x, int y, int cellState)
{
	AlterCell(x + gridDim * y,cellState);
}

void Grid::RandomGrid()
{
	srand((int)time(0));									// Use current time as seed for random number generator
	generate(currGen.begin(), currGen.end(), []() {return (rand() % 2); });
}

int Grid::GetGeneration()
{
	return generation;
}

