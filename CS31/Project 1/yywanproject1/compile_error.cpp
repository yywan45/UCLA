//
//  compile_error.cpp
//  project1compileerror
//
//  Created by Yuen Yee Wan on 10/5/15.
//  Copyright © 2015 yywan. All rights reserved.
//

// Code for Project 1
// Report survey results

#include <iostream>

int main()
{
    
    cout << "How many diesel Volkswagen owners were surveyed? ";
    cin >> numberSurveyed;
    cout << "How many of them say they will take a payment and keep their car? ";
    cin >> numKeep;
    cout << "How many of them say they will return their car for a refund? ";
    cin >> numRefund;
    
    double pctKeep = 100.0 * numKeep / numberSurveyed;
    double pctRefund = 100.0 * numRefund / numberSurveyed;
    
    cout.setf(ios::fixed);   // see pp. 32-33 in Savitch 6/e
    cout.precision(1);
    
    cout << endl;
    cout << pctKeep << "% say they will keep their car." << endl;
    cout << pctRefund << "% say they will demand a refund." << endl;
    
    if (numKeep > numRefund)
        cout << "More people will keep their car than demand a refund." << endl
    else
        cout << "More people will demand a refund than keep their car." << endl;
}