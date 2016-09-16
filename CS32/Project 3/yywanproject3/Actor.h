#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

// base class of all actors -----------------------------------------------------------------------------------------------------
class Actor : public GraphObject
{
public:
    Actor(int imageID, int startX, int startY, StudentWorld* studentWorld, Direction dir = right, double size = 1.0, unsigned int depth = 0);
    virtual ~Actor();
    StudentWorld* getWorld();
    int getHealth();
    void decHealth(int annoyPoints);
    bool isDead();
    virtual void doSomething() = 0;
private:
    StudentWorld* m_studentWorld;
    int m_health;
};

// misc. objects -----------------------------------------------------------------------------------------------------

// Dirt (- Actors)
class Dirt : public Actor
{
public:
    Dirt(int startX, int startY, StudentWorld* studentWorld);
    virtual ~Dirt();
    virtual void doSomething();
};

// Boulders (- Actors)
class Boulders : public Actor
{
public:
    Boulders(int startX, int startY, StudentWorld* studentWorld);
    virtual ~Boulders();
    virtual void doSomething();
private:
    int m_stability;
};

// Squirt (- Actors)
class Squirt : public Actor
{
public:
    Squirt(int startX, int startY, StudentWorld* studentWorld, Direction dir);
    virtual ~Squirt();
    virtual void doSomething();
};

// Frackman (- Actors) -------------------------------------------------------------------
class Frackman : public Actor
{
public:
    Frackman(StudentWorld* studentWorld);
    virtual ~Frackman();
    virtual void doSomething();
    
    int getSquirtsLeft();
    int getPlayerGoldCount();
    int getPlayerSonarChargeCount();
    int getNumberOfBarrelsRemainingToBePickedUp();
    
    void changeSquirtCount(int x);
    void decBarrel();
    void changeGoldCount(int x);
    void changeSonarCount(int x);
    
private:
    int m_waterSquirt;
    int m_gold;
    int m_sonarCharge;
    int m_barrelsLeft;
};

// Protester ------------------------------------------------------------------------------------------------

// Protester (- Actors)
class Protester : public Actor
{
public:
    Protester(int imageID, StudentWorld* studentWorld);
    ~Protester();
    virtual void doSomething();
    

    bool getLeaveField();
    void changeLeaveField(bool leaveField);
    int getTicksToDoSomething();
    void changeTicksToDoSomething(int x);
    int getTicksAfterSquirt();
    int countNMoves(string path);
    void moveOneInDirection(Direction dir);
    
private:
    virtual bool when4MoreFromFrackman() = 0;
    virtual bool getAnnoyed() = 0;
    
    int m_ticksToDoSomething;
    int m_ticksToShout;
    int m_ticksToPerpTurn;
    int m_ticksAfterSquirt;
    bool m_leaveField;
    int m_numSquaresToMoveInCurrentDirection;
    
};

// Regular Protester (- Protester - Actors)
class RegularProtester : public Protester
{
public:
    RegularProtester(StudentWorld* studentWorld);
    ~RegularProtester();
private:
    virtual bool when4MoreFromFrackman();
    virtual bool getAnnoyed();

};

// Hardcore Protester (- Protester - Actors)
class HardcoreProtester : public Protester
{
public:
    HardcoreProtester(StudentWorld* studentWorld);
    ~HardcoreProtester();
private:
    virtual bool when4MoreFromFrackman();
    virtual bool getAnnoyed();
    int m_ticksToStare;
};

// Object ------------------------------------------------------------------------------------------------

// Object (- Actors) (OilBarrel, GoldNugget, SonarKit, Waterpool)
class Object : public Actor
{
public:
    Object(int imageID, int startX, int startY, StudentWorld* studentWorld, Direction dir, double size, unsigned int depth);
    virtual ~Object();
    
    void makeAppear();
    bool isAppear();
    int getState();
    void changeState(int x);
    void incScore(int x);
    int getIncScore();
    void changeSound(int x);
    int getSound();
    
private:
    bool m_appear;
    int m_state;
    int m_incScore;
    int m_sound;
    
};

// Pickups (- Object - Actors)
class Pickups: public Object
{
public:
    Pickups(int imageID, int startX, int startY, StudentWorld* studentWorld);
    virtual ~Pickups();
    virtual void doSomething();
private:
    virtual void changeCount() = 0;
};

// Powers (- Object - Actors)
class Powers: public Object
{
public:
    Powers(int imageID, int startX, int startY, StudentWorld* studentWorld);
    virtual ~Powers();
    virtual void doSomething();
private:
    virtual void changeCount() = 0;
};


// Bribe (- Object - Actors)
class Bribe : public Object
{
public:
    Bribe(int startX, int startY, StudentWorld* studentWorld);
    ~Bribe();
    virtual void doSomething();
};


// Pickups ------------------------------------------------------------------------------------------------

// OilBarrel (- Pickups - Object - Actors)
class OilBarrel : public Pickups
{
public:
    OilBarrel(int startX, int startY, StudentWorld* studentWorld);
    virtual ~OilBarrel();
private:
    virtual void changeCount();
    
};

// GoldNugget (- Pickups - Object - Actors)
class GoldNugget : public Pickups
{
public:
    GoldNugget(int startX, int startY, StudentWorld* studentWorld);
    virtual ~GoldNugget();
private:
    virtual void changeCount();
};

// Powers ------------------------------------------------------------------------------------------------

// SonarKit (- Powers - Object - Actors)
class SonarKit : public Powers
{
public:
    SonarKit(StudentWorld* studentWorld);
    ~SonarKit();
private:
    virtual void changeCount();
};

// WaterPool (- Powers - Object - Actors)
class WaterPool : public Powers
{
public:
    WaterPool(int x, int y, StudentWorld* studentWorld);
    ~WaterPool();
private:
    virtual void changeCount();
};


#endif // ACTOR_H_
