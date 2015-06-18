////////////////////////////////
//
// University of California, Riverside
// CS170 Spring 2015 Assignment 3
// grid.cpp
//
// You are encouraged to modify and add to this file
//////////////////////////////
#include "grid.h"

#include <iostream>
#include <iomanip>
#include <cfloat>
#include <cmath>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <valarray>

using namespace std;

const double DISCOUNT = 0.9;
const int VISIT_MAX = 5;
const double OPT_REWARD = 5.0;

///////////////
/// Utility ///
///////////////

///////////
// Function centerStr
///////////
// In -> string s      - value of string we are centering.
//       int reserve   - # of characters (width).
//       char autofill - char to print for the left and right remaining space.
// Out -> string
//
// Returns a string with length of the reserved space with the string, s, centered.
string centerStr(const string & s, int reserve, char autofill)
{
    int blanks = reserve - s.length();
    int lbuf = blanks / 2; // Add one to favor remainder to right
    int rbuf = (blanks+1) / 2;
    stringstream ss;
    ss << setfill(autofill) << setw(lbuf) << "" << s << setw(rbuf) << "";
    return ss.str();
}

///////////
// Function toString
///////////
// In -> double
// Out -> string
//
// Returns a double converted to a string with fixed precision.
string toString(double value, int prec)
{
    stringstream ss;
    if (value > 0.0)
        ss << '+';
    ss << setprecision(prec) << fixed << value;
    return ss.str();
}

///////////
// Function toString
///////////
// In -> Direction
// Out -> string
//
// Returns an ascii version of Direction as a string.
string toString(Direction dir)
{
    switch (dir)
    {
        case NORTH:
            return "^";
        case EAST:
            return ">";
        case SOUTH:
            return "v";
        case WEST:
            return "<";
        default:
            return "?";
    }
}

///////////////////////
/// GridCell Object ///
///////////////////////

// Constructors
GridCell::GridCell()
    : type(BLANK), reward(0.0), start(false), visit(0), policy(NONE, 0.0)
{
}

GridCell::GridCell(GridCellType type, double reward, int visit, bool start)
    : type(type), reward(reward), start(start), visit(visit), policy(NONE, 0.0)
{
}
    
///////////
// Function print
///////////
// In -> string pstr - agent symbol, can be the empty string if not need print.
//       int reserve - the amount of characters (width) we can print for each line.
// Out -> vector<string> - index by row, the output contents of this cell
//
// Returns the output contents of this cell in four different rows.
// Refer to Grid class for more information of the ascii-grid layout
vector<string> GridCell::print(const string & pstr, int reserve) const
{
    vector<string> ret(4);
        
    string prefix1st = "+";
    string prefixedge = "|";
    if (type == OBSTACLE)
    {
        ret[0] = prefix1st + centerStr("", reserve - prefix1st.length(), '-');
        ret[1] = prefixedge + centerStr("xxxx", reserve - prefixedge.length(), ' ');
        ret[2] = prefixedge + centerStr("xxxx", reserve - prefixedge.length(), ' ');
        ret[3] = prefixedge + centerStr("xxxx", reserve - prefixedge.length(), ' ');
    }
    else
    {
        ret[0] = prefix1st + centerStr("", reserve - prefix1st.length(), '-');
        ret[1] = prefixedge + centerStr(toString(reward + policy.second), reserve - prefixedge.length());
        ret[2] = prefixedge + centerStr(start ? pstr + "S" : pstr, reserve - prefixedge.length());
        if (type == TERMINAL)
            ret[3] = prefixedge + centerStr("*", reserve - prefixedge.length());
        else
            ret[3] = prefixedge + centerStr(toString(policy.first), reserve - prefixedge.length());
    }
        
    return ret;
}
    
///////////
// Function getPolicy
///////////
// In ->
// Out -> pair<Direction, double>
//
// Returns the current policy in the form of the pair,
// The first being the action and the the second being the value.
pair<Direction, double> GridCell::getPolicy() const
{
    return policy;
}

///////////////////
/// Grid Object ///
///////////////////
    
///////////////
// Constructors
///////////////
// In -> int n, parameter described by assignment prompt
//       int m, parameter described by assignment prompt
// Out ->
Grid::Grid(int n, int m)
{
	srand(time(NULL));
    bounds = make_pair<int,int>(n, n);
    grid = vector<vector<GridCell> >(bounds.first, vector<GridCell>(bounds.second));
	pair<int,int> start(rand() % n, rand()  % n);
    startLocation = start;
	grid[startLocation.first][startLocation.second].start = true;
	
    //Assigns n obstacles
	int numObstacles = 0;
	while( numObstacles < n )
	{
		pair<int,int> fill( rand() % n, rand() % n);
		if( fill != getStartLocation() && !isObstacle( fill ) )
		{
			grid[fill.first][fill.second].type = GridCell::OBSTACLE;
			++numObstacles;
		}
	}
	
	//Assigns m positive and negative terminal states.
	int posStates = 0;
	int negStates = 0;
	pair<int,int> pos;
	while( posStates < m )
	{
		pos = make_pair(rand() % n, rand() % n);
		
		if( pos != getStartLocation() && !isObstacle(pos) && !isTerminal(pos) )
		{
			grid[pos.first][pos.second].type = GridCell::TERMINAL;
			grid[pos.first][pos.second].reward = 10.0;
			++posStates;
		}
	}
	
	while( negStates < m )
	{
		pos = make_pair(rand() % n, rand() % n);
		
		if( pos != getStartLocation() && !isObstacle(pos) && !isTerminal(pos) )
		{
			grid[pos.first][pos.second].type = GridCell::TERMINAL;
			grid[pos.first][pos.second].reward = -10.0;
			++negStates;
		}
	}
}
    
// Accessors to the 2D grid
GridCell& Grid::operator[](const pair<int,int> & pos)
{
    return grid[pos.first][pos.second];
}
const GridCell& Grid::operator[](const pair<int,int> & pos) const
{
    return grid[pos.first][pos.second];
}
pair<int,int> Grid::getBounds() const
{
    return bounds;
}
int Grid::getRows() const
{
    return bounds.first;
}
int Grid::getCols() const
{
    return bounds.second;
}
pair<int,int> Grid::getStartLocation() const
{
    return startLocation;
}
bool Grid::isObstacle( pair<int,int> n ) const
{
	return grid[n.first][n.second].type == GridCell::OBSTACLE;
}
bool Grid::isTerminal( pair<int,int> n ) const
{
	return grid[n.first][n.second].type == GridCell::TERMINAL;
}

/////////
// moveTo
/////////
// In -> pair, Direction
// Out -> pair
// return the integer pair loc in the direction provided by dir.
pair<int,int> Grid::moveTo( pair<int,int> loc, Direction dir )
{
	switch( dir )
	{
		case NORTH:
			--loc.first;
			break;
		case SOUTH:
			++loc.first;
			break;
		case WEST:
			--loc.second;
			break;
		case EAST:
			++loc.second;
			break;
		case NONE:
			break;
	}
	
	return loc;
}

/////////
// validDirection
////
// In -> Direction, pair, intended
// Out -> bool
// ensures the direction the agent is going to take is within the bounds of the grid world.
bool Grid::validDirection( pair<int,int> loc, Direction dir  )
{
	if( dir == SOUTH ) { return loc.first + 1 < getRows(); }
	else if( dir == NORTH) { return loc.first - 1 >= 0; }
	else if( dir == WEST ){ return loc.second - 1 >= 0; }
	else if( dir == EAST ){ return loc.second + 1 < getRows(); }
	return false;
}

/////////
// getReward
/////////
// In -> pair, Direction
// Out -> double
// returns the reward of the S' following the direction from state S.
// if the directions points towards a wall it'll return the value 0.0.
// Also, if S' has been visited less than VISIT_MAX it'll return OPT_REWARD.
double Grid::getReward( pair<int,int> pos, Direction dir )
{
	if( validDirection(pos,dir) )
	{
		pair<int,int> next = moveTo(pos, dir);
		if( isTerminal(next) || isObstacle(next) || grid[next.first][next.second].visit >= VISIT_MAX )
			return grid[next.first][next.second].reward;
		else
			return OPT_REWARD;
	}
	
	return 0.0;
}

///////
// max
///////
// In -> pair, pair
// Out -> pair
// max takes in two pairs and returns the one with the greater double value.
pair<double, Direction> Grid::max( pair<double, Direction> a, pair<double, Direction> b )
{
	if( a.first >= b.first )
		return a;
	return b;
}

/////////
// updateReward
/////////
// In -> pair, Direction
// Out -> double
// updates the reward of the current state and increments the visit counter.
double Grid::updateReward( pair<int,int> pos, Direction dir )
{
	int i = pos.first;
	int j = pos.second;
	double stateReward = grid[i][j].reward;
	++grid[i][j].visit;
	
	if( grid[i][j].visit < VISIT_MAX )
		return OPT_REWARD;
	
	double learn = static_cast<double>( 1.0/(grid[i][j].visit + 1) );
	double sumOfReward = 0.8 * getReward(pos,dir);
	
	switch(dir)
	{
		case NORTH:
		case SOUTH:
			sumOfReward += 0.1 * getReward(pos, EAST);
			sumOfReward += 0.1 * getReward(pos, WEST);
			break;
		case WEST:
		case EAST:
			sumOfReward += 0.1 * getReward(pos, NORTH);
			sumOfReward += 0.1 * getReward(pos, SOUTH);
			break;
		default:
			break;
	}
	
	return stateReward + learn * (DISCOUNT * sumOfReward - stateReward);
}

////////
//  updatePolicy
////////
// updates the policy of each state that has been visited.
void Grid::updatePolicy()
{
	pair<int,int> pos;
	for( int i=0; i < getRows(); ++i)
	{
		for( int j=0; j < getRows(); ++j)
		{
			pos = make_pair<int,int>(i, j);
			
			if( grid[i][j].visit > 0 && !isTerminal(pos) && !isObstacle(pos) )
			{
				
				pair<double, Direction> up( getReward(pos, NORTH), NORTH );
				pair<double, Direction> down( getReward(pos, SOUTH), SOUTH );
				pair<double, Direction> left( getReward(pos, WEST), WEST );
				pair<double, Direction> right( getReward(pos, EAST), EAST );
				
				pair<double, Direction> M = max( max(up, down), max(left, right) );
				
				grid[i][j].policy.first = M.second;
			}
		}
	}
}
///////////
// Function print
///////////
// In -> pair<int,int> agentPos -
//          position of agent provided. If indicies are negative or out
//          of bounds. This parameter is ignored.
// Out ->
//
// Prints each cell content with special information centered on its row in the following format:
//
// +------
// |value
// |start
// |policy
//
// value - displays the value of the current policy in the format [+/-]##.##
// start - displays 'S' for whether this cell is a start location. A 'P' is also appended
//          if the agentPos is located at that cell.
// policy - displays '^' '>' 'v' '<' if the best policy is NORTH, EAST, SOUTH, or WEST respectively.
//              If policy is set to NONE, it will display '?'
//
// Special cases:
// - If cell is an obstacle, the contents of the cell for each line contain "xxxx" instead
// - if cell is a terminal, the policy displayed for that cell will be '*' instead
//
// Once each cell is printed, it will append a final right and bottom edge
// to complete the ascii table.
void Grid::print(const pair<int,int> & agentPos) const
{
    const int RESERVE = 7;
    for (int i = 0; i < bounds.first; ++i)
    {
        vector<string> outputs(4);
            
        for (int j = 0; j < bounds.second; ++j)
        {
            vector<string> ret;
            if (i == agentPos.first && j == agentPos.second)
                ret = grid[i][j].print("P", RESERVE);
            else
                ret = grid[i][j].print(" ", RESERVE);
            outputs[0] += ret[0];
            outputs[1] += ret[1];
            outputs[2] += ret[2];
            outputs[3] += ret[3];
        }
        cout << outputs[0] << "+" << endl
        << outputs[1] << "|" << endl
        << outputs[2] << "|" << endl
        << outputs[3] << "|" << endl;
    }
    cout << left << setfill('-');
    for (int j = 0; j < bounds.second; ++j)
        cout << setw(RESERVE) << "+";
    cout << "+";
    cout << endl;
}
