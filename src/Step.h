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

typedef enum{
    ALIGN_LEFT,
    ALIGN_RIGHT,
    ALIGN_CENTER,
    N_ALIGN
}TEXT_ALIGN;

//Game step base class
class Step{
protected:
    std::vector<SDL_Surface *> text;
    class Game * game;
    int align = ALIGN_CENTER;
    int textOffset = 0;
    int textMargin = 0;
    int textSpacingPercent = 200;
    void printText(std::string msg);
    void clearText();
    void drawText();

public:
    unsigned int type;
    Step(){}
    ~Step();
    virtual int next()=0;
    virtual Step & update()=0;
    virtual Step & draw()=0;
};
#endif //_STEP_H_


