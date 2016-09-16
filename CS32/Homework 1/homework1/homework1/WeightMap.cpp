//
//  WeightMap.cpp
//  homework1
//
//  Created by Yuen Yee Wan on 1/16/16.
//  Copyright © 2016 yywan. All rights reserved.
//

#include "WeightMap.h"
#include <iostream>
using namespace std;

WeightMap::WeightMap()
// Create an empty weight map.
{
    //cannot call a constructor in another constructor
}

bool WeightMap::enroll(std::string name, double startWeight)
// If a person with the given name is not currently in the map,
// there is room in the map, and the startWeight is not negative,
// add an entry for that person and weight and return true.
// Otherwise make no change to the map and return false.
{
    if (startWeight > 0)
        return m_testMap.insert(name, startWeight);
    return false;
}

double WeightMap::weight(std::string name) const
// If a person with the given name is in the map, return that
// person's weight; otherwise, return -1.
{
    double weight;
    
    if (m_testMap.get(name, weight))
        return weight;
    return -1;
}

bool WeightMap::adjustWeight(std::string name, double amt)
// If no person with the given name is in the map or if
// weight() + amt is negative, make no change to the map and return
// false.  Otherwise, change the weight of the indicated person by
// the given amount and return true.  For example, if amt is -8.2,
// the person loses 8.2 pounds; if it's 3.7, the person gains 3.7
// pounds.
{
    double weight;
    if (m_testMap.get(name, weight))
    {
        if (weight + amt < 0)
            return false;
        m_testMap.update(name, weight + amt);
        return true;
    }
    return false;
}

int WeightMap::size() const
// Return the number of people in the WeightMap.
{
    return m_testMap.size();
}

void WeightMap::print() const
// Write to cout one line for every person in the map.  Each line
// has the person's name, followed by one space, followed by that
// person's weight.
{
    string name;
    double weight;
    for (int k = 0; k < size(); k++)
    {
        if (m_testMap.get(k, name, weight))
            cout << name << " " << weight << endl;
    }
}
