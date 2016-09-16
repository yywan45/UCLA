//
//  royalty.cpp
//  project2
//
//  Created by Yuen Yee Wan on 10/7/15.
//  Copyright © 2015 yywan. All rights reserved.
//

#include <iostream>
using namespace std;

int main()
{
    
    // this segment sets out the data input questions about the movie
    
    cout << "Units sent: ";
    int units;
    cin >> units;
    cin.ignore(10000, '\n');
    
    cout << "Title: ";
    string title;
    getline(cin, title);
    
    cout << "Base price: ";
    double price;
    cin >> price;
    cin.ignore(10000, '\n');
    
    cout << "Premium item? (y/n): ";
    string premium;
    getline(cin, premium);
    
    cout << "---" << endl;

    
    // this segment calculates the royalty amount through a series of if/else conditions
    
    double royalty;
    if (units <= 400)
        royalty = units * price * 0.09;
    else
    {
        if (units <= 1200)
        {
            if (premium == "n")
                royalty = (400 * price * 0.09) + ((units - 400) * price * 0.13);
            else
                royalty = (400 * price * 0.09) + ((units - 400) * price * 0.16);
        }
        else
        {
            if (units > 1200)
            {
                if (premium == "n")
                    royalty = (400 * price * 0.09) + (800 * price * 0.13) + ((units - 1200) * price * 0.14);
                else
                    royalty = (400 * price * 0.09) + (800 * price * 0.16) + ((units - 1200) * price * 0.14);
            }
        }
    }
    
    
    // this segment is for the final line after the three hyphens
    
    if (units < 0)
        cout << "The number of units sent must be nonnegative." << endl;
    else
    {
        if (title == "")
            cout << "You must enter a title." << endl;
        else
        {
            if (price < 0)
                cout << "The base price must be nonnegative." << endl;
            else
            {
                if (premium == "")
                cout << "You must enter y or n." << endl;
                else
                {
                    cout.setf(ios::fixed);
                    cout.precision(2);
                    cout << title << " earned $" << royalty << " in royalties." << endl;
                }}}}

    
}

