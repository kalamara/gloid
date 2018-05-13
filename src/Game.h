#ifndef _GAME_H
#define _GAME_H

class Game:
        public World,
        public Engine<class Game>{
public:
    int bonusMode;
    int level;
//gameplay interface
    bool isHiScoring();
    unsigned int levelType();
    //int getBonusMode();
    void setBonusMode(int type);
    //int getLives();
    void incLives();
    void divideBalls();
    void killVaus();

};
#endif //_GAME_H
