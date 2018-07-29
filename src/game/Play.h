#ifndef _PLAY_H
#define _PLAY_H

class Play:
        public World,
        public Step{
public:
    int bonusMode;
    int level;
    Play();
    ~Play();
    Play & draw();
    Play & update();
    Play & next();

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
