//
//  testnewMap.cpp
//  homework1
//
//  Created by Yuen Yee Wan on 1/16/16.
//  Copyright © 2016 yywan. All rights reserved.
//

#include "newMap.h"
#include <cassert>
using namespace std;


int main()
{

    ValueType value = 50;
    KeyType key = "before";
 
    Map a, b;
 
    assert(a.insert("Adam", 10));
    assert(!a.empty() && b.empty());
    assert(a.size() == 1);
    assert(!a.insert("Adam",5));
    assert(a.update("Adam", 5) && !a.update("Bob", 10));
    assert(a.insertOrUpdate("Adam", 10) && a.insertOrUpdate("Bob", 15));
    assert(a.size() == 2);
    assert(a.erase("Adam") && !a.erase("Chad"));
    assert(a.size() == 1);
    assert(a.contains("Bob") && !a.contains("Adam"));
    assert(a.get("Bob", value) && !a.get("Chad", value));
    //assert(!a.get(1, key, value) && a.get(0, key, value));
    
    b.insert("Alice",5);
    b.insert("Belinda", 6);
    b.insert("Carmen", 7);
    b.insert("Diana", 8);
    
    a.swap(b);
    assert(a.size() == 4 && b.size() == 1);
    assert(a.get("Alice", value) && a.get("Carmen", value) && !a.get("Bob", value) && b.get("Bob", value));
     
    
    Map c(3);
    
    assert(c.insert("A", 1) && c.insert("B", 2) && c.insert("C", 3) && !c.insert("D", 4));
    assert(c.erase("C"));
    assert(c.insert("D", 4));
    
    cout << "All tests passed." << endl;
    
}
