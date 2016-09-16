#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "GraphObject.h"
#include <string>
#include <vector>
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actor;

class Dirt;
//class Boulders;
//class Squirt;
//
//class Agents;
//class Object;

class Frackman;
class RegularProtester;
class HardcoreProtester;

//class OilBarrel;
//class GoldNugget;
//class SonarKit;
//class WaterPool;



class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
    : GameWorld(assetDir){}

    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    ~StudentWorld();
    
    // Updating Stats
    void SWchangeSquirtCount(int x);
    void SWdecBarrel();
    void SWchangeGoldCount(int x);
    void SWchangeSonarCount(int x);
    
    // Creating new Actors
    void addSquirt(int x, int y, GraphObject::Direction dir);
    void addBribe(int x, int y);
    
    // Helper functions
    void removeDirt(int x, int y);
    bool checkForActorCoord(int xmin, int xmax, int xminBound, int xmaxBound, int ymin, int ymax, int yminBound, int ymaxBound, int imageID);
    int annoyActorsWithinRadius(int x, int y, int imageID, int radius, int hitPoints);
       void setVisibleWithinRadius(int x, int y, int radius);
    GraphObject::Direction directionToFaceFrackman(int x, int y);
    bool canMoveInDirection(int x, int y, GraphObject::Direction dir);
    string pathToPoint(int startX, int startY, string exitOrFrackman);
    
//    Functions that didn't come into use in the end
//    bool annoyAgentsAt(int xmin, int xmax, int ymin, int ymax, int annoyPoints);
//    bool existsStraightPathToFrackman(int startX, int startY);
//    bool checkForActorRadius(int x, int y, int imageID, int radius);
private:
    
    void setDisplayText();
    string insertChar(int stat, int digits, char insert);
    bool checkDistribution(int x, int y);
    string stringRowField(int x);
    void stringArrayField();
    
    vector<Actor*> m_actors;
    vector<Actor*> m_tempActors;
    Frackman* m_frackman;
    Dirt* m_dirt[64][64];
    int m_ticksBeforeNewProtester;
    string m_field[64];
};

#endif // STUDENTWORLD_H_
