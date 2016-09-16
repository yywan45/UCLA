#include "StudentWorld.h"
#include "Actor.h"
#include <string>
#include <algorithm>
#include <random>
#include <queue>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Init function -----------------------------------------------------------------------------------------------------------------------------

int StudentWorld::init()
{
    // Frackman
    m_frackman = new Frackman(this);
    
    // Dirt
    for (int x = 0; x < 64; x++)
    {
        for (int y = 0; y < 64; y++)
        {
            if (x >= 30 && x <= 33 && y >= 4 && y <= 59)
            {
                m_dirt[x][y] = nullptr;
                continue;
            }
            if (y >= 60 && y < 64)
            {
                m_dirt[x][y] = nullptr;
                continue;
            }
            m_dirt[x][y] = new Dirt(x, y, this);
        }
    }
    
    int level = getLevel();
    
    time_t t;
    srand((unsigned) time(&t));
    
    // Boulders
    for (int B = min(level / 2 + 2 , 6); B > 0; B--)
    {
        int x = rand() % 60;
        int y = (rand() % 36) + 20;
        
        while ( ((x >= 26 && x <= 33) && (y > 0 && y <= 56)) || !checkDistribution(x, y) )
        {
            x = rand() % 60;
            y = (rand() % 36) + 20; // rand() % 56;
        }
        
        for (int i = x; i < x+4; i++)
        {
            for (int j = y; j < y+4; j++)
            {
                delete m_dirt[i][j];
                m_dirt[i][j] = nullptr;
            }
        }
        Boulders* newBoulder = new Boulders(x, y, this);
        m_actors.push_back(newBoulder);
    }
    

    // Barrels of Oil
    for (int L = min(2 + level, 20); L > 0; L--)
    {
        int x = rand() % 60;
        int y = (rand() % 36) + 20;
        
        while ( ((x >= 26 && x <= 33) && (y > 0 && y <= 56)) || !checkDistribution(x, y) )
        {
            x = rand() % 60;
            y = (rand() % 36) + 20; // rand() % 56;
        }
        OilBarrel* newBarrel = new OilBarrel(x, y, this);
        m_actors.push_back(newBarrel);
    }
    
    
    // Gold Nuggets
    for (int G = max(5 - level / 2, 2); G > 0; G--)
    {
        int x = rand() % 60;
        int y = (rand() % 36) + 20;
        
        while ( ((x >= 26 && x <= 33) && (y > 0 && y <= 56)) || !checkDistribution(x, y) )
        {
            x = rand() % 60;
            y = (rand() % 36) + 20; // rand() % 56;
        }
        GoldNugget* newNugget = new GoldNugget(x, y, this);
        m_actors.push_back(newNugget);
    }
    
    m_ticksBeforeNewProtester = 0;
    
    return GWSTATUS_CONTINUE_GAME;

}

// Move function -----------------------------------------------------------------------------------------------------------------------------

int StudentWorld::move()
{
    
    setDisplayText();
    
    while (!m_tempActors.empty())
    {
        m_actors.push_back(m_tempActors[0]);
        m_tempActors.erase(m_tempActors.begin());
    }
    
    // Add protesters -------------------
    
    int level = getLevel();
    int T = max(25, 200 - level);
    int P = min(15, 2 + level * (3 / 2));
    
    int probabilityOfHardcore = min(90, level * 10 + 30);
    
    time_t t;
    srand((unsigned) time(&t));
    bool newHardcore = (rand() % 100) < probabilityOfHardcore;

    
    if (m_ticksBeforeNewProtester > 0)
        m_ticksBeforeNewProtester--;
    
    int noOfProtestersOnField = 0;
    
    for (int i = 0; i < m_actors.size(); i++)
    {
        if (m_actors[i]->getID() == IID_HARD_CORE_PROTESTER || m_actors[i]->getID() == IID_PROTESTER)
        {
            noOfProtestersOnField++;
        }
    }
    
    if (m_ticksBeforeNewProtester <= 0 && noOfProtestersOnField < P)
    {
        if (newHardcore)
        {
            HardcoreProtester* newpro = new HardcoreProtester(this);
            m_actors.push_back(newpro);
        }
        else
        {
            RegularProtester* newpro = new RegularProtester(this);
            m_actors.push_back(newpro);
        }
        m_ticksBeforeNewProtester = T;
    }
    
    // Add sonar and water pools -------------------
    
    bool newGoodie = (rand() % (getLevel() * 25 + 300)) < 1;
    bool newWaterNotSonar = (rand() % 5) < 4;
    
    if (newGoodie && newWaterNotSonar)
    {
        time_t t;
        srand((unsigned) time(&t));
        
        int x = rand() % 64;
        int y = rand() % 64;
        
        while (checkForActorCoord(x, x+3, 0, 64, y, y+3, 0, 64, IID_DIRT) ||
               checkForActorCoord(x, x+3, 0, 64, y, y+3, 0, 64, IID_BOULDER) ||
               checkForActorCoord(x, x+3, 0, 64, y, y+3, 0, 64, IID_WATER_SPURT) ||
               checkForActorCoord(x, x+3, 0, 64, y, y+3, 0, 64, IID_PLAYER) ||
               checkForActorCoord(x, x+3, 0, 64, y, y+3, 0, 64, IID_PROTESTER) ||
               checkForActorCoord(x, x+3, 0, 64, y, y+3, 0, 64, IID_HARD_CORE_PROTESTER) ||
               checkForActorCoord(x, x+3, 0, 64, y, y+3, 0, 64, IID_BARREL) ||
               checkForActorCoord(x, x+3, 0, 64, y, y+3, 0, 64, IID_GOLD) ||
               checkForActorCoord(x, x+3, 0, 64, y, y+3, 0, 64, IID_SONAR))
        {
            x = rand() % 64;
            y = rand() % 64;
        }
        WaterPool* newPool = new WaterPool(x, y, this);
        m_actors.push_back(newPool);
    }
    
    if (newGoodie && !newWaterNotSonar)
    {
        SonarKit* newSonar = new SonarKit(this);
        m_actors.push_back(newSonar);
    }
    
    // Actors doSomething -------------------
    
    if (!m_frackman->isDead())
        m_frackman->doSomething();
    
    for (int i = 0; i < m_actors.size(); i++)
    {
        if (!m_actors[i]->isDead()) // TODO: add this? m_actors[i] != nullptr &&
        {
            m_actors[i]->doSomething();
            if (m_frackman->isDead())
            {
                decLives();
                return GWSTATUS_PLAYER_DIED;
            }
            if (m_frackman->getNumberOfBarrelsRemainingToBePickedUp() == 0)
            {
                playSound(SOUND_FINISHED_LEVEL);
                return GWSTATUS_FINISHED_LEVEL;
            }
            
        }
    }

    // delete dead actors -------------------

    for (int i = 0; i < m_actors.size(); i++)
    {
        if (m_actors[i]->isDead())
        {
            delete m_actors[i];
            m_actors.erase(m_actors.begin()+i);
            i--;
        }
    }
    
    if (m_frackman->isDead())
    {
        delete m_frackman;
        m_frackman = nullptr;
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    
    // if player finished level -------------------
    
    if (m_frackman->getNumberOfBarrelsRemainingToBePickedUp() == 0)
    {
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }

    return GWSTATUS_CONTINUE_GAME;
}

// Cleanup function -----------------------------------------------------------------------------------------------------------------------------

void StudentWorld::cleanUp()
{
    delete m_frackman;
    m_frackman = nullptr;
    
    for (int x = 0; x < 64; x++)
    {
        for (int y = 0; y < 60; y++)
        {
            if (m_dirt[x][y] != nullptr)
            {
                delete m_dirt[x][y];
                m_dirt[x][y] = nullptr;
            }
        }
    }
    
    for (int i = 0; i < m_actors.size(); i++)
    {
        delete m_actors[i];
        m_actors.erase(m_actors.begin()+i);
        i--;
    }
}

// Destructor -----------------------------------------------------------------------------------------------------------------------------

StudentWorld::~StudentWorld()
{
    
    delete m_frackman;
        m_frackman = nullptr;
    
    for (int x = 0; x < 64; x++)
    {
        for (int y = 0; y < 60; y++)
        {
            if (m_dirt[x][y] != nullptr)
            {
                delete m_dirt[x][y];
                m_dirt[x][y] = nullptr;
            }
        }
    }
    
    for (int i = 0; i < m_actors.size(); i++)
    {
        delete m_actors[i];
        m_actors.erase(m_actors.begin()+i);
        i--;
    }
}

// Set Display Text ------------------------------------------------------------------------------------------------

void StudentWorld::setDisplayText()
{
    string s =
    "Scr: " + insertChar(getScore(), 6, '0') +
    "  Lvl: " + insertChar(getLevel(), 2, '0') +
    "  Lives: " + to_string(getLives()) +
    "  Hlth: " + insertChar(m_frackman->getHealth(), 2, ' ') +
    "%  Wtr: " + insertChar(m_frackman->getSquirtsLeft(), 2, ' ') +
    "  Gld: " + insertChar(m_frackman->getPlayerGoldCount(), 2, ' ') +
    "  Sonar: " + insertChar(m_frackman->getPlayerSonarChargeCount(), 2, ' ') +
    "  Oil Left: " + insertChar(m_frackman->getNumberOfBarrelsRemainingToBePickedUp(), 2, ' ');

    setGameStatText(s);
}

string StudentWorld::insertChar(int stat, int digits, char insert)
{
    int statDigits = to_string(stat).length();
    int insertNumber = digits - statDigits;
    
    string s = "";
    
    for (int i = 0; i < insertNumber; i++)
        s += insert;
    
    return s += to_string(stat);
}

void StudentWorld::SWchangeSquirtCount(int x)
{
    m_frackman->changeSquirtCount(x);
}

void StudentWorld::SWdecBarrel()
{
    m_frackman->decBarrel();
}

void StudentWorld::SWchangeGoldCount(int x)
{
    m_frackman->changeGoldCount(x);
}

void StudentWorld::SWchangeSonarCount(int x)
{
    m_frackman->changeGoldCount(x);
}


// Creating New Actors -----------------------------------------------------------------------------------------------------------------------------

void StudentWorld::addSquirt(int x, int y, GraphObject::Direction dir)
{
    Squirt* newSquirt = new Squirt(x, y, this, dir);
    m_tempActors.push_back(newSquirt);
}

void StudentWorld::addBribe(int x, int y)
{
    Bribe* newBribe = new Bribe(x, y, this);
    m_tempActors.push_back(newBribe);
}


// Helper private functions ------------------------------------------------------------------------------------------------------------------------

bool StudentWorld::checkDistribution(int x, int y)
{
    if (m_actors.empty())
        return true;
    
    for (int i = 0; i < m_actors.size(); i++)
    {
        if ( sqrt(double(pow(m_actors[i]->getX() - x, 2.0) + pow(m_actors[i]->getY() - y, 2.0))) <= 6 &&
            (m_actors[i]->getID() == IID_BOULDER || m_actors[i]->getID() == IID_BARREL || m_actors[i]->getID() == IID_GOLD))
            return false;
    }
    return true;
}

string StudentWorld::stringRowField(int y)
{
    string row = "";
    for (int i = 0; i < 64; i++)
    {
        if (checkForActorCoord(i, i+3, 0, 63, y, y+3, 0, 63, IID_DIRT) || i < 0 || i > 60 || y > 60 || y < 0 ||
            annoyActorsWithinRadius(i, y, IID_BOULDER, 3, 0) > 0)
        {
            row += 'X';
        }
        else row += '.';
    }
    return row;
}

void StudentWorld::stringArrayField()
{
    for (int i = 63; i >= 0; i--)
        m_field[i] = stringRowField(i);
    
}


// Helper public functions -----------------------------------------------------------------------------------------------------------------------------

void StudentWorld::removeDirt(int x, int y)
{
    for (int i = x; i < x+4; i++)
    {
        for (int j = y; j < y+4; j++)
        {
            if (m_dirt[i][j] != nullptr)
                playSound(SOUND_DIG);
            delete m_dirt[i][j];
            m_dirt[i][j] = nullptr;
        }
    }
}

// check for frackman, actors, dirt
bool StudentWorld::checkForActorCoord(int xmin, int xmax, int xminBound, int xmaxBound, int ymin, int ymax, int yminBound, int ymaxBound, int imageID)
{
    while (xmin < xminBound)
        xmin++;
    while (ymin < yminBound)
        ymin++;
    while (xmax > xmaxBound)
        xmax--;
    while (ymax > ymaxBound)
        ymax--;
    
    bool flag = false;
    
    if (m_frackman->getX() >= xmin && m_frackman->getX() <= xmax && m_frackman->getY() >= ymin && m_frackman->getY() <= ymax && m_frackman->getID() == imageID)
        flag = true;
    
    for (int i = 0; i < m_actors.size(); i++)
    {
        if (m_actors[i]->getX() >= xmin && m_actors[i]->getX() <= xmax && m_actors[i]->getY() >= ymin && m_actors[i]->getY() <= ymax && m_actors[i]->getID() == imageID)
            flag = true;
    }
    
    for (int x = xmin; x <= xmax; x++)
    {
        for (int y = ymin; y <= ymax; y++)
        {
            if (m_dirt[x][y] != nullptr && imageID == IID_DIRT)
                flag = true;

        }
    }

    return flag;
}

int StudentWorld::annoyActorsWithinRadius(int x, int y, int imageID, int radius, int hitPoints)
{
    int flag = 0;
    
    if (imageID == IID_PLAYER)
        if (sqrt(double(pow(m_frackman->getX()-x, 2.0) + pow(m_frackman->getY()-y, 2.0))) <= radius)
        {
            m_frackman->decHealth(hitPoints);
            flag++;
        }
    
    for (int i = 0; i < m_actors.size(); i++)
    {
        if (sqrt(double(pow(m_actors[i]->getX()-x, 2.0) + pow(m_actors[i]->getY()-y, 2.0))) <= radius &&
            (m_actors[i]->getID() == imageID))
        {
            m_actors[i]->decHealth(hitPoints);
            flag++;
        }
    }
    return flag;
}



void StudentWorld::setVisibleWithinRadius(int x, int y, int radius)
{
    for (int i = 0; i < m_actors.size(); i++)
    {
        if (sqrt(double(pow(m_actors[i]->getX()-x, 2.0) + pow(m_actors[i]->getY()-y, 2.0))) <= radius &&
            (m_actors[i]->getID() == IID_GOLD || m_actors[i]->getID() == IID_BARREL) )
            m_actors[i]->setVisible(true);
    }
    
}


GraphObject::Direction StudentWorld::directionToFaceFrackman(int x, int y)
{
    if (m_frackman->getX() < x && m_frackman->getY()-4 <= y && m_frackman->getY()+4 >= y)
        return GraphObject::left;
    if (m_frackman->getX() > x && m_frackman->getY()-4 <= y && m_frackman->getY()+4 >= y)
        return GraphObject::right;
    if (m_frackman->getY() > y && m_frackman->getX()-4 <= x && m_frackman->getX()+4 >= x)
        return GraphObject::up;
    if (m_frackman->getY() < y && m_frackman->getX()-4 <= x && m_frackman->getX()+4 >= x)
        return GraphObject::down;
    return GraphObject::none;
    
}

bool StudentWorld::canMoveInDirection(int x, int y, GraphObject::Direction dir)
{
    if (dir == GraphObject::up && y < 60 &&
        !checkForActorCoord(x, x+3, 0, 63, y+4, y+4, 0, 63, IID_DIRT) &&
        annoyActorsWithinRadius(x, y+1, IID_BOULDER, 3, 0) == 0)
        return true;
    if (dir == GraphObject::down && y > 0 &&
        !checkForActorCoord(x, x+3, 0, 63, y-1, y-1, 0, 63, IID_DIRT) &&
        annoyActorsWithinRadius(x, y+1, IID_BOULDER, 3, 0) == 0)
        return true;
    if (dir == GraphObject::right && x < 60 &&
        !checkForActorCoord(x+4, x+4, 0, 63, y, y+3, 0, 63, IID_DIRT) &&
        annoyActorsWithinRadius(x+1, y, IID_BOULDER, 3, 0) == 0)
        return true;
    if (dir == GraphObject::left && x > 0 &&
        !checkForActorCoord(x-1, x-1, 0, 63, y, y+3, 0, 63, IID_DIRT) &&
        annoyActorsWithinRadius(x-1, y, IID_BOULDER, 3, 0) == 0)
        return true;
    return false;
    
        
}


string StudentWorld::pathToPoint(int startX, int startY, string exitOrFrackman)
{
    int endX = 60;
    int endY = 60;
    
    if (exitOrFrackman == "Frackman")
    {
        endX = m_frackman->getX();
        endY = m_frackman->getY();
    }
    
    class Coord
    {
    public:
        Coord(int xx, int yy) : m_x(xx), m_y(yy) {}
        int x() const { return m_x; }
        int y() const { return m_y; }
    private:
        int m_x;
        int m_y;
    };
    
    stringArrayField();
    
    queue<Coord> coordQueue;
    Coord start(startX,startY);
    
    coordQueue.push(start);
    m_field[startY][startX] = 'S';
    
    string path = "";
    
    while (!coordQueue.empty())
    {
        Coord current = coordQueue.front();
        coordQueue.pop();
        
        if (current.x() == endX && current.y() == endY)
        {
            path += m_field[endY][endX];
            int pathCurrentX = endX;
            int pathCurrentY = endY;
            
            while (m_field[pathCurrentY][pathCurrentX] != 'S')
            {
                if (m_field[pathCurrentY][pathCurrentX] == 'U')
                {
                    pathCurrentY--;
                    path += m_field[pathCurrentY][pathCurrentX];
                }
                if (m_field[pathCurrentY][pathCurrentX] == 'D')
                {
                    pathCurrentY++;
                    path += m_field[pathCurrentY][pathCurrentX];
                }
                if (m_field[pathCurrentY][pathCurrentX] == 'R')
                {
                    pathCurrentX--;
                    path += m_field[pathCurrentY][pathCurrentX];
                }
                if (m_field[pathCurrentY][pathCurrentX] == 'L')
                {
                    pathCurrentX++;
                    path += m_field[pathCurrentY][pathCurrentX];
                }
                
            }
            return path;
        }
    
        if (current.y()+1 <= 60 && m_field[current.y()+1][current.x()] == '.') // UP
        {
            Coord next(current.x(),current.y()+1);
            coordQueue.push(next);
            m_field[current.y()+1][current.x()] = 'U';
        }

        if (current.y()-1 >= 0 && m_field[current.y()-1][current.x()] == '.') // DOWN
        {
            Coord next(current.x(),current.y()-1);
            coordQueue.push(next);
            m_field[current.y()-1][current.x()] = 'D';
        }
        
        if (current.x()+1 <= 60 && m_field[current.y()][current.x()+1] == '.') // RIGHT
        {
            Coord next(current.x()+1,current.y());
            coordQueue.push(next);
            m_field[current.y()][current.x()+1] = 'R';
        }
        
        if (current.x()-1 >= 0 && m_field[current.y()][current.x()-1] == '.') // LEFT
        {
            Coord next(current.x()-1,current.y());
            coordQueue.push(next);
            m_field[current.y()][current.x()-1] = 'L';
        }
    }
    return "error";
}

// functions that didn't come into use in the end

//bool StudentWorld::existsStraightPathToFrackman(int startX, int startY)
//{
//    if (directionToFaceFrackman(startX, startY) == GraphObject::up &&
//        !checkForActorCoord(startX, startX+3, 0, 63, startY, m_frackman->getY(), 0, 63, IID_DIRT))
//    {
//        for (int i = startY; i <= m_frackman->getY(); i++)
//        {
//            if (checkForActorRadius(startX, i, IID_BOULDER, 3))
//            {
//                return false;
//            }
//        }
//        return true;
//    }
//
//    if (directionToFaceFrackman(startX, startY) == GraphObject::down &&
//        !checkForActorCoord(startX, startX+3, 0, 63, m_frackman->getY(), startY, 0, 63, IID_DIRT))
//    {
//        for (int i = m_frackman->getY(); i <= startY; i++)
//        {
//            if (checkForActorRadius(startX, i, IID_BOULDER, 3))
//            {
//                return false;
//            }
//        }
//        return true;
//    }
//
//    if (directionToFaceFrackman(startX, startY) == GraphObject::right &&
//        !checkForActorCoord(startX, m_frackman->getX(), 0, 63, startY, startY+3, 0, 63, IID_DIRT))
//    {
//        for (int i = startX; i <= m_frackman->getX(); i++)
//        {
//            if (checkForActorRadius(i, startY, IID_BOULDER, 3))
//            {
//                return false;
//            }
//        }
//        return true;
//    }
//
//    if (directionToFaceFrackman(startX, startY) == GraphObject::left &&
//        !checkForActorCoord(m_frackman->getX(), startX, 0, 63, startY, startY+3, 0, 63, IID_DIRT))
//    {
//        for (int i = m_frackman->getX(); i <= startX; i++)
//        {
//            if (checkForActorRadius(i, startY, IID_BOULDER, 3))
//            {
//                return false;
//            }
//        }
//        return true;
//    }
//    return false;
//
//}

//
//
//bool StudentWorld::annoyAgentsAt(int xmin, int xmax, int ymin, int ymax, int annoyPoints)
//{
//    bool flag = false;
//
//    if (m_frackman->getX() >= xmin && m_frackman->getX() <= xmax && m_frackman->getY() >= ymin && m_frackman->getY() <= ymax)
//    {
//        m_frackman->decHealth(annoyPoints);
//        flag = true;
//    }
//
//    for (int i = 0; i < m_actors.size(); i++)
//    {
//        if (m_actors[i]->getX() >= xmin && m_actors[i]->getX() <= xmax && m_actors[i]->getY() >= ymin && m_actors[i]->getY() <= ymax &&
//            (m_actors[i]->getID() == IID_PLAYER || m_actors[i]->getID() == IID_PROTESTER || m_actors[i]->getID() == IID_HARD_CORE_PROTESTER))
//        {
//            m_actors[i]->decHealth(annoyPoints);
//            flag = true;
//        }
//    }
//    return flag;
//}

//// check for frackman, actors, NOT dirt
//// TODO: delete this function and sub everything for annoy actors within radius function
//bool StudentWorld::checkForActorRadius(int x, int y, int imageID, int radius)
//{
//    bool flag = false;
//
//    if (imageID == IID_PLAYER)
//        if (sqrt(double(pow(m_frackman->getX()-x, 2.0) + pow(m_frackman->getY()-y, 2.0))) <= radius)
//            flag = true;
//
//    for (int i = 0; i < m_actors.size(); i++)
//    {
//        if ( m_actors[i]->getID() == imageID )
//        {
//            if(sqrt(double(pow(m_actors[i]->getX()-x, 2.0) + pow(m_actors[i]->getY()-y, 2.0))) <= radius)
//               flag = true;
//        }
//    }
//    return flag;
//}

