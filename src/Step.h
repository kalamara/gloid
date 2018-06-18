#ifndef _STEP_H_
#define _STEP_H_

// Game states
enum{
   STEP_LOADING,
   STEP_WAITING,
   STEP_INTRO,
   STEP_GO,
   STEP_PLAY,
   STEP_WIN,
   N_STEPS
};

//Game step base class
class Step{
public:
    unsigned int type;
    Step(){}
    ~Step(){}
    virtual Step* next()=0;
    virtual Step* update()=0;
    virtual Step* draw()=0;
};
#endif //_STEP_H_


