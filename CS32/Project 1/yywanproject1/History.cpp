//
//  History.cpp
//  project1
//
//  Created by Yuen Yee Wan on 1/6/16.
//  Copyright © 2016 yywan. All rights reserved.
//

#include "History.h"
#include "Robot.h"
#include "Arena.h"
#include <iostream>
using namespace std;

//The constructor initializes a History object that corresponds to an Arena with nRows rows and nCols columns. You may assume (i.e., you do not have to check) that it will be called with a first argument that does not exceed MAXROWS and a second that does not exceed MAXCOLS, and that neither argument will be less than 1.

History::History(int nRows, int nCols)
{
    m_rows = nRows;
    m_cols = nCols;
    
    int r, c;
    for (r = 0; r < m_rows; r++)
        for (c = 0; c < m_cols; c++)
            grid[r][c] = '.';
    
}

bool History::record(int r, int c)
{
    if (r < 1 || c < 1 || r > m_rows || c > m_cols)
        return false;
    
    if (grid[r-1][c-1] == '.')
    {
        grid[r-1][c-1] = 'A';
        return true;
    }
    
    if (grid[r-1][c-1] >= 'A' && grid[r][c] < 'Z')
        grid[r-1][c-1]++;
    
    return true;
    
}

void History::display() const
{
    clearScreen();
    
    int r, c;
    
    for (r = 0; r < m_rows; r++)
    {
        for (c = 0; c < m_cols; c++)
        {
            cout << grid[r][c];
        }
        cout << endl;
    }
}