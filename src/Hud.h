#ifndef _HUD_H_
#define _HUD_H_

typedef enum{
    ALIGN_LEFT,
    ALIGN_RIGHT,
    ALIGN_CENTER,
    N_ALIGN
}TEXT_ALIGN;

#define NEWLINE -1

class Hud{

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
    Hud(){}
    ~Hud();
    };
#endif //_STEP_H_


