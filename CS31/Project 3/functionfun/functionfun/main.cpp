//
//  main.cpp
//  functionfun
//
//  Created by Yuen Yee Wan on 10/27/15.
//  Copyright © 2015 yywan. All rights reserved.
//

#include <iostream>
using namespace std;

// Q4

/*int factorial(int n)
{
    int prod = 1;
    for (int i = 2; i <= n; i++)
        prod *= i;
    return prod;
}

int main()
{
    for (int k = 1; k <= 6; k++)
        cout << "The factorial of " << k << " is " << factorial(k) << endl;
}*/


// Q5

/*void printFactorial(int n)
{
    int prod = 1;
    for (int i = 2; i <= n; i++)
        prod *= i;
    cout << "The factorial of " << n << " is " << prod << endl;
}

int main()
{
    for (int k = 1; k <= 6; k++)
        printFactorial(k);
}*/

string justLetters(string s)
{
    string result = "";
    for (int k = 0; k != s.size(); k++)
    {
        if ( islower(s[k]) )
            result += s[k];
        if ( isupper(s[k]) )
            result += tolower(s[k]);
    }
    return result;
}

int main()
{
    cout << justLetters("CS 31 started September 25") << endl;
}

