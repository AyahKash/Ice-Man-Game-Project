#include "Actor.h"
#include "StudentWorld.h"
#include "GameController.h"
#include "GameWorld.h"
#include <iostream>
#include <algorithm>


        //MAKE SURE TO SET isVisible with EVERY setVisible
        //refer to page 20 to add protestors
        using namespace std;

    //GraphObject
    //Actor
    //    Agent
    //        IceMan
    //        Protester
    //            RegularProtester
    //            HardCoreProtester
    //    Ice
    //    Boulder
    //    Squirt
    //    ActivatingObject
    //        OilBarrel
    //        GoldNugget
    //        SonarKit
    //        WaterPool

    struct Coordinates;
    class StudentWorld;
    static const int BARRIER = 999;
    static const int UNVISITED = 444;
    ShortestPath::ShortestPath(Coordinates destination, Actor * (&ice)[64][64])
    {

        for (int i = 0; i < 64; i++)
        {
            for (int j = 0; j < 64; j++)
            {
                m_coordsSP[i][j] = UNVISITED;
                if (ice[i][j] != nullptr)
                {
                    m_coordsSP[i][j] = BARRIER;
                }
            }
        }
        int distance = 0;
        m_coordsSP[destination.x][destination.y] = 0;
        recurSearch(destination, ice, distance);

    }

    Coordinates ShortestPath::newCoord(Coordinates current)
    {
        Coordinates up{ current.x, current.y + 1 };
        Coordinates right{ current.x + 1,current.y };
        Coordinates down{ current.x, current.y - 1 };
        Coordinates left{ current.x - 1,current.y };

        Coordinates cheapest{ current.x,current.y };
        int cheapVal = 999;

        if (m_coordsSP[current.x][current.y] > m_coordsSP[up.x][up.y])
        {
            cheapest = up;
            cheapVal = m_coordsSP[up.x][up.y];
        }
        if (cheapVal > m_coordsSP[right.x][right.y])
        {
            cheapest = right;
            cheapVal = m_coordsSP[right.x][right.y];
        }
        if (cheapVal > m_coordsSP[down.x][down.y])
        {
            cheapest = down;
            cheapVal = m_coordsSP[down.x][down.y];
        }
        if (cheapVal > m_coordsSP[left.x][left.y])
        {
            cheapest = left;
            cheapVal = m_coordsSP[left.x][left.y];
        }
        return cheapest;
    }

    void ShortestPath::recurSearch(Coordinates currentpoint, Actor * (&ice)[64][64], int dist)
    {
        if (m_coordsSP[currentpoint.x][currentpoint.y] != UNVISITED)  return;
        for (int x = currentpoint.x; x < currentpoint.x + 4; x++)
        {
            if (x > 60) return;
            for (int y = currentpoint.y; y < currentpoint.y + 4; y++)
            {
                if (y > 60) return;
                if (m_coordsSP[x][y] == BARRIER) return;
            }
        }


        m_coordsSP[currentpoint.x][currentpoint.y] = dist;
        //search up
        if (currentpoint.y + 1 < 60) {
            Coordinates checkUp = Coordinates{ currentpoint.x,currentpoint.y + 1 };
            recurSearch(checkUp, ice, dist + 1);
        }

        //search right
        if (currentpoint.x + 1 < 60) {
            Coordinates checkRight = Coordinates{ currentpoint.x + 1,currentpoint.y };
            recurSearch(checkRight, ice, dist + 1);
        }

        //search down
        if (currentpoint.y - 1 >= 0) {
            Coordinates checkDown = Coordinates{ currentpoint.x,currentpoint.y - 1 };
            recurSearch(checkDown, ice, dist + 1);
        }

        //search left
        if (currentpoint.x - 1 >= 0) {
            Coordinates checkLeft = Coordinates{ currentpoint.x - 1,currentpoint.y };
            recurSearch(checkLeft, ice, dist + 1);
        }
    }

    //SUPER CLASS C'TOR MUST BE IN INITIALIZATION LIST, NOT IN BODY
    Actor::Actor(StudentWorld * world, int startX, int startY, Direction startDir, bool visible, int imageID, double size, int depth) : GraphObject(imageID, startX, startY, startDir, size, depth) {
        //derived constructor must pass into base class c'tor

    //page 26: When u create a new game object, always remember that if you want it to be displayed on the screen, you must call its setValue method with a value of true
        worldPtr = world;
        isVisible = visible;
        if (isVisible) {
            setVisible(true);
        }
        //visibility should be set in individual c'tors
    }

    // Action to perform each tick.

    void Actor::move() {

    }
    //page 20
          // Is this actor alive?

    //REMOVED ANNOY FUNCTION AND PLACED IN AGENT
          // Get this actor's world
    StudentWorld* Actor::getWorld() const {
        return worldPtr;
        //worldPtr is how actors interact w each other
    }

    // Can other actors pass through this actor?
    bool  Actor::canActorsPassThroughMe() const {
        return true;
        //    //must be set to true for others
    }

    // Can this actor dig through Ice?
    bool  Actor::canDigThroughIce() const {
        return true;
        //MUST BE SET TO FALSE FOR OTHERS
    }

    // Can this actor pick items up?
    bool  Actor::canPickThingsUp() const {
        return true;
        //MUST MAKE FALSE FOR INANIMATE OBJECTS LIKE GOLD
    }

    // Does this actor hunt the IceMan?
    bool  Actor::huntsIceMan() const {
        return false;
        //SHOULD BE TRUE FOR PROTESTORS
    }

    // Can this actor need to be picked up to finish the level?
    bool  Actor::needsToBePickedUpToFinishLevel() const {
        return false;
        // SHOULD BE TRUE FOR BARRELS
    }

    // Move this actor to x,y if possible, and return true; otherwise,
    // return false without moving.
    bool Actor::moveToIfPossible(int x, int y) {
        //must use GraphObject's moveTo
        //valid coordinates pg 7
        if ((x >= 0 && x <= 60) && (y >= 0 && y <= 60)) {
            //move &return true
            moveTo(x, y);
            return true;
        }
        else {
            return false;
        }
    }

    bool Actor::isAlive() {
        return isLive;
    }

    // Mark this actor as dead.
    void Actor::setDead() {
        isLive = false;
    }

    //class Agent : public Actor
        //must pass thru actors c'tor in initialization list
    Agent::Agent(StudentWorld * world, int startX, int startY, Direction startDir, int imageID, unsigned int hitPoints) :
        Actor(world, startX, startY, startDir, true, imageID, 1.0, 0) {
        m_hitPoints = hitPoints;
    }

    // Pick up a gold nugget.
    //MUST WRITE THIS FOR ALL DERIVED AGENTS TO ALLOW THEM TO BE INSTANTIATED
    //void Agent::addGold() = 0;

      // How many hit points does this actor have left?
    unsigned int Agent::getHitPoints() const {
        return m_hitPoints;
    }

    //Annoy this actor
    bool Agent::annoy(unsigned int amount) {
        m_hitPoints -= amount;
        if (m_hitPoints <= 0) {
            setDead();
        }
        return true;
    }

    //bool IceMan::annoy(unsigned int amount){
    //    m_hitPoints -= amount;
    //    if (m_hitPoints <= 0){
    //        worldPtr->decLives();
    //        setDead();
    //    }
    //    return true;
    //}


    bool Agent::canPickThingsUp() const {
        return true;
        //all agents can pick things up
    }

    bool Agent::isAlive() {
        if (m_hitPoints <= 0) {
            setDead();
            return false;
        }
        return true;
    }

    //class IceMan : public Agent
    //PAGE 27
    IceMan::IceMan(StudentWorld * world, int startX, int startY) : Agent(world, startX, startY, right, IID_PLAYER, 10) {
        setVisible(true);
        squirtsAvailable = 5;
        sonarAvailable = 1;
        numGold = 0;
    }

    //serves as our doSomething
    void IceMan::move() {
        int keyPressed;

        if (worldPtr->getKey(keyPressed))
        {
            //MOVE UP
            //w ascii is 119
            //if doesnt work, isolate
            if (keyPressed == KEY_PRESS_LEFT || keyPressed == 97) {
                if (getX() - 1 >= 0) {
                    setDirection(left);
                    moveTo(getX() - 1, getY());

                }
                //update
            }
            else if (keyPressed == KEY_PRESS_RIGHT || keyPressed == 100) {
                if (getX() + 1 <= VIEW_WIDTH - 4) {
                    setDirection(right);
                    moveTo(getX() + 1, getY());
                }
            }
            else if (keyPressed == KEY_PRESS_UP || keyPressed == 119) {
                if (getY() + 1 <= VIEW_HEIGHT - 4) {
                    setDirection(up);
                    moveTo(getX(), getY() + 1);
                }
            }
            else if (keyPressed == KEY_PRESS_DOWN || keyPressed == 115) {
                if (getY() - 1 >= 0) {
                    setDirection(down);
                    moveTo(getX(), getY() - 1);
                }
            }
        }
    }

    //inherit annoy from Agent
    //bool IceMan::annoy(unsigned int amount){
        //leave the oil state

    //}

    void IceMan::addGold() {
        numGold++;
        GameController::getInstance().playSound(SOUND_GOT_GOODIE);
        //increase score by 10points
        worldPtr->increaseScore(10);
    }
    bool IceMan::canDigThroughIce() const {
        return true;
    }

    // Pick up a sonar kit.
    void IceMan::addSonar() {
        sonarAvailable++;
        GameController::getInstance().playSound(SOUND_GOT_GOODIE);
        worldPtr->increaseScore(75);
    }

    // Pick up water.
    void IceMan::addWater() {
        squirtsAvailable += 5; //every water pool provides 5 squirts
        GameController::getInstance().playSound(SOUND_GOT_GOODIE);
        worldPtr->increaseScore(100);

    }

    // Get amount of gold
    unsigned int IceMan::getGold() const {
        return numGold;
    }

    // Get amount of sonar charges
    unsigned int IceMan::getSonar() const {
        return sonarAvailable;
    }

    // Get amount of water
    unsigned int IceMan::getWater() const {
        return squirtsAvailable;
    }

    unsigned int IceMan::getBribeScore() const {
        return bribeScore;
    }

    void IceMan::incBribeScore(int amt) {
        bribeScore += amt;
    }
    bool IceMan::annoy(unsigned int amount) {
        m_hitPoints -= amount;
        if (m_hitPoints <= 0) {
            worldPtr->decLives();
            setDead();
        }
        return true;
    }


    //class Protester : public Agent
    //{
    //public:
    //Protester::Protester(StudentWorld* world, int startX, int startY, int imageID, unsigned int hitPoints, unsigned int score)
    //^^^WHAT IS SCORE
    Protester::Protester(StudentWorld * world, int startX, int startY, int imageID, unsigned int hitPoints) : Agent(world, startX, startY, left, imageID, hitPoints) {
        setVisible(true);
    }


    void Protester::move() {

    }

    bool Protester::annoy(unsigned int amount) {
        if (!mustLeaveField) {
            m_hitPoints -= amount;
            if (m_hitPoints <= 0) {
                //setDead();
                //^^ only set dead when protesters reach (60,60),
                //cant die immediately after full annoyance
                mustLeaveField = true;
            }
            else {
                GameController::getInstance().playSound(SOUND_PROTESTER_ANNOYED);
                //PLACE IN RESTING STATE
                int secondArg = 100 - (worldPtr->getLevel()) * 10;
                ticksToWaitBetweenMoves = std::max(50, secondArg);
            }
            return true;
        }
        else {
            return false;
        }
    }

    void Protester::addGold() {

    }
    bool Protester::huntsIceMan() const {
        return true;
    }
    //
      // Set state to having gien up protest
    void Protester::setMustLeaveOilField() {
        mustLeaveField = true;
        Coordinates coords = Coordinates{ this->getX(),this->getY() };
        escape = ShortestPath(coords, getWorld()->m_ice);

    }
    //
    //      // Set number of ticks until next move
    //must use current-level-number PAGE 40
    void Protester::setTicksToNextMove() {
        int secondArg = (3 - (worldPtr->getLevel()) / 4);
        ticksToWaitBetweenMoves = max(0, secondArg);

    }

    //};

    //class RegularProtester : public Protester
    //{
    //public:
    RegularProtester::RegularProtester(StudentWorld * world, int startX, int startY, int imageID) : Protester(world, startX, startY, IID_PROTESTER, 5) {
        setTicksToNextMove();
        setVisible(true);
    }


    //HUGE IMPLEMENTATATION
    //page 40
    void RegularProtester::move() {
        if (isAlive()) {
            //determine if protestor is in rest state
            if (currentTick <= ticksToWaitBetweenMoves) {
                currentTick++;
                return;
            }
            if (mustLeaveField) {
                //RUN MAZE CODE
                auto newCoords = escape.newCoord(Coordinates{ this->getX(),this->getY() });
                moveTo(newCoords.x, newCoords.y);

                return;
            }

            bool canMoveUp = getY() + 1 < 61 && !worldPtr->iceWithinRadius(getX(), getY() + 4, 0.0) && !worldPtr->iceWithinRadius(getX() + 1, getY() + 4, 0.0) && !worldPtr->iceWithinRadius(getX() + 2, getY() + 4, 0.0) && !worldPtr->iceWithinRadius(getX() + 3, getY() + 4, 0.0);
            bool canMoveDown = getY() - 1 >= 0 && !worldPtr->iceWithinRadius(getX(), getY() - 1, 0.0) && !worldPtr->iceWithinRadius(getX() + 1, getY() - 1, 0.0) && !worldPtr->iceWithinRadius(getX() + 2, getY() - 1, 0.0) && !worldPtr->iceWithinRadius(getX() + 3, getY() - 1, 0.0);
            bool canMoveLeft = getX() - 1 >= 0 && !worldPtr->iceWithinRadius(getX() - 1, getY(), 0.0) && !worldPtr->iceWithinRadius(getX() - 1, getY() + 1, 0.0) && !worldPtr->iceWithinRadius(getX() - 1, getY() + 2, 0.0) && !worldPtr->iceWithinRadius(getX() - 1, getY() + 3, 0.0);
            bool canMoveRight = getX() + 1 < 61 && !worldPtr->iceWithinRadius(getX() + 4, getY(), 0.0) && !worldPtr->iceWithinRadius(getX() + 4, getY() + 1, 0.0) && !worldPtr->iceWithinRadius(getX() + 4, getY() + 2, 0.0) && !worldPtr->iceWithinRadius(getX() + 4, getY() + 3, 0.0);
            if (ticksInCurrentDir > 8)
            {
                if (rand() / 1024 % 8 < 2)
                {
                    int num = rand() / 1024 % 4;
                    if (num == 0 && canMoveUp)
                    {
                        setDirection(up);
                        ticksInCurrentDir = 0;
                    }
                    else if (num == 1 && canMoveDown)
                    {
                        setDirection(down);
                        ticksInCurrentDir = 0;
                    }
                    else if (num == 2 && canMoveLeft)
                    {
                        setDirection(left);
                        ticksInCurrentDir = 0;
                    }
                    else if (num == 3 && canMoveRight)
                    {
                        setDirection(right);
                        ticksInCurrentDir = 0;
                    }
                }
            }

            //move protestors
            if (this->getDirection() == up && canMoveUp) {
                moveTo(getX(), min(getY() + 1, 60));
            }
            else if (this->getDirection() == right && canMoveRight) {
                moveTo(min(getX() + 1, 60), getY());
            }
            else if (this->getDirection() == down && canMoveDown) {
                moveTo(getX(), max(0, getY() - 1));
            }
            else if (this->getDirection() == left && canMoveLeft) {
                moveTo(max(0, getX() - 1), getY());
            }
            setTicksToNextMove();
            ticksInCurrentDir++;

        }
        else {
            return;
        }
    }

    void RegularProtester::addGold() {
        GameController::getInstance().playSound(SOUND_PROTESTER_FOUND_GOLD);
        //must increase player's bribery points:
        worldPtr->getIceman()->incBribeScore(25);
        worldPtr->increaseScore(25);
        mustLeaveField = true;

    }
    //};
    //


    //class HardcoreProtester : public Protester
    //{
    //public:
    //HUGE IMPLEMENTATATION
    HardcoreProtester::HardcoreProtester(StudentWorld * world, int startX, int startY, int imageID) : Protester(world, startX, startY, IID_HARD_CORE_PROTESTER, 20) {

    }

    //HUGE IMPLEMENTATION
    void HardcoreProtester::move() {
        if (isAlive()) {
            //determine if protestor is in rest state
            if (currentTick <= ticksToWaitBetweenMoves) {
                currentTick++;
                return;
            }
            if (mustLeaveField) {
                //RUN MAZE CODE
                auto newCoords = escape.newCoord(Coordinates{ this->getX(),this->getY() });
                moveTo(newCoords.x, newCoords.y);

                return;
            }

            bool canMoveUp = getY() + 1 < 61 && !worldPtr->iceWithinRadius(getX(), getY() + 4, 0.0) && !worldPtr->iceWithinRadius(getX() + 1, getY() + 4, 0.0) && !worldPtr->iceWithinRadius(getX() + 2, getY() + 4, 0.0) && !worldPtr->iceWithinRadius(getX() + 3, getY() + 4, 0.0);
            bool canMoveDown = getY() - 1 >= 0 && !worldPtr->iceWithinRadius(getX(), getY() - 1, 0.0) && !worldPtr->iceWithinRadius(getX() + 1, getY() - 1, 0.0) && !worldPtr->iceWithinRadius(getX() + 2, getY() - 1, 0.0) && !worldPtr->iceWithinRadius(getX() + 3, getY() - 1, 0.0);
            bool canMoveLeft = getX() - 1 >= 0 && !worldPtr->iceWithinRadius(getX() - 1, getY(), 0.0) && !worldPtr->iceWithinRadius(getX() - 1, getY() + 1, 0.0) && !worldPtr->iceWithinRadius(getX() - 1, getY() + 2, 0.0) && !worldPtr->iceWithinRadius(getX() - 1, getY() + 3, 0.0);
            bool canMoveRight = getX() + 1 < 61 && !worldPtr->iceWithinRadius(getX() + 4, getY(), 0.0) && !worldPtr->iceWithinRadius(getX() + 4, getY() + 1, 0.0) && !worldPtr->iceWithinRadius(getX() + 4, getY() + 2, 0.0) && !worldPtr->iceWithinRadius(getX() + 4, getY() + 3, 0.0);
            if (ticksInCurrentDir > 8)
            {
                if (rand() / 1024 % 8 < 2)
                {
                    int num = rand() / 1024 % 4;
                    if (num == 0 && canMoveUp)
                    {
                        setDirection(up);
                        ticksInCurrentDir = 0;
                    }
                    else if (num == 1 && canMoveDown)
                    {
                        setDirection(down);
                        ticksInCurrentDir = 0;
                    }
                    else if (num == 2 && canMoveLeft)
                    {
                        setDirection(left);
                        ticksInCurrentDir = 0;
                    }
                    else if (num == 3 && canMoveRight)
                    {
                        setDirection(right);
                        ticksInCurrentDir = 0;
                    }
                }
            }

            //move protestors
            if (this->getDirection() == up && canMoveUp) {
                moveTo(getX(), min(getY() + 1, 60));
            }
            else if (this->getDirection() == right && canMoveRight) {
                moveTo(min(getX() + 1, 60), getY());
            }
            else if (this->getDirection() == down && canMoveDown) {
                moveTo(getX(), max(0, getY() - 1));
            }
            else if (this->getDirection() == left && canMoveLeft) {
                moveTo(max(0, getX() - 1), getY());
            }
            setTicksToNextMove();
            ticksInCurrentDir++;

        }
        else {
            return;
        }
    }
    void HardcoreProtester::addGold() {
        GameController::getInstance().playSound(SOUND_PROTESTER_FOUND_GOLD);
        //must increase player's bribery points:
        worldPtr->getIceman()->incBribeScore(50);
        worldPtr->increaseScore(50);
        int secondArg = 100 - (worldPtr->getLevel()) * 10;
        ticksToWaitBetweenMoves = max(50, secondArg);

    }
    //};

    //class Ice : public Actor
    Ice::Ice(StudentWorld * world, int startX, int startY) : Actor(world, startX, startY, right, true, IID_ICE, 0.25, 3) {
        setVisible(true);

    }
    void Ice::move() {
        //ice doesnt move
    }


    //class Boulder : public Actor
    //{
    //public:
    Boulder::Boulder(StudentWorld * world, int startX, int startY) : Actor(world, startX, startY, down, true, IID_BOULDER, 1, 1) {
        setVisible(true);
    }
    void Boulder::move() {
        if (isLive) {
            if (stableState) {

                if (!worldPtr->icePresentBelow(getX(), getY())) {
                    stableState = false;
                    waitingState = true;
                }
            }
            if (waitingState) {
                if (currentTick <= ticksToWait) {
                    currentTick++; //wait for certain num ticks
                }
                else {
                    waitingState = false;
                    fallingState = true;
                }
            }
            if (fallingState) {
                GameController::getInstance().playSound(SOUND_FALLING_ROCK);
                //if boulder runs into ice or another boulder while falling
                if (worldPtr->icePresentBelow(getX(), getY()) || worldPtr->boulderPresentBelow(getX(), getY())) {
                    setDead();
                }
                else {
                    bool truthVal = moveToIfPossible(getX(), getY() - 1);
                    if (!truthVal) { //this happens if we reached bounds of screen
                        setDead();
                    }
                }
                if (worldPtr->iceManWithinRadius(getX(), getY(), 3)) {
                    worldPtr->getIceman()->annoy(100);
                    worldPtr->getIceman()->setDead();
                }
                if (worldPtr->ProtestorWithinRadius(getX(), getY(), 3)) {
                    worldPtr->getProtester(worldPtr->XofProtesterinRad, worldPtr->YofProtesterinRad)->annoy(100);

                    worldPtr->getProtester(worldPtr->XofProtesterinRad, worldPtr->YofProtesterinRad)->setDead();
                    worldPtr->increaseScore(500);
                }

                //moves down, update coordinates
                //move down one square during each tick
                //when it hits ice, another boulder, reaches bottom of oil field boundary
                //then setDead();

                //if boulder within radius 3 of any protesters or iceman while falling, causes 100 annoyance points, if it is a protestor that dies, playr earns 500 points page 8

            }
        }
        else {
            return;
        }

    }

    bool Boulder::canActorsPassThroughMe() const {
        return false;
    }
    //};
    //
    //class Squirt : public Actor
    //{
    //public:
    Squirt::Squirt(StudentWorld * world, int startX, int startY, Direction startDir) : Actor(world, startX, startY, startDir, true, IID_WATER_SPURT, 1, 1) {

        squirtSaveXcoor = startX;
        squirtSaveYcoor = startY;

        setVisible(true);
    }

    void Squirt::move() {
        if (isLive) {
            if (worldPtr->getIceman()->getWater() != 0) {
                int keyPressed;

                if (worldPtr->getKey(keyPressed))
                {


                    if (keyPressed == 32 || keyPressed == KEY_PRESS_SPACE) {
                        if (worldPtr->ProtestorWithinRadius(squirtSaveXcoor, squirtSaveYcoor, 3)) {
                            worldPtr->getProtester(worldPtr->XofProtesterinRad, worldPtr->YofProtesterinRad)->annoy(2);

                            if (worldPtr->getProtester(worldPtr->XofProtesterinRad, worldPtr->YofProtesterinRad)->getHitPoints() <= 0) {
                                //reward player points for annoying protestpr to death
                                if (worldPtr->getProtester(worldPtr->XofProtesterinRad, worldPtr->YofProtesterinRad)->getID() == IID_PROTESTER) {

                                    worldPtr->increaseScore(100);
                                    worldPtr->getProtester(worldPtr->XofProtesterinRad, worldPtr->YofProtesterinRad)->setDead();
                                }
                                if (worldPtr->getProtester(worldPtr->XofProtesterinRad, worldPtr->YofProtesterinRad)->getID() == IID_HARD_CORE_PROTESTER) {

                                    worldPtr->increaseScore(250);
                                    worldPtr->getProtester(worldPtr->XofProtesterinRad, worldPtr->YofProtesterinRad)->setDead();
                                }

                            }
                            setDead(); //removing squirt after it has attacked protester
                        }
                        else {
                            //travel distance, remember this will be called in studentworld's move which gets called every tick
                            if (travelDist > 0) {
                                if (getDirection() == right && !worldPtr->iceWithinRadius(getX() + 1, getY(), 0) && !worldPtr->boulderWithinRadius(getX() + 1, getY(), 0)) {
                                    moveToIfPossible(getX() + 1, getY());
                                    travelDist--;
                                    return;
                                }
                                else if (getDirection() == left && !worldPtr->iceWithinRadius(getX() - 1, getY(), 0) && !worldPtr->boulderWithinRadius(getX() - 1, getY(), 0)) {
                                    moveToIfPossible(getX() - 1, getY());
                                    travelDist--;
                                    return;
                                }
                                else if (getDirection() == up && !worldPtr->iceWithinRadius(getX(), getY() + 1, 0) && !worldPtr->boulderWithinRadius(getX(), getY() + 1, 0)) {
                                    moveToIfPossible(getX(), getY() + 1);
                                    travelDist--;
                                    return;
                                }
                                else if (getDirection() == down && !worldPtr->iceWithinRadius(getX(), getY() - 1, 0) && !worldPtr->boulderWithinRadius(getX(), getY() - 1, 0)) {
                                    moveToIfPossible(getX(), getY() - 1);
                                    travelDist--;
                                    return;
                                }
                                else {
                                    setDead();
                                }


                            }
                            else {
                                setDead();
                            }

                        }
                        //lots of code here
                        //need fcn from student world that gives stuff within certain radius
                        //need to get hit points of protester to see if Iceman's squirts caused protestor to die so that we can increase score.
                    }
                }
            }
        }//
    }//
    //};

    //class ActivatingObject : public Actor
    //{
    //public:
    ActivatingObject::ActivatingObject(StudentWorld * world, int startX, int startY, int imageID, bool activateOnPlayer, bool activateOnProtester, bool visible) : Actor(world, startX, startY, right, visible, imageID, 1, 2) {

        isVisible = visible;
        if (isVisible) {
            setVisible(true);
        }
    }

    void ActivatingObject::move() {
    }
    //
    //      // Set number of ticks until this object dies
    void ActivatingObject::setTicksToLive() {

    }

    //};

    //class OilBarrel : public ActivatingObject
    //{
    //public:
    OilBarrel::OilBarrel(StudentWorld * world, int startX, int startY) : ActivatingObject(world, startX, startY, IID_BARREL, true, false, false) {
        //set visible in move later
        oilSaveXcoor = startX;
        oilSaveYcoor = startY;
    }

    //happens every tick
    void OilBarrel::move() {
        if (isLive) {
            if (!isVisible && worldPtr->iceManWithinRadius(oilSaveXcoor, oilSaveYcoor, 4)) {
                setVisible(true);
                isVisible = true;
                return;
            }
            //iceman catches barrel:
            if (worldPtr->iceManWithinRadius(oilSaveXcoor, oilSaveYcoor, 3)) {
                //barrel will be taken by iceman
             //set barrel's state to dead so that it can be removed by student world
             //play sound effect
                setDead();
                worldPtr->decNumBarrelsRemaining();
                GameController::getInstance().playSound(SOUND_FOUND_OIL);
                worldPtr->increaseScore(1000);
                setDead();
                //   increase score by 1000 points add 1000
                   //inform student world that barrel has been picked up so we can update game stats and prob vector of actors
                   //if all barrels picked up, advance to next level
            }

        }
        else {
            return;
        }

    }
    bool OilBarrel::needsToBePickedUpToFinishLevel() const {
        return true;
    }
    //};
    //
    //class GoldNugget : public ActivatingObject
    //{
    //public:
    //temporary is the var for visibility
    //gold nuggets are created either visible (ice man drops for protestor to pick up) or invisible (hidden in ice for iceman to find)
    GoldNugget::GoldNugget(StudentWorld * world, int startX, int startY, bool visible, bool pickableByProtester, bool pickableByIceMan, bool perm, bool temp) : ActivatingObject(world, startX, startY, IID_GOLD, pickableByIceMan, pickableByProtester, visible) {

        goldSaveXcoor = startX;
        goldSaveYcoor = startY;

        permanent = perm;
        temporary = temp;

        pickByIceMan = pickableByIceMan;
        pickByProtester = pickableByProtester;

        if (visible && pickByProtester) {
            setVisible(true);
        }

    }

    //MUST USE GOT GOLD FUNCTIONS FOR PROTESTOR AND ICE MAN

    //happens every tick
    void GoldNugget::move() {
        if (isLive) {
            if (!isVisible && worldPtr->iceManWithinRadius(goldSaveXcoor, goldSaveYcoor, 4)) {
                setVisible(true);
                isVisible = true;
                return;
            }
            else if (pickByIceMan && worldPtr->iceManWithinRadius(goldSaveXcoor, goldSaveYcoor, 3)) {

                //makes isLive false
                worldPtr->getIceman()->IceMan::addGold();
                setDead();
            }
            if (pickByProtester && worldPtr->StudentWorld::ProtestorWithinRadius(goldSaveXcoor, goldSaveYcoor, 3)) {
                setDead(); //makes isLive false, so that it can be removed
               // worldPtr
                worldPtr->getProtester(worldPtr->XofProtesterinRad, worldPtr->YofProtesterinRad)->addGold();
                //find way to call Protester::addGold
                //ASK REBECCA
            }
            if (temporary) {
                //numTicksToLive =
                if (currentTick <= numTicksToLive) {
                    currentTick++;
                }
                else {
                    setDead();
                }
            }

        }
        else {
            return;
        }

    }

    bool GoldNugget::isAlive() {
        return isLive;
    }

    void GoldNugget::setDead() {
        isLive = false;
    }

    //};
    //
    //class SonarKit : public ActivatingObject
    //{
    //public:
    //PAGE 29 FOR SONAR
    //all sonar kits can only be picked up by iceman
    SonarKit::SonarKit(StudentWorld * world, int startX, int startY) : ActivatingObject(world, startX, startY, IID_SONAR, true, false, true) {

        sonarSaveXcoor = startX;
        sonarSaveYcoor = startY;

        setTicksToLive();
        setVisible(true);

    }

    void SonarKit::setTicksToLive() {
        secondArg = 300 - 10 * (worldPtr->getLevel());
        numTicksToLive = max(100, secondArg);
    }

    void SonarKit::move() {
        if (isLive) {
            if (worldPtr->iceManWithinRadius(sonarSaveXcoor, sonarSaveYcoor, 3)) {
                worldPtr->getIceman()->IceMan::addSonar();
                setDead();
            }
            if (currentTick <= numTicksToLive) {
                currentTick++;
            }
            else {
                setDead();
            }
        }
        else {
            return;
        }
    }
    //};

    //class WaterPool : public ActivatingObject
    //{
    //public:
        //water pool only pickable by iceman
    WaterPool::WaterPool(StudentWorld * world, int startX, int startY) : ActivatingObject(world, startX, startY, IID_WATER_POOL, true, false, true) {

        setTicksToLive();
        waterSaveXcoor = startX;
        waterSaveYcoor = startY;

        setVisible(true);
    }

    void WaterPool::setTicksToLive() {
        secondArg = 300 - 10 * (worldPtr->getLevel());
        numTicksToLive = max(100, secondArg);
    }
    void WaterPool::move() {
        if (isLive) {
            if (worldPtr->iceManWithinRadius(waterSaveXcoor, waterSaveYcoor, 3)) {
                worldPtr->getIceman()->addWater();
                setDead();
            }
            if (currentTick <= numTicksToLive) {
                currentTick++;
            }
            else {
                setDead();
                //return;
            }
        }
        else {
            return;
        }
    }
