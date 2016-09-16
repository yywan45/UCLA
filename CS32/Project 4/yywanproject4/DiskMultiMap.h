#ifndef DISKMULTIMAP_H_
#define DISKMULTIMAP_H_

#include <string>
#include "BinaryFile.h"
#include "MultiMapTuple.h"

class DiskMultiMap
{
public:

	class Iterator
	{
	public:
		Iterator();
        Iterator(BinaryFile* bf, BinaryFile::Offset offset);
		bool isValid() const;
		Iterator& operator++();
		MultiMapTuple operator*();

	private:
        bool m_valid;
        BinaryFile* m_file;
        BinaryFile::Offset m_offset;
	};

	DiskMultiMap();
	~DiskMultiMap();
	bool createNew(const std::string& filename, unsigned int numBuckets);
	bool openExisting(const std::string& filename);
	void close();
	bool insert(const std::string& key, const std::string& value, const std::string& context);
	Iterator search(const std::string& key);
	int erase(const std::string& key, const std::string& value, const std::string& context);

//    void print();
    
    
private:
	// Your private member declarations will go here
    
    
    
    static const int MAX_INPUT_SIZE = 120;
    
    struct Node
    {
        char key[MAX_INPUT_SIZE];
        char value[MAX_INPUT_SIZE];
        char context[MAX_INPUT_SIZE];
        BinaryFile::Offset next;
        
    };
    BinaryFile m_bf;
    BinaryFile::Offset m_sizeOfHeader;
    int m_numBucket;
    
};

#endif // DISKMULTIMAP_H_
