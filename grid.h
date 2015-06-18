////////////////////////////////
//
// University of California, Riverside
// CS170 Spring 2015 Assignment 3
// grid.h
//
// You are encouraged to modify and add to this file
//////////////////////////////
// The Grid class provides a print utility and necessary accessors.
// Initialization of the Grid class is incomplete.
//
#include <vector>
#include <string>
#include <utility>

///////////////
/// Utility ///
///////////////

const int NUM_DIRECTIONS = 4;
enum Direction { NORTH, EAST, SOUTH, WEST, NONE };

///////////
// Function centerStr
///////////
// In -> string s      - value of string we are centering.
//       int reserve   - # of characters (width).
//       char autofill - char to print for the left and right remaining space.
// Out -> string
std::string centerStr(const std::string & s, int reserve, char autofill = ' ');
///////////
// Function toString
///////////
// In -> double
// Out -> string
std::string toString(double value, int prec = 2);

///////////
// Function toString
///////////
// In -> Direction
// Out -> string
std::string toString(Direction dir);

///////////////////////
/// GridCell Object ///
///////////////////////
//
// Contains information for a Cell in "GridWorld"
// and nice ascii printing functionality.
struct GridCell
{
    // GridCellType definitions:
    // BLANK - traversable cell
    // TERMINAL - episode ends if agent ever steps on this cell
    // OBSTACLE - non-traversable cell
    enum GridCellType { BLANK, TERMINAL, OBSTACLE };
    GridCellType type;
    
    // Reward for landing on this cell.
    double reward;
    
    // Flag which specifies whether this is a start location for the agent.
    bool start;
	
	// number of times a cell has been visited
	int visit;
    
    // Current policy where the first is the direction and the second is the value.
    std::pair<Direction, double> policy;
    
    // Constructors
    GridCell();
    GridCell(GridCellType type, double reward = 0.0, int visit = 0, bool start = false);
    
    ///////////
    // Function print
    ///////////
    // In -> string pstr - agent symbol, can be the empty string if not need print.
    //       int reserve - the amount of characters (width) we can print for each line.
    // Out -> vector<string> - index by row, the output contents of this cell
    std::vector<std::string> print(const std::string & pstr, int reserve) const;
    
    ///////////
    // Function getPolicy
    ///////////
    // In ->
    // Out -> pair<Direction, double>
    std::pair<Direction, double> getPolicy() const;
};

///////////////////
/// Grid Object ///
///////////////////
//
// Contains information about "GridWorld"
// and nice ascii printing functionality.
class Grid
{
private:
    // Number of rows and columns respectively
    std::pair<int,int> bounds;
    
    // Cell contents organized in a 2D vector
    std::vector<std::vector<GridCell> > grid;
    
    // starting (row,column) for the agent
    std::pair<int,int> startLocation;
	
public:
    // Constructors
    Grid(int n, int m);
    
    // Accessors to the 2D grid
    GridCell& operator[](const std::pair<int,int> & pos);
    const GridCell& operator[](const std::pair<int,int> & pos) const;
    std::pair<int,int> getBounds() const;
    int getRows() const;
    int getCols() const;
    std::pair<int,int> getStartLocation() const;
	bool isObstacle(std::pair<int,int>) const;
    bool isTerminal(std::pair<int,int>) const;
	std::pair<int,int> moveTo( std::pair<int,int>, Direction );
	bool validDirection(std::pair<int,int>, Direction);
	double getReward(std::pair<int,int>, Direction);
	std::pair<double,Direction> max( std::pair<double, Direction>, std::pair<double,Direction>);
	double updateReward(std::pair<int,int>, Direction);
	void updatePolicy();
    ///////////
    // Function print
    ///////////
    // In -> pair<int,int> agentPos -
    //          position of agent provided. If indicies are negative or out
    //          of bounds. This parameter is ignored.
    // Out ->
    void print(const std::pair<int,int> & agentPos = std::make_pair(-1, -1)) const;
};
