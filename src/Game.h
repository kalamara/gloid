#ifndef _GAME_H
#define _GAME_H

// this is the BASE GAME ENGINE CORE.
// TODO: make it Meyer's Singleton and use composition for mocking
typedef struct mousecntl * mousecntl_t;
typedef struct screen * screen_t;
/*
class Vaus;
class Ball;
class Brick;
class Pill;
*/

//template<class BRIK>

class Game {
public:
    //protected:
    // Global display engine parameters
    Point3f camera;      // Camera coordinates
    float theta = ZERO;
    float phi = ZERO;

    Uint32 tic;           // Game timer
    SDL_Surface *screen;  // Reference to SDL Screen

    //char Version[6]         = "2.2";
    bool mipmap      = false;
    bool vsync       = true;
    bool billboard   = false;
    bool smooth      = true;
    bool multisample = true;
    unsigned int screenW = 1024;
    unsigned int screenH = 768;

    // The default bits per pixel
#ifdef WIN32
    unsigned int bpp = 32;
#else
#ifdef __MACOSX__
    unsigned int bpp = 32;
#else
    unsigned int bpp = 24;
#endif//__MACOSX__
#endif//WIN32
    unsigned int fontSize 	= 48;

    static Game * _instance;
    Game(){
    }
//public:

    screen_t scr;
    mousecntl_t mouse;

    //audio
    //keyboard
    class Vaus *vaus;
    int bonusMode;
    int level;
    static Game * instance(){
        if(!_instance){
            _instance = new Game;
        }
        return _instance;
    }
    bool isHiScoring();
    unsigned int levelType();
    //int getBonusMode();
    void setBonusMode(int type);
    //int getLives();
    void incLives();
    void divideBalls();
   // unsigned int getFontSize();
   // unsigned int getBpp();

    //class Vaus * getVaus();
    //get first active ball, or empty
    class Ball * getActiveBall();
    class Brick * getBrickAt(Point3f& where);

    static void playSound(int sound);
    static int now();
    static void printText(bool option,
                          text2d* text,
                          SDL_Color fg,
                          SDL_Color bg,
                          int x,
                          int y,
                          const char* buf, ...);

};
#endif //_GAME_H
