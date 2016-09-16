//
//  main.cpp
//  project4
//
//  Created by Yuen Yee Wan on 10/30/15.
//  Copyright © 2015 yywan. All rights reserved.
//

#include <iostream>
#include <string>
#include <cassert>
using namespace std;

int appendToAll(string a[], int n, string value)
{
    
    if (n >= 0)
    {
        // goes through each element and appends string value to end of each element
        
        int k;
        for (k = 0; k < n; k++)
        {
            a[k] += value;
        }
        
        return n;
    }
    
    
    return -1;
}


int lookup(const string a[], int n, string target)
{
    
    if (n > 0)
    {
        // goes through each element and returns position of element that matches target
        
        int k;
        for (k = 0; k < n; k++)
        {
            if (a[k] == target)
            {
                return k;
            }
        }
    }
    
    
    return -1;

    
}

int positionOfMax(const string a[], int n)
{
    if (n > 0)
    {
        int k;
        int maxPosition = 0;

        for (k = 0; k+1 < n; k++)
            // goes through each element k and compares with next element k+1, if next element k+1 is bigger than element k, compare element k+1 with element maxPosition, if element k+1 is bigger than element maxPosition, make element k+1 element maxPosition
        {
            if (a[k] < a[k+1])
            {
                if (a[maxPosition] < a[k+1])
                {
                    maxPosition = k+1;
                }
            }
        }
        
        return maxPosition;
    }
    
    return -1;
}

int rotateLeft(string a[], int n, int pos)
{
    
    if (n > 0 && pos >= 0)
    {
        int k;
        
        string store = a[pos];
        
        for (k = pos; k+1 < n; k++)
            // replace element with the element next to it, starting with element in position pos (i.e. first replace element pos with element pos+1, etc.)
        {
            a[k] = a[k+1];
        }
        
        a[n-1] = store; // replace last element with stored value of element in position pos
        
        return pos;
    }
    
    return -1;
    
}

int countRuns(const string a[], int n)
{
    if (n == 0)
        return 0; // 0 sequences of consecutive identical items in an empty array
    
    if (n > 0)
    {
        int k;
        int count = 1; // first element is first sequence
        
        for (k = 0; k+1 < n; k++)
            // compare element k with element k+1, if they are not the same, then a new sequence has been started and add one count to number of sequences
        {
            if (a[k] != a[k+1])
            {
                count ++;
            }
        }
        return count;
    }
    
    return -1;
}

int flip(string a[], int n)
{
    if (n == 0)
        return 0; // flipping an empty array will not change the array
    
    if (n > 0)
    {
        int k;
        string store;
        
        for (k = 0; k < n/2; k++)
            // if even number n, starting with first element, swap with last element, then with second element, swap with second last element, etc.
            // if odd number n, since k is int, the middle element will be ignored as condition k < n/2 will not be fulfilled, same instructions carried out as above
        {
            store = a[k];
            a[k] = a[n-k-1];
            a[n-k-1] = store;
        }
        
        return n;
    }
    
    return -1;
    
}


int differ(const string a1[], int n1, const string a2[], int n2)
{
    if (n1 > 0 && n2 > 0)
    {
        int k;
        int count = 0;
        
        for (k = 0; k < n1 && k < n2; k++)
            // compare each element in both arrays
            // if same, add one to count, then loop again to compare next value
            // if different, return count value
        {
            
            if (a1[k] != a2[k])
            {
                return count;
            }
            
            if (a1[k] == a2[k])
            {
                count++;
            }
        }
        return count;
        
        
    }
    return -1;
}

int subsequence(const string a1[], int n1, const string a2[], int n2)
{
    
    if ((n1 == 0 && n2 == 0) || (n2 == 0))
        // 0 elements is a subsequence of any sequence starting at position 0, even of a sequence with no elements
        return 0;
    
    if (n1 > 0 && n2 > 0 && n2 <= n1)
        // array a2 needs to be smaller than array n1 if not array n1 cannot possibly contain array a2
    {
        
        int k;
        int i;
        int count = 0;
        
        for (k = 0; k < n1; k++)
        {
            if (a1[k] == a2[0]) // go through each element in a1 until it matches with element in position 0 of array a2
            {
                for (i = 0; i < n2 && k+i < n1; i++)
                    // compare each element starting from position k in array a1 and position 0 in array a2, and add 1 to count if they are the same
                {
                    if (a1[k+i] == a2[i])
                    {
                        count++;
                    }
                }
                if (count == n2) // if count is the same as n2, it means that there is a subsequence of array a2 in array a1, return k which is position in array a1 where subsequence of array a2 started
                    return k;
            }
        }
    }
    
    return -1;
}

int lookupAny(const string a1[], int n1, const string a2[], int n2)
{
    
    if (n1 > 0 && n2 > 0)
    {
        int k;
        int i;
        
        for (k = 0; k < n1; k++)
            // go through each element in array a1, and compare each element in a1 to all elements in a2
        {
            for (i = 0; i < n2; i++)
            {
                if (a2[i] == a1[k])
                    // if element k in a1 matches with any element in a2, return value k
                {
                    return k;
                }
            }
        }
    }
    return -1;
}

int split(string a[], int n, string splitter)
{
    
    if (n >= 0)
    {
        int k;
        int i;
        int count = 0;
        int splitterPresent = 0;
        string store;
        string notSplitter;
        
        for (k = 0; k < n; k++)
            // find number of elements that come before the splitter
        {
            if (a[k] < splitter)
            {
                count++;
            }
        }
        
        for (k = 0; k < n; k++)
            // find if the splitter is present in the array
        {
            if (a[k] == splitter)
            {
                splitterPresent++;
            }
        }
        
        if (splitterPresent == 0)
            // if splitter is not present, for every element > splitter in position before splitter, find an element < splitter in position after splitter and swap positions, break out of for loop to continue to find next element > splitter in position before splitter
        {
            for (k = 0; k < count; k++)
            {
                if (a[k] > splitter)
                {
                    for (i = count; i < n; i++)
                    {
                        if (a[i] < splitter)
                        {
                            store = a[k];
                            a[k] = a[i];
                            a[i] = store;
                            break;
                        }
                    }
                }
                
            }
        }
        
        if (splitterPresent != 0)
        {
            notSplitter = a[count];
            a[count] = splitter;
            // replace element in splitter position with splitter, store this element as notSplitter
            
            for (k = 0; k < count; k++)
                // go through each element before splitter position and if it is splitter, replace with notSplitter
            {
                if (a[k] == splitter)
                {
                    a[k] = notSplitter;
                }
            }
            
            for (k = count + 1; k < n; k++)
                // go through each element after splitter position and if it is splitter, replace with notSplitter
            {
                if (a[k] == splitter)
                {
                    a[k] = notSplitter;
                }
            }
            
            for (k = 0; k < count; k++)
                // for every element > splitter in position before splitter, find an element < splitter in position after splitter and swap positions, break out of for loop to continue to find next element > splitter in position before splitter
            {
                if (a[k] > splitter)
                {
                    for (i = count + 1; i < n; i++)
                    {
                        if (a[i] < splitter)
                        {
                            store = a[k];
                            a[k] = a[i];
                            a[i] = store;
                            break;
                        }
                    }
                }
                
            }
            
        }
        
        for (k = 0; k < n; k++)
            // goes through each element and reurns position of first element that, after rearrangement using splitter, is not < splitter i.e. is >= splitter
        {
            if (a[k] >= splitter)
            {
                return k;
            }
        }
        
        
        return n; //  if there are no elements >= splitter i.e. if all elements are < splitter
        
        
    }
    
    return -1;
}


int main()
{
    

    
    /*
   
    string a[7] = { "bernie", "hillary", "donald", "jeb", "", "carly", "ben" };

    cout << appendToAll(a, 0, "?") << endl;
    cout << a[0] << endl;
    cout << a[1] << endl;
  
    
    
    string a[7] = { "bob", "hillary", "bernie", "jeb", "", "carly", "ben" };
    string b[7] = { "bernie", "hillary", "bernie", "jeb", "", "carly", "ben" };
    
    cout << lookup(a, 7, "bernie") << endl;
    cout << lookup(b, 7, "bernie") << endl;
    cout << lookup(a, 7, "adam") << endl;

    
    
    
    
    string a[7] = { "bob", "hillary", "bernie", "jeb", "", "carly", "ben" };
    string b[7] = { "bernie", "jeb", "bernie", "jeb", "", "carly", "ben" };
    cout << positionOfMax(a, 7) << endl;
    cout << positionOfMax(b, 7) << endl;
    cout << positionOfMax(a, 3) << endl;
    cout << positionOfMax(a, 0) << endl;
    
     
     
    string a[4] = { "bob", "hillary", "bernie", "jeb" };
    cout << rotateLeft(a, 3, -2) << endl;
    cout << a[0] << endl;
    cout << a[1] << endl;
    cout << a[2] << endl;
    cout << a[3] << endl;
    
    
    string a[7] = { "hillary", "hillary", "hillary", "ben", "ben", "joe", "joe"  };
    cout << flip(a, -2) << endl;
    cout << a[0] << endl;
    cout << a[1] << endl;
    cout << a[2] << endl;
    cout << a[3] << endl;
    cout << a[4] << endl;
    cout << a[5] << endl;
    cout << a[6] << endl;

    
    
    string a[7] = { "bernie", "jeb", "bernie", "bob", "", "carly", "ben" };
    string b[7] = { "bernie", "jeb", "bernie", "jeb", "", "carly", "ben" };
    
    cout << differ(a, 5, b, -2) << endl;
    
    
    
    
    
    string a[7] = { "bernie", "jeb", "bernie", "bob", "", "carly", "ben" };
    string b[7] = { "jeb", "bernie", "bob", "" };
    cout << subsequence(a, 7, b, 4) << endl;
    cout << subsequence(a, 7, b, 2) << endl;
    cout << subsequence(a, 0, b, 0) << endl;
    cout << subsequence(a, 5, b, 0) << endl;
    cout << subsequence(a, 3, b, 4) << endl;
    
    
    
    
    string a[7] = { "adam", "jeb", "bernie", "bob", "", "carly", "ben" };
    string b[3] = { "ben", "carly", "bob" };
    cout << lookupAny(a, 7, b, 3) << endl;
    cout << lookupAny(a, 7, b, 2) << endl;
    cout << lookupAny(a, 3, b, 2) << endl;
    cout << lookupAny(a, -2, b, 3) << endl;
    cout << lookupAny(a, 5, b, -2) << endl;
    

    string a[6] = { "adam", "jeb", "hillary", "bob", "carly", "ben" };
    
    
    cout << split(a, -1, "aarl") << endl;
    cout << a[0] << ", " << a[1] << ", " << a[2] << ", " << a[3] << ", " << a[4] << ", " << a[5] << endl;

    
    cout << split(a, 6, "aarl") << endl;
    cout << a[0] << ", " << a[1] << ", " << a[2] << ", " << a[3] << ", " << a[4] << ", " << a[5] << endl;
    
    cout << split(a, 6, "zarl") << endl;
    cout << a[0] << ", " << a[1] << ", " << a[2] << ", " << a[3] << ", " << a[4] << ", " << a[5] << endl;
    
    cout << split(a, 6, "carl") << endl;
    cout << a[0] << ", " << a[1] << ", " << a[2] << ", " << a[3] << ", " << a[4] << ", " << a[5] << endl;
    
    cout << split(a, 6, "carly") << endl;
    cout << a[0] << ", " << a[1] << ", " << a[2] << ", " << a[3] << ", " << a[4] << ", " << a[5] << endl;
    
    
    
    
    string h[7] = { "bernie", "hillary", "donald", "jeb", "", "carly", "ben" };
    assert(lookup(h, 7, "carly") == 5);
    assert(lookup(h, 7, "donald") == 2);
    assert(lookup(h, 2, "donald") == -1);
    assert(positionOfMax(h, 7) == 3);
    
    string g[4] = { "bernie", "hillary", "jeb", "carly" };
    assert(differ(h, 4, g, 4) == 2);
    assert(appendToAll(g, 4, "?") == 4 && g[0] == "bernie?" && g[3] == "carly?");
    assert(rotateLeft(g, 4, 1) == 1 && g[1] == "jeb?" && g[3] == "hillary?");
    
    string e[4] = { "donald", "jeb", "", "carly" };
    assert(subsequence(h, 7, e, 4) == 2);
    
    string d[5] = { "hillary", "hillary", "hillary", "ben", "ben" };
    assert(countRuns(d, 5) == 2);
    
    string f[3] = { "jeb", "donald", "marco" };
    assert(lookupAny(h, 7, f, 3) == 2);
    assert(flip(f, 3) == 3 && f[0] == "marco" && f[2] == "jeb");
    
    assert(split(h, 7, "carly") == 3);
    
    cout << "All tests succeeded" << endl;
    
    
    
    
    
   
    
    string c[7] = { "bernie", "hillary", "donald", "jeb", "", "carly", "ben" };
    string b[7] = { "bernie", "hillary", "donald", "jeb", "", "carly", "ben" };
    assert(differ(c, 2, b, 2) == 2);
    
    
    
    string x[5] = { "hillary", "ben", "hillary", "jackie", "ben" };
    assert(countRuns(x, 5) == 5);

    
   
    cout << split(h, 7, "carly") << endl;
    cout << h[0] << endl;
    cout << h[1] << endl;
    cout << h[2] << endl;
    cout << h[3] << endl;
    cout << h[4] << endl;
    cout << h[5] << endl;
    cout << h[6] << endl;

    
    string can[6] = { "bernie", "hillary", "donald", "marco", "carly", "ben" };
    cout << split(cand, 6, "chris") << endl;  //  returns 3
    
    cout << can[0] << endl;
    cout << can[1] << endl;
    cout << can[2] << endl;
    cout << can[3] << endl;
    cout << can[4] << endl;
    cout << can[5] << endl;
    
     
     
    
    string cand[6] = { "bernie", "hillary", "donald", "marco", "carly", "ben" };
    cout << split(cand, 6, "chris") << endl;  //  returns 3
    
    cout << cand[0] << endl;
    cout << cand[1] << endl;
    cout << cand[2] << endl;
    cout << cand[3] << endl;
    cout << cand[4] << endl;
    cout << cand[5] << endl;
    
    // cand must now be
    //      "bernie"  "carly"  "ben"  "hillary"  "marco"  "donald"
    // or   "carly"  "ben"  "bernie"  "donald"  "hillary"  "marco"
    // or one of several other orderings.
    // All elements < "chris" (i.e., "carly", "ben", and "bernie")
    //   come before all others
    // All elements > "chris" (i.e., "marco", "hillary", and "donald")
    //   come after all others
    
    string cand2[4] = { "donald", "hillary", "jeb", "ben" };
    cout << split(cand2, 4, "donald") << endl;  //  returns 1
    
    cout << cand2[0] << endl;
    cout << cand2[1] << endl;
    cout << cand2[2] << endl;
    cout << cand2[3] << endl;
    
    // cand2 must now be either
    //      "ben"  "donald"  "hillary"  "jeb"
    // or   "ben"  "donald"  "jeb"  "hillary"
    // All elements < "donald" (i.e., "ben") come before all others.
    // All elements > "donald" (i.e., "jeb" and "hillary") come after all
    // others.
    
    string cand3[4] = { "donald", "hillary", "jeb", "ben" };
    cout << split(cand3, 4, "zbacus") << endl;  //  returns 4
    
    cout << cand3[0] << endl;
    cout << cand3[1] << endl;
    cout << cand3[2] << endl;
    cout << cand3[3] << endl;
    
    
    
    
    cout << "All tests succeeded" << endl;
    
    */
    
    string h[7] = { "bernie", "hillary", "donald", "jeb", "", "carly", "ben" };
    assert(lookup(h, 7, "carly") == 5);
    assert(lookup(h, 7, "donald") == 2);
    assert(lookup(h, 2, "donald") == -1);
    assert(positionOfMax(h, 7) == 3);
    
    string g[4] = { "bernie", "hillary", "jeb", "carly" };
    assert(differ(h, 4, g, 4) == 2);
    assert(appendToAll(g, 4, "?") == 4 && g[0] == "bernie?" && g[3] == "carly?");
    assert(rotateLeft(g, 4, 1) == 1 && g[1] == "jeb?" && g[3] == "hillary?");
    
    string e[4] = { "donald", "jeb", "", "carly" };
    assert(subsequence(h, 7, e, 4) == 2);
    
    string d[5] = { "hillary", "hillary", "hillary", "ben", "ben" };
    assert(countRuns(d, 5) == 2);
    
    string f[3] = { "jeb", "donald", "marco" };
    assert(lookupAny(h, 7, f, 3) == 2);
    assert(flip(f, 3) == 3 && f[0] == "marco" && f[2] == "jeb");
    
    assert(split(h, 7, "carly") == 3);
    
    cout << "All tests succeeded" << endl;
    
    
}

