#ifndef _GAME_H
#define _GAME_H

// this is the BASE GAME ENGINE CORE.
// it's a Meyer's Singleton.
typedef struct mousecntl * mousecntl_t;
typedef struct screen * screen_t;

class Game {
public:
    //protected:

    static Game * _instance;
    Game(){
    }
//public:
    int bonusMode;
    screen_t scr;
    mousecntl_t mouse;
    //audio
    //keyboard
    static Game * instance(){
        if(!_instance){
            _instance = new Game;
        }
        return _instance;
    }
/*    virtual mousecntl_t getMouse(){

        return mouse;
    }
    virtual screen_t getScreen(){

        return scr;
    }
    virtual int getBonus(){
        return bonusMode;
    }
    virtual Game * setMouse(mousecntl_t m){
        mouse = m;
        return *this;
    }
    virtual Game * setScreen(screen_t s){
        scr = s;
        return *this;
    }
    virtual Game * setBonus(screen_t s){
        scr = s;
        return *this;
    }

  */
    static void playSound(int sound);
};
#endif //_GAME_H
