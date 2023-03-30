#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Board.h"
#include <string>

#include "Actor.h"
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp



class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    void text();                                            //Sets up scoreboard text
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    bool canMove(int x, int y);                             //Determines if MovingObject can move in a certain direction
    bool checkPlayerOn(int playerNum, int x, int y);        //Determines if player is at given coordinates
    int balance();                                          //Returns the total bank balance
    void changeBalance(int change);                         //Allows for altering of bank balance
    void resetBank();                                       //Set bank balance to 0
    Players* accessYoshi();                                 //Pointer to Yoshi player
    Players* accessPeach();                                 //Pointer to Peach player
    void createDropping(int x, int y);                      //Create dropping square
    baseActors* baddieOverlap(int x, int y);                //Return pointer to baddie that overlaps with vortex coordinates
    void createVortex(int x, int y, int dir);               //Create a vortex object with given coordinates and direction
    bool baddieShot(int x, int y);                          //Determines if a baddie was intercepted by vortex
    virtual ~StudentWorld();
private:
    std::string file;
    std::vector<baseActors*> actor;                         //Vector containing player pointers
    std::vector<Players*> player;                           //Vector containing all actors excluding players
    int bank;                                               //bank balance
};

#endif // STUDENTWORLD_H_
