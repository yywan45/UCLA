//
//  History.hpp
//  project1
//
//  Created by Yuen Yee Wan on 1/6/16.
//  Copyright © 2016 yywan. All rights reserved.
//

#ifndef History_hpp
#define History_hpp

#include <stdio.h>
#include "globals.h"

class Arena;

class History
{
public:
    History(int nRows, int nCols);
    bool record(int r, int c);
    void display() const;
    
private:
    int     m_rows;
    int     m_cols;
    char     grid[MAXROWS][MAXCOLS];
};

#endif /* History_hpp */
