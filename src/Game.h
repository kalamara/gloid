#ifndef _GAME_H
#define _GAME_H

class Game: public Engine<class Game>{
public:
    Game();
    ~Game();
    class Step * step = NULL;
    int queryStep(){

            return step?(step->type):-1;
    }
    Game * loop();
    /*Game* start(class Step * at){
        step = at;
    }*///this might merge with nextStep if at == NULL
    class Step * nextStep();



};
#endif //_GAME_H
