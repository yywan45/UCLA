//
//  DiskList.h
//  project4warmup
//
//  Created by Yuen Yee Wan on 3/3/16.
//  Copyright © 2016 yywan. All rights reserved.

#include "BinaryFile.h"

#ifndef DiskList_h
#define DiskList_h

class DiskList
{
public:
    DiskList(const std::string& filename);
    bool push_front(const char* data);
    bool remove(const char* data);
    void printAll();
private:
    BinaryFile m_binaryFile;
    BinaryFile::Offset m_firstNodeOffset;
    struct DiskNode
    {
        DiskNode(const char* v, BinaryFile::Offset n): next(n)
        {
            strcpy(value, v);
        }
        char value[257];
        BinaryFile::Offset next;
    };
};




#endif /* DiskList_h */
