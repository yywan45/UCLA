//
//  testMap.cpp
//  homework1
//
//  Created by Yuen Yee Wan on 1/16/16.
//  Copyright © 2016 yywan. All rights reserved.
//


#include "Map.h"
#include <iostream>
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
    assert(a.get("Bob", value) && value == 15 && !a.get("Chad", value));
    assert(!a.get(1, key, value) && a.get(0, key, value) && key == "Bob" && value == 15);
    
    b.insert("Alice",5);
    b.insert("Belinda", 6);
    b.insert("Carmen", 7);
    b.insert("Diana", 8);
    a.swap(b);
    
    assert(a.size() == 4 && b.size() == 1);
    assert(a.get("Alice", value) && a.get("Carmen", value) && !a.get("Bob", value) && b.get("Bob", value));
    
    
    cout << "All tests passed." << endl;
    
}