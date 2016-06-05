#include "mazesolver.h"
#include "mazedisplay.h"
#include "visitedtracker.h"
#include <QMessageBox>
#include <queue>
#include <stack>
#include <vector>
#include "heap/heap.h"
#include <cmath>


MazeSolver::MazeSolver(Maze * m, MazeDisplay * md)
    : maze(m), display(md)
{}



void MazeSolver::solveByDFSRecursive()
{
    int numSquares = maze->numRows() * maze->numCols();
    VisitedTracker vt(maze->numRows(), maze->numCols());

    std::vector<Direction> parent( numSquares );
    int numExplored = 0;
    solveByDFSRecursiveHelper(vt, parent, numExplored, maze->getStartRow(), maze->getStartCol());

    // report
    std::vector<Direction> path;
    std::stack<Direction> st;

    int r = maze->getGoalRow(), c = maze->getGoalCol();
    while( r != maze->getStartRow() || c != maze->getStartCol()) {
        st.push( parent[ squareNumber(r,c) ]);
        switch( st.top() )
        {
        case UP: r++; break; // yes, r++.  I went up to get here...
        case DOWN: r--; break;
        case LEFT: c++; break;
        case RIGHT: c--; break;
        }
    }
    while ( ! st.empty() ) {
        path.push_back(st.top());
        st.pop();
    }
    display->reportSolution(path, vt, numExplored);
}

bool MazeSolver::solveByDFSRecursiveHelper(VisitedTracker& vt, std::vector<Direction>& parent, 
        int& numExplored, int row, int col) {

    if ( row == maze->getGoalRow() && col == maze->getGoalCol() ) return true;
    if ( vt.isVisited(row, col) ) return false;
    vt.setVisited(row, col);
    numExplored++;
    if ( maze->canTravel(UP, row, col) && ! vt.isVisited(row - 1, col)) {
        parent[ squareNumber(row - 1, col) ] = UP;
        if (solveByDFSRecursiveHelper(vt, parent, numExplored, row - 1, col))
            return true;
    }
    if ( maze->canTravel(DOWN, row, col) && ! vt.isVisited(row + 1, col)) {
        parent[ squareNumber(row + 1, col) ] = DOWN;
        if (solveByDFSRecursiveHelper(vt, parent, numExplored, row + 1, col))
            return true;
    }
    if ( maze->canTravel(LEFT, row, col) && ! vt.isVisited(row, col - 1)) {
        parent[ squareNumber(row, col - 1) ] = LEFT;
        if (solveByDFSRecursiveHelper(vt, parent, numExplored, row, col - 1))
            return true;
    }
    if ( maze->canTravel(RIGHT, row, col) && ! vt.isVisited(row, col + 1)) {
        parent[ squareNumber(row, col + 1) ] = RIGHT;
        if (solveByDFSRecursiveHelper(vt, parent, numExplored, row, col + 1))
            return true;
    }
    return false;
}

int MazeSolver::heuristic(int choice, int row, int col) {
    int ex = maze->getGoalRow() - row, ey = maze->getGoalCol() - col;
    switch (choice) {
        case 1:
            return 0;
        case 2:           
            return sqrt( ex * ex + ey * ey );
        case 3:
            return ex + ey;
        default:
            return 0;
    }
}

void MazeSolver::solveByAStar(int choice)
{
    // TODO:
    //    if choice is 1, solve by A* using heuristic of "return 0"
    //    else if choice is 2, solve by A* using heuristic of Manhattan Distance
    //    else if choice is 3, solve by A* using heuristic of Euclidean Distance

    // else completely up to you.

    int r, c;
    int numSquares = maze->numRows() * maze->numCols();
    int d[ numSquares ];
    VisitedTracker vt(maze->numRows(), maze->numCols());
    std::vector<Direction> parent( numSquares );
    int numExplored = 0;

    MinHeap<std::pair<int, int> > mh(2); // tentative 2
    d[ squareNumber(maze->getStartRow(), maze->getStartCol()) ] = 0;
    mh.add(std::pair<int,int>(maze->getStartRow(), maze->getStartCol()), 0);

    while ( !mh.isEmpty() ) {
        std::pair<int, int> v = mh.peek();
        mh.remove();
        numExplored++;

        r = v.first;
        c = v.second;
        if ( vt.isVisited(r, c) ) continue;
        vt.setVisited(r, c);

        // if reach goal, report
        if( r == maze->getGoalRow() && c == maze->getGoalCol() )
        {
            std::vector<Direction> path;
            std::stack<Direction> st;

            while( r != maze->getStartRow() || c != maze->getStartCol())
            {
                st.push( parent[ squareNumber(r,c) ]);
                switch( st.top() )
                {
                case UP: r++; break; // yes, r++.  I went up to get here...
                case DOWN: r--; break;
                case LEFT: c++; break;
                case RIGHT: c--; break;
                }
            }
            while ( ! st.empty() )
            {
                path.push_back(st.top());
                st.pop();
            }
            display->reportSolution(path, vt, numExplored);
            return;
        }

        if ( maze->canTravel(UP, r, c) ) {
            if (!vt.isVisited(r - 1, c) || d[ squareNumber(r, c) ] + 1 < d[ squareNumber(r - 1, c) ] ) {
                d[ squareNumber(r - 1, c) ] = d[ squareNumber(r, c) ] + 1;
                parent[squareNumber(r - 1, c)] = UP;
                mh.add(std::pair<int,int>(r - 1, c), d[ squareNumber(r - 1, c) ] + heuristic(choice, r, c));
            }
        }
        if ( maze->canTravel(DOWN, r, c) ) {
            if (!vt.isVisited(r + 1, c) || d[ squareNumber(r, c) ] + 1 < d[ squareNumber(r + 1, c) ] ) {
                d[ squareNumber(r + 1, c) ] = d[ squareNumber(r, c) ] + 1;
                parent[squareNumber(r + 1, c)] = DOWN;
                mh.add(std::pair<int,int>(r + 1, c), d[ squareNumber(r + 1, c) ] + heuristic(choice, r, c));
            }
        }
        if ( maze->canTravel(LEFT, r, c) ) {
            if (!vt.isVisited(r, c - 1) || d[ squareNumber(r, c) ] + 1 < d[ squareNumber(r, c - 1) ] ) {
                d[ squareNumber(r, c - 1) ] = d[ squareNumber(r, c) ] + 1;
                parent[squareNumber(r, c - 1)] = LEFT;
                mh.add(std::pair<int,int>(r, c - 1), d[ squareNumber(r, c - 1) ] + heuristic(choice, r, c));
            }
        }
        if ( maze->canTravel(RIGHT, r, c) ) {
            if (!vt.isVisited(r, c + 1) || d[ squareNumber(r, c) ] + 1 < d[ squareNumber(r, c + 1) ] ) {
                d[ squareNumber(r, c + 1) ] = d[ squareNumber(r, c) ] + 1;
                parent[squareNumber(r, c + 1)] = RIGHT;
                mh.add(std::pair<int,int>(r, c + 1), d[ squareNumber(r, c + 1) ] + heuristic(choice, r, c));
            }
        }

    }

}




void MazeSolver::solveByDFSIterative()
{
    int r, c;
    int numSquares = maze->numRows() * maze->numCols();
    VisitedTracker vt(maze->numRows(), maze->numCols());

    std::vector<Direction> parent( numSquares ); // what was my immediate prior direction to get here?
    int numExplored = 0;
    vt.setVisited(maze->getStartRow(), maze->getStartCol());
    std::stack<std::pair<int, int>> q;
    q.push(std::pair<int,int>(maze->getStartRow(), maze->getStartCol()));

    while( ! q.empty() )
    {
        std::pair<int, int> v = q.top();
        q.pop();
        numExplored++;

        r = v.first;
        c = v.second;

        /* This one if statement is different from the pseudo-code provided
           in lecture, because we want to stop when we've reached the goal.
           The code provided in lecture was for if you wanted to do a BFS
           that explored the entire graph.
        */
        if( r == maze->getGoalRow() && c == maze->getGoalCol() )
        {
            std::vector<Direction> path;
            std::stack<Direction> st;

            while( r != maze->getStartRow() || c != maze->getStartCol())
            {
                st.push( parent[ squareNumber(r,c) ]);
                switch( st.top() )
                {
                case UP: r++; break; // yes, r++.  I went up to get here...
                case DOWN: r--; break;
                case LEFT: c++; break;
                case RIGHT: c--; break;
                }
            }
            while ( ! st.empty() )
            {
                path.push_back(st.top());
                st.pop();
            }
            display->reportSolution(path, vt, numExplored);
            return;
        }

        /*
         * Now we're back to code that looks like the pseudo-code you've seen.
         * The difference here is that we aren't keeping track of distances;
           that's similar to the difference above.  You could add, and ignore,
           the vector that would result if you wanted to do so.
         */
        if ( maze->canTravel(UP, r, c) && ! vt.isVisited(r-1,c))
        {
            parent[squareNumber(r-1, c)] = UP;
            vt.setVisited(r-1,c);
            q.push(std::pair<int,int>(r-1, c));
        }
        // Note:  this is NOT "else if" ...
        if ( maze->canTravel(DOWN, r, c) && ! vt.isVisited(r+1,c) )
        {
            parent[squareNumber(r+1, c)] = DOWN;
            vt.setVisited(r+1, c);
            q.push(std::pair<int,int>(r+1, c));
        }
        if ( maze->canTravel(LEFT, r, c) && ! vt.isVisited(r,c-1) )
        {
            parent[squareNumber(r, c-1)] = LEFT;
            vt.setVisited(r, c-1);
            q.push(std::pair<int,int>(r, c-1));
        }
        if ( maze->canTravel(RIGHT, r, c) && ! vt.isVisited(r, c+1) )
        {
            parent[squareNumber(r, c+1)] = RIGHT;
            vt.setVisited(r, c+1);
            q.push(std::pair<int,int>(r, c+1));
        }
    }
}

void MazeSolver::solveByBFS()
{
    /* In lecture on Tuesday March 22, we had a graph
       with vertices numbered 0 to n-1, inclusive.
       Instead, we have vertices with two numbers,
       row and col, in the range:
       [0, maze->numRows()-1], [0, maze->numCols() -1 ]
       to assign each a unique number [0, maze->numRows() * maze->numCols() -1]
       we will say that maze square (r,c) is really number
       r * maze->numCols() + c
    */
    int r, c;
    int numSquares = maze->numRows() * maze->numCols();
    VisitedTracker vt(maze->numRows(), maze->numCols());

    std::vector<Direction> parent( numSquares ); // what was my immediate prior direction to get here?
    int numExplored = 0;
    vt.setVisited(maze->getStartRow(), maze->getStartCol());
    std::queue<std::pair<int, int>> q;
    q.push(std::pair<int,int>(maze->getStartRow(), maze->getStartCol()));

    while( ! q.empty() )
    {
        std::pair<int, int> v = q.front();
        q.pop();
        numExplored++;

        r = v.first;
        c = v.second;

        /* This one if statement is different from the pseudo-code provided
           in lecture, because we want to stop when we've reached the goal.
           The code provided in lecture was for if you wanted to do a BFS
           that explored the entire graph.
        */
        if( r == maze->getGoalRow() && c == maze->getGoalCol() )
        {
            std::vector<Direction> path;
            std::stack<Direction> st;

            while( r != maze->getStartRow() || c != maze->getStartCol())
            {
                st.push( parent[ squareNumber(r,c) ]);
                switch( st.top() )
                {
                case UP: r++; break; // yes, r++.  I went up to get here...
                case DOWN: r--; break;
                case LEFT: c++; break;
                case RIGHT: c--; break;
                }
            }
            while ( ! st.empty() )
            {
                path.push_back(st.top());
                st.pop();
            }
            display->reportSolution(path, vt, numExplored);
            return;
        }

        /*
         * Now we're back to code that looks like the pseudo-code you've seen.
         * The difference here is that we aren't keeping track of distances;
           that's similar to the difference above.  You could add, and ignore,
           the vector that would result if you wanted to do so.
         */
        if ( maze->canTravel(UP, r, c) && ! vt.isVisited(r-1,c))
        {
            parent[squareNumber(r-1, c)] = UP;
            vt.setVisited(r-1,c);
            q.push(std::pair<int,int>(r-1, c));
        }
        // Note:  this is NOT "else if" ...
        if ( maze->canTravel(DOWN, r, c) && ! vt.isVisited(r+1,c) )
        {
            parent[squareNumber(r+1, c)] = DOWN;
            vt.setVisited(r+1, c);
            q.push(std::pair<int,int>(r+1, c));
        }
        if ( maze->canTravel(LEFT, r, c) && ! vt.isVisited(r,c-1) )
        {
            parent[squareNumber(r, c-1)] = LEFT;
            vt.setVisited(r, c-1);
            q.push(std::pair<int,int>(r, c-1));
        }
        if ( maze->canTravel(RIGHT, r, c) && ! vt.isVisited(r, c+1) )
        {
            parent[squareNumber(r, c+1)] = RIGHT;
            vt.setVisited(r, c+1);
            q.push(std::pair<int,int>(r, c+1));
        }
    }
}


int MazeSolver::squareNumber(int r, int c) const
{
    return maze->numCols() * r + c;
}


