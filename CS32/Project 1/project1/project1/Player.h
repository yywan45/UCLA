//
//  Player.hpp
//  project1
//
//  Created by Yuen Yee Wan on 1/6/16.
//  Copyright © 2016 yywan. All rights reserved.
//

#ifndef Player_hpp
#define Player_hpp

#include <stdio.h>

class Arena;

class Player
{
public:
    // Constructor
    Player(Arena *ap, int r, int c);
    
    // Accessors
    int  row() const;
    int  col() const;
    int  age() const;
    bool isDead() const;
    
    // Mutators
    void   stand();
    void   moveOrAttack(int dir);
    void   setDead();
    
private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    int    m_age;
    bool   m_dead;
};

#endif /* Player_hpp */
