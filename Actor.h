#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
///////////////////////////////////////////////////////////////////////////////////////                 Base Class for all Actors
///////////////////////////////////////////////////////////////////////////////////////

class StudentWorld;

class baseActors : public GraphObject           //Abstract base class for all actors
{
public:
    baseActors(StudentWorld* world, int imageID, int startX, int startY, int dir = right, int depth = 0, double size = 1.0);
    virtual void doSomething() = 0;
    bool isActive();                            //Determines if actor is still active
    void changeState(bool newState);            //Change state of actor to active or inactive
    StudentWorld* getWorld() const;             //Returns pointer to StudentWorld
    virtual bool isSquare() = 0;                //Determines if an actor is a square
    virtual bool vortexAttack() = 0;            //Determines if an actor can be impacted by a vortex
    void changeStatus(int playerNum, bool newStatus);       //Allows for modification of isNew to keep track of interactions
    bool isNew(int playerNum);                              //Keeps track of interactions between player and actors
    bool impacted();                            //Determines if a baddie was impacted
    void changeImpact(bool action);             //Can change whether or not a baddie was impacted by a vortex
    virtual ~baseActors();
private:
    int state;                                  //Member var for isActive state
    StudentWorld* m_world;
    bool newPeach;                              //Used to keep track of Peach interactions
    bool newYoshi;                              //Keep track of Yoshi interactions
    bool impact;                                //Keep track of if a baddie was impacted
};

///////////////////////////////////////////////////////////////////////////////////////                 Base Class for all Moving Objects
///////////////////////////////////////////////////////////////////////////////////////
class MovingObjects : public baseActors
{
public:
    MovingObjects(StudentWorld* world, int imageID, int startX, int startY, int dir = right, int depth = 0, double size = 1.0);
    virtual void doSomething() = 0;
    virtual bool isSquare() = 0;
    virtual bool vortexAttack() = 0;
    void obstacleMove(int x, int y);                    //What to do if moving object is in a corner
    void changeSpriteDir(int dir);                      //Changes sprite direction based on walking direction
    int walkDirection();                                //Return walking direction
    void changeWalkDirection(int dir);                  //Change walking direction
    void teleport();                                    //What to do if a player or baddie is teleported
    int ticks();                                        //Return tick count
    void changeTicks(int newT);                         //Change tick count (add onto original amount)
    bool isWalking();                                   //Determine if actor is walking based on status
    void changeWalkStatus(bool newStatus);              //Change walk to waiting or vice versa
    void swap(MovingObjects* other, int x, int y);      //Swap players after encountering event square
    bool checkFork(int x, int y);                       //Check if there's a fork in the road
    void junctionChoice(int dir, int x, int y);         //What to do if there is a fork in the road
    bool choiceMade();                                  //Make sure player stays in junction until directional choice made
    void changeChoiceMade(bool newStatus);              //Determines if player made a choice
    void baddiesWalk(int x, int y);                     //How baddies are supposed to move
    void randomDirection(int x, int y);                 //Find a random direction in a 1 square radius that's valid
    void allWalk();                                     //How all actors walk
    int getRolls();                                     //Return # of roll
    void changeRolls(int change);                       //Change # of roll
    virtual ~MovingObjects();
private:
    int walkDir;                                        //0: right; 1: up; 2: left; 3:down
    int ticks_to_move;                                  //Keeps track of ticks
    bool status;                                        //Keeps track of walking or waitng/pausing
    bool m_choiceMade;
    int m_roll;                                         //Keeps track of player rolls
        
};

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////                 Derived Class for Yoshi and Peach
class Players : public MovingObjects
{
public:
    Players(StudentWorld* world, int imageID, int startX, int startY, int side, int dir = right, int depth = 0, double size = 1.0);
    virtual void doSomething();                         //Gets player to do something each tick
    int coins();                                        //Return coin count
    int stars();                                        //Return star count
    void adjustCoins(const int coins);                  //Change coin count by an amount
    void adjustStars(const int stars);                  //Change star count by an amount
    virtual bool isSquare();                            //Not a square
    virtual bool vortexAttack();                        //Cannot be attacked by vortex
    bool landedOn(int x, int y);                        //Determine if player has landed, not only passed, a square
    bool passed(int x, int y);                          //Determine if player has only passed, not landed on, a square
    bool walkingDirChanged(bool changed);               //Determine if walking direction was changed
    int newWalkingDirection(int change);                //Determine new walking direction
    void getVortex();                                   //Gives player a vortex (max 1)
    bool hasVortex();                                   //Determines if player has a vortex
    void swapCoins(Players* other);                     //Swap Peach and Yoshi coins
    void swapStars(Players* other);                     //Swap peach and Yoshi stars
    virtual ~Players();
private:
    int player;
    int m_coins;
    int m_stars;
    bool m_walkingDirChanged;
    int m_Dchange;
    int m_vortex;
    int prevWalkDir;                  //Keeps track of the previous walking direction so player cannot move back at junction
};

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////              Derived Class for ALL Squares
class Squares : public baseActors
{
public:
    Squares(StudentWorld* world, int imageID, int startX, int startY, int dir = right, int depth = 0, double size = 1.0);
    virtual void doSomething()=0;
    virtual bool isSquare();
    virtual bool vortexAttack();
    virtual ~Squares();
private:
    Players* m_player;
};

//////////////////////////////////////////////////////////////////////////////////////////              Derived Classes for Squares
class CoinSquare : public Squares
{
public:
    CoinSquare(StudentWorld* world, int imageID, int startX, int startY, char type, int dir = right, int depth = 0, double size = 1.0);
    virtual void doSomething();
    virtual bool isSquare();
    virtual bool vortexAttack();
    virtual ~CoinSquare();
private:
    char m_type;                                    //Keep track of if blue or red coin square
};

class directionSquare : public Squares
{
public:
    directionSquare(StudentWorld* world, int imageID, int startX, int startY, int forcingDirection, int dir = right, int depth = 0, double size = 1.0);
    virtual void doSomething();
    virtual bool isSquare();
    virtual bool vortexAttack();
    virtual ~directionSquare();
private:
    int orientation;                                //Keep track of forcing direction of square
};

class eventSquare : public Squares
{
public:
    eventSquare(StudentWorld* world, int imageID, int startX, int startY, int dir = right, int depth = 0, double size = 1.0);
    virtual void doSomething();
    virtual bool isSquare();
    virtual bool vortexAttack();
    virtual ~eventSquare();
};

class bankSquare : public Squares
{
public:
    bankSquare(StudentWorld* world, int imageID, int startX, int startY, int dir = right, int depth = 0, double size = 1.0);
    virtual void doSomething();
    virtual bool isSquare();
    virtual bool vortexAttack();
    virtual ~bankSquare();
};

class starSquare : public Squares
{
public:
    starSquare(StudentWorld* world, int imageID, int startX, int startY, int dir = right, int depth = 0, double size = 1.0);
    virtual void doSomething();
    virtual bool isSquare();
    virtual bool vortexAttack();
    virtual ~starSquare();
};

class droppingSquare : public Squares
{
public:
    droppingSquare(StudentWorld* world, int imageID, int startX, int startY, int dir = right, int depth = 0, double size = 1.0);
    virtual void doSomething();
    virtual bool isSquare();
    virtual bool vortexAttack();
    virtual ~droppingSquare();
};

//////////////////////////////////////////////////////////////////////////////////////////              Base Class for Bowser and Boo
class Baddies : public MovingObjects
{
public:
    Baddies(StudentWorld* world, int imageID, int startX, int startY, int dir = right, int depth = 0, double size = 1.0);
    virtual void doSomething() = 0;
    virtual bool isSquare();                        //Baddies are not squares
    virtual bool vortexAttack();                    //Can be attacked by vortexes
    int getPauseCounter();                          //Return the pause counter
    void changePauseCounter();                      //Alter pause counter
    void resetPauseCounter();                       //Set counter back to 180
    void hit();                                     //What to do if a baddie is hit by a vortex
    virtual ~Baddies();
private:
    int pauseCounter;
};

//////////////////////////////////////////////////////////////////////////////////////////              Derived Classes for Baddies
class Bowser : public Baddies
{
public:
    Bowser(StudentWorld* world, int imageID, int startX, int startY, int dir = right, int depth = 0, double size = 1.0);
    virtual void doSomething();
    virtual bool isSquare();
    virtual bool vortexAttack();
    virtual ~Bowser();
};

class Boo : public Baddies
{
public:
    Boo(StudentWorld* world, int imageID, int startX, int startY, int dir = right, int depth = 0, double size = 1.0);
    virtual void doSomething();
    virtual bool isSquare();
    virtual bool vortexAttack();
    virtual ~Boo();
};


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////              Derived Class for Vortex
class Vortex : public MovingObjects
{
public:
    Vortex(StudentWorld* world, int imageID, int startX, int startY, int dir = right, int depth = 0, double size = 1.0);
    virtual void doSomething();
    virtual bool isSquare();
    virtual bool vortexAttack();
private:
    int fly;                                        //Keep track of initial fly direction of vortex
};

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

#endif // ACTOR_H_
