//
//  testWeightMap.cpp
//  homework1
//
//  Created by Yuen Yee Wan on 1/16/16.
//  Copyright © 2016 yywan. All rights reserved.
//


#include "WeightMap.h"
#include <cassert>
using namespace std;

int main()
{
    WeightMap wm;
    assert(wm.enroll("Adam", 50) && !wm.enroll("Bob", -50) && !wm.enroll("Adam", 40));
    assert(wm.weight("Adam") == 50 && wm.weight("Bob") == -1);
    assert(!wm.adjustWeight("Bob", 5) && !wm.adjustWeight("Adam", -200) && wm.adjustWeight("Adam", -3));
    assert(wm.size() == 1);
    wm.enroll("Bob", 60);
    assert(wm.size() == 2);
    wm.print();
    
    cout << "All tests passed." << endl;
}

