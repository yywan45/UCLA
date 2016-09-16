//
//  Robot.hpp
//  project1
//
//  Created by Yuen Yee Wan on 1/6/16.
//  Copyright © 2016 yywan. All rights reserved.
//

#ifndef Robot_hpp
#define Robot_hpp

#include <stdio.h>

class Arena;

class Robot
{
public:
    // Constructor
    Robot(Arena* ap, int r, int c);
    
    // Accessors
    int  row() const;
    int  col() const;
    
    // Mutators
    void move();
    bool getAttacked(int dir);
    
private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    int    m_health;
};

#endif /* Robot_hpp */

