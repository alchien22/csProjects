#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <sstream>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath), file(""), bank(0)
{}

int StudentWorld::init()
{
    Board bd;
    file = assetPath();
    
    int boardSel = getBoardNumber();
    switch (boardSel)                               //Change file string based on board selection
    {
        case 1:
            file+="board01.txt";
            break;
        case 2:
            file+="board02.txt";
            break;
        case 3:
            file+="board03.txt";
            break;
        case 4:
            file+="board04.txt";
            break;
        case 5:
            file+="board05.txt";
            break;
        case 6:
            file+="board06.txt";
            break;
        case 7:
            file+="board0.txt";
            break;
        case 8:
            file+="board08.txt";
            break;
        case 9:
            file+="board09.txt";
            break;
    }
    
    Board::LoadResult result = bd.loadBoard(file);                      //Load board using file string
    
    if (result == Board::load_fail_file_not_found)
    {
        cerr << "Could not find board01.txt data file\n";
        return GWSTATUS_BOARD_ERROR;
    }
    else if (result == Board::load_fail_bad_format)
    {
        cerr << "Your board was improperly formatted\n";
        return GWSTATUS_BOARD_ERROR;
    }
    else if (result == Board::load_success)
        cerr << "Successfully loaded board\n";
        
        for (int x = 0; x < BOARD_WIDTH; x++)
        {
            for (int y = 0; y < BOARD_HEIGHT; y++)
            {
                Board::GridEntry ge = bd.getContentsOf(x, y);
                switch (ge) {                                           //Determine board contents and create objects
                    case Board::empty:
                        break;
                    case Board::player:
                    {
                        player.push_back(new Players(this, 0, x*SPRITE_WIDTH, y*SPRITE_HEIGHT, 1));
                        player.push_back(new Players(this, 1, x*SPRITE_WIDTH, y*SPRITE_HEIGHT, 2));
                        actor.push_back(new CoinSquare(this, 2, x*SPRITE_WIDTH, y*SPRITE_HEIGHT, 'b', 0, 2));
                    }
                        break;
                    case Board::blue_coin_square:
                        actor.push_back(new CoinSquare(this, 2, x*SPRITE_WIDTH, y*SPRITE_HEIGHT, 'b', 0, 2));
                        break;
                    case Board::red_coin_square:
                        actor.push_back(new CoinSquare(this, 3, x*SPRITE_WIDTH, y*SPRITE_HEIGHT, 'r', 0, 2));
                        break;
                    case Board::up_dir_square:
                        actor.push_back(new directionSquare(this, 4, x*SPRITE_WIDTH, y*SPRITE_HEIGHT, 90, 0, 2));
                        break;
                    case Board::down_dir_square:
                        actor.push_back(new directionSquare(this, 4, x*SPRITE_WIDTH, y*SPRITE_HEIGHT, 270, 0, 2));
                        break;
                    case Board::left_dir_square:
                        actor.push_back(new directionSquare(this, 4, x*SPRITE_WIDTH, y*SPRITE_HEIGHT, 180, 0, 2));
                        break;
                    case Board::right_dir_square:
                        actor.push_back(new directionSquare(this, 4, x*SPRITE_WIDTH, y*SPRITE_HEIGHT, 0, 0, 2));
                        break;
                    case Board::event_square:
                        actor.push_back(new eventSquare(this, 5, x*SPRITE_WIDTH, y*SPRITE_HEIGHT, 0, 2));
                        break;
                    case Board::bank_square:
                        actor.push_back(new bankSquare(this, 6, x*SPRITE_WIDTH, y*SPRITE_HEIGHT, 0, 2));
                        break;
                    case Board::star_square:
                        actor.push_back(new starSquare(this, 7, x*SPRITE_WIDTH, y*SPRITE_HEIGHT, 0, 2));
                        break;
                    case Board::bowser:
                    {
                        actor.push_back(new Bowser(this, 9, x*SPRITE_WIDTH, y*SPRITE_HEIGHT, 0, 1));
                        actor.push_back(new CoinSquare(this, 2, x*SPRITE_WIDTH, y*SPRITE_HEIGHT, 'b', 0, 2));
                    }
                        break;
                    case Board::boo:
                    {
                        actor.push_back(new Boo(this, 10, x*SPRITE_WIDTH, y*SPRITE_HEIGHT, 0, 1));
                        actor.push_back(new CoinSquare(this, 2, x*SPRITE_WIDTH, y*SPRITE_HEIGHT, 'b', 0, 2));
                    }
                        break;
                }
            }
        }
    startCountdownTimer(99);                                                //Begin countdown of game at 99 seconds
    
    text();                                                                 //Set the game scoreboard
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    for (int i = 0; i < 2; i++)                             //If players are still active, then call do something
    {
        if (player[i]->isActive())
            player[i]->doSomething();
    }
 
    for (int i = 0; i < actor.size(); i++)                  //If board pieces still active, then call do something for each
    {
        if (actor[i]->isActive())
            actor[i]->doSomething();
    }

    vector<Players*>::iterator q = player.begin();          //Erase any nonactive players
    while (q != player.end())
    {
        if (!((*q)->isActive()))
        {
            delete *q;
            q = player.erase(q);
        }
        else
            q++;
    }
     
    vector<baseActors*>::iterator p = actor.begin();        //Erase any nonactive board pieces other than players
    while (p != actor.end())
    {
        if (!((*p)->isActive()))
        {
            delete *p;
            p = actor.erase(p);
        }
        else
            p++;
    }

    text(); //Update scoreboard
    
    if (timeRemaining() <= 0)                               //If time is up, determine winner
    {
        playSound(SOUND_GAME_FINISHED);
        
        int totalP1 = player[0]->coins() + player[0]->stars();
        int totalP2 = player[1]->coins() + player[1]->stars();
        int winner = 0;
        
        if (player[1]->stars() > player[0]->stars())                            //P2 wins if they have more stars
            winner = 2;
        
        else if (player[1]->stars() < player[0]->stars())                       //P1 wins if they have more stars
            winner = 1;
        
        else if (player[1]->stars() == player[0]->stars())                      //Same # of stars = coins are tiebreaker
        {
            if (player[1]->coins() > player[0]->coins())
                winner = 2;
            else
                winner = 1;
        }
        
        else if (totalP2 == totalP1)                          //Tie Game = random winner chosen
            winner = randInt(1, 2);
        
        if (winner == 1)                                                    //Set final scoreboard for winner
        {
            setFinalScore(player[0]->stars(), player[0]->coins());
            return GWSTATUS_PEACH_WON;
        }
        else if (winner == 2)
        {
            setFinalScore(player[1]->stars(), player[1]->coins());
            return GWSTATUS_YOSHI_WON;
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::createDropping(int x, int y)
{
    vector<baseActors*>::iterator p = actor.begin();
    bool found = false;
    while (p != actor.end() && !found)                                  //Find the square that Bowser is on and delete it
    {
        if ((*p)->getX() == x && (*p)->getY() == y && !(*p)->vortexAttack())
        {
            (*p)->changeState(false);
            found = true;
        }
        else
            p++;
    }
    actor.push_back(new droppingSquare(this, 8, x, y, 0, 2));           //Create new dropping square replacing old square
}

bool StudentWorld::canMove(int x, int y)
{
    Board bd;
    bd.loadBoard(file);
    for (int i = 0; i < actor.size(); i++)                              //Check if there is a square at given location
    {
        if (actor[i]->getX() == x && actor[i]->getY() == y && !actor[i]->vortexAttack())
            return true;
    }
    return false;
}

bool StudentWorld::checkPlayerOn(int playerNum, int x, int y)
{
    Board bd;
    bd.loadBoard(file);
    if (player[playerNum-1]->getX() == x && player[playerNum-1]->getY() == y)   //Check if player is at given location
        return true;
    return false;
}

baseActors* StudentWorld::baddieOverlap(int x, int y)
{
    for (int i = 0; i < actor.size(); i++)
        if (actor[i]->vortexAttack())                   //check if it's an actor that can be targetted
            if (((x + 15) >= actor[i]->getX() - 15) && ((x - 15) <= actor[i]->getX() + 15))         //check x bounds
                if (((y + 15) >= actor[i]->getY() - 15) && ((y - 15) <= actor[i]->getY() + 15))     //check y bounds
                    return actor[i];
    return nullptr;
}

bool StudentWorld::baddieShot(int x, int y)
{
    for (int i = 0; i < actor.size(); i++)
        if (actor[i]->vortexAttack())                   //check if it's an actor that can be targetted
            if (((x + 15) >= actor[i]->getX() - 15) && ((x - 15) <= actor[i]->getX() + 15))         //check x bounds
                if (((y + 15) >= actor[i]->getY() - 15) && ((y - 15) <= actor[i]->getY() + 15))     //check y bounds
                    return true;                        //Return true if baddie intercepted, else return false
    return false;
}

void StudentWorld::createVortex(int x, int y, int dir)      //Create vortex using player direction and location
{
    actor.push_back(new Vortex(this, 11, x, y, dir, 0));
}

Players* StudentWorld::accessYoshi()
{
    return player[1];
}

Players* StudentWorld::accessPeach()
{
    return player[0];
}

int StudentWorld::balance()
{
    return bank;
}

void StudentWorld::changeBalance(int change)
{
    bank += change;
}

void StudentWorld::resetBank()
{
    bank = 0;
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

void StudentWorld::text()
{
    string vortex[2] = {};
    string rolls[2]={};
    string coins[2]={};
    string stars[2]={};
    for (int i = 0; i < player.size(); i++)                     //Used stringstream to convert integer values to strings
    {
        stringstream convertP1R;
        string rollsP1;
        convertP1R << player[i]->getRolls();
        convertP1R >> rolls[i];
        
        stringstream convertP1C;
        string coinsP1;
        convertP1C << player[i]->coins();
        convertP1C >> coins[i];
        
        stringstream convertP1S;
        string starsP1;
        convertP1S << player[i]->stars();
        convertP1S >> stars[i];
        
        if (player[i]->hasVortex())                 //Determine if player has a vortex
        {
            vortex[i] = "VOR";
        }
    }
    
    stringstream convertTR;
    string timeLeft;
    convertTR << timeRemaining();
    convertTR >> timeLeft;
    
    stringstream convertBank;
    string bankBalance;
    convertBank << balance();
    convertBank >> bankBalance;
    
    //Finalized display text
    string text = "P1 Roll: " + rolls[0] + " Stars: " + stars[0] + " $$: " + coins[0] + " " + vortex[0] + " | Time: " + timeLeft + " | Bank: " + bankBalance + " | P2 Roll: " + rolls[1] + " Stars: " + stars[1] + " $$: " + coins[1] + " " + vortex[1];
    
    setGameStatText(text);                      //Set scoreboard text
}

void StudentWorld::cleanUp()                    //Used iterators to delete all remaining actors at end of game
{
    
    for (int i = 0; i < player.size(); i++)
    {
        delete player[i];
    }
    player.clear();
    
    for (int i = 0; i < actor.size(); i++)
    {
        delete actor[i];
    }
    actor.clear();
     
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}
