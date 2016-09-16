#include "DiskMultiMap.h"
#include "MultiMapTuple.h"
#include <functional>

using namespace std;


bool DiskMultiMap::stringsMatch(Node n, const std::string& key, const std::string& value, const std::string& context)
{
    if (strcmp(n.key, key.c_str()) == 0 && strcmp(n.value, value.c_str()) == 0 && strcmp(n.context, context.c_str()) == 0)
        return true;
    return false;
}

BinaryFile::Offset DiskMultiMap::findBucketOffset(const std::string& key)
{
    hash<string> hashFunction;
    size_t hashResult = hashFunction(key);  //returns a size_t value, now we have to convert into an offset
    cout << hashResult << endl;
    
    Header header;
    bf.read(header, 0);
    hashResult = hashResult % header.numBuckets;  //hashResult is now a bucket NUMBER, to convert into offset we match the bucket number with its index
    cout << hashResult << endl;
    
    BinaryFile::Offset BucketOffset = sizeof(Header) + (hashResult * sizeof(Bucket));
    return BucketOffset;
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
        Node currNode;
        m_file->read(currNode, m_offset);
        Node nextNode;
        for (BinaryFile::Offset i = m_offset; i != 0; i = currNode.next)
        {
            m_file->read(nextNode, m_offset);
            if (nextNode.key == currNode.key)
            {
                Iterator it = Iterator(m_file, i);
                return it;
            }
        }
        Iterator it = Iterator();
        return it;
    }
}

MultiMapTuple DiskMultiMap::Iterator::operator*()
{
    MultiMapTuple result;
    if (!isValid())
    {
        result.key = "";
        result.value = "";
        result.context = "";
    }
    Node currNode;
    m_file->read(currNode, m_offset);
    result.key = currNode.key;
    result.value = currNode.value;
    result.context = currNode.context;
}


DiskMultiMap::DiskMultiMap()
{}

DiskMultiMap::~DiskMultiMap()
{
    bf.close();
}

bool DiskMultiMap::createNew(const std::string& filename, unsigned int numBuckets)
{
    bf.close();
    
    bool success = bf.createNew(filename);
    if (!success)
        return false;
    
    Header header;
    header.numBuckets = numBuckets;
    header.spaceHead = NULL_OFFSET;
    success = bf.write(header, 0);  //store header first
    if (!success)
        return false;
    
    int headerSize = sizeof(header);  //then store the buckets (8)
    int bucketSize = sizeof(Bucket);  //keep track of the size of a Bucket (4)
    for (int i = 0; i < numBuckets; i++)
    {
        Bucket b;
        b.head = NULL_OFFSET;
        success = bf.write(b, headerSize + (i * bucketSize));
        if (!success)
            return false;
    }
    return true;
}

bool DiskMultiMap::openExisting(const std::string& filename)
{
    bf.close();
    return bf.openExisting(filename);
}

void DiskMultiMap::close()
{
    bf.close();
}

bool DiskMultiMap::insert(const std::string& key, const std::string& value, const std::string& context)
{
    if (key.size() > MAX_STRING_SIZE || value.size() > MAX_STRING_SIZE || context.size() > MAX_STRING_SIZE)
        return false;
    
    Bucket b;  //find the bucket that will hold key
    BinaryFile::Offset BucketOffset = findBucketOffset(key);
    cout << BucketOffset << endl;
    bf.read(b, BucketOffset);  //get the bucket at BucketOffset
    
    Node newNode;
    strcpy_s(newNode.key, key.c_str()); //initialize newNode
    strcpy_s(newNode.value, value.c_str());
    strcpy_s(newNode.context, context.c_str());
    newNode.next = b.head;  //set newNode's next to the current head
    
    Header header;
    bf.write(header, 0);
    if (header.spaceHead == 0)
    {
        b.head = bf.fileLength();  //the nw head offset should point to newNode's current offset
        bf.write(newNode, bf.fileLength());  //write the newNode into the list at the last open offset
        bf.write(b.head, 0);  //update the head variable to point to the new offset in the list
        return true;
    }
    else
    {
        Node filledNode;
        bf.read(filledNode, header.spaceHead);  //sets filledNode equal to the existing node at spaceHead
        bf.write(newNode, header.spaceHead);  //overwrite the newNode into the first available open space
        b.head = header.spaceHead;  //set head equal to newNode's offset
        header.spaceHead = filledNode.next;  //set spaceHead equal to the next "deleted" node
        bf.write(b.head, 0);
        bf.write(header.spaceHead, 4);
        return true;
    }
}

DiskMultiMap::Iterator DiskMultiMap::search(const std::string& key)
{
    Bucket b;
    BinaryFile::Offset BucketOffset = findBucketOffset(key);  //find the bucket b that is returned by the hash function
    bf.read(b, BucketOffset);  //b now refers to the bucket that contains all nodes that have key string key
    
    Node currNode;
    for (BinaryFile::Offset i = b.head; i != 0; i = currNode.next)  //i starts at the head of b, and goes to the next node until it hits the end
    {
        bf.read(currNode, i);  //update currNode every time you loop through to point to the Node at offset i
        if (currNode.key == key)
        {
            Iterator it = Iterator(&bf, i);  //if the keys match, have the iterator point to currNode's offset
            return it;
        }
        Iterator it = Iterator();  //else, return an invalid default iterator
        return it;
    }
}

int DiskMultiMap::erase(const std::string& key, const std::string& value, const std::string& context)
{
    Bucket b;
    BinaryFile::Offset BucketOffset = findBucketOffset(key);
    cout << BucketOffset << endl;
    bf.read(b, BucketOffset);  //get the bucket at BucketOffset
    
    Header h;
    bf.read(h, 0);
    bool removed = false;
    if (b.head == 0)
        return false;
    Node currNode;
    bf.read(currNode, b.head); //sets currNode equal to the head node
    if (stringsMatch(currNode, key, value, context))  //if head's value is equal to data, remove
    {
        h.spaceHead = b.head;  //node at head is now the head of the deleted nodes
        b.head = currNode.next;  //head is now the node after currNode
        bf.write(b.head, 0);
        bf.write(h.spaceHead, 4);
        removed = true;
    }
    
    Node nextNode;
    for (BinaryFile::Offset i = b.head; i != 0; i = currNode.next)
    {
        bf.read(currNode, i);
        bf.read(nextNode, currNode.next);
        if (currNode.next != 0 && stringsMatch(nextNode, key, value, context))
        {
            BinaryFile::Offset temp = h.spaceHead;
            BinaryFile::Offset temp2 = currNode.next;
            h.spaceHead = currNode.next;  //spaceHead is now the offset of nextNode
            currNode.next = nextNode.next;  //set the node after currNode to be the node after nextNode
            nextNode.next = temp;  //set nextNode's next offset to the next "deleted" node
            bf.write(nextNode, temp2);
            bf.write(currNode, i);
            bf.write(h.spaceHead, 4);
            removed = true;
        }
    }
    return removed;
}



int main()
{
    DiskMultiMap d;
    d.createNew("temp", 10);
    d.insert("ke", "a", "a");
}