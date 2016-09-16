//
//  Map.h
//  homework1
//
//  Created by Yuen Yee Wan on 1/16/16.
//  Copyright © 2016 yywan. All rights reserved.
//

#ifndef Map_h
#define Map_h

#include <stdio.h>
#include <iostream>

typedef std::string KeyType;
typedef double ValueType;

const int DEFAULT_MAX_ITEMS = 200;

class Map
{
public:
    Map();
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
private:
    struct KeyValuePair
    {
        KeyType m_key;
        ValueType m_value;
    };
    KeyValuePair m_map[DEFAULT_MAX_ITEMS];
    int m_size;

};

#endif /* Map_h */



