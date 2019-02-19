#ifndef _HUD_H_
#define _HUD_H_

#define NEWLINE -1

class Hud{
private:
    class Game * _game = nullptr;
public:

    Hud(){}
    ~Hud();
    void setGame(Game &g){
        _game = &g;
    }
    void printText(std::string msg,
                   class TextBody &text,
                   int line = NEWLINE);
    void clearText();
    void drawText(TextBody &text);
};
#endif //_STEP_H_


