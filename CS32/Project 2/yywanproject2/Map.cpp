//
//  main.cpp
//  project2
//
//  Created by Yuen Yee Wan on 1/21/16.
//  Copyright © 2016 yywan. All rights reserved.
//

#include <iostream>
#include "Map.h"
using namespace std;

Map::Map()
:m_size(0)
{
    head = nullptr;
    tail = nullptr;
}

Map::~Map()
{
    KeyValuePair *temp1;
    temp1 = head;
    
    while (temp1 != nullptr)
    {
        KeyValuePair *temp2 = temp1->next;
        delete temp1;
        temp1 = temp2;
    }
}

bool Map::empty() const
{
    return size() == 0;
}

Map::Map(const Map &src)
{
    head = nullptr;
    tail = nullptr;
    m_size = 0;
    KeyValuePair *temp = src.tail;
    
    while (temp != nullptr)
    {
        insert(temp->m_key, temp->m_value);
        temp = temp->prev;
    }
}

Map& Map::operator=(const Map &src) // note syntax
{
    if (&src == this)
        return (*this);
    
    KeyType key;
    ValueType value;
    int mapSize = size();
    
    for (int i = 0; i < mapSize; i++) // empty original map
    {
        get(0, key, value);
        erase(key);
    }
    
    KeyValuePair *temp = src.tail;
    m_size = 0;
    while (temp != nullptr)
    {
        insert(temp->m_key, temp->m_value);
        temp = temp->prev;
    }
    
    return *this;
    
}

int Map::size() const
{
    return m_size;
}

Map::KeyValuePair* Map::findKey(const KeyType& key) const
// return pointer to KeyValuePair struct containing key
{
    KeyValuePair *temp;
    temp = head;
    while (temp != nullptr)
    {
        if (temp->m_key == key)
            return temp;
        temp = temp->next;
    }
    return nullptr;
}

bool Map::insert(const KeyType& key, const ValueType& value)
{
    if (m_size == 0) // when adding item to empty map
    {
        KeyValuePair *p;
        p = new KeyValuePair;
        p->m_key = key;
        p->m_value = value;
        p->next = nullptr;
        p->prev = nullptr;
        head = p;
        tail = p;
        m_size++;
        return true;
    }
    if (!contains(key)) // when adding item to non-empty map
    {
        KeyValuePair *p;
        p = new KeyValuePair;
        p->m_key = key;
        p->m_value = value;
        p->next = head;
        p->prev = nullptr;
        head->prev = p;
        head = p;
        m_size++;
        return true;
    }
    return false;
}

bool Map::update(const KeyType& key, const ValueType& value)
{
    if (contains(key))
    {
        findKey(key)->m_value = value;
        return true;
    }
    return false;
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
{
    if (contains(key)) //  if key is present, update value
    {
        findKey(key)->m_value = value;
        return true;
    }
    return insert(key, value); // if key is not present, use insert function
}

bool Map::erase(const KeyType& key)
{
    if (contains(key)) // if map contains key
    {
        if (size() == 1) // erase key if it is the only item in map
        {
            KeyValuePair *temp = head;
            head = nullptr;
            tail = nullptr;
            delete temp;
            m_size--;
            return true;
        }
        if (head->m_key == key) // erase key if it is the head item
        {
            KeyValuePair *temp = head;
            head = head->next;
            head->prev = nullptr;
            delete temp;
            m_size--;
            return true;
        }
        if (tail->m_key == key) // erase key if it is the tail item
        {
            KeyValuePair *temp = tail;
            tail = tail->prev;
            tail->next = nullptr;
            delete temp;
            m_size--;
            return true;
        }
        KeyValuePair *temp = findKey(key);
        temp->next = findKey(key)->next;
        temp->prev = findKey(key)->prev;
        temp->next->prev = temp->prev;
        temp->prev->next = temp->next;
        delete temp;
        m_size--;
        return true;
    }
    return false;
}

bool Map::contains(const KeyType& key) const
{
    KeyValuePair *temp;
    temp = head;
    while (temp != nullptr) // loop through linked list to check if key is present
    {
        if (temp->m_key == key)
            return true;
        temp = temp->next;
    }
    return false;
}

bool Map::get(const KeyType& key, ValueType& value) const
{
    if (contains(key))
    {
        value = findKey(key)->m_value;
        return true;
    }
    return false;
}

bool Map::get(int i, KeyType& key, ValueType& value) const
{
    if (i >= 0 && i < size())
    {
        KeyValuePair* temp;
        temp = tail;
        for (int k = 0; k < i; k++)
            temp = temp->prev;
        key = temp->m_key;
        value = temp->m_value;
        return true;
    }
    return false;
}

void Map::swap(Map& other)
{
    KeyValuePair* tempPair;
    int tempSize;
    
    // swap head pointers
    tempPair = head;
    head = other.head;
    other.head = tempPair;
    
    // swap tail pointers
    tempPair = tail;
    tail = other.tail;
    other.tail = tempPair;
    
    // swap sizes of both maps
    tempSize = m_size;
    m_size = other.m_size;
    other.m_size = tempSize;
}

bool combine(const Map& m1, const Map& m2, Map& result)
{
    KeyType key1, key2;
    ValueType value1, value2;
    int resultSize = result.size();
    
    if (m1.size() == 0 && m2.size() == 0)
    {
        for (int i = 0; i < resultSize; i++) // empty result map
        {
            result.get(0, key1, value1);
            result.erase(key1);
        }
        return true;
    }
    
    if (m1.size() == 0 && m2.size() != 0)
    {
        for (int i = 0; i < resultSize; i++) // empty result map
        {
            result.get(0, key1, value1);
            result.erase(key1);
        }
        result = m2;
        return true;
    }
    
    if (m1.size() != 0 && m2.size() == 0)
    {
        for (int i = 0; i < resultSize; i++) // empty result map
        {
            result.get(0, key1, value1);
            result.erase(key1);
        }
        result = m1;
        return true;
    }
    
    Map m1Copy = m1;
    Map m2Copy = m2;
    int count = 0;
    
    for (int k = 0; k < m1Copy.size(); k++) // loop through m1Copy
    {
        m1Copy.get(k, key1, value1); // get values for key and value from an item in m1Copy
        for (int j = 0; j < m2Copy.size(); j++) // loop through m2Copy
        {
            m2Copy.get(j, key2, value2); // get values for key and value from an item in m2Copy
            if (key1 == key2 && value1 == value2)
            {
                m2Copy.erase(key2); // erase key from m2Copy if key is present in both maps and value is equal
                j--; // decrement j to take into account that key was erased from m2Copy
            }
            if (key1 == key2 && value1 != value2)
            {
                m1Copy.erase(key1); // erase key from m1Copy if key is present in both maps and value is different
                m2Copy.erase(key2); // erase key from m2Copy if key is present in both maps and value is different
                count++; // increment count to indicate that function should return false
                j--; // decrement j to take into account that key was erased from m2Copy
                k--; // decrement k to take into account that key was erased from m1Copy
            }
        }
    }
    for (int i = 0; i < resultSize; i++) // empty result map
    {
        result.get(0, key1, value1);
        result.erase(key1);
    }
    
    result = m1Copy; // use operator assignment to put items in m1Copy into result
    /*
    for (int k = 0; k < m1Copy.size(); k++) // insert items in m1Copy into result map
    {
        m1Copy.get(k, key1, value1);
        result.insert(key1, value1);
    }
    */
    for (int k = 0; k < m2Copy.size(); k++) // insert items in m2Copy into result map
    {
        m2Copy.get(k, key1, value1);
        result.insert(key1, value1);
    }
    if (count > 0)
        return false;
    return true;
}

void subtract(const Map& m1, const Map& m2, Map& result)
{
    KeyType key1, key2;
    ValueType value1, value2;
    int resultSize = result.size();
    
    if (m1.size() == 0) // if m1 is empty, then just empty result map
    {
        for (int i = 0; i < resultSize; i++) // empty result map
        {
            result.get(0, key1, value1);
            result.erase(key1);
        }
        return;
    }
    
    Map m1Copy = m1;
    
    for (int k = 0; k < m1Copy.size(); k++) // loop through m1Copy
    {
        m1Copy.get(k, key1, value1); // get values for key and value from an item in m1Copy
        for (int j = 0; j < m2.size(); j++) // loop through m2
        {
            m2.get(j, key2, value2); // get values for key and value from an item in m2
            if (key1 == key2)
            {
                m1Copy.erase(key1); // erase key from m1Copy if it is also present in m2
                k--; // decrement k to take into account that key was erased from m1Copy
            }
        }
    }
    for (int i = 0; i < resultSize; i++) // empty result map
    {
        result.get(0, key1, value1);
        result.erase(key1);
    }
    for (int k = 0; k < m1Copy.size(); k++) // insert items in m1Copy into result map
    {
        m1Copy.get(k, key1, value1);
        result.insert(key1, value1);
    }
}

void Map::dump() const
{
    KeyValuePair *temp = head;
    cerr << "size based" << endl;
    for (int k = 0; k < size(); k++) {
        cerr << temp->m_key << " " << temp->m_value << endl;
        temp = temp->next;
    }
    cerr << endl << "pointer based" << endl;
    temp = head;
    while (temp != nullptr) {
        cerr << temp->m_key << " " << temp->m_value << endl;
        temp = temp->next;
    }
    cerr << endl;
}