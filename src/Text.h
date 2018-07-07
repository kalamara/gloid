#ifndef _TEXT_H_
#define _TEXT_H_
#include <sstream>
#include <string>
class text2d{
    std::stringstream str;
    unsigned int timestamp = 0;//If timestamp is zero, the message is always on.
    unsigned int lifetime = 0; //If (ticks - timestamp) > lifetime, the popup dies.

public:
    bool blended = true;
    SDL_Color foreground = {0};
    SDL_Color background = {0};

    std::string msg(){

        return str.str();
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
           int life = 0,
           int toc = 0,
           bool blend = true){
        lifetime = (unsigned int)life;
        timestamp = life >= 0 ? toc : 0;
        blended = blend;
        foreground = fg;
        background = bg;
    }
    template<typename T,typename... Args> text2d * print(T value, Args... args){
        log(&str, value, args...);

        return this;
    }
    template <typename T> static void log(std::ostream * to, T s){
        if (to) {
#ifdef DEBUG
            *to << s << std::endl;
#endif
            std::cout << s << std::endl;
        }
    }
    template<typename T, typename... Args> static void log(std::ostream * to,
                                                           T value,
                                                           Args... args){
        if (to) {
            std::cout << value;		// use first non-format argument
#ifdef DEBUG
            *to << value;
#endif
            return log(to, args ...); 	// ``peel off'' first argument
         }
         *to << value << std::endl;
    }
};
#endif //_TEXT_H_
