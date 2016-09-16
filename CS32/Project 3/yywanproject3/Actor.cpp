#include "Actor.h"
#include "StudentWorld.h"
#include <algorithm>

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp


// base class of all actors -----------------------------------------------------------------------------------------------------

// Actor -------------------------------------------------------------------------------------------------

Actor::Actor(int imageID, int startX, int startY, StudentWorld* studentWorld, Direction dir, double size, unsigned int depth)
:GraphObject(imageID, startX, startY, dir, size, depth), m_studentWorld(studentWorld)
{
    setVisible(true);
    m_health = 100;
}

Actor::~Actor(){}

StudentWorld* Actor::getWorld()
{
    return m_studentWorld;
}

int Actor::getHealth()
{
    return m_health;
}

void Actor::decHealth(int annoyPoints)
{
    m_health -= annoyPoints;
}

bool Actor::isDead()
{
    return m_health <= 0;
}

// misc. objects -----------------------------------------------------------------------------------------------------

// Dirt (- Actors) -------------------------------------------------------------------------------------------------

Dirt::Dirt(int startX, int startY, StudentWorld* studentWorld)
:Actor(IID_DIRT, startX, startY, studentWorld, right, 0.25, 3)
{}

Dirt::~Dirt()
{}

void Dirt::doSomething(){}

// Boulders (- Actors) -------------------------------------------------------------------------------------------------

Boulders::Boulders(int startX, int startY, StudentWorld* studentWorld)
:Actor(IID_BOULDER, startX, startY, studentWorld, down, 1.0, 1)
{
    m_stability = 30;
}

Boulders::~Boulders(){}

void Boulders::doSomething()
{
    if (isDead())
        return;
    
    if (getY() == 0 && m_stability > 0)
        return;
    
    if (m_stability > 0 && !getWorld()->checkForActorCoord(getX(), getX()+3, 0, 63, getY()-1, getY()-1, 0, 63, IID_DIRT))
        m_stability--;
    
    if (getY() <= 0 && m_stability <= 0)
        decHealth(100);
    
    if (m_stability <= 0 && getWorld()->checkForActorCoord(getX(), getX()+3, 0, 63, getY()-1, getY()-1, 0, 63, IID_DIRT))
        decHealth(100);
    
    if (m_stability <= 0 && getWorld()->checkForActorCoord(getX() - 3 , getX() + 3, 0, 63, getY()-1 , getY() - 1, 0, 60, IID_BOULDER))
        decHealth(100);
    
    else if (m_stability <= 0)
    {
        getWorld()->playSound(SOUND_FALLING_ROCK);
        getWorld()->increaseScore(500 * getWorld()->annoyActorsWithinRadius(getX(), getY(), IID_PROTESTER, 3, 100));
        getWorld()->increaseScore(500 * getWorld()->annoyActorsWithinRadius(getX(), getY(), IID_HARD_CORE_PROTESTER, 3, 100));
        getWorld()->annoyActorsWithinRadius(getX(), getY(), IID_PLAYER, 3, 100);
        moveTo(getX(), getY()-1);
    }
}

// Squirt (- Actors) -------------------------------------------------------------------------------------------------

Squirt::Squirt(int startX, int startY, StudentWorld* studentWorld, Direction dir)
:Actor(IID_WATER_SPURT, startX, startY, studentWorld, dir, 1.0, 1) {}

Squirt::~Squirt(){}

void Squirt::doSomething()
{
    if (isDead())
        return;
    
    switch (getDirection())
    {
        case (up):
            if(getWorld()->annoyActorsWithinRadius(getX(), getY(), IID_PROTESTER, 3, 0) > 0 ||
               getWorld()->annoyActorsWithinRadius(getX(), getY(), IID_HARD_CORE_PROTESTER, 3, 0) > 0)
                decHealth(100);

            if (getWorld()->annoyActorsWithinRadius(getX(), getY(), IID_BOULDER, 3, 0) > 0 ||
                getWorld()->checkForActorCoord(getX(), getX() + 3, 0, 63, getY() + 4, getY() + 7, 0, 60, IID_DIRT) ||
                getY() > 56)
                decHealth(100);
            else
            {
                moveTo(getX(), getY()+1);
                decHealth(25);
            }
            break;
        case (down):
            if(getWorld()->annoyActorsWithinRadius(getX(), getY(), IID_PROTESTER, 3, 0) > 0 ||
               getWorld()->annoyActorsWithinRadius(getX(), getY(), IID_HARD_CORE_PROTESTER, 3, 0) > 0)
                decHealth(100);

            
            if (getWorld()->annoyActorsWithinRadius(getX(), getY(), IID_BOULDER, 3, 0) > 0 ||
                getWorld()->checkForActorCoord(getX(), getX() + 3, 0, 63, getY() - 4 , getY() - 1, 0, 60, IID_DIRT) ||
                getY() < 4)
                decHealth(100);
            else
            {
                moveTo(getX(), getY()-1);
                decHealth(25);
            }
            break;
        case (left):
            if(getWorld()->annoyActorsWithinRadius(getX(), getY(), IID_PROTESTER, 3, 0) > 0 ||
               getWorld()->annoyActorsWithinRadius(getX(), getY(), IID_HARD_CORE_PROTESTER, 3, 0) > 0)
                decHealth(100);

            
            if (getWorld()->annoyActorsWithinRadius(getX(), getY(), IID_BOULDER, 3, 0) > 0 ||
                getWorld()->checkForActorCoord(getX() - 4, getX() - 1, 0, 63, getY(), getY() + 3, 0, 60, IID_DIRT) ||
                getX() < 4)
                decHealth(100);
            else
            {
                moveTo(getX()-1, getY());
                decHealth(25);
            }
            break;
        case (right):
            if(getWorld()->annoyActorsWithinRadius(getX(), getY(), IID_PROTESTER, 3, 0) > 0 ||
               getWorld()->annoyActorsWithinRadius(getX(), getY(), IID_HARD_CORE_PROTESTER, 3, 0) > 0)
                decHealth(100);
            
            if (getWorld()->annoyActorsWithinRadius(getX(), getY(), IID_BOULDER, 3, 0) > 0 ||
                getWorld()->checkForActorCoord(getX() + 1, getX() + 4, 0, 63, getY(), getY() + 3, 0, 60, IID_DIRT) ||
                getX() > 59)
                decHealth(100);
            
            else
            {
                moveTo(getX()+1, getY());
                decHealth(25);
            }
            break;
            default:
            break;
    }
}



// Frackman ( - Actors) -------------------------------------------------------------------------------------------------

Frackman::Frackman(StudentWorld* studentWorld)
:Actor(IID_PLAYER, 30, 60, studentWorld)
{
    m_waterSquirt = 5;
    m_gold = 0;
    m_sonarCharge = 1;
    
    int level = getWorld()->getLevel();
    int L = min(2 + level, 20);
    m_barrelsLeft = L;
};

Frackman::~Frackman(){}


int Frackman::getSquirtsLeft()
{
    return m_waterSquirt;
}

int Frackman::getPlayerGoldCount()
{
    return m_gold;
}

int Frackman::getPlayerSonarChargeCount()
{
    return m_sonarCharge;
}

int Frackman::getNumberOfBarrelsRemainingToBePickedUp()
{
    return m_barrelsLeft;
}

void Frackman::changeSquirtCount(int x)
{
    m_waterSquirt += x;
}

void Frackman::decBarrel()
{
    m_barrelsLeft--;
}

void Frackman::changeGoldCount(int x)
{
    m_gold += x;
}

void Frackman::changeSonarCount(int x)
{
    m_sonarCharge += x;
}

void Frackman::doSomething()
{
    if (isDead())
    {
        getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
        return;
    }

    getWorld()->removeDirt(getX(), getY());
    
    int dir;
    if (getWorld()->getKey(dir) == true)
    {
        // user hit a key this tick!
        switch (dir)
        {
            case KEY_PRESS_LEFT:
                if (getDirection() != left)
                {
                    setDirection(left);
                    break;
                }
                if (getX() <= 0)
                    break;
                if(getWorld()->annoyActorsWithinRadius(getX()-1, getY(), IID_BOULDER, 3, 0) == 0)
                    moveTo(getX()-1, getY());
                if(getWorld()->annoyActorsWithinRadius(getX(), getY(), IID_BOULDER, 3, 0) > 0)
                   break;
                
                break;
            case KEY_PRESS_RIGHT:
                if (getDirection() != right)
                {
                    setDirection(right);
                    break;
                }
                if (getX() >= 60)
                    break;
                if(getWorld()->annoyActorsWithinRadius(getX()+1, getY(), IID_BOULDER, 3, 0) == 0)
                    moveTo(getX()+1, getY());
                if(getWorld()->annoyActorsWithinRadius(getX(), getY(), IID_BOULDER, 3, 0) > 0)
                    break;
                break;
            case KEY_PRESS_UP:
                if (getDirection() != up)
                {
                    setDirection(up);
                    break;
                }
                if (getY() >= 60)
                    break;
                if(getWorld()->annoyActorsWithinRadius(getX(), getY()+1, IID_BOULDER, 3, 0) == 0)
                    moveTo(getX(), getY()+1);
                if(getWorld()->annoyActorsWithinRadius(getX(), getY(), IID_BOULDER, 3, 0) > 0)
                    break;
                break;
            case KEY_PRESS_DOWN:
                if (getDirection() != down)
                {
                    setDirection(down);
                    break;
                }
                if (getY() <= 0)
                    break;
                if(getWorld()->annoyActorsWithinRadius(getX(), getY()-1, IID_BOULDER, 3, 0) == 0)
                    moveTo(getX(), getY()-1);
                if(getWorld()->annoyActorsWithinRadius(getX(), getY(), IID_BOULDER, 3, 0) > 0)
                    break;
                break;
            case KEY_PRESS_ESCAPE:
                decHealth(100);
                break;
            case 'Z':
            case 'z':
                if (m_sonarCharge > 0)
                {
                    m_sonarCharge--;
                    getWorld()->setVisibleWithinRadius(getX(), getY(), 12);
                }
                break;
            case KEY_PRESS_TAB:
                if (m_gold == 0)
                    break;
                m_gold--;
                getWorld()->addBribe(getX(), getY());
                break;
            case KEY_PRESS_SPACE:
                if (m_waterSquirt == 0)
                    break;
                m_waterSquirt--;
                getWorld()->playSound(SOUND_PLAYER_SQUIRT);
                if (getDirection() == up)
                {
                    if (getY() > 56)
                        break;
                    
                    if (!getWorld()->checkForActorCoord(getX(), getX() + 3, 0, 63, getY() + 4, getY() + 7, 0, 60, IID_DIRT) &&
                        !getWorld()->checkForActorCoord(getX() -3, getX() + 3, 0, 63, getY() + 4, getY() + 7, 0, 60, IID_BOULDER))
                        getWorld()->addSquirt(getX(), getY() + 4, getDirection());
                }
                if (getDirection() == down)
                {
                    if (getY() < 4)
                        break;
                    
                    if (!getWorld()->checkForActorCoord(getX(), getX() + 3, 0, 63, getY() - 4 , getY() - 1, 0, 63, IID_DIRT) &&
                        !getWorld()->checkForActorCoord(getX() -3, getX() + 3, 0, 63, getY() - 7, getY() - 4, 0, 63, IID_BOULDER))
                        getWorld()->addSquirt(getX(), getY() - 4, getDirection());
                }
                
                if (getDirection() == left)
                {
                    if (getX() < 4)
                        break;
                    
                    if (!getWorld()->checkForActorCoord(getX() - 4, getX() - 1, 0, 63, getY(), getY() + 3, 0, 63, IID_DIRT) &&
                        !getWorld()->checkForActorCoord(getX() - 7, getX() - 4, 0, 63, getY() - 3, getY() + 3, 0, 63, IID_BOULDER))
                        getWorld()->addSquirt(getX() - 4, getY(), getDirection());
                }
                
                if (getDirection() == right)
                {
                    if (getX() > 59)
                        break;
                    
                    if (!getWorld()->checkForActorCoord(getX() + 4, getX() + 7, 0, 63, getY(), getY() + 3, 0, 63, IID_DIRT) &&
                        !getWorld()->checkForActorCoord(getX() + 4, getX() + 7, 0, 63, getY() - 3, getY() + 3, 0, 63, IID_BOULDER))
                        getWorld()->addSquirt(getX() + 4, getY(), getDirection());
                }
                break;
        }
    }
}


// Protester (- Actors) -------------------------------------------------------------------------------------------------
Protester::Protester(int imageID, StudentWorld* studentWorld)
:Actor(imageID, 60, 60, studentWorld, left)
{
    int level = getWorld()->getLevel();
    m_ticksToDoSomething = max(0, 3 - level/4);
    m_ticksToShout = 15;
    m_ticksToPerpTurn = 200;
    m_leaveField = false;
    decHealth(-400);
    m_numSquaresToMoveInCurrentDirection = (rand() % 53) + 8; // 8 <= num <= 60
    m_ticksAfterSquirt = max(50, 100 - level * 10);
}

Protester::~Protester(){}

bool Protester::getLeaveField()
{
    return m_leaveField;
}

void Protester::changeLeaveField(bool leaveField)
{
    m_leaveField = leaveField;
}

int Protester::getTicksToDoSomething()
{
    return m_ticksToDoSomething;
}

void Protester::changeTicksToDoSomething(int x)
{
    m_ticksToDoSomething += x;
}

int Protester::getTicksAfterSquirt()
{
    return m_ticksAfterSquirt;
}

int Protester::countNMoves(string path)
{
    int nMoves = 1;
    
    for (int i = 0; i < path.length()-2; i++)
    {
        if (path[i] != path[i+1])
            nMoves++;
    }
    return nMoves;
}

void Protester::moveOneInDirection(Direction dir)
{
    switch (dir)
    {
        case up:
            moveTo(getX(), getY()+1);
            break;
        case down:
            moveTo(getX(), getY()-1);
            break;
        case right:
            moveTo(getX()+1, getY());
            break;
        case left:
            moveTo(getX()-1, getY());
            break;
        default:
            break;
    }
}


void Protester::doSomething()
{
    if (getAnnoyed())
        return;
    
    if (isDead())
        return;
    
    // rest state  --------------------------
    if (m_ticksToDoSomething > 0)
    {
        m_ticksToDoSomething--;
        return;
    }
    
    if (m_ticksToDoSomething == 0)
    {
        m_ticksToShout--;
        m_ticksToPerpTurn--;
        int level = getWorld()->getLevel();
        m_ticksToDoSomething = max(0, 3 - level/4);
    }
    
    //3
    if (m_leaveField)
    {
        string path = getWorld()->pathToPoint(getX(), getY(), "exit");
        
        char direction = path[path.length()-2];
        
        switch (direction) {
            case 'U':
                setDirection(up);
                moveTo(getX(), getY()+1);
                return;
                break;
            case 'D':
                setDirection(down);
                moveTo(getX(), getY()-1);
                return;
                break;
            case 'L':
                setDirection(left);
                moveTo(getX()-1, getY());
                return;
                break;
            case 'R':
                setDirection(right);
                moveTo(getX()+1, getY());
                return;
                break;
            default:
                break;
        }
        if (getX() == 60 && getY() == 60)
        {
            decHealth(500);
            return;
        }
        return;
    }
    
    //4
    
    if (getWorld()->annoyActorsWithinRadius(getX(), getY(), IID_PLAYER, 4, 0) > 0 &&
        getWorld()->directionToFaceFrackman(getX(), getY() == getDirection()))
    {
        if (m_ticksToShout <= 0)
        {
            getWorld()->annoyActorsWithinRadius(getX(), getY(), IID_PLAYER, 4, 20);
            m_ticksToShout = 15;
            getWorld()->playSound(SOUND_PROTESTER_YELL);
            return;
            
        }
    }

    else if (getWorld()->annoyActorsWithinRadius(getX(), getY(), IID_PLAYER, 4, 0) == 0)
    {
        
        if(when4MoreFromFrackman())
            return;
        
        if ((getWorld()->checkForActorCoord(getX(), getX(), 0, 63, 0, 63, 0, 63, IID_PLAYER) ||
             getWorld()->checkForActorCoord(0, 63, 0, 63, getY(), getY(), 0, 63, IID_PLAYER)) &&
            countNMoves(getWorld()->pathToPoint(getX(), getY(), "Frackman")) == 1)
        {
            setDirection(getWorld()->directionToFaceFrackman(getX(), getY()));
            moveOneInDirection(getDirection());
            m_numSquaresToMoveInCurrentDirection = 0;
            return;
        }
        
        //6
        else
        {
            m_numSquaresToMoveInCurrentDirection--;
            
            //6
            if (m_numSquaresToMoveInCurrentDirection <= 0)
            {
                bool flag = false;
                
                while (!flag)
                {
                    int x = (rand() % 4);
                    switch (x) {
                        case 0:
                            if (getWorld()->canMoveInDirection(getX(), getY(), up))
                            {
                                setDirection(up);
                                flag = true;
                            }
                            break;
                        case 1:
                            if (getWorld()->canMoveInDirection(getX(), getY(), down))
                            {
                                setDirection(down);
                                flag = true;
                            }
                            break;
                        case 2:
                            if (getWorld()->canMoveInDirection(getX(), getY(), right))
                            {
                                setDirection(right);
                                flag = true;
                            }
                            break;
                        case 3:
                            if (getWorld()->canMoveInDirection(getX(), getY(), left))
                            {
                                setDirection(left);
                                flag = true;
                            }
                            break;
                        default:
                            break;
                    }
                }
                m_numSquaresToMoveInCurrentDirection = (rand() % 53) + 8; // 8 <= num <= 60
            }
            
            //7
            else if ( (  (  (getDirection() == right || getDirection() == left)  &&  (getWorld()->canMoveInDirection(getX(), getY(), up) || getWorld()->canMoveInDirection(getX(), getY(), down))  ) ||
                       (  (getDirection() == up || getDirection() == down)  &&  (getWorld()->canMoveInDirection(getX(), getY(), right) || getWorld()->canMoveInDirection(getX(), getY(), left)) ) ) &&
                     m_ticksToPerpTurn <= 0)
            {
                int x = rand() % 2;
                bool prob50 = (x < 1);
                
                if (getDirection() == right || getDirection() == left)
                {
                    if (prob50 && getWorld()->canMoveInDirection(getX(), getY(), up))
                        setDirection(up);
                    if (!prob50 && getWorld()->canMoveInDirection(getX(), getY(), down))
                        setDirection(down);
                    if (prob50 && !getWorld()->canMoveInDirection(getX(), getY(), up) && getWorld()->canMoveInDirection(getX(), getY(), down))
                        setDirection(down);
                    if (!prob50 && !getWorld()->canMoveInDirection(getX(), getY(), down) && getWorld()->canMoveInDirection(getX(), getY(), up))
                        setDirection(up);
                }
                
                if (getDirection() == up || getDirection() == down)
                {
                    if (prob50 && getWorld()->canMoveInDirection(getX(), getY(), right))
                        setDirection(right);
                    if (!prob50 && getWorld()->canMoveInDirection(getX(), getY(), left))
                        setDirection(left);
                    if (prob50 && !getWorld()->canMoveInDirection(getX(), getY(), right) && getWorld()->canMoveInDirection(getX(), getY(), left))
                        setDirection(left);
                    if (!prob50 && !getWorld()->canMoveInDirection(getX(), getY(), left) && getWorld()->canMoveInDirection(getX(), getY(), right))
                        setDirection(right);
                }
                m_ticksToPerpTurn = 200;
                m_numSquaresToMoveInCurrentDirection = (rand() % 53) + 8; // 8 <= num <= 60
            }
            
            // step 8
            if (getWorld()->canMoveInDirection(getX(), getY(), getDirection()))
                moveOneInDirection(getDirection());
            
            //9
            else m_numSquaresToMoveInCurrentDirection = 0;
        }
    }
}

// Regular Protester (- Protester - Actors) -------------------------------------------------------------------------------------------------

RegularProtester::RegularProtester(StudentWorld* studentWorld)
:Protester(IID_PROTESTER, studentWorld){}

RegularProtester::~RegularProtester(){}

bool RegularProtester::when4MoreFromFrackman()
{
    return false;
}

bool RegularProtester::getAnnoyed()
{
    if (!isDead() && !getLeaveField() && getHealth() < 391) // found gold
    {
        getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
        changeTicksToDoSomething(-getTicksToDoSomething());
        changeLeaveField(true);
        return true;
    }
    
    if (!isDead() && !getLeaveField() && getHealth() < 491)
    {
        getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
        changeTicksToDoSomething(-getTicksToDoSomething());
        changeLeaveField(true);
        return true;
    }
    
    if (!isDead() && !getLeaveField() && getHealth() < 492)
    {
        getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
        changeTicksToDoSomething(-getTicksToDoSomething());
        changeLeaveField(true);
        return true;
    }
    
    if (!isDead() && !getLeaveField() && getWorld()->annoyActorsWithinRadius(getX(), getY(), IID_WATER_SPURT, 3, 0) > 0 && getHealth() == 492) // hit by 3rd squirt
    {
        getWorld()->increaseScore(100);
        getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
        changeTicksToDoSomething(-getTicksToDoSomething());
        changeLeaveField(true);
        return true;
        
    }
    
    if (!isDead() && !getLeaveField() && getWorld()->annoyActorsWithinRadius(getX(), getY(), IID_WATER_SPURT, 3, 0) > 0 && getHealth() > 492) // hit by 1st or 2nd squirt
    {
        getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
        decHealth(4);
        changeTicksToDoSomething(getTicksAfterSquirt());
        return true;
        
    }
    return false;
}


// Hardcore Protester (- Protester - Actors) -------------------------------------------------------------------------------------------------

HardcoreProtester::HardcoreProtester(StudentWorld* studentWorld)
:Protester(IID_HARD_CORE_PROTESTER, studentWorld)
{
    int level = getWorld()->getLevel();
    m_ticksToStare = max(50, 100 - level * 10);
}

HardcoreProtester::~HardcoreProtester(){}

bool HardcoreProtester::when4MoreFromFrackman()
{
    int level = getWorld()->getLevel();
    int M = 16 + level * 2;
    
    if (countNMoves(getWorld()->pathToPoint(getX(), getY(), "Frackman")) <= M)
    {
        string path = getWorld()->pathToPoint(getX(), getY(), "Frackman");
        
        char direction = path[path.length()-2];
        
        switch (direction) {
            case 'U':
                setDirection(up);
                moveTo(getX(), getY()+1);
                return true;
                break;
            case 'D':
                setDirection(down);
                moveTo(getX(), getY()-1);
                return true;
                break;
            case 'L':
                setDirection(left);
                moveTo(getX()-1, getY());
                return true;
                break;
            case 'R':
                setDirection(right);
                moveTo(getX()+1, getY());
                return true;
                break;
            default:
                break;
        }
    }
    return false;
}

bool HardcoreProtester::getAnnoyed()
{
    
    if (!isDead() && !getLeaveField() && getHealth() < 391) // found gold
    {
        getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
        changeTicksToDoSomething(m_ticksToStare);
        decHealth(-300);
        return true;
    }
    
    if (!isDead() && !getLeaveField() && getHealth() < 491)
    {
        getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
        changeTicksToDoSomething(-getTicksToDoSomething());
        changeLeaveField(true);
        return true;
    }
    
    if (!isDead() && !getLeaveField() && getWorld()->annoyActorsWithinRadius(getX(), getY(), IID_WATER_SPURT, 3, 0) > 0 && getHealth() == 491) // hit by 10th squirt
    {
        getWorld()->increaseScore(100);
        getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
        changeTicksToDoSomething(-getTicksToDoSomething());
        changeLeaveField(true);
        return true;
        
    }
    
    if (!isDead() && !getLeaveField() && getWorld()->annoyActorsWithinRadius(getX(), getY(), IID_WATER_SPURT, 3, 0) > 0 && getHealth() > 491) // hit by 1st to 9th squirt
    {
        getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
        decHealth(1);
        changeTicksToDoSomething(getTicksAfterSquirt());
        return true;
    }
    return false;
}


// Object (- Actors) (OilBarrel, GoldNugget, SonarKit, Waterpool) -------------------------------------------------------------------------------------------------

Object::Object(int imageID, int startX, int startY, StudentWorld* studentWorld, Direction dir, double size, unsigned int depth)
:Actor(imageID, startX, startY, studentWorld, dir, size, depth)
{
    m_appear = false;
    m_state = 0;
}

Object::~Object(){}

void Object::makeAppear()
{
    m_appear = true;
}

bool Object::isAppear()
{
    return m_appear;
}

int Object::getState()
{
    return m_state;
}

void Object::changeState(int x)
{
    m_state += x;
}

void Object::incScore(int x)
{
    m_incScore = x;
}

void Object::changeSound(int x)
{
    m_sound = x;
}

int Object::getSound()
{
    return m_sound;
}

int Object::getIncScore()
{
    return m_incScore;
}

// Pickups (- Object - Actors)

Pickups::Pickups(int imageID, int startX, int startY, StudentWorld* studentWorld)
:Object(imageID, startX, startY, studentWorld, right, 1.0, 2){}

Pickups::~Pickups(){}

void Pickups::doSomething()
{
    if (isDead())
        return;
    
    if (!isAppear() && getWorld()->annoyActorsWithinRadius(getX(), getY(), IID_PLAYER, 4, 0) > 0)
    {
        setVisible(true);
        makeAppear();
        return;
    }
    
    if (getWorld()->annoyActorsWithinRadius(getX(), getY(), IID_PLAYER, 3, 0) > 0)
    {
        decHealth(100);
        getWorld()->GameWorld::playSound(getSound());
        getWorld()->GameWorld::increaseScore(getIncScore());
        changeCount();
    }
};



// Powers (- Object - Actors)

Powers::Powers(int imageID, int startX, int startY, StudentWorld* studentWorld)
:Object(imageID, startX, startY, studentWorld, right, 1.0, 2){}

Powers::~Powers(){}


void Powers::doSomething()
{
    if (isDead())
        return;
    
    if (getWorld()->annoyActorsWithinRadius(getX(), getY(), IID_PLAYER, 3, 0) > 0)
    {
        decHealth(100);
        getWorld()->GameWorld::playSound(SOUND_GOT_GOODIE);
        getWorld()->GameWorld::increaseScore(getIncScore());
        changeCount();
    }
    
    if (getState() > 0)
    {
        changeState(-1);
        return;
    }
    
    if (getState() <= 0)
        decHealth(100);
};

// OilBarrel (- Pickups - Object - Actors) ------------------------------------------------------------------------------------------------


OilBarrel::OilBarrel(int startX, int startY, StudentWorld* studentWorld)
:Pickups(IID_BARREL, startX, startY, studentWorld)
{
    setVisible(false);
    incScore(1000);
    changeSound(SOUND_FOUND_OIL);
};

OilBarrel::~OilBarrel(){}

void OilBarrel::changeCount()
{
    getWorld()->SWdecBarrel();
}

// GoldNugget (- Pickups - Object - Actors) ------------------------------------------------------------------------------------------------

GoldNugget::GoldNugget(int startX, int startY, StudentWorld* studentWorld)
:Pickups(IID_GOLD, startX, startY, studentWorld)
{
    setVisible(false);
    incScore(10);
    changeSound(SOUND_GOT_GOODIE);
};

GoldNugget::~GoldNugget()
{};

void GoldNugget::changeCount()
{
    getWorld()->SWchangeGoldCount(1);
}


//// Bribe (- Object - Actors) ------------------------------------------------------------------------------------------------

Bribe::Bribe(int startX, int startY, StudentWorld* studentWorld)
:Object(IID_GOLD, startX, startY, studentWorld, right, 1.0, 2)
{
    setVisible(true);
    changeState(100);
    incScore(25);
};

Bribe::~Bribe(){}

void Bribe::doSomething()
{
    if (isDead())
        return;
    
    if (getState() > 0)
        changeState(-1);
    
    if (getState() == 0)
        decHealth(100);
    
    if (getWorld()->annoyActorsWithinRadius(getX(), getY(), IID_PROTESTER, 3, 0) > 0 ||
        getWorld()->annoyActorsWithinRadius(getX(), getY(), IID_HARD_CORE_PROTESTER, 3, 0) > 0)
    {
        decHealth(100);
        getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
        if (getWorld()->annoyActorsWithinRadius(getX(), getY(), IID_PROTESTER, 3, 300) > 0) {
            getWorld()->increaseScore(25);
        }
        if (getWorld()->annoyActorsWithinRadius(getX(), getY(), IID_HARD_CORE_PROTESTER, 3, 300) > 0) {
            getWorld()->increaseScore(50);
        }
        return;
    }
    
}

// SonarKit (- Powers - Object - Actors) ------------------------------------------------------------------------------------------------

SonarKit::SonarKit(StudentWorld* studentWorld)
:Powers(IID_SONAR, 0, 60, studentWorld)
{
    int level = getWorld()->getLevel();
    changeState(max(100, 300 - 10 * level));
    incScore(75);
}

SonarKit::~SonarKit(){}

void SonarKit::changeCount()
{
    getWorld()->SWchangeSonarCount(1);
}



// WaterPool (- Powers - Object - Actors) ------------------------------------------------------------------------------------------------

WaterPool::WaterPool(int x, int y, StudentWorld* studentWorld)
:Powers(IID_WATER_POOL, x, y, studentWorld)
{
    int level = getWorld()->getLevel();
    changeState(max(100, 300 - 10 * level));
    incScore(100);
}

WaterPool::~WaterPool(){}

void WaterPool::changeCount()
{
    getWorld()->SWchangeSquirtCount(1);
}









































