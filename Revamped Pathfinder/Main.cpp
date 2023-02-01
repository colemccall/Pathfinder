#include <iostream>
#include <math.h>
#include <iomanip>
#include <fstream>
#include <string>
#include <memory>
#include <vector>
#include <deque>          
#include <list>           
#include <queue> 
#include <stack>
#include <chrono>
#include "Map.h"
using namespace std;

//function prototypes
void returnInstructions();
void driver();


//example command line
//"map.txt" "5" "3" "8" "1" "b8"


//main
int main(int argc, char** argv)
{
	cout << "PathFinder Project Setup" << endl;

	//if there are less than 5 arguments or more than 6, an error messge is returned to the user
	if (argc != 7)
	{
		cout << "Invalid number of arguments entered by command line" << endl;
		returnInstructions();
	}

	//five arguments need to be entered to run the program
	else if (argc == 7)
	{
		//create a map based on the command line arguments
		//then set the start and goal coordinates to the remaining command line arguments
		string fileName = argv[1];
		string algorithmType = argv[6];
		int startX = stoi(argv[2]);
		int startY = stoi(argv[3]);
		int goalX = stoi(argv[4]);
		int goalY = stoi(argv[5]);
		Map* pathfinder = new Map(fileName);
		int mapWidth = pathfinder->getWidth();
		int mapHeight = pathfinder->getHeight();

		//if all the parameters entered are valid, set the map parameters and run the program
		if (startX >= 0 && startX < mapWidth
			&& startY >= 0 && startY < mapHeight
			&& goalX >= 0 && goalX < mapWidth
			&& goalY >= 0 && goalY < mapHeight)
		{
			//set start and end points
			pathfinder->setStartX(startX);
			pathfinder->setStartY(startY);
			pathfinder->setGoalX(goalX);
			pathfinder->setGoalY(goalY);

			//shows all tiles and makes sure that the map is accurate
			//pathfinder->showAllTileData();

			//depending on the command line parameter entered, a path will be found using the algorithm entered
			//breadth first
			if (algorithmType == "b")
				pathfinder->runBreadthFirst();


			else if (algorithmType == "b8")
				pathfinder->runEightNeighborBreadthFirst();


			//iterative deepening
			else if (algorithmType == "i" )
				pathfinder->iterativeDeepeningSearch(true); 
			else if( algorithmType == "ia")
				pathfinder->iterativeDeepeningSearch(false);


			//uniform cost search
			else if (algorithmType == "l")
				//pathfinder->uniformCostDeepening();
				pathfinder->uniformCostSearch();


			//a star heuristics
			//manhattan
			else if (algorithmType == "a1")
				pathfinder->runHeuristic('m');

			//euclidean
			else if (algorithmType == "a2")
				pathfinder->runHeuristic('e');

			//greedy first
			else if (algorithmType == "a3")
				pathfinder->runHeuristic('g');

			//run all searches
			else if (algorithmType == "d" || algorithmType == "driver")
				driver();
			else
				returnInstructions();


		}
		else
		{
			//error message displayed when coordinates entered through command line are not valid
			cout << endl << "Improper coordinates entered. Make sure coordinates entered are within the size of map" << endl;
			returnInstructions();
		}
	}


	return 0;
}



//this method returns the instructions on how to run the program and enter command line arguments
//this method is displayed when the command line arguments are entered improperly
void returnInstructions()
{
	cout << endl << "Instructions for Pathfinder Program"
		<< endl << endl << "First Argument: text file that contains the map"
		<< endl << endl << "Starting and ending coordinates"
		<< endl << setw(42) << "Second Argument: starting x coordinate"
		<< endl << setw(41) << "Third Argument: starting y coordinate"
		<< endl << setw(38) << "Fourth Argument: goal x coordinate"
		<< endl << setw(37) << "Fifth Argument: goal y coordinate"
		<< endl << endl << "Sixth Argument: type of algorithm"
		//<< endl << setw(40) << "Valid Command Line Algorithm Arguments"
		<< endl << setw(29) << "\"b\"  Breadth First Search"
		<< endl << setw(37) << "\"b8\" Eight Neighbor Breadth First"
		<< endl << setw(46) << "\"ia\" Iterative Deepening without Repeating"
		<< endl << setw(43) << "\"i\"  Iterative Deepening with Repeating"
		<< endl << setw(20) << "\"l\"  Lowest Cost"
		<< endl << setw(18) << "\"a1\" Manhattan"
		<< endl << setw(18) << "\"a2\" Euclidean"
		<< endl << setw(15) << "\"a3\" Greedy"
		<< endl << setw(33) << "\"d\"  Driver: Run All Searches"
		<< endl << endl << "Example: \"map.txt\" \"5\" \"3\" \"12\" \"2\" \"b8\""
		<< endl << "Example: \"map.txt\" \"7\" \"5\" \"6\" \"13\" \"b\""
		<< endl << "Coordinates need to exist on the map within the bounds of the size of the map"
		<< endl << "Map will start with (0,0) in the upper left hand corner"
		<< endl;
}


void driver()
{
	//test coordinates
	//int startX = 4, startY = 8, goalX = 13, goalY = 13;
	int startX = 2, startY = 0, goalX = 13, goalY = 18;
	//"map.txt" "7" "5" "6" "13" "b"

	//test breadth first four neighbor
	Map* pathfinder = new Map("map.txt");
	pathfinder->setStartX(startX);
	pathfinder->setStartY(startY);
	pathfinder->setGoalX(goalX);
	pathfinder->setGoalY(goalY);
	pathfinder->runBreadthFirst();
	

	Map* findPathtwo = new Map("map.txt");
	findPathtwo->setStartX(startX);
	findPathtwo->setStartY(startY);
	findPathtwo->setGoalX(goalX);
	findPathtwo->setGoalY(goalY);
	findPathtwo->runEightNeighborBreadthFirst();

	Map* findPathThree = new Map("map.txt");
	findPathThree->setStartX(startX);
	findPathThree->setStartY(startY);
	findPathThree->setGoalX(goalX);
	findPathThree->setGoalY(goalY);
	findPathThree->runHeuristic('m');

	Map* findPathFour = new Map("map.txt");
	findPathFour->setStartX(startX);
	findPathFour->setStartY(startY);
	findPathFour->setGoalX(goalX);
	findPathFour->setGoalY(goalY);
	findPathFour->runHeuristic('e');

	Map* findPathFive = new Map("map.txt");
	findPathFive->setStartX(startX);
	findPathFive->setStartY(startY);
	findPathFive->setGoalX(goalX);
	findPathFive->setGoalY(goalY);
	findPathFive->runHeuristic('g');

	Map* findPathSeven = new Map("map.txt");
	findPathSeven->setStartX(startX);
	findPathSeven->setStartY(startY);
	findPathSeven->setGoalX(goalX);
	findPathSeven->setGoalY(goalY);
	findPathSeven->uniformCostSearch(); 
	
	
	Map* findPathSix = new Map("map.txt");
	findPathSix->setStartX(startX);
	findPathSix->setStartY(startY);
	findPathSix->setGoalX(goalX);
	findPathSix->setGoalY(goalY);
	findPathSix->iterativeDeepeningSearch(false);

	


	returnInstructions();
}


