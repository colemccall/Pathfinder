#include <string>
#include <fstream>
#include <vector>
#include <queue>
#include <stack>
#include <iostream>
#include <chrono>
#include "Map.h"
using namespace std;


vector<MapTile*> mapData;
vector<char> checkedMatrix;
vector<MapTile*> closedList;


Map::Map(string fileName)
{
	ifstream inputFile;
	mapData.clear();

	//open the file and read in data from map
	inputFile.open(fileName);
	if (inputFile.is_open())
	{
		string temp, trash;

		//read in size of the map 
		getline(inputFile, temp, ' ');
		int width = stoi(temp);
		setWidth(width);

		getline(inputFile, temp, '\n');
		int height = stoi(temp);
		setHeight(height);

		//read in the rest of the file
		char data;
		int x = 0, y = 0, count = 0;
		while (!inputFile.eof())
		{
			inputFile >> data;
			MapTile* newTile = new MapTile(data, x, y);
			mapData.push_back(newTile);

			//cout << "Tile " << count << " added" << endl;

			x++;
			if (x == width)
			{
				x = 0;
				y++;
			}
			count++;
		}

		//set neighbors
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				MapTile* current = mapData.at(x + (y * width));
				
				if (y - 1 >= 0)
					current->north = mapData.at(x + ((y - 1) * width));
				if (x - 1 >= 0)
					current->west = mapData.at((x - 1) + (y * width));
				if (y + 1 < height)
					current->south = mapData.at(x + ((y + 1) * width));
				if (x + 1 < width)
					current->east = mapData.at((x + 1) + (y * width));


				if (x - 1 >= 0 && y - 1 >= 0)
					current->northwest = mapData.at((x - 1) + ((y - 1) * width));
				if (x - 1 >= 0 && y + 1 < height)
					current->southwest = mapData.at((x - 1) + ((y + 1) * width));
				if (x + 1 < width && y + 1 < height)
					current->southeast = mapData.at((x + 1) + ((y + 1) * width));
				if (x + 1 < width && y - 1 >= 0)
					current->northeast = mapData.at((x + 1) + ((y - 1) * width));

			}
			cout << endl;
		}

		//set default values
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
				checkedMatrix.push_back('-');
		}
	}

	//if the file name did not open a file, display the error message
	else
		cout << "File: " << fileName << " could not be opened" << endl;
}


//takes an x coordinate and y coordinate and returns the information of that tile
void Map::displayCurrentTile(int currentX, int currentY)
{
	//using the map, the current map tile is examined
	int count = currentY * width + currentX;
	MapTile* currentTile = mapData.at(count);
	string currentTileType = currentTile->getType();
	int currentTileCost = currentTile->getCost();

	//the tile type, cost, and constraints are then displayed
	cout << endl
		<< "Tile @ " << currentX << "," << currentY << " is a " << currentTileType << " tile, costs " << currentTileCost << " to travel,";
	if (currentTile->isPassible())
		cout << " and is passable" << endl;
	else
		cout << " and is not passable" << endl;
}


void Map::showAllTileData()
{
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			displayCurrentTile(x, y);
		}
	}
}



//setter defintions
void Map::setHeight(int height)
{
	this->height = height;
}

void Map::setWidth(int width)
{
	this->width = width;
}

void Map::setStartX(int startX)
{
	this->startX = startX;
}

void Map::setStartY(int startY)
{
	this->startY = startY;
}

void Map::setGoalX(int goalX)
{
	this->goalX = goalX;
}

void Map::setGoalY(int goalY)
{
	this->goalY = goalY;
}


//getter defintions
int Map::getHeight() { return height; }
int Map::getWidth() { return width; }
int Map::getStartX() { return startX; }
int Map::getStartY() { return startY; }
int Map::getGoalX() { return goalX; }
int Map::getGoalY() { return goalY; }



//algorithm method definitions
void Map::runBreadthFirst()
{
	cout << endl << "Breadth first path from (" << startX << "," << startY << ") to (" << goalX << ", " << goalY << ")"
		<< endl;

	//set default values
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
			checkedMatrix.at(x + (y * width)) = '-';
	}
	
	auto start = chrono::steady_clock::now();


	//create a queue that holds the unvisited neighbors (children) of each tile
	queue<MapTile*> fringeTiles;

	//once the path has been found, each tile on the path is pushed onto the stack
	//this stack will be returned to the user 
	stack<MapTile*> pathList;

	int count = startX + (startY * width);
	if (count < mapData.size())
	{
		//put the start state on the fringe
		MapTile* currentTile = mapData.at(count);
		fringeTiles.push(currentTile);

		//while (fringe is not empty)
		while (!fringeTiles.empty())
		{
			//visit first state on the fringe and then remove it from the fringe list
			currentTile = fringeTiles.front();
			int currentX = currentTile->getX();
			int currentY = currentTile->getY();
			currentTile->setVisited(true);

			//take the current value from the fringe and mark it as checked
			checkedMatrix.at(currentX + (currentY * width)) = 'X';

			//if the current tile on the fringe equals the goal
			//then the path is pushed onto a stack and the loop is broken
			if (currentX == goalX && currentY == goalY)
			{
				//create a link between the tiles that form the path
				pathList.push(currentTile);
				while (currentTile->previous != NULL)
				{
					pathList.push(currentTile->previous);
					currentTile = currentTile->previous;
				}
				break;
			}

			//check to see if their is a neighbor to the right, if it can be passed, and if it has not been visited
			//if all these are true, then at it the fringe and create a link
			if (currentTile->east != NULL && currentTile->east->isPassible() && !currentTile->east->isVisited())
			{
				fringeTiles.push(currentTile->east);
				checkedMatrix.at(currentTile->east->getX() + (currentTile->east->getY() * width)) = 'F';
				currentTile->east->previous = currentTile;
			}


			//check to see if their is a neighbor to the left, if it can be passed, and if it has not been visited
			//if all these are true, then at it the fringe and create a link
			if (currentTile->west != NULL && currentTile->west->isPassible() && !currentTile->west->isVisited())
			{
				fringeTiles.push(currentTile->west);
				checkedMatrix.at(currentTile->west->getX() + (currentTile->west->getY() * width)) = 'F';
				currentTile->west->previous = currentTile;
			}
			

			//check to see if their is a neighbor above, if it can be passed, and if it has not been visited
			//if all these are true, then at it the fringe and create a link
			if(currentTile->north != NULL && currentTile->north->isPassible() && !currentTile->north->isVisited())
			{
				fringeTiles.push(currentTile->north);
				checkedMatrix.at(currentTile->north->getX() + (currentTile->north->getY() * width)) = 'F';
				currentTile->north->previous = currentTile;
			}


			//check to see if their is a neighbor down, if it can be passed, and if it has not been visited
			//if all these are true, then at it the fringe and create a link
			if (currentTile->south != NULL && currentTile->south->isPassible() && !currentTile->south->isVisited())
			{
				fringeTiles.push(currentTile->south);
				checkedMatrix.at(currentTile->south->getX() + (currentTile->south->getY() * width)) = 'F';
				currentTile->south->previous = currentTile;
			}
			

			//once children have been added to fringe, tile is removed from fringe and placed on the checked list
			fringeTiles.pop();
		}
	}

	auto end = chrono::steady_clock::now();
	auto diff = static_cast<chrono::duration<double>>(end - start);
	
	//return path found
	//vector contains tiles in reverse order, so tile in the back will be displayed and returned,
	//then popped off and continue to the next tiles in the path
	if (!pathList.empty())
	{
		MapTile* currentTile = pathList.top();
		pathList.pop();
		int sum = 0;
		int moveCount = 0;

		//first element
		cout << "(" << currentTile->getX() << ", " << currentTile->getY() << ")";
		//sum += currentTile->getCost();


		while (pathList.empty() == false)
		{
			//assign current tile and pop off back from vector
			currentTile = pathList.top();
			pathList.pop();

			//then return information of the current tile
			cout << " -> (" << currentTile->getX() << ", " << currentTile->getY() << ")";
			checkedMatrix.at(currentTile->getX() + (currentTile->getY() * width)) = '#';
			sum += currentTile->getCost();
			moveCount += 1;
		}


		//finally display cost of the path
		cout << endl << "Cost of path: " << sum << endl
			<< "Total moves: " << moveCount << endl << endl;
	}

	//mark start and end tiles
	//then display the tiles checked
	checkedMatrix.at(startX + (startY * width)) = 'S';
	checkedMatrix.at(goalX + (goalY * width)) = 'E';
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			cout << checkedMatrix.at(x + (y * width));
		}
		cout << endl;
	}

	cout  << "Time: " << diff.count() << " seconds" << endl;
}


void Map::runEightNeighborBreadthFirst()
{
	cout << endl << "Eight Neighbor breadth first path from (" << startX << "," << startY << ") to (" << goalX << ", " << goalY << ")"
	<< endl;

	//set default values
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
			checkedMatrix.at(x + (y * width)) = '-';
	}

	auto start = chrono::steady_clock::now();
	
	//create a queue that holds the unvisited neighbors (children) of each tile
	queue<MapTile*> fringeTiles;

	//once the path has been found, each tile on the path is pushed onto the stack
	//this stack will be returned to the user 
	stack<MapTile*> pathList;

	//put the start state on the fringe
	int count = startX + (startY * width);
	if (count < mapData.size())
	{
		MapTile* currentTile = mapData.at(count);
		fringeTiles.push(currentTile);

		//while (fringe is not empty)
		while (!fringeTiles.empty())
		{
			//visit first state on the fringe and then remove it from the fringe list
			currentTile = fringeTiles.front();
			int currentX = currentTile->getX();
			int currentY = currentTile->getY();
			currentTile->setVisited(true);

			//cout << currentX << "," << currentY << " has been visited" << endl;
			checkedMatrix.at(currentX + (currentY * width)) = 'X';


			//if the current tile on the fringe is the end goal
			//then the path is pushed onto the stack to display and the loop is broken
			if (currentX == goalX && currentY == goalY)
			{
				//create a link between the tiles that form the path
				pathList.push(currentTile);
				while (currentTile->previous != NULL)
				{
					pathList.push(currentTile->previous);
					currentTile = currentTile->previous;
				}
				break;
			}


			

			//check to see if the agent can move down and to the right (southeast)
			//then add it to the fringe and mark it as fringed
			if (currentTile->southeast != NULL && currentTile->southeast->east != NULL && currentTile->southeast->isPassible() && !currentTile->southeast->isVisited())
			{
				fringeTiles.push(currentTile->southeast);
				checkedMatrix.at(currentTile->southeast->getX() + (currentTile->southeast->getY() * width)) = 'F';
				currentTile->southeast->previous = currentTile;
			}


			//check to see if their is a neighbor down, if it can be passed, and if it has not been visited
			//if all these are true, then at it the fringe and create a link
			if (currentTile->south != NULL && currentTile->south->isPassible() && !currentTile->south->isVisited())
			{
				fringeTiles.push(currentTile->south);
				checkedMatrix.at(currentTile->south->getX() + (currentTile->south->getY() * width)) = 'F';
				currentTile->south->previous = currentTile;
			}

			//check to see if their is a neighbor to the right, if it can be passed, and if it has not been visited
			//if all these are true, then at it the fringe and create a link
			if (currentTile->east != NULL && currentTile->east->isPassible() && !currentTile->east->isVisited())
			{
				fringeTiles.push(currentTile->east);
				checkedMatrix.at(currentTile->east->getX() + (currentTile->east->getY() * width)) = 'F';
				currentTile->east->previous = currentTile;
			}


			//check to see if their is a neighbor to the left, if it can be passed, and if it has not been visited
			//if all these are true, then at it the fringe and create a link
			if (currentTile->west != NULL && currentTile->west->isPassible() && !currentTile->west->isVisited())
			{
				fringeTiles.push(currentTile->west);
				checkedMatrix.at(currentTile->west->getX() + (currentTile->west->getY() * width)) = 'F';
				currentTile->west->previous = currentTile;
			}


			//check to see if their is a neighbor above, if it can be passed, and if it has not been visited
			//if all these are true, then at it the fringe and create a link
			if (currentTile->north != NULL && currentTile->north->isPassible() && !currentTile->north->isVisited())
			{
				fringeTiles.push(currentTile->north);
				checkedMatrix.at(currentTile->north->getX() + (currentTile->north->getY() * width)) = 'F';
				currentTile->north->previous = currentTile;
			}


			// check to see if the agent can move upand to the left(northwest)
			//then add it to the fringe and mark it as fringed
			if (currentTile->northwest != NULL && currentTile->northwest->isPassible() && !currentTile->northwest->isVisited())
			{
				fringeTiles.push(currentTile->northwest);
				checkedMatrix.at(currentTile->northwest->getX() + (currentTile->northwest->getY() * width)) = 'F';
				currentTile->northwest->previous = currentTile;
			}

			//check to see if the agent can move up and to the right (northeast)
			//then add it to the fringe and mark it as fringed
			if (currentTile->northeast != NULL && currentTile->northeast->isPassible() && !currentTile->northeast->isVisited())
			{
				fringeTiles.push(currentTile->northeast);
				checkedMatrix.at(currentTile->northeast->getX() + (currentTile->northeast->getY() * width)) = 'F';
				currentTile->northeast->previous = currentTile;
			}


			//check to see if the agent can move down and to the left (southwest)
			//then add it to the fringe and mark it as fringed
			if (currentTile->southwest != NULL && currentTile->southwest->isPassible() && !currentTile->southwest->isVisited())
			{
				fringeTiles.push(currentTile->southwest);
				checkedMatrix.at(currentTile->southwest->getX() + (currentTile->southwest->getY() * width)) = 'F';
				currentTile->southwest->previous = currentTile;
			}

			//once children have been added to fringe, tile is removed from fringe and placed on the checked list
			fringeTiles.pop();
		}
	}

	auto end = chrono::steady_clock::now();
	auto diff = static_cast<chrono::duration<double>>(end - start);
	
	//return path found
	//vector contains tiles in reverse order, so tile in the back will be displayed and returned,
	//then popped off and continue to the next tiles in the path
	if (pathList.empty() == false)
	{
		MapTile* currentTile = pathList.top();
		pathList.pop();
		int sum = 0;
		int moveCount = 0;

		//first element
		cout << "(" << currentTile->getX() << ", " << currentTile->getY() << ")";
		//sum += currentTile->getCost();


		while (pathList.empty() == false)
		{
			//assign current tile and pop off back from vector
			currentTile = pathList.top();
			pathList.pop();

			//then return information of the current tile
			cout << " -> (" << currentTile->getX() << ", " << currentTile->getY() << ")";
			checkedMatrix.at(currentTile->getX() + (currentTile->getY() * width)) = '#';
			sum += currentTile->getCost();
			moveCount += 1;
		}


		//finally display cost of the path
		cout << endl << "Cost of path: " << sum 
			<< endl << "Total moves: " << moveCount << endl;
	}

	//mark start and end tiles
	//then display the tiles checked
	checkedMatrix.at(startX + (startY * width)) = 'S';
	checkedMatrix.at(goalX + (goalY * width)) = 'E';
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			cout << checkedMatrix.at(x + (y * width));
		}
		cout << endl;
	}


	cout << endl << "Time to find Eight Neighbor Breadth First Path: " << diff.count() << " seconds" << endl << endl;


}


void Map::runHeuristic(char starType)
{
	if (starType == 'm')
		cout << endl << "Manhattan path from (";
	
	else if (starType == 'e')
		cout << endl << "Euclidean path from (";

	else if( starType == 'g')
		cout << endl << "Greedy path from (";
	

	//set default values
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
			checkedMatrix.at(x + (y * width)) = '-';
	}


	cout << startX << "," << startY << ") to (" << goalX << ", " << goalY << ")" << endl;
	auto start = chrono::steady_clock::now();
	

	int diffX, diffY;
	char directionX, directionY;
	
	//if debug is true, code will be displayed to help understand what is going on
	bool debug = false;


	queue<MapTile*> fringeList;	
	queue<MapTile*> pathList;
	

	//starting with the first tile
	MapTile* current = mapData.at(startX + (startY * width));
	pathList.push(current);


	while (!pathList.empty())
	{
		current = pathList.back();
		int currentX = current->getX();
		int currentY = current->getY();
		current->setVisited(true);
		checkedMatrix.at(currentX + (currentY * width)) = '#';


		if (currentX == goalX && currentY == goalY)
			break;


		//check to see if their is a neighbor to the right, if it can be passed, and if it has not been visited
		//if all these are true, then at it the fringe and create a link
		if (current->east != NULL && current->east->isPassible() && !current->east->isVisited())
		{
			fringeList.push(current->east);
			checkedMatrix.at(current->east->getX() + (current->east->getY() * width)) = 'F';
			current->east->previous = current;
		}


		//check to see if their is a neighbor to the left, if it can be passed, and if it has not been visited
		//if all these are true, then at it the fringe and create a link
		if (current->west != NULL && current->west->isPassible() && !current->west->isVisited())
		{
			fringeList.push(current->west);
			checkedMatrix.at(current->west->getX() + (current->west->getY() * width)) = 'F';
			current->west->previous = current;
		}


		//check to see if their is a neighbor above, if it can be passed, and if it has not been visited
		//if all these are true, then at it the fringe and create a link
		if (current->north != NULL && current->north->isPassible() && !current->north->isVisited())
		{
			fringeList.push(current->north);
			checkedMatrix.at(current->north->getX() + (current->north->getY() * width)) = 'F';
			current->north->previous = current;
		}


		//check to see if their is a neighbor down, if it can be passed, and if it has not been visited
		//if all these are true, then at it the fringe and create a link
		if (current->south != NULL && current->south->isPassible() && !current->south->isVisited())
		{
			fringeList.push(current->south);
			checkedMatrix.at(current->south->getX() + (current->south->getY() * width)) = 'F';
			current->south->previous = current;
		}

		//find the best move from the fringe, weighing cost and potential movement
		if (!fringeList.empty())
		{			
			MapTile* nextMove = fringeList.front();
			fringeList.pop();
			
			//debugging code
			if (debug)
			{
				cout << endl << "Default next best path set for (" << currentX << "," << currentY
					<< ") to (" << nextMove->getX() << ", " << nextMove->getY() << ")" << endl;
			}


			int fromX = abs(nextMove->getX() - startX);
			int fromY = abs(nextMove->getY() - startY);
			int toX = abs(nextMove->getX() - goalX);
			int toY = abs(nextMove->getY() - goalY);
			double leastCost;


			//if heuristic is manhattan, least cost = traveled + remaining
			if (starType == 'm')
			{
				int hn = fromX + fromY;
				int gn = toX + toY;
				
				leastCost = hn + gn;
			}

			//if heuristic is euclidean, least cost = x^2 + y^2 square root
			else if (starType == 'e')
			{
				double hn = sqrt((fromX * fromX) + (fromY * fromY));
				double gn = sqrt((toX * toX) + (toY * toY));
				
				leastCost = (hn * 0.7) + gn;
			}

			//if heuristic is greedy, least cost = remaining only
			else
			{
				leastCost = toX + toY;
			}

			
			
			while (!fringeList.empty())
			{
				MapTile* fringe = fringeList.front();
				fringeList.pop();
				double fringeHeuristic;

				//if heuristic is manhattan, fringecost = traveled + remaining
				if (starType == 'm')
				{
					int xTraveled = abs(fringe->getX() - startX);
					int yTraveled = abs(fringe->getY() - startY);
					int xRemaining = abs(fringe->getX() - goalX);
					int yRemaining = abs(fringe->getY() - goalY);

					int hn = xTraveled + yTraveled;
					int gn = xRemaining + yRemaining;
					
					fringeHeuristic = hn + gn;
				}

				//if heuristic is euclidean, fringecost = x^2 + y^2 square root
				else if (starType == 'e')
				{
					int xTraveled = abs(fringe->getX() - startX);
					int yTraveled = abs(fringe->getY() - startY);
					int xRemaining = abs(fringe->getX() - goalX);
					int yRemaining = abs(fringe->getY() - goalY);
					
					double hn = sqrt((xTraveled * xTraveled) + (yTraveled * yTraveled));					
					double gn = sqrt((xRemaining * xRemaining) + (yRemaining * yRemaining));
					
					
					fringeHeuristic = (hn * 0.7) + gn;
				}


				//if heuristic is greedy, fringecost = remaining only
				else
					fringeHeuristic = abs(fringe->getX() - goalX) + abs(fringe->getY() - goalY);
				


				//debugging code
				if (debug)
				{
					cout << "Is (" << fringe->getX() << "," << fringe->getY() << ") closer to ("
						<< goalX << ", " << goalY << ") than (" << nextMove->getX() << "," << nextMove->getY() << ")?" << endl;
				}
				
			
				if(fringeHeuristic < leastCost)
				{
					//debugging code
					if (debug)
					{
						cout << "YES, (" << fringe->getX() << "," << fringe->getY()
							<< ") set as best next path because heuristic of fringe is " << fringeHeuristic
							<< " and lowest available heuristic is: " << leastCost << endl;
					}

					
					leastCost = fringeHeuristic;
					nextMove = fringe;
				}
				else if (fringeHeuristic == leastCost)
				{
					if (debug)
					{
						cout << "KINDOF, (" << fringe->getX() << "," << fringe->getY() << ") and ("
							<< nextMove->getX() << "," << nextMove->getY() << ") are the same distance from goal" << endl
							<< "(" << fringe->getX() << "," << fringe->getY() << ") costs " << fringe->getCost() << endl
							<< "(" << nextMove->getX() << "," << nextMove->getY() << ") costs " << nextMove->getCost() << endl;
					}
				

					
					if (starType == 'm')
					{
						int fringeRemaining = abs(fringe->getX() - goalX) + abs(fringe->getY() - goalY);
						int bestNextRemaining = abs(nextMove->getX() - goalX) + abs(nextMove->getY() - goalY);

						if (fringeRemaining < bestNextRemaining)
						{
							nextMove = fringe;
							leastCost = fringeHeuristic;
						}
					}



					if (fringe->getCost() < nextMove->getCost())
					{
						leastCost = fringeHeuristic;
						nextMove = fringe;
					}


				}
				else
				{
					//debugging code
					if (debug)
					{
						cout << "NO, best next path is still (" << nextMove->getX() << "," << nextMove->getY()
							<< ") because heuristic of fringe is " << fringeHeuristic
							<< " and lowest available heuristic is: " << leastCost << endl;
					}
				}
			}


			current->next = nextMove;
			pathList.push(nextMove);
			
			
			//cout << "(" << currentX << "," << currentY << ") to (" << nextMove->getX() << "," << nextMove->getY() << ")" << endl << endl;
		}
		else
		{
			cout << "FRINGE EMPTY BUT NO SOLUTION FOUND, NOT GOOD" << endl << endl;
			break;
		}
	}



	auto end = chrono::steady_clock::now();
	auto diff = static_cast<chrono::duration<double>>(end - start);


	//if the solution was found, the return it
	if (pathList.back()->getX() == goalX && pathList.back()->getY() == goalY)
	{
		//cost counter
		int cost = 0;
		int moveCount = 0;

		//return start tile and path information
		cout << "Path: (" << pathList.front()->getX() << "," << pathList.front()->getY() << ")";
		//cost += pathList.front()->getCost();
		pathList.pop();

		while (!pathList.empty())
		{
			cout << " -> (" << pathList.front()->getX() << "," << pathList.front()->getY() << ")";
			cost += pathList.front()->getCost();
			moveCount += 1;
			pathList.pop();
		}

		cout  << endl << "Time: " << diff.count() << " seconds" 
			<< endl << "Cost of path: " << cost
			<< endl << "Total moves: " << moveCount << endl << endl;
	}
	else
	{
		cout << endl << "Path could not be found" << endl;
	}


	checkedMatrix.at(startX + (startY * width)) = 'S';
	checkedMatrix.at(goalX + (goalY * width)) = 'E';
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			cout << checkedMatrix.at(x + (y * width));
		}
		cout << endl;
	}

}


void Map::recursiveDepthFirst(MapTile* current, int depth, int limit, bool repeating)
{
	//boolean that decides whether debug code is shown
	bool debug = false;
	

	if (!pathFound)
	{
		//debugging code
		if (debug) cout << "(" << current->getX() << "," << current->getY() << ") checked at depth " << depth << endl;


		if(repeating == false)
			current->setVisited(true);


		//if the current tile is the end result, then return the path
		if (current->getX() == goalX && current->getY() == goalY && depth <= limit)
		{
			//path has been found so set to true and end the search
			pathFound = true;
			//cout << "Path found at level " << depth << endl << endl;


			//take the path and add it to a stack while also setting the path on the graph
			stack<MapTile*> pathStack;
			pathStack.push(current);
			MapTile* temp = current;
			while (temp->previous != NULL)
			{
				//cout << "(" << temp->getX() << "," << temp->getY() << ") to ";
				temp = temp->previous;
				pathStack.push(temp);
				checkedMatrix.at(temp->getX() + (temp->getY() * width)) = '#';
			}

			//set start and end points on the graph
			checkedMatrix.at(startX + (startY * width)) = 'S';
			checkedMatrix.at(goalX + (goalY * width)) = 'E';

			//return visual path graph
			for (int y = 0; y < height; y++)
			{
				for (int x = 0; x < width; x++)
				{
					cout << checkedMatrix.at(x + (y * width));
				}
				cout << endl;
			}

			//return path to the user
			cout << "Path: (" << pathStack.top()->getX() << "," << pathStack.top()->getY() << ")";
			int cost = 0, moveCount = 0;
			pathStack.pop();
			while (!pathStack.empty())
			{
				cout << " -> (" << pathStack.top()->getX() << "," << pathStack.top()->getY() << ")";
				cost += pathStack.top()->getCost();
				moveCount += 1;
				pathStack.pop();
			}


			//show cost of path in terms of moves take and movement cost
			cout << endl << "Cost of path: " << cost
				<< endl << "Total moves: " << moveCount << endl;
		}

		//make sure that the current depth is within limit of search
		else if (depth <= limit && !pathFound)
		{
			//try to add north neighbor to tree and then check it and its branches
			if (current->north != NULL && current->north->isPassible() && !current->north->isVisited())
			{
				if (debug)
				{
					cout << "(" << current->north->getX() << "," << current->north->getY() << ") added to fringe with link from ("
						<< current->getX() << "," << current->getY() << ")" << endl;
				}
				checkedMatrix.at(current->north->getX() + (current->north->getY() * width)) = 'X';
				current->north->previous = current;
				recursiveDepthFirst(current->north, depth + 1, limit, repeating);
			}

			//try to add south neighbor to tree and then check it and its branches
			if (current->south != NULL && current->south->isPassible() && !current->south->isVisited())
			{
				if (debug)
				{
					cout << "(" << current->south->getX() << "," << current->south->getY() << ") added to fringe with link from ("
						<< current->getX() << "," << current->getY() << ")" << endl;
				}
				checkedMatrix.at(current->south->getX() + (current->south->getY() * width)) = 'X';
				current->south->previous = current;
				recursiveDepthFirst(current->south, depth + 1, limit, repeating);
			}

			//try to add east neighbor to tree and then check it and its branches
			if (current->east != NULL && current->east->isPassible() && !current->east->isVisited())
			{
				if (debug)
				{
					cout << "(" << current->east->getX() << "," << current->east->getY() << ") added to fringe with link from ("
						<< current->getX() << "," << current->getY() << ")" << endl;
				}
				checkedMatrix.at(current->east->getX() + (current->east->getY() * width)) = 'X';
				current->east->previous = current;
				recursiveDepthFirst(current->east, depth + 1, limit, repeating);
			}

			//try to add west neighbor to tree and then check it and its branches
			if (current->west != NULL && current->west->isPassible() && !current->west->isVisited())
			{
				if (debug)
				{
					cout << "(" << current->west->getX() << "," << current->west->getY() << ") added to fringe with link from ("
						<< current->getX() << "," << current->getY() << ")" << endl;
				}
				checkedMatrix.at(current->west->getX() + (current->west->getY() * width)) = 'X';
				current->west->previous = current;
				recursiveDepthFirst(current->west, depth + 1, limit, repeating);
			}
		}


		current->setVisited(false);
	}
}


void Map::iterativeDeepeningSearch(bool repeating)
{
	//display type of search to user
	cout << "Iterative Deepening ";

	if (repeating)
		cout << "with repeating ";
	else
		cout << "without repeating ";

	cout << "Search from (" << startX << "," << startY
	<< ") to (" << goalX << "," << goalY << ")" << endl << endl;
	

	//set starting depth at 0
	int depth = 0;

	//EDIT THIS: DEPTH GROWTH FACTOR
	//this should be set at 1, this is the increment factor,
	//but it takes too long at 1
	int depthIncrement = 10;

	//reset display graph
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
			checkedMatrix.at(x + (y * width)) = '-';
	}


	auto start = chrono::steady_clock::now();

	//as long as the path has not been found, keep incrementing the depth and running the search
	while (!pathFound)
	{
		MapTile* root = mapData.at(startX + (startY * width));
		root->depth = 1;
		root->previous = NULL;
		

		//reset closed list every time we increment the depth
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
				if (mapData.at(x + (y * width))->isVisited())
				{
					mapData.at(x + (y * width))->setVisited(false);
					mapData.at(x + (y * width))->previous = NULL;
					//cout << "(" << mapData.at(x + (y * width))->getX() << "," << mapData.at(x + (y * width))->getY() << ")" << endl;
				}
		}
			
		if(repeating == true)
			recursiveDepthFirst(root, 1, depth, true);
		else
			recursiveDepthFirst(root, 1, depth, false);
		//iterativeDepthFirst(root, depth);
		
		//if path has not been found, return a message showing the depth is being incremented
		/*if (!pathFound)
		{
			cout << "Path not found at depth: " << depth
				<< endl << "Increasing depth by 1" << endl << endl;
		}*/
		depth += depthIncrement;
	}

	auto end = chrono::steady_clock::now();
	auto diff = static_cast<chrono::duration<double>>(end - start);
	cout << endl << "Time to find Iterative Deepening Path: " << diff.count() << " seconds" << endl << endl;
}


void Map::depthLimitedSearch(int depth)
{
	cout << "Depth Limited Cost Search from (" << startX << "," << startY
		<< ") to (" << goalX << "," << goalY << ")" << endl;
	
	//reset display graph
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
			checkedMatrix.at(x + (y * width)) = '-';
	}

	auto start = chrono::steady_clock::now();

	MapTile* root = mapData.at(startX + (startY * width));
	recursiveDepthFirst(root, 1, depth, true);


	if (!pathFound)
		cout << "Path could not be found searching " << depth << " levels" << endl << endl;

	auto end = chrono::steady_clock::now();
	auto diff = static_cast<chrono::duration<double>>(end - start);
	cout << endl << "Time to find path using Depth Limited Search " << diff.count() << " seconds" << endl << endl;
}


void Map::uniformCostSearch()
{
	//display type of search to user
	cout << "Uniform Cost Search from (" << startX << "," << startY
		<< ") to (" << goalX << "," << goalY << ")" << endl << endl;

	//reset display graph
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
			checkedMatrix.at(x + (y * width)) = '-';
	}

	//starting tile
	MapTile* root = mapData.at(startX + (startY * width));
	

	struct CompareCost {
		bool operator()(MapTile* const& tileOne, MapTile* const& tileTwo)
		{
			if (tileOne->getCost() == tileTwo->getCost())
			{
				return tileTwo->getY() < tileOne->getY();
			}

			if (tileOne->getType() == "Mountain")
			{
				return tileTwo->getCost() < tileOne->getCost();
			}
			
			else
				return tileTwo->getCost() < tileOne->getCost();
		}
	};


	//fringe list
	//priority_queue<MapTile*, vector<MapTile*>, greater<MapTile*>> fringeList;
	priority_queue<MapTile*, vector<MapTile*>, CompareCost> fringeList;
	//fringeList.empty();
	
	//stack the contains the path
	stack<MapTile*> pathStack;
	pathStack.empty();

	auto start = chrono::steady_clock::now();

	//add starting element to fringe list
	fringeList.push(root); //.push_back(root);
	
	//repeat until path is found
	while (!pathFound)
	{
		//if the fringe is empty, the program ends
		if (fringeList.empty())
		{
			cout << "Fringe empty, no solution found" << endl;
			pathFound = true;
		}

		else
		{
			//current tile is the lowest cost on the fringe
			MapTile* currentTile = fringeList.top(); //front();
			fringeList.pop(); 
			currentTile->setVisited(true);

			
			//check to see if current is the end of the path
			if (currentTile->getX() == goalX && currentTile->getY() == goalY)
			{
				int cost = 0;
				while (currentTile->previous != NULL)
				{
					pathStack.push(currentTile);
					checkedMatrix.at(currentTile->getX() + (currentTile->getY() * width)) = '#';
					cost += currentTile->getCost();
					currentTile = currentTile->previous;
				}

				//if (cost < costLimit)
					pathFound = true;

				//break;
			}


			//check to see if their is a neighbor to the right, if it can be passed, and if it has not been visited
			//if all these are true, then at it the fringe and create a link
			if (currentTile->east != NULL && currentTile->east->isPassible() && !currentTile->east->isVisited())
			{
				fringeList.push(currentTile->east);
				//cout << "(" << currentTile->east->getX() << "," << currentTile->east->getY() << ") added to fringe" << endl;
				//fringeList.push_back(currentTile->east);
				checkedMatrix.at(currentTile->east->getX() + (currentTile->east->getY() * width)) = 'F';
				currentTile->east->previous = currentTile;
				
			}


			//check to see if their is a neighbor to the left, if it can be passed, and if it has not been visited
			//if all these are true, then at it the fringe and create a link
			if (currentTile->west != NULL && currentTile->west->isPassible() && !currentTile->west->isVisited())
			{
				//fringeList.push_back(currentTile->west);
				fringeList.push(currentTile->west);
				//cout << "(" << currentTile->west->getX() << "," << currentTile->west->getY() << ") added to fringe" << endl;
				checkedMatrix.at(currentTile->west->getX() + (currentTile->west->getY() * width)) = 'F';
				currentTile->west->previous = currentTile;
			}


			//check to see if their is a neighbor above, if it can be passed, and if it has not been visited
			//if all these are true, then at it the fringe and create a link
			if (currentTile->north != NULL && currentTile->north->isPassible() && !currentTile->north->isVisited())
			{
				//fringeList.push_back(currentTile->north);
				fringeList.push(currentTile->north);
				//cout << "(" << currentTile->north->getX() << "," << currentTile->north->getY() << ") added to fringe" << endl;
				checkedMatrix.at(currentTile->north->getX() + (currentTile->north->getY() * width)) = 'F';
				currentTile->north->previous = currentTile;
			}


			//check to see if their is a neighbor down, if it can be passed, and if it has not been visited
			//if all these are true, then at it the fringe and create a link
			if (currentTile->south != NULL && currentTile->south->isPassible() && !currentTile->south->isVisited())
			{
				//fringeList.push_back(currentTile->south);
				fringeList.push(currentTile->south);
				//cout << "(" << currentTile->south->getX() << "," << currentTile->south->getY() << ") added to fringe" << endl;
				//checkedMatrix.at(currentTile->south->getX() + (currentTile->south->getY() * width)) = 'F';
				currentTile->south->previous = currentTile;
			}			
		}
	}


	//return time taken
	auto end = chrono::steady_clock::now();
	auto diff = static_cast<chrono::duration<double>>(end - start);


	int cost = 0, moves = 0;
	if (!pathStack.empty() && pathFound)
	{
		//return step by step path
		cout << "Path: (" << pathStack.top()->getX() << "," << pathStack.top()->getY() << ")";
		pathStack.pop();

		while (!pathStack.empty())
		{
			cout << " -> (" << pathStack.top()->getX() << "," << pathStack.top()->getY() << ")";
			moves += 1;
			cost += pathStack.top()->getCost();
			pathStack.pop();
		}

		cout << endl << "Time: " << diff.count() << " seconds" 
			<< endl << "Cost: " << cost
			<< endl << "Moves: " << moves << endl;
		

		//set start and end points on the graph
		checkedMatrix.at(startX + (startY * width)) = 'S';
		checkedMatrix.at(goalX + (goalY * width)) = 'E';

		//return visual path graph
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				cout << checkedMatrix.at(x + (y * width));
			}
			cout << endl;
		}
	}
}


void Map::iterativeDepthFirst(MapTile* root, int limit)
{
	bool debug = false;

	  //set default values
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
			checkedMatrix.at(x + (y * width)) = '-';
	}

	//create fringe stack
	stack<MapTile*> fringeList;
	
	//add root to the stack
	fringeList.push(root);

	while (!fringeList.empty() && !pathFound)
	{
		//get the top tile on the stack and mark it visited, then examine the branches and add them to the stack
		MapTile* current = fringeList.top();
		fringeList.pop();
		current->setVisited(true);


		if (debug) cout << "(" << current->getX() << "," << current->getY() 
			<< ") checked and popped from fringe @ level " << current->depth  << endl;

		//if the current tile on the fringe equals the goal
		//then the path is pushed onto a stack and the loop is broken
		if (current->getX() == goalX && current->getY() == goalY)
		{
			cout << "Solution Found" << endl;
			pathFound = true;
			break;
		}



		//check to see if their is a neighbor to the right, if it can be passed, and if it has not been visited
		//if all these are true, then at it the fringe and create a link
		if (current->east != NULL && current->east->isPassible() && !current->east->isVisited() && current->depth < limit)
		{
			current->east->depth = current->depth + 1; 
			if (debug)
			{
				cout << "(" << current->east->getX() << "," << current->east->getY() << ") added to fringe with link from ("
					<< current->getX() << "," << current->getY() << ") with depth " << current->east->depth << endl;
			}
			fringeList.push(current->east);
			checkedMatrix.at(current->east->getX() + (current->east->getY() * width)) = 'X';
			current->east->previous = current;
			
		}


		//check to see if their is a neighbor to the left, if it can be passed, and if it has not been visited
		//if all these are true, then at it the fringe and create a link
		if (current->west != NULL && current->west->isPassible() && !current->west->isVisited() && current->depth < limit)
		{
			current->west->depth = current->depth + 1; 
			if (debug)
			{
				cout << "(" << current->west->getX() << "," << current->west->getY() << ") added to fringe with link from ("
					<< current->getX() << "," << current->getY() << ") with depth " << current->west->depth << endl;
			}
			
			fringeList.push(current->west);
			checkedMatrix.at(current->west->getX() + (current->west->getY() * width)) = 'X';
			current->west->previous = current;
			
		}


		//check to see if their is a neighbor above, if it can be passed, and if it has not been visited
		//if all these are true, then at it the fringe and create a link
		if (current->south != NULL && current->south->isPassible() && !current->south->isVisited() && current->depth < limit)
		{
			current->south->depth = current->depth + 1; 
			if (debug)
			{
				cout << "(" << current->south->getX() << "," << current->south->getY() << ") added to fringe with link from ("
					<< current->getX() << "," << current->getY() << ") with depth " << current->south->depth << endl;
			}
			
			fringeList.push(current->south);
			checkedMatrix.at(current->south->getX() + (current->south->getY() * width)) = 'X';
			current->south->previous = current;		
		}


		//check to see if their is a neighbor down, if it can be passed, and if it has not been visited
		//if all these are true, then at it the fringe and create a link
		if (current->north != NULL && current->north->isPassible() && !current->north->isVisited() && current->depth < limit)
		{
			current->north->depth = current->depth + 1;
			if (debug)
			{
				cout << "(" << current->north->getX() << "," << current->north->getY() << ") added to fringe with link from ("
					<< current->getX() << "," << current->getY() << ") with depth " << current->north->depth << endl;
			}

			fringeList.push(current->north);
			checkedMatrix.at(current->north->getX() + (current->north->getY() * width)) = 'X';
			current->north->previous = current;

		}
	}
}

