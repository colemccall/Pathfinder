#ifndef MAP
#define MAP
#include <string>
#include <vector>
#include "MapTile.h"


class Map
{
private:
	//size of map
	int height, width, startX, startY, goalX, goalY;
	bool pathFound = false;

public:

	//default constructor
	Map(std::string);

	//setter methods
	void setHeight(int);
	void setWidth(int);
	void setStartX(int);
	void setStartY(int);
	void setGoalX(int);
	void setGoalY(int);

	//getter methods
	int getHeight();
	int getWidth();
	int getStartX();
	int getStartY();
	int getGoalX();
	int getGoalY();


	//map display methods
	void displayCurrentTile(int, int);
	void showAllTileData();

	//agent algorithm methods
	void runBreadthFirst();
	void runEightNeighborBreadthFirst();
	void runHeuristic(char);
	void depthLimitedSearch(int);
	//void runEuclidean();


	void iterativeDeepeningSearch(bool);
	void iterativeDepthFirst(MapTile*, int);
	void recursiveDepthFirst(MapTile*, int, int, bool);
	void uniformCostSearch();
	

};

#endif