//
//  main.cpp
//  project6
//
//  Created by Yuen Yee Wan on 11/23/15.
//  Copyright © 2015 yywan. All rights reserved.
//

#include <iostream>
using namespace std;

/* 1a

int main()
{
    int arr[3] = { 5, 10, 15 };
    int* ptr = arr;
    
    *ptr = 30;          // set arr[0] to 30
    *(ptr + 1) = 20;      // set arr[1] to 20
    ptr += 2;
    ptr[0] = 10;        // set arr[2] to 10
    
    ptr = arr; // set ptr to 0

    while (ptr <= (arr+2))
    {
        cout << *ptr << endl;
        ptr++;
    }
}
 
 */

/* 1b
 
 

void findMax(int arr[], int n, int*& pToMax)
{
    if (n <= 0)
        return;      // no items, no maximum!
    
    pToMax = arr;
    
    for (int i = 1; i < n; i++)
    {
        if (arr[i] > *pToMax)
            pToMax = arr + i;
    }
}

int main()
{
    int nums[4] = { 5, 3, 15, 6 };
    int* ptr;
    
    findMax(nums, 4, ptr);
    cout << "The maximum is at address " << ptr << endl;
    cout << "It's at position " << ptr - nums << endl;
    cout << "Its value is " << *ptr << endl;
}

*/

/* 1c
 
 

void computeCube(int n, int* ncubed)
{
    *ncubed = n * n * n;
    cout << *ncubed << endl;
}

int main()
{
    int x;
    int* ptr;
    *ptr = x;
    computeCube(5, ptr);
    //cout << "Five cubed is " << *ptr << endl;
}

 */

/* 1d
 
 
// return true if two C strings are equal
bool strequal(const char str1[], const char str2[])
{
    while (*str1 != 0  &&  *str2 != 0)
    {
        if (*str1 != *str2)  // compare corresponding characters
            return false;
        str1++;            // advance to the next character
        str2++;
    }
    return *str1 == *str2;   // both ended at same time?
}

int main()
{
    char a[15] = "Noor";
    char b[15] = "Noor";
    
    if (strequal(a,b))
        cout << "They're the same person!\n";
}
 */

/* 1e
 
#include <iostream>
using namespace std;

int* getPtrToArray(int& m)
{
    int anArray[100];
    for (int j = 0; j < 100; j++)
        anArray[j] = 100-j;
    m = 100;
    return anArray;
}

void f()
{
    int junk[100];
    for (int k = 0; k < 100; k++)
        junk[k] = 123400000 + k;
}

int main()
{
    int n;
    int* ptr = getPtrToArray(n);
    f();
    for (int i = 0; i < 3; i++)
        cout << ptr[i] << ' ';
    for (int i = n-3; i < n; i++)
        cout << ptr[i] << ' ';
    cout << endl;
}
*/
 
 
/*

double computeAverage(const double* scores, int nScores)
{
    int k = 0;
    double tot = 0;
    while (k != nScores)
    {
        tot += *(scores + k);
        k++;
    }
    return tot/nScores;
}

const char* findTheChar(const char* str, char chr)
{
    for (int k = 0; *(str+k) != 0; k++)
        if (*(str+k) == chr)
            return str + k;
    
    return nullptr;
}


const char* findTheChar(const char* str, char chr)
{
    for ( ; *str != 0; str++)
        if (*str == chr)
            return str;
    
    return nullptr;
}

*/

/* 5
 void removeS(char* source)
{
    char* destination = source;
    for ( ; *source != '\0'; source++)
    {
        if (*source != 's'  &&  *source != 'S')
        {
            *destination = *source;
            destination++;
        }
    }
    *destination = '\0';  // Don't forget the zero byte at the end
}



void removeS(char* source)
{
    char* destination = source;
    while (*source != '\0')
    {
        if (*source != 's'  &&  *source != 'S')
        {
            *destination = *source;
            destination++;
        }
        source++;
    }
    *destination = '\0';  // Don't forget the zero byte at the end
}
    
 



int main()
{
    char msg[50] = "She'll be a massless princess.";
    removeS(msg);
    cout << msg;  // prints   he'll be a male prince.
}

*/


#include <iostream>
using namespace std;

int* maxwell(int* a, int* b)
{
    if (*a > *b)
        return a;
    else
        return b;
}

void swap1(int* a, int* b)
{
    int* temp = a;
    a = b;
    b = temp;
}

void swap2(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main()
{
    int array[6] = { 5, 3, 4, 17, 22, 19 };
    
    int* ptr = maxwell(array, &array[2]);
    *ptr = -1;
    ptr += 2;
    ptr[1] = 9;
    *(array+1) = 79;
    
    cout << &array[5] - ptr << endl;
    
    swap1(&array[0], &array[1]);
    swap2(array, &array[2]);
    
    for (int i = 0; i < 6; i++)
        cout << array[i] << endl;
}



