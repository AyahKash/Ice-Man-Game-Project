#ifndef STUDENTWORLDH
#define STUDENTWORLDH

#include <string>
#include <memory>
#include <vector>
#include <unordered_set>
#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"

struct Coordinates
{
    int x;
    int y;

};
bool operator== (const Coordinates& a, const Coordinates& b);

struct KeyHash {
    std::size_t operator()(const Coordinates& coord) const
    {
        return std::hash<int>()(coord.x) ^ (std::hash<int>()(coord.y)) << 1;
    }
};

Coordinates randInanimate();

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    int XofBoulderinRad = 0;
    int YofBoulderinRad = 0;

    int XofIcemanInRad = 0;
    int YofIcemanInRad = 0;

    int XofProtesterinRad = 0;
    int YofProtesterinRad = 0;
    //A.Initialize the data structures used to keep track of your gameï¿½s virtual world
    //B.Construct a new oil field that meets the requirements stated in the section below
        //(filled with Ice, Barrels of oil, Boulders, Gold Nuggets, etc.)
    //C.Allocate and insert a valid Iceman object into the game world at the proper
        //location

    StudentWorld(std::string assetDir)
        : GameWorld(assetDir)
    {

    }

    virtual int init();

    void setDisplayText();

    virtual int move();

    virtual void cleanUp();

    IceMan* getIceman();

    Protester* getProtester(int xcoord, int ycoord);

    virtual ~StudentWorld();

    int getNumBarrelsRemaining();

    void decNumBarrelsRemaining();

    //gives whether the iceman is within the radius of the coordinates passed in
    bool iceManWithinRadius(int Xcoor, int Ycoor, double radius);
    bool ProtestorWithinRadius(int Xcoor, int Ycoor, double radius);
    bool iceWithinRadius(int Xcoor, int Ycoor, int radius);
    bool boulderWithinRadius(int Xcoor, int Ycoor, double radius);

    bool icePresentBelow(int Xcoor, int Ycoor);
    bool boulderPresentBelow(int Xcoor, int Ycoor);

    Actor* m_ice[64][64];
private:

    // vector of vectors of ice

    Actor* m_inanimate[64][64];
    Protester* m_protestor[64];
    Boulder* m_boulders[64];
    // vector of pointers to objects besides ice
    std::vector<Actor*> m_actors;
    // separate indiv pointer to Iceman
    IceMan* m_Iceman;
    Coordinates m_IcemanCoord;
    std::unordered_set<Coordinates, KeyHash> randomCoords;
    std::unordered_set<Coordinates, KeyHash> randomWaterCoords;
    size_t numBarrelsRemaining = 0;
    size_t numGold = 0;
    size_t numBoulders = 0;
    size_t numProtestors = 0;
    size_t numHCProtestors = 0;

};

#endif // STUDENTWORLDH
