//
//  main.cpp
//  project3helper
//
//  Created by Yuen Yee Wan on 2/20/16.
//  Copyright © 2016 yywan. All rights reserved.
//

#include <iostream>
#include "time.h"
#include <algorithm>
#include <string>
using namespace std;

//int main()
//{
//    srand(time(NULL));
//    int x = rand() % 50 + 1;
//    int y = rand() % 50 + 1;
//    int z = rand() % 50 + 1;
//    int a = rand() % 50 + 1;
//    
//    cout << rand() % 50 + 1 << endl;
//    cout << rand() % 50 + 1 << endl;
//    cout << y << endl;
//    cout << z << endl;
//    cout << a << endl;
//    
//}


//int main()
//{
//    int n;
//    time_t t;
//    
//    n = 5;
//    
//    /* Intializes random number generator */
//    
//    srand((unsigned) time(&t));
//    
//    /* Print 5 random numbers from 0 to 49 */
//    
//    cout << rand() % 4 << endl;
//    cout << rand() % 4 << endl;
//    cout << rand() % 4 << endl;
//    cout << rand() % 4 << endl;
//    cout << rand() % 4 << endl;
//    cout << rand() % 4 << endl;
//    cout << rand() % 4 << endl;
//    cout << rand() % 4 << endl;
//    
//    // 0123 for 4
//    
//    

int countNMoves(string path)
{
    int nMoves = 1;
    
    // calculate number of moves and append to end
    for (int i = 0; i < path.length()-1; i++)
    {
        if (path[i] != path[i+1])
            nMoves++;
    }
    return nMoves;
}


int main()
{
//    int level = 0;
//
//    time_t t;
//    srand((unsigned) time(&t));
//
//    // Boulders
//    for (int B = min(level / 2 + 2 , 6); B > 0; B--)
//    {
//        int x = rand() % 64;
//        int y = rand() % 60;
//        
//        while (x >= 30 && x <= 33)
//        {
//            x = rand() % 64;
//        }
//        
//        while (y >= 4 && y <= 59)
//        {
//            y = rand() % 60;
//        }
//        cout << x << endl << y << endl;
//    }
//
    
    
    cout << countNMoves("LLUUDDDRRRRLLL");


    

}




