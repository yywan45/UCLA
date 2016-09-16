//
//  DiskMultiMap.cpp
//  project4
//
//  Created by Yuen Yee Wan on 3/8/16.
//  Copyright © 2016 yywan. All rights reserved.
//

#include <stdio.h>
#include <functional>
#include "DiskMultiMap.h"

DiskMultiMap::DiskMultiMap(){}

DiskMultiMap::~DiskMultiMap()
{
    m_bf.close();
}

DiskMultiMap::Iterator::Iterator()
{
    m_valid = false;
}

DiskMultiMap::Iterator::Iterator(BinaryFile* bf, BinaryFile::Offset offset)
{
    m_valid = true;
    m_file = bf;
    m_offset = offset;
}

bool DiskMultiMap::Iterator::isValid() const
{
    return m_valid;
}

DiskMultiMap::Iterator& DiskMultiMap::Iterator::operator++()
{
    if (isValid())
    {
        Node currentNode;
        m_file->read(currentNode, m_offset);
        Node nextNode;
        for (BinaryFile::Offset i = currentNode.next; i != 0; i = nextNode.next)
        {
            m_file->read(nextNode, i);
            if (strcmp(nextNode.key, currentNode.key) == 0)
            {
                m_offset = i;
                return *this;
            }
        }
        m_valid = false;
        return *this;
    }
    return *this;
}

MultiMapTuple DiskMultiMap::Iterator::operator*()
{
    MultiMapTuple result;
    
    if (!isValid())
    {
        result.key = "";
        result.value = "";
        result.context = "";
        return result;
    }
    
    Node currentNode;
    m_file->read(currentNode, m_offset);
    result.key = currentNode.key;
    result.value = currentNode.value;
    result.context = currentNode.context;
    return result;
}

bool DiskMultiMap::createNew(const std::string& filename, unsigned int numBuckets)
{
    close();
    m_bf.createNew(filename);
    m_bf.write(numBuckets, 0);
    
    for (int i = 0; i < numBuckets; i++)
        m_bf.write(0, m_bf.fileLength());
    
    m_sizeOfHeader = m_bf.fileLength();
    m_numBucket = numBuckets;
    return true;
}

bool DiskMultiMap::openExisting(const std::string& filename)
{
    close();
    return m_bf.openExisting(filename);
}

void DiskMultiMap::close()
{
    m_bf.close();
}

bool DiskMultiMap::insert(const std::string& key, const std::string& value, const std::string& context)
{
    // SET UP ----------
    
    // check if arguments are more than 120 characters and return false if so
    if (key.size() > MAX_INPUT_SIZE || value.size() > MAX_INPUT_SIZE || context.size() > MAX_INPUT_SIZE)
        return false;
    
    // change arguments into C-string and put into newNode (newNode.next hasn't been initialized yet)
    Node newNode;
    strcpy(newNode.key, key.c_str());
    strcpy(newNode.value, value.c_str());
    strcpy(newNode.context, context.c_str());
    newNode.next = 0;
    
    // PUT NODE IN BINARY FILE ----------
    
    BinaryFile::Offset newNodeOffset = m_bf.fileLength(); // set it first at the end of the file
    
    // check if there are empty spaces to put new node in (check if next value is -1)
    
    for (BinaryFile::Offset i = m_sizeOfHeader; i < m_bf.fileLength(); i += sizeof(Node))
    {
        Node checkEmptyNode;
        m_bf.read(checkEmptyNode, i);
        if (checkEmptyNode.next == -1)
        {
            newNodeOffset = i;
            break;
        }
    }
    
    m_bf.write(newNode, newNodeOffset);
    
    // LINK NODE TO HASH TABLE ----------
    
    // find hash value of key
    std::hash<string> hashValue;
    unsigned long keyHash = hashValue(key) % m_numBucket;
    
    // check if there is already a key inserted in hash spot
    int hashSpotValue;
    BinaryFile::Offset hashOffset = 4 + (keyHash * 4); // take into account offset 0 is filled by numBucket
    m_bf.read(hashSpotValue, hashOffset);
    if (hashSpotValue == 0) // this key being inserted for the first time
    {
        m_bf.write(newNodeOffset, hashOffset);
    }
    else // this key has been inserted before
    {
        BinaryFile::Offset firstNodeOffset;
        m_bf.read(firstNodeOffset, hashOffset); // sets firstNodeOffset to offset of first linked node

        Node linkedNodes;
        m_bf.read(linkedNodes, firstNodeOffset);
        BinaryFile::Offset currentOffset = firstNodeOffset;
        
        while (linkedNodes.next != 0)
        {
            currentOffset = linkedNodes.next;
            m_bf.read(linkedNodes, linkedNodes.next);
        }
        linkedNodes.next = newNodeOffset;
        m_bf.write(linkedNodes, currentOffset);
    }
    return true;
}

DiskMultiMap::Iterator DiskMultiMap::search(const std::string& key)
{
    // find hash value of key
    std::hash<string> hashValue;
    unsigned long keyHash = hashValue(key) % m_numBucket;
    
    // check if there is already a key inserted in hash spot
    int hashSpotValue;
    BinaryFile::Offset hashOffset = 4 + (keyHash * 4); // take into account offset 0 is filled by numBucket
    m_bf.read(hashSpotValue, hashOffset);
    if (hashSpotValue != 0) // there is at least one key
    {
        BinaryFile::Offset firstNodeOffset;
        m_bf.read(firstNodeOffset, hashOffset); // sets firstNodeOffset to offset of first linked node
        Iterator it = Iterator(&m_bf, firstNodeOffset);
        return it;
    }
    Iterator it = Iterator();
    return it;
}

int DiskMultiMap::erase(const std::string& key, const std::string& value, const std::string& context)
{
    int numberErased = 0;
    
    // check if arguments are more than 120 characters and return false if so
    if (key.size() > MAX_INPUT_SIZE || value.size() > MAX_INPUT_SIZE || context.size() > MAX_INPUT_SIZE)
        return false;
    
    // change arguments into C-string and put into newNode (newNode.next hasn't been initialized yet)
    Node eraseNode;
    strcpy(eraseNode.key, key.c_str());
    strcpy(eraseNode.value, value.c_str());
    strcpy(eraseNode.context, context.c_str());
    
    // find hash value of key
    std::hash<string> hashValue;
    unsigned long keyHash = hashValue(key) % m_numBucket;
    
    // check if there is already a key inserted in hash spot
    BinaryFile::Offset hashSpotValue;
    BinaryFile::Offset hashOffset = 4 + (keyHash * 4); // take into account offset 0 is filled by numBucket
    m_bf.read(hashSpotValue, hashOffset);
    if (hashSpotValue == 0) // this key being inserted for the first time
    {
        return numberErased;
    }
    else // this key has been inserted before
    {
        Node linkedNodes;
        
        BinaryFile::Offset prevOffset; // = firstNodeOffset;
        
        for (BinaryFile::Offset i = hashSpotValue; i != 0; i = linkedNodes.next)
        {
            m_bf.read(linkedNodes, i);
            
            if (strcmp(eraseNode.key, linkedNodes.key) == 0 &&
                strcmp(eraseNode.value, linkedNodes.value) == 0 &&
                strcmp(eraseNode.context, linkedNodes.context) == 0)
            {
                BinaryFile::Offset firstInList;
                m_bf.read(firstInList, hashOffset); // sets firstNodeOffset to offset of first linked node
                
                if (i == firstInList) // if erasing the first node connected to header
                {
                    numberErased++;
                    BinaryFile::Offset secondNodeOffset = linkedNodes.next; // set variable to retrieve address of second node
                    linkedNodes.next = -1;
                    m_bf.write(linkedNodes, i); // set first node's next value to -1 to indicate it is erased
                    m_bf.write(secondNodeOffset, hashOffset); // set value of hash header to second node i.e. it is now the first node
                    linkedNodes.next = secondNodeOffset; // ????
                }
                else
                {
                    numberErased++;
                    BinaryFile::Offset nextNodeOffset;
                    nextNodeOffset = linkedNodes.next; // set variable to retrieve address of next node
                    linkedNodes.next = -1;
                    m_bf.write(linkedNodes, i); // set erased node's next value to -1
                    
                    Node tempNode;
                    m_bf.read(tempNode, prevOffset); // set tempNode to equal to previously linked node
                    tempNode.next = nextNodeOffset;
                    m_bf.write(tempNode, prevOffset); // set previous node's next to next of erased node
                    linkedNodes.next = prevOffset;
                    
                }
            }
            
            else
            prevOffset = i;
        }
    }

    return numberErased;
}

//void DiskMultiMap::print()
//{
//    for (BinaryFile::Offset i = 4; i < m_sizeOfHeader - 4; i = i+4)
//    {
//        BinaryFile::Offset position;
//        m_bf.read(position, i);
//        if (position != 0)
//        {
//            cout << position << endl;
//            
//            Node readNode;
//            for (BinaryFile::Offset j = position; j != 0; j = readNode.next)
//            {
//                m_bf.read(readNode, j);
//                cerr << readNode.key << ", " << readNode.value << ", " << readNode.context << ", " << readNode.next << endl;
//            }
//            
//        }
//    }
//}


//
//int main()
//{
//    DiskMultiMap x;
//    
//    x.createNew("myhashtable.dat",100); // empty, with 100 buckets
//    
//    x.insert("hmm.exe", "pfft.exe", "m52902");
//    x.insert("hmm.exe", "pfft.exe", "m52902");
//    x.insert("hmm.exe", "pfft.exe", "m10001");
//    x.insert("blah.exe", "bletch.exe", "m0003");
//    
//    DiskMultiMap::Iterator it = x.search("hmm.exe");
//    if (it.isValid())
//    {
//        cout << "I found at least 1 item with a key of hmm.exe\n";
//        do
//        {
//            MultiMapTuple m = *it; // get the association
//            cout << "The key is: " << m.key << endl;
//            cout << "The value is: " << m.value << endl;
//            cout << "The context is: " << m.context << endl;
//            cout << endl;
//            ++it; // advance iterator to the next matching item
//        }
//        while (it.isValid());
//    }
//    
////    DiskMultiMap::Iterator it = x.search("goober.exe");
////    if (!it.isValid())
////    {
////        cout << "I couldn't find goober.exe\n";
////    }
////    
//
//    DiskMultiMap x;
//    x.createNew("myhashtable.dat",100); // empty, with 100 buckets
//    x.insert("hmm.exe", "pfft.exe", "m52902");
//    x.insert("hmm.exe", "pfft.exe", "m52902");
//    x.insert("hmm.exe", "pfft.exe", "m10001");
//    x.insert("blah.exe", "bletch.exe", "m0003");
//    
//    
//    x.print();
//    cerr << endl;
//    
//    
//    // line 1
//    if (x.erase("hmm.exe", "pfft.exe", "m52902") == 2)
//        cout << "Just erased 2 items from the table!\n";
//    x.print();
//    cerr << endl;
//    // line 2
//    if (x.erase("hmm.exe", "pfft.exe", "m10001") > 0)
//        cout << "Just erased at least 1 item from the table!\n";
//    x.print();
//    cerr << endl;
//    // line 3
//    if (x.erase("blah.exe", "bletch.exe", "m66666") == 0)
//        cout << "I didn't erase this item cause it wasn't there\n";
//    x.print();
//    cerr << endl;
//
//}
