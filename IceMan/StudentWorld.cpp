// contains your StudentWorld class implementation

#include "StudentWorld.h"
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
using namespace std;

Coordinates randInanimate() {

    Coordinates randCoord;
    int x, y;
    do {
        x = (rand() / 1024 % 16) * 4 + 3;
    } while (x > 26 && x < 34);
    y = (rand() / 1024 % 14) * 4 + 3;
    randCoord.x = x;
    randCoord.y = y;
    return randCoord;
}

bool operator== (const Coordinates& a, const Coordinates& b)
{
    return a.x == b.x && a.y == b.y;
}

GameWorld* createStudentWorld(string assetDir)
{
    srand((unsigned)time(0));
    return new StudentWorld(assetDir);
}

int StudentWorld::init()
{
    //create the current level�s oil field and populating
    //it with Ice, Boulders, Barrels of Oil, and Gold Nuggets
    //TODO:
    numBarrelsRemaining = 2 + getLevel();
    numGold = 2 + getLevel();
    numBoulders = 2 + getLevel();
    numProtestors = 3 + getLevel();
    numHCProtestors = 2 + getLevel();

    //clear the random coords sets
    randomCoords.clear();
    randomWaterCoords.clear();

    for (int i = 0; i < 64; i++)
    {
        m_boulders[i] = nullptr;
    }

    int setsize = 0;
    while (setsize < numBarrelsRemaining + numGold + numBoulders + numBoulders)
    {
        randomCoords.insert(randInanimate());
        setsize = randomCoords.size();
    }

    for (int x = 0; x < VIEW_WIDTH; x++)
    {
        // ICE FIELD:
        // the hole in the middle
        if (x == 30 || x == 31 || x == 32 || x == 33)
        {
            for (int y = 0; y < 4; y++)
            {
                m_ice[x][y] = new Ice(this, x, y);
            }
            for (int y = 4; y < VIEW_HEIGHT;y++)
            {
                m_ice[x][y] = nullptr;
            }

        }
        else
        {
            for (int y = 0; y < VIEW_HEIGHT - 4; y++)
            {
                m_ice[x][y] = new Ice(this, x, y);
            }
            for (int y = VIEW_HEIGHT - 4; y < VIEW_HEIGHT;y++)
            {
                m_ice[x][y] = nullptr;
            }
        }

    }

    for (int x = 0; x < VIEW_WIDTH; x++) {

        // GOLD, OIL, WATER IN THE FIELD, INITIALIZE AT NULLPTR:
        // 4x4s cant spawn past x=60 and y=56
        for (int y = 0; y < VIEW_HEIGHT; y++) {
            m_inanimate[x][y] = nullptr;
        }
    }

    int count = 0;
    for (Coordinates coord : randomCoords)
    {

        if (count < numGold)
        {
            m_inanimate[coord.x][coord.y] = new GoldNugget(this, coord.x, coord.y, false, false, true, true, false);
        }

        else if (count < numGold + numBarrelsRemaining)
        {
            m_inanimate[coord.x][coord.y] = new OilBarrel(this, coord.x, coord.y);

        }
        else if (count < numGold + numBarrelsRemaining + numBoulders)
        {
            //boulder, not finished--just to see if we can spawn it in and clear the ice

            m_boulders[count - numGold - numBarrelsRemaining] = new Boulder(this, coord.x, coord.y);
            //clear the ice
            for (int xtemp = coord.x; xtemp < coord.x + 4;xtemp++)
            {
                for (int ytemp =
                    coord.y; ytemp < coord.y + 4;ytemp++)
                {
                    delete m_ice[xtemp][ytemp]; //exception thrown
                    m_ice[xtemp][ytemp] = nullptr;
                }
            }

        }

        else
        {
            break;
        }
        count++;



    }

    //m_inanimate[30][36] = new WaterPool(this, 30, 36);

    // REGULAR PROTESTORS
    for (size_t i = 0;i < numProtestors;i++)
    {
        m_protestor[i] = new RegularProtester(this, 60, 60, IID_PROTESTER);
    }
    // HARDCORE PROTESTORS
    for (size_t i = numProtestors;i < numProtestors + numHCProtestors;i++)
    {
        m_protestor[i] = new HardcoreProtester(this, 60, 60, IID_HARD_CORE_PROTESTER);
    }
    for (size_t i = numProtestors + numHCProtestors; i < 64;i++)
    {
        m_protestor[i] = nullptr;
    }

    // ICEMAN:
    m_Iceman = new IceMan(this, 30, 60);
    setDisplayText();

    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::setDisplayText()
{
    int level = getLevel();
    int lives = getLives();
    // NEED GET HEALTH FOR ICEMAN
    //health is percentage
    int health = (m_Iceman->getHitPoints() / 10) * 100;
    int squirts = m_Iceman->getWater();
    int gold = m_Iceman->getGold();
    int barrelsLeft = getNumBarrelsRemaining();
    int sonar = m_Iceman->getSonar();
    std::stringstream score;
    score << setw(6) << setfill('0') << getScore();
    string scoreVal = score.str();
    //  int score = getScore(); //getScore is from gameworld
      // Next, create a string from your statistics, of the form:
      // Lvl: 52 Lives: 3 Hlth: 80% Wtr: 20 Gld: 3 Oil Left: 2 Sonar: 1 Scr: 321000
  //    string s = someFunctionYouUseToFormatThingsNicely(level, lives, health,
  //    squirts, gold, barrelsLeft, sonar, score);


    std::string s = "Lvl: " + to_string(level) + " Lives: " + to_string(lives) + " Hlth: " + to_string(health) + "% Wtr: " + to_string(squirts) + " Gld: " + to_string(gold) + " Oil Left: " + to_string(barrelsLeft) + " Sonar: " + to_string(sonar) + " Scr: " + scoreVal;
    //   Finally, update the display text at the top of the screen with your
     //  newly created stats



     // string s = "testing testing testing";
    setGameStatText(s);
}

//6/2 changes
int StudentWorld::move()
{
    m_Iceman->move();
    //since ice block is only 1 sqaure big and ice man is 4 squares, we increment 4 times to get rid of 4 block of ice so
    //that iceMan can go thru, ice disappears as iceMan walks thru it
    for (int x = m_Iceman->getX(); x < m_Iceman->getX() + 4;x++)
    {
        for (int y = m_Iceman->getY(); y < m_Iceman->getY() + 4;y++)
        {
            delete m_ice[x][y];
            m_ice[x][y] = nullptr;
        }
    }
    for (int x = 0; x < VIEW_WIDTH;x++)
    {
        for (int y = 0; y < VIEW_HEIGHT; y++)
        {
            if (m_inanimate[x][y] != nullptr)
            {
                m_inanimate[x][y]->move();
                if (!m_inanimate[x][y]->isAlive())
                {
                    delete m_inanimate[x][y];
                    m_inanimate[x][y] = nullptr;
                }
            }
        }
    }

    int xmin = m_Iceman->getX() - 3;
    int ymin = m_Iceman->getY() - 3;
    for (int x = max(0, xmin); x < m_Iceman->getX() + 4;x++)
    {
        for (int y = max(0, ymin); y < m_Iceman->getY() + 4;y++)
        {
            for (int i = 0; i < numProtestors + numHCProtestors; i++) {
                if (m_protestor[i] != nullptr && m_protestor[i]->getX() == x && m_protestor[i]->getY() == y) {
                    //this means that there is a protestor that is touching the iceman
                    m_Iceman->annoy(2);
                    break;
                }
            }
        }
    }

    // BOULDERS:
    for (int i = 0; i < 64; i++)
    {
        if (m_boulders[i] != nullptr)
        {
            m_boulders[i]->move();
            if (!m_boulders[i]->isAlive())
            {
                delete m_boulders[i];
                m_boulders[i] = nullptr;
            }
        }
    }

    for (int i = 0; i < numProtestors + numHCProtestors; i++)
        if (m_protestor[i] != nullptr) {
            //this means that there is a protestor that is touching the iceman
            m_protestor[i]->move();
        }

    // Spawn SonarKit
    if ((rand() / 4) % 5121 == 0)
    {
        if (m_inanimate[0][60] == nullptr)
        {
            m_inanimate[0][60] = new SonarKit(this, 0, 60);
        }
    }

    // Randomly collect cordinates of iceman, guarantees a 4x4 space to spawn water pools
    if ((rand() / 4) % 307 == 2)
    {
        randomWaterCoords.insert(Coordinates{ m_Iceman->getX(), m_Iceman->getY() });
    }

    // Spawn water pools
    if ((rand() / 4) % 717 == 6)
    {
        if (!randomWaterCoords.empty())
        {
            m_inanimate[randomWaterCoords.begin()->x][randomWaterCoords.begin()->y] = new WaterPool(this, randomWaterCoords.begin()->x, randomWaterCoords.begin()->y);
            randomWaterCoords.erase(*randomWaterCoords.begin());
        }
    }

    setDisplayText();

    if (!m_Iceman->isAlive())
    {
        return GWSTATUS_PLAYER_DIED;
    }

    if (getNumBarrelsRemaining() == 0)
    {

        return GWSTATUS_FINISHED_LEVEL;
    }

    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    // Happens when Iceman lost a life or completed the current level, the code will call it on its own
    // EVERY actor in the whole field will be deleted and removed from your StudentWorld�s container(s) of active objects
    // Result: an empty oil field

    for (int x = 0; x < VIEW_WIDTH; x++)
    {
        delete m_protestor[x];
        m_protestor[x] = nullptr;
        for (int y = 0; y < VIEW_HEIGHT; y++)
        {
            delete m_ice[x][y];
            m_ice[x][y] = nullptr;
            delete m_inanimate[x][y];
            m_inanimate[x][y] = nullptr;
        }
    }
    for (int i = 0; i < 64; i++)
    {
        delete m_boulders[i];
        m_boulders[i] = nullptr;
    }

    delete m_Iceman;
    m_Iceman = nullptr;

}

IceMan* StudentWorld::getIceman()
{
    return m_Iceman;
}

Protester* StudentWorld::getProtester(int xcoord, int ycoord)
{
    for (int i = 0; i < 64; i++)
    {
        if (m_protestor[i] != nullptr && m_protestor[i]->getX() == xcoord && m_protestor[i]->getY() == ycoord)
        {
            return m_protestor[i];
        }
    }
    return nullptr;
}



StudentWorld::~StudentWorld()
{
    cleanUp();
}

int StudentWorld::getNumBarrelsRemaining() {
    return numBarrelsRemaining;
}

void StudentWorld::decNumBarrelsRemaining() {
    numBarrelsRemaining--;
}

bool StudentWorld::iceManWithinRadius(int Xcoor, int Ycoor, double radius) {
    for (int x = Xcoor - radius; x < Xcoor + radius + 1; x++)
    {
        for (int y = Ycoor - radius; y < Ycoor + radius + 1; y++)
        {
            if (m_Iceman->getX() == x && m_Iceman->getY() == y) {
                //save these coordinates
                XofIcemanInRad = m_Iceman->getX();
                YofIcemanInRad = m_Iceman->getY();
                return true;
            }
        }
    }
    return false;
}


bool StudentWorld::ProtestorWithinRadius(int Xcoor, int Ycoor, double radius) {

    for (int i = 0; i < 64; i++)
    {
        for (int x = Xcoor - radius; x < Xcoor + radius + 1; x++)
        {
            for (int y = Ycoor - radius; y < Ycoor + radius + 1;y++)
            {

                if (m_protestor[i] != nullptr && m_protestor[i]->getX() == x && m_protestor[i]->getY() == y) {
                    //save these coordinates
                    XofProtesterinRad = m_protestor[i]->getX();
                    YofProtesterinRad = m_protestor[i]->getY();
                    return true;
                }
            }
        }
    }
    return false;
}

bool StudentWorld::iceWithinRadius(int Xcoor, int Ycoor, int radius) {

    for (int x = Xcoor - radius; x < Xcoor + radius + 1; x++)
    {
        for (int y = Ycoor - radius; y < Ycoor + radius + 1; y++)
        {
            //this means there is ice at this coordinate
            if (m_ice[x][y] != nullptr) {
                return true;
            }
        }
    }
    return false;
}

bool StudentWorld::boulderWithinRadius(int Xcoor, int Ycoor, double radius) {


    for (int i = 0; i < 64; i++)
    {
        for (int x = Xcoor - radius; x < Xcoor + radius + 1; x++)
        {
            for (int y = Ycoor - radius; y < Ycoor + radius + 1;y++)
            {

                if (m_boulders[i] != nullptr && m_boulders[i]->getX() == x && m_boulders[i]->getY() == y) {
                    //save these coordinates
                    XofBoulderinRad = m_boulders[i]->getX();
                    YofBoulderinRad = m_boulders[i]->getY();
                    return true;
                }
            }
        }
    }
    return false;
}


bool StudentWorld::icePresentBelow(int Xcoor, int Ycoor) {

    for (int x = Xcoor; x < Xcoor + 4; x++)
    {
        int y = Ycoor - 1; //to check row below boulder
        //this means there is ice at this coordinate
        if (m_ice[x][y] != nullptr) {
            return true;
        }

    }
    return false;
}

bool StudentWorld::boulderPresentBelow(int Xcoor, int Ycoor) {
    for (int i = 0; i < 64; i++)
    {
        for (int x = Xcoor; x < Xcoor + 4; x++)
        {
            int y = Ycoor - 1;

            if (m_boulders[i] != nullptr && m_boulders[i]->getX() == x && m_boulders[i]->getY() == y) {
                return true;
            }

        }
    }
    return false;
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
