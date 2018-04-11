#ifndef _GAME_H
#define _GAME_H

// this is the BASE GAME ENGINE CORE.
// it's a Meyer's Singleton.

class Game {
private:
    Game(){

    }
public:
    ~Game(){

    }
    static void playSound(int sound);
};
#endif //_GAME_H
