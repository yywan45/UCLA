//
//  main.cpp
//  hw test
//
//  Created by Yuen Yee Wan on 10/15/15.
//  Copyright © 2015 yywan. All rights reserved.
//

#include <iostream>
using namespace std;

int main()
{
    int side;
    
    cout << "Enter a number: ";
    cin >> side;
    
    for (int i = 0; i < side; i++)
    {
        int j = i;
        while (j >= 0)
        {
            cout << "#";
            j--;
        }
        cout << "\n";
    }
}
