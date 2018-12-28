#ifndef _WAITING_H
#define _WAITING_H

typedef enum{

}WAIT_PHASE;

class Waiting: public Step{
    std::vector<SDL_Surface *> text;
public:
    Waiting(Game &g);
    ~Waiting();
    int next();
    Waiting & update();
    Waiting & draw();

protected:
    void printText(std::string msg);
};

#endif //_WAITING_H

