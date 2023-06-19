#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

struct Coordinates;
class Actor;
class StudentWorld;
//virtual fcns have "virtual" in front of them only in header file, remove keyword "virtual" in cpp file

//FOR FINDSHORTESTPATH
class ShortestPath {
public:
    ShortestPath(Coordinates destination, Actor* (&ice)[64][64]);
    ShortestPath() = default;
    Coordinates newCoord(Coordinates current);

private:
    int m_coordsSP[64][64];
    void recurSearch(Coordinates currentpoint, Actor* (&ice)[64][64], int dist);
};


class Actor : public GraphObject
{
protected:
    bool isLive = true;
    StudentWorld* worldPtr;
    bool isVisible;
public:
    Actor(StudentWorld* world, int startX, int startY, Direction startDir,
        bool visible, int imageID, double size, int depth);

    // Action to perform each tick.
    virtual void move() = 0; //serves as our doSomething

    //REMOVED ANNOY FUNCTION AND PLACED IN AGENT

    //Get this actor's world
    StudentWorld* getWorld() const;

    // Can other actors pass through this actor?
    virtual bool canActorsPassThroughMe() const;

    // Can this actor dig through Ice?
    virtual bool canDigThroughIce() const;

    // Can this actor pick items up?
    virtual bool canPickThingsUp() const;

    // Does this actor hunt the IceMan?
    virtual bool huntsIceMan() const;

    // Can this actor need to be picked up to finish the level?
    virtual bool needsToBePickedUpToFinishLevel() const;

    // Move this actor to x,y if possible, and return true; otherwise,
    // return false without moving.
    bool moveToIfPossible(int x, int y);

    // Is this actor alive?
    virtual bool isAlive();
    // Mark this actor as dead.
    virtual void setDead();


};

class Agent : public Actor
{
protected:
    int m_hitPoints;
    int numGold = 0; //ALL AGENTS CAN PICK UP GOLD
public:
    Agent(StudentWorld* world, int startX, int startY, Direction startDir,
        int imageID, unsigned int hitPoints);

    // Pick up a gold nugget.
    virtual void addGold() = 0;

    // How many hit points does this actor have left?
    unsigned int getHitPoints() const;

    virtual bool annoy(unsigned int amount);
    virtual bool canPickThingsUp() const;
    // Is this actor alive?

    virtual bool isAlive();


};

class IceMan : public Agent
{
protected:
    int squirtsAvailable = 0;
    int sonarAvailable = 0;
    //gold nuggi var in Agent
    int bribeScore = 0;

public:
    IceMan(StudentWorld* world, int startX, int startY);
    virtual void move();
    //inherit annoy from Agent,
    //virtual bool annoy(unsigned int amount);
    virtual void addGold();
    virtual bool canDigThroughIce() const;

    // Pick up a sonar kit.
    void addSonar();

    // Pick up water.
    void addWater();

    // Get amount of gold
    unsigned int getGold() const;

    // Get amount of sonar charges
    unsigned int getSonar() const;

    // Get amount of water
    unsigned int getWater() const;

    unsigned int getBribeScore() const;
    void incBribeScore(int amt);
    bool annoy(unsigned int amount);

};

class Protester : public Agent
{
protected:
    int ticksToWaitBetweenMoves = 50;
    int currentTick = 0;
    bool mustLeaveField = false;
    // added by reb 8:09pm
    int ticksInCurrentDir = 0;
    ShortestPath escape;

public:
    //Protester::Protester(StudentWorld* world, int startX, int startY, int imageID, unsigned int hitPoints, unsigned int score)
    //^^^WHAT IS SCORE
    Protester(StudentWorld* world, int startX, int startY, int imageID,
        unsigned int hitPoints);
    virtual void move();


    virtual bool annoy(unsigned int amount);
    virtual void addGold();
    virtual bool huntsIceMan() const;

    // Set state to having gien up protest
    void setMustLeaveOilField();

    // Set number of ticks until next move
    void setTicksToNextMove();
};

class RegularProtester : public Protester
{
public:
    RegularProtester(StudentWorld* world, int startX, int startY, int imageID);
    virtual void move();
    virtual void addGold();
};

class HardcoreProtester : public Protester
{
public:
    HardcoreProtester(StudentWorld* world, int startX, int startY, int imageID);
    virtual void move();
    virtual void addGold();
};

class Ice : public Actor
{
public:
    Ice(StudentWorld* world, int startX, int startY);
    virtual void move();
    ~Ice() {};
};

class Boulder : public Actor
{
protected:
    bool stableState = true;
    bool waitingState = false;
    bool fallingState = false;
    int currentTick = 0;
    int ticksToWait = 30;

public:
    Boulder(StudentWorld* world, int startX, int startY);
    virtual void move();
    virtual bool canActorsPassThroughMe() const;
};
//
class Squirt : public Actor
{
protected:
    int travelDist = 4;
    int squirtSaveXcoor;
    int squirtSaveYcoor;
public:
    Squirt(StudentWorld* world, int startX, int startY, Direction startDir);
    virtual void move();
};
//
class ActivatingObject : public Actor
{
protected:

    //bool isVisible;
public:
    ActivatingObject(StudentWorld* world, int startX, int startY, int imageID, bool activateOnPlayer, bool activateOnProtester, bool visible);
    virtual void move();

    // Set number of ticks until this object dies
    void setTicksToLive();
};
//
class OilBarrel : public ActivatingObject
{
protected:
    int oilSaveXcoor;
    int oilSaveYcoor;
public:
    OilBarrel(StudentWorld* world, int startX, int startY);
    virtual void move();
    virtual bool needsToBePickedUpToFinishLevel() const;
};
//
class GoldNugget : public ActivatingObject
{
protected:
    int goldSaveXcoor;
    int goldSaveYcoor;
    int currentTick = 0;
    int numTicksToLive = 100;
    bool pickByIceMan = false;
    bool pickByProtester = false;
    bool permanent;
    bool temporary;
public:
    GoldNugget(StudentWorld* world, int startX, int startY, bool visible, bool pickableByProtester, bool pickableByIceMan, bool perm, bool temp);
    virtual void move();
    virtual bool isAlive();
    virtual void setDead();

};
//
class SonarKit : public ActivatingObject
{
protected:
    int sonarSaveXcoor;
    int sonarSaveYcoor;
    int currentTick = 0;
    int secondArg;
    int numTicksToLive;
public:
    SonarKit(StudentWorld* world, int startX, int startY);
    virtual void setTicksToLive();
    virtual void move();
};
//
class WaterPool : public ActivatingObject
{
protected:
    int waterSaveXcoor;
    int waterSaveYcoor;
    int currentTick = 0;
    int secondArg;
    int numTicksToLive;
public:
    WaterPool(StudentWorld* world, int startX, int startY);
    virtual void setTicksToLive();
    virtual void move();
};
//
#endif // ACTOR_H_
