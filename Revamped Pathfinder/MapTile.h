#include <string>
#include <vector>
#ifndef MAPTILE
#define MAPTILE


class MapTile
{
private:
	std::string tileType;
	int tileCost;
	bool tilePassable = true;
	bool tileVisited = false;
	int x, y;

public:
	//default constructor
	MapTile();

	//parameter constructor;
	MapTile(char, int, int);

	//setters
	void setCost(int);
	void setType(std::string);
	void setPassable(bool);
	void setVisited(bool);

	//getters
	int getCost();
	std::string getType();
	bool isPassible();
	bool isVisited();
	int getX();
	int getY();

	//path 
	MapTile* previous;
	MapTile* next;

	//neighbors
	MapTile* north;
	MapTile* east;
	MapTile* south;
	MapTile* west;

	//eight neighbor
	MapTile* northwest;
	MapTile* northeast;
	MapTile* southwest;
	MapTile* southeast;

	//test
	int depth = 0;

};

#endif

