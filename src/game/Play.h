#ifndef _PLAY_H
#define _PLAY_H

class Play:
        public Step{
public:

    Play();
    ~Play();
    Play & draw();
    Play & update();
    int next();





};
#endif //_GAME_H
