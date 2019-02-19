#ifndef _TEXT_H_
#define _TEXT_H_

#include <SDL/SDL_video.h>
#include <SDL/SDL_ttf.h>

#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>
#include <memory>

typedef enum{
    ALIGN_LEFT,
    ALIGN_RIGHT,
    ALIGN_CENTER,
    N_ALIGN
}TEXT_ALIGN;

static constexpr SDL_Color White = {255, 255, 255, 128};
static constexpr SDL_Color Black = {0, 0, 0, 128};

class text2d{
    std::stringstream str;

public:
    bool blended = true;


    SDL_Color foreground = White;
    SDL_Color background = Black;

    std::string msg(){

        return str.str();
    }

    std::string trim(){
        std::string str = msg();
        str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
        return str;
    }

    text2d * clear(){
        str.str( std::string() );
        str.clear();

        return this;
    }

    text2d(){
    }

    text2d(const SDL_Color& fg,
           const SDL_Color& bg,
           bool blend = false){

        blended = blend;
        foreground = fg;
        background = bg;
    }
    ~text2d(){}

    template<typename T,typename... Args> text2d * print(T value, Args... args){
        log(&str, value, args...);

        return this;
    }

    template <typename T> static void log(std::ostream * to, T s){
        if (to) {
            *to << s << std::endl;
#ifdef DEBUG
            std::cout << s << std::endl;
#endif
        }
    }

    template<typename T, typename... Args> static void log(std::ostream * to,
                                                           T value,
                                                           Args... args){
        if (to) {
#ifdef DEBUG
            std::cout << value;		// use first non-format argument
#endif
            *to << value;

            return log(to, args ...); 	// ``peel off'' first argument
         }
         *to << value << std::endl;
    }
};

class TextBody{
public:
    unsigned int timestamp = 0;//If timestamp is zero, the message is always on.
    unsigned int lifetime = 0; //If (ticks - timestamp) > lifetime, the popup dies.
    int alignment = ALIGN_CENTER;
    int offset = 0;
    int margin = 0;
    int spacingPercent = 150;

    std::vector<SDL_Surface *> body;
    bool splash = false;
    TextBody(int life = 0,
             int toc = 0){
        lifetime = (unsigned int)life;
        timestamp = life >= 0 ? toc : 0;
    }
    ~TextBody(){

        body.clear();
    }
};

#endif //_TEXT_H_
