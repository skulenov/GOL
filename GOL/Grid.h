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
	void Clear();
	void RandomGrid();
	void StepGeneration();
	int GetGeneration();
	void Resize(int new_Size);
	bool CellState(int index);
	bool CellState(int i, int j);
	void AlterCell(int index, int state);
	void AlterCell(int i, int j, int state);
	int Length();
	int Size();
	Grid(int);
	~Grid();
};

