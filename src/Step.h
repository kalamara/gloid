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

#define NEWLINE -1

//Game step base class
class Step{
protected:
    std::vector<SDL_Surface *> text;
    class Game * game;
    int textAlignment = ALIGN_CENTER;
    int textOffset = 0;
    int textMargin = 50;
    int textSpacingPercent = 150;

    void printText(std::string msg, int line = NEWLINE);
    void clearText();
    void drawText(int line);

public:
    unsigned int type;
    Step(){}
    ~Step();
    virtual int next()=0;
    virtual Step & update()=0;
    virtual Step & draw()=0;
};
#endif //_STEP_H_


