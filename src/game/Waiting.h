#ifndef _WAITING_H
#define _WAITING_H

typedef enum{
    WAIT_HOF, //display hall of fame
    WAIT_RDY, //prompt to press fire
    N_WAIT,
}WAIT_PHASE;

#define FLIP_PHASE 5000 //flip phase every 5 sec

class Waiting: public Step,  public Hud{
    int phase = N_WAIT;
    std::pair<GLuint, SDL_Surface> logo;
    class TextBody hallOfame;
    class TextBody prompt;
public:
    Waiting(Game &g);
    ~Waiting();
    int next();
    Waiting & update();
    Waiting & draw();

    int flip(unsigned int millis);
    void drawLogo();
};

#endif //_WAITING_H

