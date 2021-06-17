

#define ROW 256
#define COLUMN 256
#include "Entity.h"
//#include "utils.h"

// A C++ Program to implement A* Search Algorithm
//#include <bits/stdc++.h>
using namespace std;

bool punto_visible(Matrix44 actual, Vector3 destino);
Vector3 new_move(std::vector<Vector3> auxiliar_points, Vector3 actual_position, Vector3 destino, int jugador, bool lado);


// Creating a shortcut for int, int pair type
typedef pair<int, int> Pair;
 
// Creating a shortcut for pair<int, pair<int, int>> type
typedef pair<double, pair<int, int> > pPair;
 
// A structure to hold the neccesary parameters
/*struct cell {
    // Row and Column index of its parent
    // Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1
    int parent_i, parent_j;
    // f = g + h
    double f, g, h;
};*/

class cell {
public:
    // Row and Column index of its parent
    // Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1
    int parent_i, parent_j;
    // f = g + h
    double f, g, h;
    cell(){
    };
    

};

 
// A Utility Function to check whether given cell (row, col)
// is a valid cell or not.
bool isValid(int row, int col);
 
// A Utility Function to check whether the given cell is
// blocked or not
bool isUnBlocked(int grid[ROW][COLUMN], int row, int col);
 
// A Utility Function to check whether destination cell has
// been reached or not
bool isDestination(int row, int col, Vector3 dest);
 
// A Utility Function to calculate the 'h' heuristics.
double calculateHValue(int row, int col, Vector3 dest);
 
// A Utility Function to trace the path from the source
// to destination
vector<Vector3> tracePath(std::vector<std::vector<cell>> cellDetails, Vector3 dest);






vector<Vector3> aStarSearch(int **grid, Vector3 src, Vector3 dest);

 
 
 

