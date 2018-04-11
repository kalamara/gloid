#ifndef _GAME_H
#define _GAME_H

// this is the BASE GAME ENGINE CORE.
// it's a Meyer's Singleton.

class Game {

    static Game * _instance;
    Game(){

    }
public:

    static Game * instance(){
        if(!_instance){
            _instance = new Game;
        }
        return _instance;
    }

    static void playSound(int sound);
};
#endif //_GAME_H
