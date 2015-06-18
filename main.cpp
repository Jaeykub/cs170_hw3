#include "grid.h"

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <iomanip>

using namespace std;

const int ITERATIONS = 10000;
const int UPDATE = 100;

// Function dirTaken
/////
// In -> Direction
// Out -> Direction
// Given the intended direction the agent wants to go, dirTaken returns the
// direction the agent will actually go using the 80/10/10 proability.
Direction dirTaken( Direction dir )
{
	int n = rand() % 10;
	
	// take the intended direction
	if( n <= 7 ){ return dir; }
	
	//go to the left direction 10% of the time
	else if( n == 8 )
	{
		switch (dir)
		{
			case NORTH:
				return WEST;
			case SOUTH:
				return EAST;
			case WEST:
				return SOUTH;
			case EAST:
				return NORTH;
			case NONE:
				return NORTH;
		}
	}
	//go to the right direction 10% of the time
	else
	{
		switch (dir)
		{
			case NORTH:
				return EAST;
			case SOUTH:
				return WEST;
			case WEST:
				return NORTH;
			case EAST:
				return SOUTH;
			case NONE:
				return NORTH;
		}
	}
	
	return NONE;
}

/////////
// agent
/////////
// In -> Grid
// the agent will take the grid and run thru it a number of times and learn the optimal policy.
void agent( Grid & world ) 
{
	//the current position of the agent, initally assigne the start state.
	pair<int,int> pos = world.getStartLocation();
	// the state that the agent will move to next.
	pair<int,int> next;
	Direction dir;
	// numOfiterations increases when the agent has reached a terminal state, obstacle,
	// or the number of movesMAX is reached.
	int numOfiterations = 0;
	const int movesMAX = world.getRows() * world.getRows() - world.getRows();
	// a move is when an agent transitions from one state to another.
	int numOfMoves = 0;
	
	while( numOfiterations < ITERATIONS )
	{
		// update the policy after UPDATE iterations.
		if( numOfiterations != 0 && numOfiterations % UPDATE == 0 )
			world.updatePolicy();
		
		if( !world.isTerminal(pos) && !world.isObstacle(pos) && world[pos].policy.first == NONE )
			world[pos].policy.first = static_cast<Direction>( rand() & NUM_DIRECTIONS );
		
		dir = dirTaken(world[pos].policy.first );
		next = world.moveTo(pos, dir);
		
		// if the agent is not moving into an edge of the world update the reward and move the agent.
		if( world.validDirection(pos, dir) )
		{
			world[pos].reward = world.updateReward(pos, dir);
			pos = next;
			++numOfMoves;
			
			// if the position the agent has moved to is a terminal state or an obstacle
			// increment numOfiterations and put the agent back in the starting position.
			if( world.isTerminal(pos) || world.isObstacle(pos) || numOfMoves == movesMAX )
			{
				++numOfiterations;
				numOfMoves = 0;
				pos = world.getStartLocation();
			}
		}
	}
}

int main( int argc, char** argv )
{
	if( argc != 3 )
	{
		cerr << "Please provide two inputs." << endl;
		return -1;
	}
	
	unsigned seed = time(NULL);
	int n = atoi(argv[1]);
	int m = atoi(argv[2]);
	cout << "n: " << n << " m: " << m << "\nseed: " << seed << endl;
	srand(seed);
	
	Grid world(n,m);
	
	world.print();

	agent( world );
	cout << "\nIterations: " << ITERATIONS << endl;
	world.print();
	return 0;
}
