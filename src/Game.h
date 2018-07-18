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

#define SCORELIFE  100000  // The score required for an extra life
#define SCOREBONUS   1000  // Score if you lose or catch a pill
#define SCOREWIN    10000  // Score if you pass a level
#define SCOREALIEN    100  // Score if you kill an alien
#define SCOREBRICK     70  // Score if you break a brick

#define MINSPEED  1.0f//minimum speed on any axis = 5% of initial speed on Z

#define VAUS_PARTICLES  128  //asplozans!
#define ALIEN_PARTICLES 1024

#define VAUS_COLORS  4
#define ALIEN_COLORS 3

class Game: public Engine<class Game>{
public:
    Game();
    ~Game();
    class Step * step = nullptr;
    int queryStep(){

            return step?(step->type):-1;
    }
    Game * loop();
    /*Game* start(class Step * at){
        step = at;
    }*///this might merge with nextStep if at == nullptr
    class Step * nextStep();

};
#endif //_GAME_H
