//
//  Map.h
//  project2
//
//  Created by Yuen Yee Wan on 1/21/16.
//  Copyright © 2016 yywan. All rights reserved.
//

#ifndef Map_h
#define Map_h

#include <stdio.h>
#include <iostream>

typedef std::string KeyType;
typedef double ValueType;

class Map
{
public:
    Map();
    ~Map();
    bool empty() const;
    Map(const Map &src);
    Map &operator=(const Map &src);
    int size() const;
    bool insert(const KeyType& key, const ValueType& value);
    bool update(const KeyType& key, const ValueType& value);
    bool insertOrUpdate(const KeyType& key, const ValueType& value);
    bool erase(const KeyType& key);
    bool contains(const KeyType& key) const;
    bool get(const KeyType& key, ValueType& value) const;
    bool get(int i, KeyType& key, ValueType& value) const;
    void swap(Map& other);
    
    void dump() const;
    
private:
    struct KeyValuePair
    {
        KeyType m_key;
        ValueType m_value;
        KeyValuePair* next;
        KeyValuePair* prev;
        
    };
    KeyValuePair* head;
    KeyValuePair* tail;
    int m_size;
    KeyValuePair* findKey(const KeyType& key) const;

};

bool combine(const Map& m1, const Map& m2, Map& result);
void subtract(const Map& m1, const Map& m2, Map& result);

#endif /* Map_h */



