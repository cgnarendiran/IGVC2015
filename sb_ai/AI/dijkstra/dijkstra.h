#pragma once

struct Location
{
	int x;
	int y;
};

class Dijkstra
{
private:
	
	int * map;
	int map_width, map_height, map_size;
	int start = -1;
	int goal = -1;
	int * parent;	// predecessors
	int * distance;	// distance from start, -1 if not travelled
	int destination;

	int ConvertToIndex(Location xy);
	Location ConvertToLocation(int n);
	void CheckBoundaries(Location neighbor, int current);
	

public:

	// set these before starting
	void SetStart(int s);
	void SetGoal(int g);
	
	int GetStart();
	int GetGoal();
	
	bool Init(int * main_map, int width, int height);	
	void Search(int location);	// Executes the algorithm
	void ReconstructPath();
};