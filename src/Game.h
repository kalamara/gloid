#ifndef _GAME_H
#define _GAME_H

#define ALIENHOME  3  // Where aliens spawn
#define BALLNUM    4  // Maximum number of balls in play
#define ALIENNUM  10  // Maximum number of aliens
#define SHOTNUM   32  // Maximum number of shots

// Dimensions of level, in bricks
#define INTX   6
#define INTY   6
#define INTZ  13

#define BRIKTYPES   2  // Types of bricks
#define LEVELTYPES  4  // Types of levels
#define MAXLEVEL   11  // Maximum level
#define LIVES 3
#define SCORELIFE  100000  // The score required for an extra life
#define SCOREBONUS   1000  // Score if you lose or catch a pill
#define SCOREWIN    10000  // Score if you pass a level
#define SCOREALIEN    100  // Score if you kill an alien
#define SCOREBRICK     70  // Score if you break a brick

#define MINSPEED  1.0f//minimum speed on any axis = 5% of initial speed on Z
#define MINFRAMEDURATION  10 //milliseconds per frame
#define MAXFRAMEDURATION  100

#define CAMERA_ROTATION   0.02f


class Game: public Engine<class Game>, public World{
public:
    Game();
    ~Game();

    unsigned int brickCount = 0;
    unsigned int bonusMode = 0;
    unsigned int lives = LIVES;
    unsigned int score = 0;
    class Step * step = nullptr;
    std::map<int, std::unique_ptr<class Step>> steps;
    unsigned int level = 1;
    std::multimap<int, std::string, std::greater<int>> hiscore;
/************** Engine ********************************************************/
    int queryStep(){
            return step->type;
    }
    Game & loop();
    class Step * nextStep();

    //gameplay interface
    bool isHiScoring(){
        return score > SCORELIFE;
    }
    unsigned int levelType(){
        return ((level - 1) % 4) + 1;
    }
    void setBonusMode(unsigned int mode){
        bonusMode = mode;
    }
    //int getLives();
    void incLives(){
        lives++;
    }

    std::string getBackgroundTextureFilename(){
        std::stringstream str;
        str << "arka" << levelType() << "_hires";
        return str.str();
    }

    void handleKeys();
/************** World *********************************************************/
    void divideBalls(){}
    void killVaus(){}
    Ball * getActiveBall(){return nullptr;}
    std::optional<Brick> getBrickAt(const Point3f& where);
};
#endif //_GAME_H
