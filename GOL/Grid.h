#pragma once
#include <vector>

class Grid
{
private:
	int gridDim;
	int generation;
	std::vector<int> currGen;
	std::vector<int> nextGen;
	void CountNeighbours();
	void SurviveOrDie();
	void NewWave();

public:
	void RandomGrid();
	bool CellState(int);
	bool CellState(int, int);
	void AlterCell(int, int);
	void AlterCell(int, int, int);
	void StepGeneration();
	int GetGeneration();

	Grid(int);
	~Grid();
};

