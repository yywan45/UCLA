//
//  WeightMap.hpp
//  homework1
//
//  Created by Yuen Yee Wan on 1/16/16.
//  Copyright © 2016 yywan. All rights reserved.
//

#ifndef WeightMap_hpp
#define WeightMap_hpp

#include <iostream>

#include "Map.h"

class WeightMap
{
public:
    WeightMap();
    bool enroll(std::string name, double startWeight);
    double weight(std::string name) const;
    bool adjustWeight(std::string name, double amt);
    int size() const;  // Return the number of people in the WeightMap.
    void print() const;
    
private:
    Map m_testMap;

};

#endif /* WeightMap_hpp */
