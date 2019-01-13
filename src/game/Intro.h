#ifndef _INTRO_H
#define _INTRO_H

typedef enum{
    INTRO_STARTING,
    INTRO_STARTED,
    INTRO_DONE
} INTRO_PHASE;

class Intro: public Step{
    int phase = INTRO_STARTING;
public:

    Intro(Game &g);
    ~Intro();
    int next();
    Intro & update();
    Intro & draw();

};

#endif //_INTRO_H

