//
//  linear.cpp
//  homework3
//
//  Created by Yuen Yee Wan on 2/4/16.
//  Copyright © 2016 yywan. All rights reserved.
//

#include <iostream>
#include <cassert>
using namespace std;

bool somePredicate(double x)
{
    return x < 0;
}

// Return true if the somePredicate function returns true for at
// least one of the array elements, false otherwise.
bool anyTrue(const double a[], int n)
{
    if (n < 1)
        return false;
    if (somePredicate(a[n-1]))
        return true;
    return anyTrue(a, n-1);

}

int countTrue(const double a[], int n)
{
//    int keyCount;
//    if (n < 1)
//        return 0;
//    if (somePredicate(a[0]))
//        keyCount = 1;
//    else
//        keyCount = 0;
//    return keyCount + countTrue(a+1, n-1);
    
    if (n < 1)
        return 0;
    if (somePredicate(a[n-1]))
        return 1 + countTrue(a, n-1);
    return 0 + countTrue(a, n-1);
    
}

// Return the subscript of the first element in the array for which
// the somePredicate function returns true.  If there is no such
// element, return -1.
int firstTrue(const double a[], int n)
{
    if (n < 1)
        return -1;
    if (n == 1)
    {
        if (somePredicate(a[0]))
            return 0;
        if (!somePredicate(a[0]))
            return -1;
    }
    if (somePredicate(a[n-1]))
    {
        if (firstTrue(a, n-1) < n-1 && firstTrue(a, n-1) >= 0)
            return firstTrue(a, n-1);
        else
            return n-1;
    }
    return firstTrue(a, n-1);
}

// Return the subscript of the smallest element in the array (i.e.,
// the one whose value is <= the value of all elements).  If more
// than one element has the same smallest value, return the smallest
// subscript of such an element.  If the array has no elements to
// examine, return -1.
int indexOfMin(const double a[], int n)
{
    if (n < 1)
        return -1;
    if (n == 1)
        return 0;
    if (a[n-1] < a[0])
    {
        if (a[indexOfMin(a, n-1)] <= a[n-1] && indexOfMin(a, n-1) >= 0)
            return indexOfMin(a, n-1);
        else
            return n-1;
    }
    return indexOfMin(a, n-1);
}

//int fib(int n)
//{
//    if (n == 0 || n == 1)
//        return 1;
//    else
//        return fib(n-1) + fib(n-2);
//}

// If all n2 elements of a2 appear in the n1 element array a1, in
// the same order (though not necessarily consecutively), then
// return true; otherwise (i.e., if the array a1 does not include
// a2 as a not-necessarily-contiguous subsequence), return false.
// (Of course, if a2 is empty (i.e., n2 is 0), return true.)
// For example, if a1 is the 7 element array
//    10 50 40 20 50 40 30
// then the function should return true if a2 is
//    50 20 30
// or
//    50 40 40
// and it should return false if a2 is
//    50 30 20
// or
//    10 20 20

bool includes(const double a1[], int n1, const double a2[], int n2)
{
    if (n2 == 0)
        return true;
    if (n2 < 0)
        return false;
    if (n1 < 1)
        return false;
    if (a1[n1-1] == a2[n2-1])
        return includes(a1, n1-1, a2, n2-1);
    return includes(a1, n1-1, a2, n2);
}

//int main()
//{
//    
//    double b[7] = {20, 50, 40, 20, 50, 40, 30};
//    double c1[3] = {20, 50, 30};
//    
//    double c2[3] = {50, 20, 40};
//    double c3[3] = {50, 30, 20};
//    double c4[3] = {20, 20, 50};
//    
//    //cout << includes(b, 1, c1, 1);
//    assert(includes(b, 2, c1, 2));
//    assert(includes(b, 7, c1, 3));
//    assert(!includes(b, 2, c2, 2));
//    
//    assert(includes(b, 7, c2, 3));
//    assert(!includes(b, 7, c3, 3));
//    assert(includes(b, 7, c4, 3));
//    
//    
//    double a[5] = {3, 2, -7, 7, -4};
//    
//    assert(anyTrue(a, 3));
//    assert(countTrue(a, 1) == 0);
//    assert(countTrue(a, 2) == 0);
//    assert(countTrue(a, 3) == 1);
//    assert(countTrue(a, 4) == 1);
//    assert(countTrue(a, 5) == 2);
//    
//    assert(firstTrue(a, 1) == -1);
//    assert(firstTrue(a, 2) == -1);
//    assert(firstTrue(a, 3) == 2);
//    assert(firstTrue(a, 4) == 2);
//    assert(firstTrue(a, 5) == 2);
//    
//    assert(indexOfMin(a, 1) == 0);
//    assert(indexOfMin(a, 2) == 1);
//    assert(indexOfMin(a, 3) == 2);
//    assert(indexOfMin(a, 4) == 2);
//    assert(indexOfMin(a, 5) == 2);
////    
////    cout << fib(0) << endl;
////    cout << fib(1) << endl;
////    cout << fib(2) << endl;
////    cout << fib(3) << endl;
////    cout << fib(4) << endl;
//    
//    cerr << "cool beans" << endl;
//    
//}

