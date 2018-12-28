#ifndef _INTRO_H
#define _INTRO_H

typedef enum{

} INTRO_PHASE;

class Intro: public Step{
    std::vector<SDL_Surface *> text;
public:

    Intro(Game &g);
    ~Intro();
    int next();
    Intro & update();
    Intro & draw();

};

#endif //_INTRO_H

