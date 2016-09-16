//
//  newMap.hpp
//  homework1
//
//  Created by Yuen Yee Wan on 1/16/16.
//  Copyright © 2016 yywan. All rights reserved.
//

#ifndef newMap_hpp
#define newMap_hpp

#include <stdio.h>
#include <iostream>

typedef std::string KeyType;
typedef double ValueType;

const int DEFAULT_MAX_ITEMS = 200;

class Map
{
public:
    Map();
    Map(int size);
    Map(const Map &src);
    Map &operator=(const Map &src);
    bool empty() const;
    int size() const;
    bool insert(const KeyType& key, const ValueType& value);
    bool update(const KeyType& key, const ValueType& value);
    bool insertOrUpdate(const KeyType& key, const ValueType& value);
    bool erase(const KeyType& key);
    bool contains(const KeyType& key) const;
    bool get(const KeyType& key, ValueType& value) const;
    bool get(int i, KeyType& key, ValueType& value) const;
    void swap(Map& other);
    ~Map();
private:
    struct KeyValuePair
    {
        KeyType m_key;
        ValueType m_value;
    };
    KeyValuePair* m_map;
    int m_size;
    int m_maxSize;
    
};


#endif /*newMap_hpp*/
