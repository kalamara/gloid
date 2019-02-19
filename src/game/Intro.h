#ifndef _INTRO_H
#define _INTRO_H

typedef enum{
    INTRO_STARTING,
    INTRO_STARTED,
    INTRO_DONE
} INTRO_PHASE;

class Intro: public Step, public Hud{
    int phase = INTRO_STARTING;
    unsigned int cursor = 0;
    unsigned int tic = 0;
    class TextBody console;
public:

    Intro(Game &g);
    ~Intro();
    int next();
    Intro & update();
    Intro & draw();

    std::pair<int, std::string> readLine(unsigned int cursor);

};

#endif //_INTRO_H

