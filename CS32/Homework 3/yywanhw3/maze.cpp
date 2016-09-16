//
//  maze.cpp
//  homework3
//
//  Created by Yuen Yee Wan on 2/4/16.
//  Copyright © 2016 yywan. All rights reserved.
//

#include <stdio.h>
#include <iostream>
using namespace std;


class Coord
{
public:
    Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
    int r() const { return m_r; }
    int c() const { return m_c; }
private:
    int m_r;
    int m_c;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{

    Coord start(sr,sc);
    Coord current(sr, sc);
    maze[sr][sc] = 'O';

    if (current.r() == er && current.c() == ec)
        return true;
    
    if (current.r()-1 >= 0 && maze[current.r()-1][current.c()] != 'O' && maze[current.r()-1][current.c()] != 'X') // NORTH
    {
        if(pathExists(maze, nRows, nCols, current.r()-1, current.c(), er, ec))
            return true;
    }
    
    if (current.c()+1 < nCols && maze[current.r()][current.c()+1] != 'O' && maze[current.r()][current.c()+1] != 'X') // EAST
    {
        if(pathExists(maze, nRows, nCols, current.r(), current.c()+1, er, ec))
            return true;
    }
    
    if (current.r()+1 < nRows && maze[current.r()+1][current.c()] != 'O' && maze[current.r()+1][current.c()] != 'X') // SOUTH
    {
        if(pathExists(maze, nRows, nCols, current.r()+1, current.c(), er, ec))
            return true;
    }
    
    if (current.c()-1 >= 0 && maze[current.r()][current.c()-1] != 'O' && maze[current.r()][current.c()-1] != 'X') // WEST
    {
        if(pathExists(maze, nRows, nCols, current.r(), current.c()-1, er, ec))
            return true;
    }
    return false;
}

//
//int main()
//{
//    string maze[10] = {
//        "XXXXXXXXXX",
//        "X........X",
//        "XX.X.XXXXX",
//        "X..X.X...X",
//        "X..X...X.X",
//        "XXXX.XXX.X",
//        "X.X....XXX",
//        "X..XX.XX.X",
//        "X...X....X",
//        "XXXXXXXXXX"
//    };
//    
//    if (pathExists(maze, 10,10, 6,4, 1,1))
//        cout << "Solvable!" << endl;
//    else
//        cout << "Out of luck!" << endl;
//}
