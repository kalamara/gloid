#ifndef _WAITING_H
#define _WAITING_H

typedef enum{

}WAIT_PHASE;

class Waiting: public Step{
    bool waiting = false;
public:
    Waiting(Game &g);
    ~Waiting();
    int next();
    Waiting & update();
    Waiting & draw();
};

#endif //_WAITING_H

