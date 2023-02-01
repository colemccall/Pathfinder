#include <string>
#include "MapTile.h"
using namespace std;





//default constructor
MapTile::MapTile()
{

}



//single parameter constructor
MapTile::MapTile(char type, int x, int y)
{
	if (type == 'R')
	{
		tileCost = 1;
		tileType = "Road";
	}
	else if (type == 'f')
	{
		tileCost = 2;
		tileType = "Field";
	}
	else if (type == 'F')
	{
		tileCost = 4;
		tileType = "Forest";
	}
	else if (type == 'h')
	{
		tileCost = 5;
		tileType = "Hill";
	}
	else if (type == 'r')
	{
		tileCost = 7;
		tileType = "River";
	}
	else if (type == 'M')
	{
		tileCost = 10;
		tileType = "Mountain";
	}
	else
	{
		tilePassable = false;
		tileType = "Water";
		tileCost = 10000;
	}

	this->x = x;
	this->y = y;
}


//getters
int MapTile::getCost() { return tileCost; }
string MapTile::getType() { return tileType; }
bool MapTile::isPassible() { return tilePassable; }
bool MapTile::isVisited() { return tileVisited; }
int MapTile::getX() { return x; };
int MapTile::getY() { return y; };


//setters
void MapTile::setCost(int cost) { tileCost = cost; }
void MapTile::setType(string type) { tileType = type; }
void MapTile::setPassable(bool pass) { tilePassable = pass; }
void MapTile::setVisited(bool visit) { tileVisited = visit; }