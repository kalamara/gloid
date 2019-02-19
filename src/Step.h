#ifndef _STEP_H_
#define _STEP_H_

// Game states
typedef enum{
   STEP_LOADING,
   STEP_WAITING,
   STEP_INTRO,
   STEP_GO,
   STEP_PLAY,
   STEP_WIN,
   N_STEPS
}STEP_TYPE;

//Game step base class
class Step{
protected:

class Game * game;
public:
    unsigned int type;
    Step(){}
    ~Step(){}
    virtual int next()=0;
    virtual Step & update()=0;
    virtual Step & draw()=0;
};
#endif //_STEP_H_


