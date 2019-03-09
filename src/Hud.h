#ifndef _HUD_H_
#define _HUD_H_

#define NEWLINE -1

class TextBody{
public:
    unsigned int timestamp = 0;//If timestamp is zero, the message is always on.
    unsigned int lifetime = 0; //If (ticks - timestamp) > lifetime, the popup dies.
    int alignment = ALIGN_CENTER;
    int offset = 0;
    int margin = 0;
    int spacingPercent = 150;
    std::vector<SDL_Surface*> body;
    bool splash = false;
    TextBody(){

        //lifetime = life;
        //timestamp = life >= 0 ? toc : 0;
    }
    ~TextBody(){
        body.clear();
    }
};

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
                   const SDL_Color &fg = White,
                   int line = NEWLINE);
    void clearText();
    void drawText(TextBody &text);
};
#endif //_STEP_H_


