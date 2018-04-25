#ifndef _GAME_H
#define _GAME_H

// this is the BASE GAME ENGINE CORE.
// TODO: make it Meyer's Singleton and use composition for mocking
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
    static void playSound(int sound);
    static int now();
};
#endif //_GAME_H
