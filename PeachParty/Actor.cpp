#include "Actor.h"
#include "StudentWorld.h"
#include <random>

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////                 baseActors Class Implementation
baseActors::baseActors(StudentWorld* world, int imageID, int startX, int startY, int dir, int depth, double size)
:GraphObject(imageID, startX, startY, dir, depth, size), state(true), m_world(world), newPeach(true), newYoshi(true), impact(false)
{}

bool baseActors::isActive()                                 //Check if an actor is still active
{
    return state;
}

void baseActors::changeState(bool newState)                 //Change active state
{
    state = newState;
}

StudentWorld* baseActors::getWorld() const                  //Access student world
{
    return m_world;
}

void baseActors::changeStatus(int playerNum, bool newStatus)
{
    if (playerNum == 1)
        newPeach = newStatus;
    else
        newYoshi = newStatus;
}

bool baseActors::isNew(int playerNum)
{
    if (playerNum == 1)
        return newPeach;
    else
        return newYoshi;
}

bool baseActors::impacted()
{
    return impact;
}

void baseActors::changeImpact(bool action)
{
    impact = action;
}

baseActors::~baseActors()
{}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////                 MovingActors Class Implementation
MovingObjects::MovingObjects(StudentWorld* world, int imageID, int startX, int startY, int dir, int depth, double size)
:baseActors(world, imageID, startX, startY, dir, depth, size), walkDir(0), ticks_to_move(0), status(false), m_choiceMade(false), m_roll(0)
{}

int MovingObjects::walkDirection()
{
    return walkDir;
}

void MovingObjects::changeWalkDirection(int dir)
{
    walkDir = dir;
}

void MovingObjects::obstacleMove(int x, int y)
{
    getPositionInThisDirection(walkDir*90, 16, x, y);
    if (!getWorld()->canMove(x, y))                          //Change direction if cannot move forward
    {
        if (walkDir == 0 || walkDir == 2)//moving left or right      //Prioritize up over down
        {
            getPositionInThisDirection(up, 16, x, y);
            if (getWorld()->canMove(x, y))
                walkDir = 1;
            else
                walkDir = 3;
        }
        else if (walkDir == 1 || walkDir == 3)//moving up or down    //Prioritize right over left
        {
            getPositionInThisDirection(right, 16, x, y);
            if (getWorld()->canMove(x, y))
                walkDir = 0;
            else
                walkDir = 2;
        }
        
    }
    changeSpriteDir(walkDir);
}

void MovingObjects::changeSpriteDir(int dir)
{
    if (dir == 2)                                 //Change sprite direction
    {
        //sprite facing 180 degrees
        setDirection(left);
    }
    else
        //sprite facing 0 degrees
        setDirection(right);
}

int MovingObjects::ticks()
{
    return ticks_to_move;
}

void MovingObjects::changeTicks(int newT)
{
    if (!(ticks_to_move + newT < 0))
    ticks_to_move += newT;
}

bool MovingObjects::isWalking()
{
    return status;
}

void MovingObjects::changeWalkStatus(bool newStatus)
{
    status = newStatus;
}

bool MovingObjects::checkFork(int x, int y)
{
    int directions = 0;
    for (int i = 0; i <= 3; i++)                                 //Check all 4 directions to see if there are valid squares
    {
        if (x%16==0 && y%16==0)
        {
            getPositionInThisDirection(i*90, 16, x, y);
            if (getWorld()->canMove(x, y))
                directions++;
        }
    }
    if (directions > 2)                                         //If more than 2 valid paths then at a junction
        return true;
    else
        return false;
}

bool MovingObjects::choiceMade()
{
    return m_choiceMade;
}

void MovingObjects::changeChoiceMade(bool newStatus)
{
    m_choiceMade = newStatus;
}

void MovingObjects::junctionChoice(int dir, int x, int y)
{
    int xtemp = x;
    int ytemp = y;
    getPositionInThisDirection(dir*90, 16, xtemp, ytemp);
    if ((walkDir != (dir + 2)%4) && getWorld()->canMove(xtemp, ytemp))      //Cannot be in previous direction
    {
        changeWalkDirection(dir);
        changeSpriteDir(dir);
        m_choiceMade = true;
    }
}

void MovingObjects::teleport()
{
    bool success = false;
    while(!success)
    {
        int x = randInt(0, BOARD_WIDTH);
        int y = randInt(0, BOARD_HEIGHT);
        x*=SPRITE_WIDTH;
        y*=SPRITE_HEIGHT;
        
        if (getWorld()->canMove(x, y))                      //If can move to valid location, then teleport
        {
            moveTo(x, y);
            success = true;
            randomDirection(x, y);                          //Gives random walking direction after teleporting
            obstacleMove(x, y);
        }
    }
}

void MovingObjects::baddiesWalk(int x, int y)
{
    if (x%16 == 0 && y%16 == 0)
    {
        if (checkFork(x, y))                            //There's a fork in the road
        {
            randomDirection(x, y);
            changeSpriteDir(walkDir);
        }
        obstacleMove(x, y);                             //Checks at all times if baddie is in a corner
    }
}

void MovingObjects::randomDirection(int x, int y)
{
    bool validDir = false;
    while(!validDir)
    {
        int direction = randInt(0, 3);
        getPositionInThisDirection(direction*90, 16, x, y);
        if (getWorld()->canMove(x, y))
        {
            changeWalkDirection(direction);
            validDir = true;
        }
    }
}

void MovingObjects::allWalk()                           //Walk 2 pixels each tick in given walking direction
{
    switch(walkDirection())
    {
        case 0:
            moveAtAngle(right, 2);
            break;
        case 1:
            moveAtAngle(up, 2);
            break;
        case 2:
            moveAtAngle(left, 2);
            break;
        case 3:
            moveAtAngle(down, 2);
            break;
    }
}

void MovingObjects::swap(MovingObjects* other, int x, int y)
{
    int tempX = x;                                         //swap location
    int tempY = y;
    moveTo(other->getX(), other->getY());
    other->moveTo(tempX, tempY);
    
    std::swap(ticks_to_move, other->ticks_to_move);             //swap ticks
    
    std::swap(m_roll, other->m_roll);                           //swap roll count
    
    std::swap(walkDir, other->walkDir);                         //swap direction
    
    changeSpriteDir(walkDirection());                           //adapt sprite direction
    other->changeSpriteDir(other->walkDirection());
    
    std::swap(status, other->status);                           //swap walking status
}

int MovingObjects::getRolls()
{
    return m_roll;
}

void MovingObjects::changeRolls(int change)
{
    if (!(m_roll + change < 0))
        m_roll += change;
}

MovingObjects::~MovingObjects()
{}

///////////////////////////////////////////////////////////////////////////////////////                 Players Class Implementation
Players::Players(StudentWorld* world, int imageID, int startX, int startY, int playerNum, int dir, int depth, double size)
:MovingObjects(world, imageID, startX, startY, dir, depth, size), player(playerNum), m_coins(0), m_stars(0), m_walkingDirChanged(false), m_Dchange(0), m_vortex(0), prevWalkDir(0)
{}

void Players::doSomething()
{

    if (!isWalking())                                         //If waiting to roll, get dice rolls
    {
        int action = getWorld()->getAction(player);
        switch(action)
        {
            case ACTION_NONE:
                break;
            case ACTION_RIGHT:
                changeWalkDirection(0);
                break;
            case ACTION_UP:
                changeWalkDirection(1);
                break;
            case ACTION_LEFT:
                changeWalkDirection(2);
                break;
            case ACTION_DOWN:
                changeWalkDirection(3);
                break;
            case ACTION_ROLL:
            {
                changeRolls(randInt(1, 10));
                //changeRolls(8);
                changeTicks(getRolls() * 8);
                changeWalkStatus(true);
            }
                break;
            case ACTION_FIRE:
            {
                if (m_vortex == 1)
                {
                    int x = getX();
                    int y = getY();
                    getPositionInThisDirection(walkDirection()*90, 16, x, y);
                    getWorld()->createVortex(x, y, walkDirection()*90);
                    getWorld()->playSound(SOUND_PLAYER_FIRE);
                    m_vortex = 0;
                }
            }
                break;
        }
    }
     
    else if (isWalking())                                        //If in walking state
    {
        int x = getX();
        int y = getY();
        
        if (m_walkingDirChanged)                                 //If player encounters direction square
        {
            changeWalkDirection(m_Dchange);
            changeSpriteDir(walkDirection());
            walkingDirChanged(false);
        }
        
        else if (checkFork(x, y))                               //If player encounters a fork in the path
        {
            int action = getWorld()->getAction(player);
            switch(action)
            {
                case ACTION_RIGHT:
                    if (0 != (prevWalkDir+2)%4)
                        junctionChoice(0, x, y);
                    break;
                case ACTION_UP:
                    if (1 != (prevWalkDir+2)%4)
                        junctionChoice(1, x, y);
                    break;
                case ACTION_LEFT:
                    if (2 != (prevWalkDir+2)%4)
                        junctionChoice(2, x, y);
                    break;
                case ACTION_DOWN:
                    if (3 != (prevWalkDir+2)%4)
                        junctionChoice(3, x, y);
                    break;
                case ACTION_NONE:
                    break;
                case ACTION_ROLL:
                    break;
                case ACTION_FIRE:
                    break;
            }
            if (!choiceMade())
                return;
            changeChoiceMade(false);
        }
        
        else if (x%16 == 0 && y%16 == 0)                    //Tests if the player hits a corner
        {
            obstacleMove(x, y);
        }
    
        //Move 2 pixels in walking direction and decrement ticks
        allWalk();
        
        //Decrement ticks to move by 1
        changeTicks(-1);
        
        //Decrement roll count each movement
        if ((x%16 == 0) && (y%16 == 0) && getRolls() >= 1)
            changeRolls(-1);

        //if ticks to move is 0, change to waiting to roll state
        if (ticks() == 0)
        {
            changeWalkStatus(false);
        }
    }
    prevWalkDir = walkDirection();
}

int Players::coins()                            //Get # of coins
{
    return m_coins;
}

int Players::stars()                            //Get # of stars
{
    return m_stars;
}

void Players::adjustCoins(int coins)      //change # of coins
{
    if (!(coins + m_coins < 0))
        m_coins+=coins;
    else
        m_coins = 0;
}

void Players::adjustStars(int stars)      //change # of stars
{
    if (!(m_stars + stars < 0))
        m_stars += stars;
}

bool Players::isSquare()
{
    return false;
}

bool Players::vortexAttack()
{
    return true;
}

bool Players::landedOn(int x, int y)            //player landed on an object and is in waiting to roll state
{
    return (ticks() == 0 && (getWorld()->checkPlayerOn(player, x, y)));
}

bool Players::passed(int x, int y)              //player passed an object but did not land
{
    return (ticks() != 0 && (getWorld()->checkPlayerOn(player, x, y)));
}

bool Players::walkingDirChanged(bool changed)   //confirm that walking direction was changed
{
    m_walkingDirChanged = changed;
    return m_walkingDirChanged;
}

int Players::newWalkingDirection(int change)    //change walking direction and conver it to 0, 1, 2, 3
{
    m_Dchange = change / 90;
    return m_Dchange;
}

void Players::getVortex()                   //given vortex to player (max 1)
{
    if (m_vortex == 0)
        m_vortex = 1;
}

bool Players::hasVortex()
{
    if (m_vortex == 1)
        return true;
    return false;
}

void Players::swapCoins(Players* other)
{
    std::swap(m_coins, other->m_coins);     //swap player coins
}

void Players::swapStars(Players* other)
{
    std::swap(m_stars, other->m_stars);     //swap player stars
}

Players::~Players()
{}

///////////////////////////////////////////////////////////////////////////////////////                 Vortex Class Implementation
Vortex::Vortex(StudentWorld* world, int imageID, int startX, int startY, int dir, int depth, double size)
:MovingObjects(world, imageID, startX, startY, dir, depth, size), fly(dir)
{}

void Vortex::doSomething()
{
        if (!isActive())                //make sure is active
            return;
        moveAtAngle(fly, 2);        //move 2 pixels in spawn direction
        
        int x = getX();
        int y = getY();
        
        if ((x < 0 || x > VIEW_WIDTH) || (y < 0 || y > VIEW_HEIGHT))        //check if out of bounds
            changeState(false);
        
        //check for overlapping objects
        if (getWorld()->baddieShot(x, y))
        {
            getWorld()->baddieOverlap(x, y)->changeImpact(true);
            getWorld()->playSound(SOUND_HIT_BY_VORTEX);
            changeState(false);
        }
}

bool Vortex::isSquare()
{
    return false;
}

bool Vortex::vortexAttack()
{
    return false;
}



///////////////////////////////////////////////////////////////////////////////////////'
///////////////////////////////////////////////////////////////////////////////////////             Squares Class Implementation
Squares::Squares(StudentWorld* world, int imageID, int startX, int startY, int dir, int depth, double size)
:baseActors(world, imageID, startX, startY, dir, depth, size)
{}

bool Squares::isSquare()
{
    return true;
}

bool Squares::vortexAttack()
{
    return false;
}

Squares::~Squares()
{}

///////////////////////////////////////////////////////////////////////////////////////                 Coin Square Class Implementation
CoinSquare::CoinSquare(StudentWorld* world, int imageID, int startX, int startY, char type, int dir, int depth, double size)
:Squares(world, imageID, startX, startY, dir, depth, size), m_type(type)
{}

void CoinSquare::doSomething()
{
    if (!isActive())
        return;
    Players* accessPlayer[]={getWorld()->accessPeach(), getWorld()->accessYoshi()};
    
    for (int i = 0; i < 2; i++)
    {
        if (!accessPlayer[i]->landedOn(getX(), getY()) && !isNew(i+1))
            changeStatus(i+1, true);
        
        if (accessPlayer[i]->landedOn(getX(), getY()) && isNew(i+1))
        {
            if (m_type == 'b')                                          //Give 3 coins if determined blue, else take 3
            {
                accessPlayer[i]->adjustCoins(3);
                getWorld()->playSound(SOUND_GIVE_COIN);
            }
            else
            {
                accessPlayer[i]->adjustCoins(-3);
                getWorld()->playSound(SOUND_TAKE_COIN);
            }
            changeStatus(i+1, false);
        }
    }
}

bool CoinSquare::isSquare()
{
    return true;
}

bool CoinSquare::vortexAttack()
{
    return false;
}

CoinSquare::~CoinSquare()
{}
         
///////////////////////////////////////////////////////////////////////////////////////                 Star Square Class Implementation
starSquare::starSquare(StudentWorld* world, int imageID, int startX, int startY, int dir, int depth, double size)
:Squares(world, imageID, startX, startY, dir, depth, size)
{}

void starSquare::doSomething()
{
    Players* accessPlayer[]={getWorld()->accessPeach(), getWorld()->accessYoshi()};
    for (int i = 0; i < 2; i++)
    {
        if (accessPlayer[i]->coins() < 20)      //Check if eligible to receive star, otherwise just exit function
            return;
        
        //Prevents player from collecting more than 1 star at a time
        if (!accessPlayer[i]->landedOn(getX(), getY()) && !accessPlayer[i]->passed(getX(), getY()) && !isNew(i+1))
            changeStatus(i+1, true);
        
        //If player landed on or passed star sqaure and is a new player, take 20 coins away and add 1 star
        //Let the program know that the player has now been here = no longer a new player
        if (isNew(i+1) && (accessPlayer[i]->landedOn(getX(), getY()) || accessPlayer[i]->passed(getX(), getY())))
        {
            accessPlayer[i]->adjustCoins(-20);
            accessPlayer[i]->adjustStars(1);
            getWorld()->playSound(SOUND_GIVE_STAR);
            changeStatus(i+1, false);
        }
    }
}

bool starSquare::isSquare()
{
    return true;
}

bool starSquare::vortexAttack()
{
    return false;
}

starSquare::~starSquare()
{}

///////////////////////////////////////////////////////////////////////////////////////                 Direction Square Class Implementation
directionSquare::directionSquare(StudentWorld* world, int imageID, int startX, int startY, int forcingDirection, int dir, int depth, double size)
:Squares(world, imageID, startX, startY, dir, depth, size), orientation(forcingDirection)
{}

void directionSquare::doSomething()
{
    //Set orientation of the image based on the data from the board file
    switch(orientation)
    {
        case 0:
            setDirection(right);
            break;
        case 90:
            setDirection(up);
            break;
        case 180:
            setDirection(left);
            break;
        case 270:
            setDirection(down);
            break;
    }
    
    Players* accessPlayer[]={getWorld()->accessPeach(), getWorld()->accessYoshi()};
    for (int i = 0; i < 2; i++)
    {
        if (accessPlayer[i]->landedOn(getX(), getY()) || accessPlayer[i]->passed(getX(), getY()))
        {
            accessPlayer[i]->walkingDirChanged(true);                   //Change walking direction based on square direction
            accessPlayer[i]->newWalkingDirection(orientation);
        }
    }
    
}

bool directionSquare::isSquare()
{
    return true;
}

bool directionSquare::vortexAttack()
{
    return false;
}

directionSquare::~directionSquare()
{}

///////////////////////////////////////////////////////////////////////////////////////                 Bank Square Class Implementation
bankSquare::bankSquare(StudentWorld* world, int imageID, int startX, int startY, int dir, int depth, double size)
:Squares(world, imageID, startX, startY, dir, depth, size)
{}

void bankSquare::doSomething()
{
    Players* accessPlayer[]={getWorld()->accessPeach(), getWorld()->accessYoshi()};
    
    for (int i = 0; i < 2; i++)
    {
        if (!accessPlayer[i]->landedOn(getX(), getY()) && !accessPlayer[i]->passed(getX(), getY()) && !isNew(i+1))
            changeStatus(i+1, true);
        
        if (accessPlayer[i]->landedOn(getX(), getY()) && isNew(i+1))       //If player lands on bank, player withdraws total
        {
            int balance = getWorld()->balance();
            accessPlayer[i]->adjustCoins(balance);
            getWorld()->resetBank();
            getWorld()->playSound(SOUND_WITHDRAW_BANK);
            changeStatus(i+1, false);
        }
        
        if (accessPlayer[i]->passed(getX(), getY()) && isNew(i+1))         //If player pases bank, take 5 coins from player
        {
            int lessCoins = -5;
            if (accessPlayer[i]->coins() < 5)
                lessCoins = 0 - accessPlayer[i]->coins();
            accessPlayer[i]->adjustCoins(lessCoins);
            getWorld()->changeBalance(0-lessCoins);
            getWorld()->playSound(SOUND_DEPOSIT_BANK);
            changeStatus(i+1, false);
        }
    }
}

bool bankSquare::isSquare()
{
    return true;
}

bool bankSquare::vortexAttack()
{
    return false;
}

bankSquare::~bankSquare()
{}

///////////////////////////////////////////////////////////////////////////////////////                 Event Square Class Implementation
eventSquare::eventSquare(StudentWorld* world, int imageID, int startX, int startY, int dir, int depth, double size)
:Squares(world, imageID, startX, startY, dir, depth, size), swapped(false)
{}

void eventSquare::doSomething()
{
    Players* accessPlayer[]={getWorld()->accessPeach(), getWorld()->accessYoshi()};
    
    for (int i = 0; i < 2; i++)
    {
        if (!accessPlayer[i]->landedOn(getX(), getY()) && !isNew(i+1))
        {
            changeStatus(i+1, true);
        }
        
        
        if (accessPlayer[i]->landedOn(getX(), getY()) && isNew(i+1) && swapped == false)
        {
            int x = randInt(1, 3);
            if (x == 1)
            {
                //Teleport to random valid location on board
                accessPlayer[i]->teleport();
                getWorld()->playSound(SOUND_PLAYER_TELEPORT);
            }
            else if (x == 2)
            {
                    //swap player locations
                    if (i == 0)
                        accessPlayer[i]->swap(getWorld()->accessYoshi(), getX(), getY());
                    else
                        accessPlayer[i]->swap(getWorld()->accessPeach(), getX(), getY());
                    //accessPlayer[i]->swap(accessPlayer[(i+1)%2], getX(), getY());
                    getWorld()->playSound(SOUND_PLAYER_TELEPORT);
                    changeStatus(i+1, false);
                    swapped = true;
                return;
            }
            else
            {
                //Give Vortex to player (max 1 at a time)
                accessPlayer[i]->getVortex();
                getWorld()->playSound(SOUND_GIVE_VORTEX);
            }
            changeStatus(i+1, false);
            swapped = false;
        }
    }
}

bool eventSquare::isSquare()
{
    return true;
}

bool eventSquare::vortexAttack()
{
    return false;
}

eventSquare::~eventSquare()
{}

///////////////////////////////////////////////////////////////////////////////////////                 Dropping Square Class Implementation
droppingSquare::droppingSquare(StudentWorld* world, int imageID, int startX, int startY, int dir, int depth, double size)
:Squares(world, imageID, startX, startY, dir, depth, size)
{}

void droppingSquare::doSomething()
{
    Players* accessPlayer[]={getWorld()->accessPeach(), getWorld()->accessYoshi()};
    
    for (int i = 0; i < 2; i++)
    {
        if (!accessPlayer[i]->landedOn(getX(), getY()) && !isNew(i+1))
            changeStatus(i+1, true);
        
        if (isNew(i+1) && accessPlayer[i]->landedOn(getX(), getY()))
        {
            int choice = randInt(1, 2);
                if (choice == 1)
                    accessPlayer[i]->adjustCoins(-10);                          //Either take 10 coins
                else
                    accessPlayer[i]->adjustStars(-1);                           //Or take 1 star
            getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
            changeStatus(i+1, false);
        }
    }
}

bool droppingSquare::isSquare()
{
    return true;
}

bool droppingSquare::vortexAttack()
{
    return false;
}

droppingSquare::~droppingSquare()
{}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////                 Baddies Class Implementation

Baddies::Baddies(StudentWorld* world, int imageID, int startX, int startY, int dir, int depth, double size)
:MovingObjects(world, imageID, startX, startY, dir, depth, size), pauseCounter(180)
{}

bool Baddies::isSquare()
{
    return false;
}

bool Baddies::vortexAttack(){
    return true;
}

int Baddies::getPauseCounter()
{
    return pauseCounter;
}

void Baddies::changePauseCounter()
{
    if (pauseCounter > 0)
        pauseCounter--;
}

void Baddies::resetPauseCounter()
{
    pauseCounter = 180;
}

void Baddies::hit()                                 //Teleport once hit
{
    teleport();
    changeWalkDirection(right);                     //Set direction to right
    setDirection(0);                                //Adapt sprite direction to right
    changeWalkStatus(false);                        //Baddie no longer walking
    resetPauseCounter();                            //Baddie pause timer reset to 180
}

Baddies::~Baddies()
{}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////                 Bowser Class Implementation
Bowser::Bowser(StudentWorld* world, int imageID, int startX, int startY, int dir, int depth, double size)
:Baddies(world, imageID, startX, startY, dir, depth, size)
{}

void Bowser::doSomething()
{
    if (impacted())
    {
        hit();
        changeImpact(false);
    }
    
    Players* accessPlayer[]={getWorld()->accessPeach(), getWorld()->accessYoshi()};
    if (!isWalking())
    {
        for (int i = 0; i < 2; i++)
        {
            if (!accessPlayer[i]->landedOn(getX(), getY()) && !isNew(i+1))
                changeStatus(i+1, true);
            
            if (accessPlayer[i]->landedOn(getX(), getY()) && isNew(i+1))            //Bowser and player on same square
            {
                int choice = randInt(1,2);
                if (choice == 1)
                {
                    accessPlayer[i]->adjustCoins(0-accessPlayer[i]->coins());       //50% change to reset player[i] coins
                    getWorld()->playSound(SOUND_BOWSER_ACTIVATE);
                }
                changeStatus(i+1, false);
            }
        }
        
        changePauseCounter();
        
        if (getPauseCounter() == 0)
        {
            int squares_to_move = randInt(1, 10);
            changeTicks(squares_to_move*8);
            randomDirection(getX(), getY());
            changeSpriteDir(walkDirection());
            changeWalkStatus(true);
        }
    }
    else if (isWalking())
    {
        //get direction of movement
        baddiesWalk(getX(), getY());
        
        //Walk and decrement ticks by 1
        allWalk();
        
        changeTicks(-1);
        
        if (ticks() == 0)
        {
            changeWalkStatus(false);
            resetPauseCounter();
            int rand = randInt(1, 4);
            if (rand == 1)
            {
                //delete current square and create a dropping square
                getWorld()->createDropping(getX(), getY());                 //1/4 chance to make dropping square once paused
                getWorld()->playSound(SOUND_DROPPING_SQUARE_CREATED);
            }
            
        }
    }
}

bool Bowser::isSquare()
{
    return false;
}

bool Bowser::vortexAttack()
{
    return true;
}


Bowser::~Bowser()
{}

///////////////////////////////////////////////////////////////////////////////////////                 Boo Class Implementation
Boo::Boo(StudentWorld* world, int imageID, int startX, int startY, int dir, int depth, double size)
:Baddies(world, imageID, startX, startY, dir, depth, size)
{}

void Boo::doSomething()
{
    if (impacted())
    {
        hit();
        changeImpact(false);
    }
    
    Players* accessPlayer[]={getWorld()->accessPeach(), getWorld()->accessYoshi()};
    
    if (!isWalking())
    {
        for (int i = 0; i < 2; i++)
        {
            if (!accessPlayer[i]->landedOn(getX(), getY()) && !isNew(i+1))
                changeStatus(i+1, true);
            
            if (accessPlayer[i]->landedOn(getX(), getY()) && isNew(i+1))        //if player and boo are on same square
            {
                int choice = randInt(1,2);
                if (choice == 1)
                {
                    accessPlayer[i]->swapCoins(accessPlayer[(i+1)%2]);      //1/2 chance to swap player coins
                    getWorld()->playSound(SOUND_BOO_ACTIVATE);
                    changeStatus(i+1, false);
                }
                else if (choice == 2)
                {
                    accessPlayer[i]->swapStars(accessPlayer[(i+1)%2]);      //1/2 change to swap player stars
                    getWorld()->playSound(SOUND_BOO_ACTIVATE);
                    changeStatus(i+1, false);
                }
            }
        }
        
        changePauseCounter();
        
        if (getPauseCounter() == 0)
        {
            int squares_to_move = randInt(1, 3);
            changeTicks(squares_to_move*8);
            randomDirection(getX(), getY());
            changeSpriteDir(walkDirection());
            changeWalkStatus(true);
        }
    }
    
    else if (isWalking())
    {
        //get direction of movement
        baddiesWalk(getX(), getY());
        
        //Walk and decrement ticks by 1
        allWalk();
        
        changeTicks(-1);
        
        if (ticks() == 0)
        {
            changeWalkStatus(false);
            resetPauseCounter();
        }
    }
}

bool Boo::isSquare()
{
    return false;
}

bool Boo::vortexAttack()
{
    return true;
}

Boo::~Boo()
{}


