#ifndef _GAME_H
#define _GAME_H

class Game: public Engine<class Game>{
public:
    Game();
    ~Game();
    class Step * step = NULL;
    //Game* handleEvent(SDL_Event & e);

    Game* loop();
    /*Game* start(class Step * at){
        step = at;
    }*///this might merge with nextStep if at == NULL
    Game* nextStep();



};
#endif //_GAME_H
