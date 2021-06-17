
#include <utility> 
#define ROW 256
#define COLUMN 256
#include "IA.h"
#include "utils.h"
#include "game.h"

// A C++ Program to implement A* Search Algorithm
//#include <bits/stdc++.h>
 

#include <iostream>
#include <iomanip>
#include <math.h>
#include <ctime>
#include <string>
#include <queue>







bool punto_visible(Matrix44 actual, Vector3 destino){
	
	Vector3 target_direction_2 = (destino - actual.getTranslation());
	float ang =dot(actual.frontVector(), target_direction_2.normalize());

    bool visible = true;
	float dist1= sqrt( pow(abs(actual.getTranslation().x-destino.x)  ,2) + pow( abs(actual.getTranslation().z-destino.z) ,2) );
	for(int i=0;i<Game::instance->wall2_model.size(); i++){
		Vector3 target_direction_3 = (Game::instance->wall2_model[i].getTranslation() - actual.getTranslation());
		float ang2 =dot(actual.frontVector(), target_direction_3.normalize());
		if(ang + 0.005 >=ang2 && ang - 0.005 <= ang2){

           
			float dist2= sqrt( pow(abs(actual.getTranslation().x-Game::instance->wall2_model[i].getTranslation().x ) ,2) + pow( abs(actual.getTranslation().z-Game::instance->wall2_model[i].getTranslation().z) ,2) );
            if (dist1 > dist2) {
              
                visible = false;
                break;
            }
           /*else {
                std::cout << "entraaaaaaaaaaaaaaaaaaagggggggggggggggggggggggaaaaaaaaaaaaaa2222222222222222" << std::endl;
            }*/
			
		
		}
	
	}
	return visible;
		
}
	





Vector3 new_move(std::vector<Vector3> auxiliar_points, Vector3 actual_position,Vector3 destino, int jugador,bool lado){

	int auxiliar_i=0;
	int distancia=-1;

    //std::cout << "estamos en new_move" << std::endl;
    if (lado) {
        if (auxiliar_points.size() > 0) {
            if (abs(actual_position.x - auxiliar_points[Game::instance->terror_auxiliar[jugador]].x) < 5 && abs(actual_position.z - auxiliar_points[Game::instance->terror_auxiliar[jugador]].z) < 5) {
                if (Game::instance->terror_auxiliar[jugador] < (auxiliar_points.size() - 2)) {
                    Game::instance->terror_auxiliar[jugador]++;
                }




            }

            return auxiliar_points[Game::instance->terror_auxiliar[jugador]];
        }
      

    }
    else {
        if (abs(actual_position.x - auxiliar_points[Game::instance->ct_auxiliar[jugador]].x) < 5 && abs(actual_position.z - auxiliar_points[Game::instance->ct_auxiliar[jugador]].z) < 5) {
            if (Game::instance->ct_auxiliar[jugador] < auxiliar_points.size() - 2) {
                Game::instance->ct_auxiliar[jugador]++;
            }



        }

        return auxiliar_points[Game::instance->ct_auxiliar[jugador]];


    }
   
  

}









// Creating a shortcut for int, int pair type
 
// A Utility Function to check whether given cell (row, col)
// is a valid cell or not.
bool isValid(int row, int col)
{
    // Returns true if row number and column number
    // is in range
    return (row >= 0) && (row < ROW) && (col >= 0)
           && (col < COLUMN);
}
 
// A Utility Function to check whether the given cell is
// blocked or not
bool isUnBlocked(int **grid, int row, int col)
{
    
    // Returns true if the cell is not blocked else false
    if (grid[row][col] == 1)
        return (true);
    else
        return (false);
}
 
// A Utility Function to check whether destination cell has
// been reached or not
bool isDestination(int row, int col, Vector3 dest)
{
    if (row == dest[0] && col == dest[2])
        return (true);
    else
        return (false);
}
 
// A Utility Function to calculate the 'h' heuristics.
double calculateHValue(int row, int col, Vector3 dest)
{
    // Return using the distance formula
    return ((double)sqrt(
        (row - dest[0]) * (row - dest[0])
        + (col - dest[2]) * (col - dest[2])));
}
 
// A Utility Function to trace the path from the source
// to destination
std::vector<Vector3> tracePath(std::vector<std::vector<cell>> cellDetails, Vector3 dest)
{
    //printf("\nThe Path is ");
    int row = dest[0];
    int col = dest[2];
 
    std::vector<Vector3> Path;
    //printf("\njjjjjjjjjjj ");
    while (!(cellDetails[row][col].parent_i == row
             && cellDetails[row][col].parent_j == col && row==-1 && col==-1) ) {
        
        Path.push_back(Vector3(row,0, col));
        /*std::cout << "blocked" << std::endl;
        std::cout << row << std::endl;
        std::cout << col << std::endl;
        std::cout << cellDetails[row][col].parent_i << std::endl;
        std::cout << cellDetails[row][col].parent_j << std::endl;
        */
        //int temp_row = cellDetails[row][col].parent_i;
        //int temp_col = cellDetails[row][col].parent_j;
        row = cellDetails[row][col].parent_i;
        col = cellDetails[row][col].parent_j;
        //printf("\nzzzzzzzzz ");
       /* std::cout << row << std::endl;
        std::cout << col << std::endl;
        */
    }
 
    //printf("\naaaaaaaaaaaaaa ");
    Path.push_back(Vector3(row,0, col));
    std::vector<Vector3> Path_aux;
    for(int i=Path.size()-1;i==0;i--){
    	Path_aux.push_back(Path[i]);
    
    
    }
    //printf("\nbbbbbbbbbbbbb ");
    /*while (!Path.empty()) {
        pair<int, int> p = Path.top();
        Path.pop();
        printf("-> (%d,%d) ", p.first, p.second);
    }*/
 
    return Path_aux;
}


using namespace std;

const int IDIM = ROW; // horizontal size of the squares
const int JDIM = ROW; // vertical size size of the squares
const int NDIR = 4; // number of possible directions to go at any position

// if NDIR = 4
const int iDir[NDIR] = { 1, 0, -1, 0 };
const int jDir[NDIR] = { 0, 1, 0, -1 };

// if NDIR = 8
//const int iDir[NDIR] = {1, 1, 0, -1, -1, -1, 0, 1};
//const int jDir[NDIR] = {0, 1, 1, 1, 0, -1, -1, -1};

int squares[IDIM][JDIM];

// list of closed (check-out) nodes
int closedNodes[IDIM][JDIM];

// list of open (not-yet-checked-out) nodes
int openNodes[IDIM][JDIM];

// map of directions (0: East, 1: North, 2: West, 3: South) 
int dirMap[IDIM][JDIM];

struct Location
{
    int row, col;

    Location()
    {
        row = col = 0;
    };

    Location(int r, int c)
    {
        row = r;
        col = c;
    };
};

class Node
{
    // current position
    int rPos;
    int cPos;

    // total distance already travelled to reach the node
    int GValue;

    // FValue = GValue + remaining distance estimate
    int FValue;  // smaller FValue gets priority

public:
    Node(const Location& loc , int g, int f)
    {
        rPos = loc.row; cPos = loc.col; GValue = g; FValue = f;
    }

    Location getLocation() const { return Location(rPos, cPos); }
    int getGValue() const { return GValue; }
    int getFValue() const { return FValue; }

    void calculateFValue(const Location& locDest)
    {
        FValue = GValue + getHValue(locDest) * 10;
    }


    void updateGValue(const int& i) // i: direction
    {
        GValue += (NDIR == 8 ? (i % 2 == 0 ? 10 : 14) : 10);
    }

    // Estimation function for the remaining distance to the goal.
    const int& getHValue(const Location& locDest) const
    {
        static int rd, cd, d;
        rd = locDest.row - rPos;
        cd = locDest.col - cPos;

        // Euclidian Distance
        // d = static_cast<int>(sqrt((double)(rd*rd+cd*cd)));

        // Manhattan distance
        d = abs(rd) + abs(cd);

        // Chebyshev distance
        //d = max(abs(rd), abs(cd));

        return(d);
    }

    // Determine FValue (in the priority queue)
    friend bool operator<(const Node& a, const Node& b)
    {
        return a.getFValue() > b.getFValue();
    }
};

// A-star algorithm.
// The path returned is a string of direction digits.
std::vector<Vector3> pathFind(const Location& locStart,
    const Location& locFinish )

{
    // list of open (not-yet-checked-out) nodes
    static priority_queue<Node> q[2];

    // q index
    static int qi;

    static Node* pNode1;
    static Node* pNode2;
    static int i, j, row, col, iNext, jNext;
    static char c;
    qi = 0;
    
    std::vector<Vector3> short_path;
	
    // reset the Node lists (0 = ".")
    for (j = 0; j < JDIM; j++) {
        for (i = 0; i < IDIM; i++) {
            closedNodes[i][j] = 0;
            openNodes[i][j] = 0;
        }
    }

    // create the start node and push into list of open nodes
    pNode1 = new Node(locStart, 0, 0);
    pNode1->calculateFValue(locFinish);
    q[qi].push(*pNode1);

    // A* search
    while (!q[qi].empty()) {
        // get the current node w/ the lowest FValue
        // from the list of open nodes
        pNode1 = new Node(q[qi].top().getLocation(),
            q[qi].top().getGValue(), q[qi].top().getFValue());

        row = (pNode1->getLocation()).row;
        col = pNode1->getLocation().col;
        //short_path.push_back(Vector3(row- 128,0,col- 128));
        //cout << "row, col=" << row << "," << col << endl;

        // remove the node from the open list
        q[qi].pop();
        openNodes[row][col] = 0;

        // mark it on the closed nodes list
        closedNodes[row][col] = 1;

        // stop searching when the goal state is reached
        if (row == locFinish.row && col == locFinish.col) {
          /*  // drawing direction map
            cout << endl;
            for (j = JDIM - 1; j >= 0; j--) {
                for (i = 0; i < IDIM; i++) {
                    cout << dirMap[i][j];
                }
                cout << endl;
            }
            cout << endl;*/

            // generate the path from finish to start from dirMap
            string path = "";
            while (!(row == locStart.row && col == locStart.col)) {
                j = dirMap[row][col];
                c = '0' + (j + NDIR / 2) % NDIR;
                path = c + path;
                short_path.push_back(Vector3(row - 128, 0, col - 128));
                row += iDir[j];
                col += jDir[j];
            }

            // garbage collection
            delete pNode1;

            // empty the leftover nodes
            while (!q[qi].empty()) q[qi].pop();
            std::vector<Vector3> short_path_aux1;
            for (int i = 0; i < short_path.size(); i++) {
                short_path_aux1.push_back(short_path[short_path.size()-i]);

            }
            return short_path_aux1;
        }

        // generate moves in all possible directions
        for (i = 0; i < NDIR; i++) {
            iNext = row + iDir[i];
            jNext = col + jDir[i];

            // if not wall (obstacle) nor in the closed list
            if (!(iNext < 0 || iNext > IDIM - 1 || jNext < 0 || jNext > JDIM - 1 ||
                squares[iNext][jNext] == 1 || closedNodes[iNext][jNext] == 1)) {

                // generate a child node
                pNode2 = new Node(Location(iNext, jNext), pNode1->getGValue(), pNode1->getFValue());
                pNode2->updateGValue(i);
                pNode2->calculateFValue(locFinish);

                // if it is not in the open list then add into that
                if (openNodes[iNext][jNext] == 0) {
                    openNodes[iNext][jNext] = pNode2->getFValue();
                    q[qi].push(*pNode2);
                    // mark its parent node direction
                    dirMap[iNext][jNext] = (i + NDIR / 2) % NDIR;
                }

                // already in the open list
                else if (openNodes[iNext][jNext] > pNode2->getFValue()) {
                    // update the FValue info
                    openNodes[iNext][jNext] = pNode2->getFValue();

                    // update the parent direction info,  mark its parent node direction
                    dirMap[iNext][jNext] = (i + NDIR / 2) % NDIR;

                    // replace the node by emptying one q to the other one
                    // except the node to be replaced will be ignored
                    // and the new node will be pushed in instead
                    while (!(q[qi].top().getLocation().row == iNext &&
                        q[qi].top().getLocation().col == jNext)) {
                        q[1 - qi].push(q[qi].top());
                        q[qi].pop();
                    }

                    // remove the wanted node
                    q[qi].pop();

                    // empty the larger size q to the smaller one
                    if (q[qi].size() > q[1 - qi].size()) qi = 1 - qi;
                    while (!q[qi].empty()) {
                        q[1 - qi].push(q[qi].top());
                        q[qi].pop();
                    }
                    qi = 1 - qi;

                    // add the better node instead
                    q[qi].push(*pNode2);
                }
                else delete pNode2;
            }
        }
        delete pNode1;
    }
    // no path found

    std::vector<Vector3> short_path_aux;
    for (int i = 0; i < short_path.size(); i++) {
        short_path_aux.push_back(short_path[short_path.size() -1 - i]);

    }



    return  short_path_aux;
}


// A Function to find the shortest path between
// a given source cell to a destination cell according
// to A* Search Algorithm
std::vector<Vector3> aStarSearch(int** grid, Vector3 src, Vector3 dest)
{
    // create empty squares
    for (int j = 0; j < JDIM; j++) {
        for (int i = 0; i < IDIM; i++) squares[i][j] = grid[i][j];
    }

    

    // make wall
    //squares[4][2] = 1;
    //squares[4][3] = 1;
    //squares[4][4] = 1;

    // starting and ending positions
    int iStart = src[0], jStart = src[2];
    int iEnd = dest[0], jEnd = dest[2];

    //cout << "Grid Size (IDIM,JDIM): " << IDIM << "," << JDIM << endl;
    //cout << "Start: " << iStart << "," << jStart << endl;
    //cout << "Finish: " << iEnd << "," << jEnd << endl;

    clock_t start = clock();

    // get the path
    std::vector<Vector3> path = pathFind(Location(iStart, jStart), Location(iEnd, jEnd));

    clock_t end = clock();
    double time = double(end - start);
    //cout << "Time (ms): " << time << endl;
    //cout << "path: " << path << endl;

    // follow the path on the squares and display it 
   
        
    
   
    
    
    return path;
}
 
 
 

