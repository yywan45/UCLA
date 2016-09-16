//
//  tree.cpp
//  homework3
//
//  Created by Yuen Yee Wan on 2/4/16.
//  Copyright © 2016 yywan. All rights reserved.
//

#include <stdio.h>
#include <iostream>
using namespace std;

// Return the number of ways that all n2 elements of a2 appear
// in the n1 element array a1 in the same order (though not
// necessarily consecutively).  The empty sequence appears in a
// sequence of length n1 in 1 way, even if n1 is 0.
// For example, if a1 is the 7 element array
//	10 50 40 20 50 40 30
// then for this value of a2     the function must return
//	10 20 40			1
//	10 40 30			2
//	20 10 40			0
//	50 40 30			3

int countIncludes(const double a1[], int n1, const double a2[], int n2)
{
    if (n2 == 0)
        return 1;
    if (n1 < n2 || n2 < 0 || n1 < 1)
        return 0;
    if (a1[n1-1] == a2[n2-1])
        return countIncludes(a1, n1-1, a2, n2) + countIncludes(a1, n1-1, a2, n2-1);
    else
        return countIncludes(a1, n1-1, a2, n2);
}

// Exchange two doubles
void exchange(double& x, double& y)
{
    double t = x;
    x = y;
    y = t;
}

// Rearrange the elements of the array so that all the elements
// whose value is > splitter come before all the other elements,
// and all the elements whose value is < splitter come after all
// the other elements.  Upon return, firstNotGreater is set to the
// index of the first element in the rearranged array that is
// <= splitter, or n if there is no such element, and firstLess is
// set to the index of the first element that is < splitter, or n
// if there is no such element.
// In other words, upon return from the function, the array is a
// permutation of its original value such that
//   * for 0 <= i < firstNotGreater, a[i] > splitter
//   * for firstNotGreater <= i < firstLess, a[i] == splitter
//   * for firstLess <= i < n, a[i] < splitter
// All the elements > splitter end up in no particular order.
// All the elements < splitter end up in no particular order.
void split(double a[], int n, double splitter, int& firstNotGreater, int& firstLess)
{
    if (n < 0)
        n = 0;
    
    // It will always be the case that just before evaluating the loop
    // condition:
    //  firstNotGreater <= firstUnknown and firstUnknown <= firstLess
    //  Every element earlier than position firstNotGreater is > splitter
    //  Every element from position firstNotGreater to firstUnknown-1 is
    //    == splitter
    //  Every element from firstUnknown to firstLess-1 is not known yet
    //  Every element at position firstLess or later is < splitter
    
    firstNotGreater = 0;
    firstLess = n;
    int firstUnknown = 0;
    while (firstUnknown < firstLess)
    {
        if (a[firstUnknown] < splitter)
        {
            firstLess--;
            exchange(a[firstUnknown], a[firstLess]);
        }
        else
        {
            if (a[firstUnknown] > splitter)
            {
                exchange(a[firstNotGreater], a[firstUnknown]);
                firstNotGreater++;
            }
            firstUnknown++;
        }
    }
}

void order(double a[], int n)
{
    int fNG = 0, fL = n;
    if (n < 2)
        return;
    split(a, fL, a[fNG], fNG, fL);
    order(a, fNG);
    order(a+fL, n-fL);
    return;
}

//int main()
//{
//    double a[7] = {10, 50, 40, 20, 50, 40, 30};
//    double b[3] = {10, 20, 40};
//    double c[3] = {10, 40, 30};
//    double d[3] = {20, 10, 40};
//    double e[3] = {50, 40, 30};
//    
//    double f[8] = {10, 20, 20, 30, 40, 40, 50};
//    double g[4] = {20, 30, 40, 50};
//    
//    cout << countIncludes(a, 1, b, 1) << endl; // 1
//    cout << countIncludes(a, 7, b, 3) << endl; // 1
//    cout << countIncludes(a, 7, c, 2) << endl; // 2
//    cout << countIncludes(a, 7, c, 3) << endl; // 2
//    cout << countIncludes(a, 7, d, 3) << endl; // 0
//    cout << countIncludes(a, 7, e, 3) << endl; // 3
//    
//    cout << countIncludes(f, 8, g, 4) << endl; // 4
//    
//    cout << endl;
//    
//    double ord[7] = {3, 4, 8, 4, 6, 1, 1};
//    int n = 7;
//    order(ord, n);
//    
//    for (int k = 0; k < n; k++)
//        cout << ord[k] << endl;
//    
}