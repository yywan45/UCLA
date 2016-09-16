//
//  DiskList.cpp
//  project4warmup
//
//  Created by Yuen Yee Wan on 3/3/16.
//  Copyright © 2016 yywan. All rights reserved.
//

#include <stdio.h>
#include "DiskList.h"

DiskList::DiskList(const std::string& filename)
{
    m_binaryFile.createNew(filename);
    m_firstNodeOffset = 0;
    m_binaryFile.write(m_firstNodeOffset, 0);
    
}

bool DiskList::push_front(const char* data)
{
    if (strlen(data) > 256)
        return false;
    
    DiskNode node(data, m_firstNodeOffset);
    m_firstNodeOffset = m_binaryFile.fileLength();
    m_binaryFile.write(m_firstNodeOffset, 0);
    m_binaryFile.write(node, m_binaryFile.fileLength());
    
    return true;
    
    // For the push_front function, if data is a C string of fewer than 256 characters (not counting the zero byte), then a new node with a copy of the C string (including a zero byte) is added to the front of the list, and the function returns true.
    // If the C string has at least 256 non-zero-byte characters, the function returns false without changing anything.
    
    
    //    m_binaryFile.write(data, strlen(data), m_binaryFile.fileLength());
    //    m_binaryFile.write(m_firstNodeOffset, m_binaryFile.fileLength()+strlen(data));
    
}

bool DiskList::remove(const char* data)
{
    char v[257];
    BinaryFile::Offset next;
    DiskNode current(v, next);
    m_binaryFile.read(current, m_firstNodeOffset);
    
    BinaryFile::Offset prevOffset = 0;
    BinaryFile::Offset currentOffset = m_firstNodeOffset;
    
    bool flag = false;
    
    while (current.next != 0)
    {
        if (strcmp(current.value, data) == 0)
        {
            flag = true;
            if (currentOffset == m_firstNodeOffset)
            {
                m_firstNodeOffset = current.next;
                current.next = -1;
                m_binaryFile.write(current, currentOffset);
                m_binaryFile.read(current, m_firstNodeOffset);
                currentOffset = m_firstNodeOffset;
            }
            else
            {
                char v[257];
                BinaryFile::Offset next;
                DiskNode temp(v, next);
                m_binaryFile.read(temp, prevOffset);
                
                temp.next = current.next;
                m_binaryFile.write(temp, prevOffset);
                current.next = -1;
                m_binaryFile.write(current, currentOffset);
                m_binaryFile.read(current, temp.next);
                currentOffset = temp.next;
            }
        }
        else
        {
            prevOffset = currentOffset;
            currentOffset = current.next;
            m_binaryFile.read(current, current.next);
        }
        
    }
    
    // Every node whose C string is equal to data is removed from the list by the remove function.
    // Nodes not removed must remain in the same order in the linked list as they were in before.
    // The function returns true if at least one node was removed, otherwise false.
    
    return flag;
    
}

void DiskList::printAll()
{
    const char* v;
    BinaryFile::Offset next;
    DiskNode current(v, next);
    
    m_binaryFile.read(current, m_firstNodeOffset);

    for(;;)
    {
        cout << current.value << endl;
        m_binaryFile.read(current, current.next);
        if (current.next == 0)
        {
            cout << current.value << endl;
            break;
        }
    }
    // The printAll function writes, one per line, all the C strings in the list in the order they appear in the list.
}



int main()
{
    DiskList x("mylist.dat");
    x.push_front("Fred");
    x.push_front("Lucy");
    x.push_front("Ethel");
    x.push_front("Ethel");
    x.push_front("Lucy");
    x.push_front("Fred");
    x.push_front("Ethel");
    x.push_front("Ricky");
    x.push_front("Lucy");
//    x.remove("Lucy");
    x.push_front("Fred");

    x.push_front("Ricky");
        x.remove("Fred");
    x.printAll();
    // writes, one per line
    // Ricky  Fred  Ricky  Ethel  Fred  Ethel  Ethel  Fred
}