//
//  newMap.cpp
//  homework1
//
//  Created by Yuen Yee Wan on 1/16/16.
//  Copyright © 2016 yywan. All rights reserved.
//

#include <iostream>
#include "newMap.h"
using namespace std;


Map::Map()
// Create an empty map (i.e., one with no key/value pairs)
:m_size(0)
{
    m_map = new KeyValuePair[DEFAULT_MAX_ITEMS];
    m_maxSize = DEFAULT_MAX_ITEMS;
}

Map::Map(int size)
:m_size(0)
{
    
    m_map = new KeyValuePair[size];
    m_maxSize = size;
}

Map::Map(const Map &src)
{
    m_size = src.m_size;
    m_maxSize = src.m_maxSize;
    m_map = new KeyValuePair[m_maxSize];
    for (int k = 0; k < m_maxSize; k++)
        m_map[k] = src.m_map[k];
}

Map& Map::operator=(const Map &src) // note syntax
{
    if (&src == this)
        return (*this);
    delete [] m_map;
    m_size = src.m_size;
    m_maxSize = src.m_maxSize;
    m_map = new KeyValuePair[m_maxSize];
    for (int k = 0; k < m_maxSize; k++)
        m_map[k] = src.m_map[k];
    return (*this);
}

Map::~Map()
{
    delete [] m_map; // [] because it is a pointer to an array
}

bool Map::empty() const
// Return true if the map is empty, otherwise false.
{
    return size() == 0;
}

int Map::size() const
// Return the number of key/value pairs in the map.
{
    return m_size;
}

bool Map::insert(const KeyType& key, const ValueType& value)
// If key is not equal to any key currently in the map, and if the
// key/value pair can be added to the map, then do so and return true.
// Otherwise, make no change to the map and return false (indicating
// that either the key is already in the map, or the map has a fixed
// capacity and is full).
{
    if (m_size >= m_maxSize)
        return false;
    
    for (int k = 0; k < m_size; k++)
    {
        if (m_map[k].m_key == key)
            return false;
    }
    m_map[m_size].m_key = key;
    m_map[m_size].m_value = value;
    m_size++;
    return true;
}

bool Map::update(const KeyType& key, const ValueType& value)
// If key is equal to a key currently in the map, then make that key no
// longer map to the value it currently maps to, but instead map to
// the value of the second parameter; return true in this case.
// Otherwise, make no change to the map and return false.
{
    for (int k = 0; k < m_size; k++)
    {
        if (m_map[k].m_key == key)
        {
            m_map[k].m_value = value;
            return true;
        }
    }
    return false;
}


bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
// If key is equal to a key currently in the map, then make that key no
// longer map to the value it currently maps to, but instead map to
// the value of the second parameter; return true in this case.
// If key is not equal to any key currently in the map and if the
// key/value pair can be added to the map, then do so and return true.
// Otherwise, make no change to the map and return false (indicating
// that the key is not already in the map and the map has a fixed
// capacity and is full).
{
    for (int k = 0; k < m_size; k++)
    {
        if (m_map[k].m_key == key)
        {
            m_map[k].m_value = value;
            return true;
        }
    }
    if (m_size < DEFAULT_MAX_ITEMS)
    {
        m_map[m_size].m_key = key;
        m_map[m_size].m_value = value;
        m_size++;
        return true;
    }
    return false;
}

bool Map::erase(const KeyType& key)
// If key is equal to a key currently in the map, remove the key/value
// pair with that key from the map and return true.  Otherwise, make
// no change to the map and return false.
{
    for (int k = 0; k < m_size; k++)
    {
        if (m_map[k].m_key == key)
        {
            m_map[k].m_key = m_map[m_size-1].m_key;
            m_map[k].m_value = m_map[m_size-1].m_value;
            m_size--;
            return true;
        }
    }
    return false;
}

bool Map::contains(const KeyType& key) const
// Return true if key is equal to a key currently in the map, otherwise
// false.
{
    for (int k = 0; k < m_size; k++)
    {
        if (m_map[k].m_key == key)
            return true;
    }
    return false;
}

bool Map::get(const KeyType& key, ValueType& value) const
// If key is equal to a key currently in the map, set value to the
// value in the map that that key maps to, and return true.  Otherwise,
// make no change to the value parameter of this function and return
// false.
{
    for (int k = 0; k < m_size; k++)
    {
        if (m_map[k].m_key == key)
        {
            value = m_map[k].m_value;
            return true;
        }
    }
    return false;
}

bool Map::get(int i, KeyType& key, ValueType& value) const
// If 0 <= i < size(), copy into the key and value parameters the
// key and value of one of the key/value pairs in the map and return
// true.  Otherwise, leave the key and value parameters unchanged and
// return false.  (See below for details about this function.)
{
    if (i >= 0 && i < size())
    {
        key = m_map[i].m_key;
        value = m_map[i].m_value;
        return true;
    }
    return false;
}

void Map::swap(Map& other)
// Exchange the contents of this map with the other one.
{
    KeyValuePair* tempPair;
    int tempSize, tempmaxSize;
    
    tempPair = m_map;
    m_map = other.m_map;
    other.m_map = tempPair;
    
    tempSize = m_size;
    m_size = other.m_size;
    other.m_size = tempSize;
    
    tempmaxSize = m_maxSize;
    m_maxSize = other.m_maxSize;
    other.m_maxSize = tempmaxSize;
    
}
