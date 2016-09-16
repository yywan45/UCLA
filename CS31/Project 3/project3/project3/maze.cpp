//
//  maze.cpp
//  project3
//
//  Created by Yuen Yee Wan on 10/22/15.
//  Copyright © 2015 yywan. All rights reserved.
//

#include <iostream>
#include <cctype>
#include <stdlib.h>
#include <cassert>
#include "grid.h"
using namespace std;


bool isDirection(char direction)
//checks if direction input is valid
{
    if (direction == 'N' || direction == 'n' ||
        direction == 'E' || direction == 'e' ||
        direction == 'S' || direction == 's' ||
        direction == 'W' || direction == 'w')
        return true;
    else
        return false;
}

bool isValidGridPosition(int r, int c)
//checks if grid position input is valid
//return false if position is off the grid or is a wall
{
    if (r > getRows() || r < 0 ||
        c > getCols() || c < 0 ||
        isWall(r,c))
        return false;
    else
        return true;
}

void executeSegment(int& sr, int& sc, char dir, int numSteps)
//moves robot by changing the starting (or current) position by numSteps direction specified
{
    if (dir == 'n' || dir == 'N')
        sr = sr - numSteps;
    if (dir == 'e' || dir == 'E')
        sc = sc + numSteps;
    if (dir == 's' || dir == 'S')
        sr = sr + numSteps;
    if (dir == 'w' || dir == 'W')
        sc = sc - numSteps;
}

// TASK 1 -------------------------------------------------------------------------

bool isRouteWellFormed(string route)
{
    if (route.size() == 0)
        return true;
    //return true for empty string
    
    if (!isDirection(route[0]))
        return false;
    //return false for routes that do not start with a direction letter

    int k;
    for (k = 0; k < route.size(); k++)
    {
        if (!isDirection(route[k]) && !isdigit(route[k]))
            return false;
        
    }
    //goes through each letter in route string to check if all are digits and valid direction letters
    
    for (k = 0; k+2 < route.size(); k++)
    {
        if (isdigit(route[k]) && isdigit(route[k+1]) && isdigit(route[k+2]))
            return false;
    }
    //return false if there are more than 2 digits together in route string
    //k+2 < route.size() in for loop condition to make sure that loop doesn't try to retrieve character beyond route string
    
    return true;
    //return true if route string:
    // 1. starts with a direction letter
    // 2. is an empty string
    // 3. only contains direction letters and digits
    // 4. contains maximum of two digits together
}



// TASK 2 -------------------------------------------------------------------------

int navigateSegment(int r, int c, char dir, int maxSteps)
{
    if (!isValidGridPosition(r,c) || !isDirection(dir) || maxSteps < 0)
        return -1;
    //return -1 if grid position is not valid (refer to isValidGridPosition function)
    
    else
    {
        
        if (dir == 'N' || dir == 'n')
        {
            int steps = 0;
            for (steps = 0; r-1 > 0 && !isWall(r-1,c); r--)
                //condition of loop: next position is in the grid and not a wall
                steps ++; //add step
            if (steps < maxSteps)
                //if possible steps are less than maxSteps, change maxSteps to steps
                maxSteps = steps;
        }
        
        //comments similar for the rest of the directions
        
        if (dir == 'S' || dir == 's')
        {
            int steps = 0;
            for (steps = 0; r+1 <= getRows() && !isWall(r+1,c); r++)
                steps ++;
            if (steps < maxSteps)
                maxSteps = steps;
        }
        
        if (dir == 'W' || dir == 'w')
        {
            int steps = 0;
            for (steps = 0; c-1 > 0 && !isWall(r,c-1); c--)
                steps ++;
            if (steps < maxSteps)
                maxSteps = steps;
        }
        
        if (dir == 'E' || dir == 'e')
        {
            int steps = 0;
            for (steps = 0; c+1 <= getCols() && !isWall(r,c+1); c++)
                steps ++;
            if (steps < maxSteps)
                maxSteps = steps;
        }
    }

    return maxSteps;

    
}


// TASK 3 -------------------------------------------------------------------------

int navigateRoute(int sr, int sc, int er, int ec, string route, int& nsteps)
{

    if (!isValidGridPosition(sr,sc) || !isValidGridPosition(er,ec) || !isRouteWellFormed(route))
        return 2;
    
    nsteps = 0;
    //set nsteps to 0 to start counting steps moved
    //empty string (assuming valid start and end grid positions) will have nsteps set to 0, but will not pass through loop below, so will return 1 with nsteps = 0
    
    int k;
    for (k = 0; k < route.size(); k++)
    {
        if (isalpha(route[k]))
            //every segment begins with a letter, loop keeps checking for new segment
        {
            char dir = route[k];
            if (k == route.size()-1 || (k+1 <= route.size()-1 && isalpha(route[k+1])))
                //if letter k is at the end of the string, or, if letter k is followed by another character, and this character is a letter
                //instruction to take one step
            {
                int steps = 1;
                if (navigateSegment(sr, sc, dir, steps) <= 0)
                    //if return to navigateSegment is -1 or 0 then instruction goes into wall or off the grid
                    //add 0 to nsteps
                    //return 3
                {
                    nsteps += 0;
                    return 3;
                }
                else
                    //navigateSegment returns >0 so instruction was valid, add steps to nsteps
                {
                    executeSegment(sr, sc, dir, steps);
                    nsteps += steps;
                }
            }
            
            if (k+1 <= route.size()-1 && isdigit(route[k+1]))
                //if letter k is followed by another character k+1, and character k+1 is a number
            {
                if (k+2 <= route.size()-1 && isdigit(route[k+2]))
                    //if number k+1 is followed by another character k+2, and character k+2 is a number
                {
                    //instruction to convert string segment of route[k+1] and route[k+2] into integer
                    string strsteps = "";
                    strsteps += route[k+1];
                    strsteps += route[k+2];
                    int steps = atoi(strsteps.c_str());
                
                    if (navigateSegment(sr, sc, dir, steps) < steps)
                        //navigateSegment returns <steps so input steps instruction goes beyond wall/grid
                    {
                        nsteps += navigateSegment(sr, sc, dir, steps);
                        //add maximum number of steps robot can take to nsteps
                        return 3;
                    }
                    else
                        //navigateSegment returns steps so instruction was valid, add steps to nsteps
                    {
                        executeSegment(sr, sc, dir, steps);
                        nsteps += steps;
                    }
                }
                
                if (k+2 > route.size()-1 || (k+2 <= route.size()-1 && isalpha(route[k+2])))
                    //if number k+1 is not followed by another character k+2, or is followed by a letter character k+2
                {
                    string strsteps = "";
                    strsteps += route[k+1];
                    int steps = atoi(strsteps.c_str());
                    //instruction to convert string segment of route[k+1] into integer
                    
                    if (navigateSegment(sr, sc, dir, steps) < steps)
                        //navigateSegment returns <steps so input steps instruction goes beyond wall/grid

                    {
                        nsteps += navigateSegment(sr, sc, dir, steps);
                        //add maximum number of steps robot can take to nsteps
                        return 3;
                    }
                    else
                        //navigateSegment returns steps so instruction was valid, add steps to nsteps
                    {
                        executeSegment(sr, sc, dir, steps);
                        nsteps += steps;
                    }
                }
            }
        }
    }
    
    if (sr == er && sc == ec)
        //if after carrying out route instructions, start position is same as end position
    {
        return 0;
    }
    else
        //if after carrying out route instructions without returning 2 or 3, and does not end up in end position (i.e. return 0)
        return 1;

}


// TESTING -------------------------------------------------------------------------

int main()
{
    setSize(20,20);
    setWall(5,5);
    setWall(10,5);
    setWall(15,5);
    setWall(5,10);
    setWall(10,10);
    setWall(15,10);
    setWall(5,15);
    setWall(10,15);
    setWall(15,15);
    draw(18,3, 3,18);
    
    // TESTING isRouteWellFormed
    
    assert(isRouteWellFormed(""));
    assert(isRouteWellFormed("nNn01EN05n2E2e4eEn4W02w2S1s01e10nNn"));
    assert(isRouteWellFormed("NnNnssnnNnnnNnnNnnNeeeeEwweeeeeeeeEeee"));
    assert(!isRouteWellFormed("N14+e15"));
    assert(!isRouteWellFormed("North14east15"));
    assert(!isRouteWellFormed("N014e15"));
    
    
    // TESTING navigate Segment
    
    assert(navigateSegment(21, 21, 'N', 5) == -1);
    assert(navigateSegment(18, 3, 'x', 5) == -1);
    assert(navigateSegment(18, 3, 'n', 5) == 5);
    assert(navigateSegment(15, 3, 'e', 5) == 1);
    assert(navigateSegment(18, 3, 's', 3) == 2);
    
    
    // TESTING navigateRoute
    
    int len;
    
    len = -999;
    assert(navigateRoute(18,3, 18,3, "", len) == 0  &&  len == 0);
    //check empty string starting in end position
    
    len = -999;
    assert(navigateRoute(18,3, 3,18, "", len) == 1  &&  len == 0);
    //check empty string starting in non-end position
    
    len = -999;
    assert(navigateRoute(18,3, 3,18, "nNn01EN05n2E2e4eEn4W02w2S1s01e10nNn", len) == 0  &&  len == 42);
    //check random route string ending in end position
    //random route contains double digits, single digits, upper and lower case letters, letters stringed together
    //random route ends with letter
    
    len = -999;
    assert(navigateRoute(18,3, 3,18, "nNn01EN05n2E2e4eEn4W02w2S1s01e10nN", len) == 1  &&  len == 41);
    //check random route string ending in non-end position
    //random route contains double digits, single digits, upper and lower case letters, letters stringed together
    //random route ends with letter
    
    len = -999;
    assert(navigateRoute(18,3, 3,18, "NnNnssnnNnnnNnnNnnNeeeeEwweeeeeeeeEeee", len) == 0  &&  len == 38);
    //check random route string with only letters
    //ending in end position
    
    len = -999;
    assert(navigateRoute(18,3, 3,18, "NnNnssnnNnnnNnnNnnNeeeeEwweeeeeeeeEee", len) == 1  &&  len == 37);
    //check random route string with only letters
    //ending in non-end position
    
    len = -999;
    assert(navigateRoute(18,3, 3,18, "N15e15", len) == 0  &&  len == 30);
    //check random route string ending in end position
    //random route contains double digits, single digits, upper and lower case letters, letters stringed together
    //random route ends with number
    
    len = -999;
    assert(navigateRoute(18,3, 3,18, "N14e15", len) == 1  &&  len == 29);
    //check random route string ending in non-end position
    //random route contains double digits, single digits, upper and lower case letters, letters stringed together
    //random route ends with number
    
    len = -999;
    assert(navigateRoute(15,5, 3,18, "", len) == 2  &&  len == -999);
    //start position is wall

    len = -999;
    assert(navigateRoute(18,3, 5,15, "", len) == 2  &&  len == -999);
    //end position is wall
    
    len = -999;
    assert(navigateRoute(21,21, 3,18, "", len) == 2  &&  len == -999);
    //start position is off the grid
    
    len = -999;
    assert(navigateRoute(18,3, 21,21, "", len) == 2  &&  len == -999);
    //end position is off the grid
    
    len = -999;
    assert(navigateRoute(18,3, 3,18, "N14+e15", len) == 2  &&  len == -999);
    //route is not syntactically valid, contains non-letter/number
    
    len = -999;
    assert(navigateRoute(18,3, 3,18, "North14east15", len) == 2  &&  len == -999);
    //route is not syntactically valid, contains non-direction letter
    
    len = -999;
    assert(navigateRoute(18,3, 3,18, "N014e15", len) == 2  &&  len == -999);
    //route is not syntactically valid, contains more than two numbers in a row
    
    len = -999;
    assert(navigateRoute(18,3, 3,18, "N3E3N12E12", len) == 3  &&  len == 4);
    //route moves to cell containing a wall (but without wall would have reached end position)
    
    len = -999;
    assert(navigateRoute(18,3, 3,18, "N3E3", len) == 3  &&  len == 4);
    //route moves to cell containing a wall (but without wall would not have reached end position)
    
    len = -999;
    assert(navigateRoute(18,3, 3,18, "N18", len) == 3  &&  len == 17);
    //route moves to cell off the grid (but with extra row on top would not have reached end position)
    
    len = -999;
    assert(navigateRoute(18,3, 18,3, "N3S3", len) == 0  &&  len == 6);
    //start and end positions are the same
    //route moves away from start/end position and back to start/end position (same as start position)
    
    len = -999;
    assert(navigateRoute(18,3, 18,3, "N3S2", len) == 1  &&  len == 5);
    //start and end positions are the same
    //route moves away from start/end position and does not move back to start/end positionposition)
    
     
    cout << "All tests succeeded" << endl;


}



