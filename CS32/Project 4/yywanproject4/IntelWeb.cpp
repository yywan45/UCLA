//
//  IntelWeb.cpp
//  project4
//
//  Created by Yuen Yee Wan on 3/8/16.
//  Copyright © 2016 yywan. All rights reserved.
//

#include <stdio.h>
#include <sstream>
#include <string>
#include <set>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include "IntelWeb.h"
using namespace std;


#include "BinaryFile.h"

#include <iostream>
#include <fstream>
#include <sstream>



IntelWeb::IntelWeb()
{
    
}

IntelWeb::~IntelWeb()
{
    close();
}

bool IntelWeb::createNew(const std::string& filePrefix, unsigned int maxDataItems)
{
    close();
    if (!m_forwardFile.createNew(filePrefix + "Forward", maxDataItems/0.75))
        return false;
    if (!m_reverseFile.createNew(filePrefix + "Reverse", maxDataItems/0.75))
        return false;
    return true;
    
        
}

bool IntelWeb::openExisting(const std::string& filePrefix)
{
    if (!m_forwardFile.openExisting(filePrefix + "Forward"))
        return false;
    if (!m_reverseFile.openExisting(filePrefix + "Reverse"))
        return false;
    return true;
}

void IntelWeb::close()
{
    m_forwardFile.close();
    m_reverseFile.close();
}

bool IntelWeb::ingest(const std::string& telemetryFile)
{
    ifstream inf(telemetryFile);
    if (!inf)
        return false;
    string line;
    while (getline(inf,line))
    {
        istringstream iss(line);
        string key,value,context;
        if (!(iss >> context >> key >> value))
        {
            cerr << "Ignoring line due to bad input format: " << line;
        }
        
        m_forwardFile.insert(key, value, context);
        m_reverseFile.insert(value, key, context);

        cerr << "context: " << context << "key: " << key << "value: " << value << endl;
    }
    return true;
}

// -----------------------------------------------------------------------------------------------------------------------------------------------------------

bool operator==(InteractionTuple a, InteractionTuple b)
{
    if (a.to == b.to && a.from == b.from && a.context == b.context)
        return true;
    return false;
}

bool operator<(const InteractionTuple &a, const InteractionTuple &b)
{
    if (a.context < b.context)
        return true;
    
    if(a.context == b.context)
    {
        if(a.from < b.from)
            return true;
        if(a.from == b.from)
            if(a.to < b.to)
                return true;
    }
    return false;
}

unsigned int IntelWeb::crawl(const std::vector<std::string>& indicators,unsigned int minPrevalenceToBeGood, std::vector<std::string>& badEntitiesFound, std::vector<InteractionTuple>& badInteractions)
{
    int maliciousNumber = 0;
    int count;
    
    InteractionTuple tupleFwd, tupleRvs;
    DiskMultiMap::Iterator itr, itFwdCount, itRvsCount, itFwd, itRvs;
    queue<string> indicatorsQueue;
    set<string> setBadEntitiesFound;
    set<InteractionTuple> setBadInteractions;
    
    for (int x = 0; x < indicators.size(); x++)
    {
        indicatorsQueue.push(indicators[x]);
        setBadEntitiesFound.insert(indicators[x]);
    }
    
    while (!indicatorsQueue.empty())
    {
        string checkingMalicious = indicatorsQueue.front();
        indicatorsQueue.pop();
        
        if (setBadEntitiesFound.count(checkingMalicious) > 0)
            continue;
        
        itFwdCount = m_forwardFile.search(checkingMalicious);
        itRvsCount = m_reverseFile.search(checkingMalicious);
        
        count = 0;
        
        while (itFwdCount.isValid())
        {
            count++;
            ++itFwdCount;
        }
        
        while (itRvsCount.isValid())
        {
            count++;
            ++itRvsCount;
        }
        
        if (count < minPrevalenceToBeGood)
        {
            maliciousNumber++;
            setBadEntitiesFound.insert(checkingMalicious);
            itFwd = m_forwardFile.search(checkingMalicious);
            itRvs = m_reverseFile.search(checkingMalicious);
            
            while (itFwd.isValid())
            {
                MultiMapTuple mmt1 = *itFwd;
                indicatorsQueue.push(mmt1.value);
                
                tupleFwd.from = mmt1.key;
                tupleFwd.to = mmt1.value;
                tupleFwd.context = mmt1.context;
                setBadInteractions.insert(tupleFwd);
                
            }
            
            while (itRvs.isValid())
            {
                MultiMapTuple mmt2 = *itRvs;
                indicatorsQueue.push(mmt2.value);
                
                tupleRvs.from = mmt2.key;
                tupleRvs.to = mmt2.value;
                tupleRvs.context = mmt2.context;
                setBadInteractions.insert(tupleRvs);
            }
        }
    }
    
    badEntitiesFound.clear();
    badInteractions.clear();
    
    set<string>::iterator it;
    for (it = setBadEntitiesFound.begin(); it != setBadEntitiesFound.end(); ++it)
        badEntitiesFound.push_back(*it);
    
    set<InteractionTuple>::iterator it2;
    for (it2 = setBadInteractions.begin(); it2 != setBadInteractions.end(); ++it2)
        badInteractions.push_back(*it2);
    
    return maliciousNumber;
    
}


// -------------------------------------------------------------------------------------------------------------------------------------------------

bool IntelWeb::purge(const std::string& entity)
{
    bool flag = false;
    // use an itrator to get the entity from each map. Using the itrator, find the key if entity is a value and erase the entire item.
    DiskMultiMap::Iterator itFwd = m_forwardFile.search(entity);
    DiskMultiMap::Iterator itRvs = m_reverseFile.search(entity);
    while(itFwd.isValid())
    {
        if (m_forwardFile.erase((*itFwd).key, (*itFwd).value, (*itFwd).context) > 0)
        {
            m_reverseFile.erase((*itFwd).value, (*itFwd).key, (*itFwd).context);
            flag = true;
        }
        ++itFwd;
    }
    while(itRvs.isValid())
    {
        if (m_reverseFile.erase((*itRvs).key, (*itRvs).value, (*itRvs).context) > 0)
        {
            m_forwardFile.erase((*itRvs).value, (*itRvs).key, (*itRvs).context);
            flag = true;
        }
        ++itRvs;
    }
    return flag;
}