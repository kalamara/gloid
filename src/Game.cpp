#include "GLoid.h"

#include "game/Loading.h"


Game::Game(){
    //step = nextStep();
}

Game::~Game(){
    if(step){
        delete step;
    }
}

Game* Game::loop(){
    SDL_Event evt;    // SDL event
    if(SDL_PollEvent(&evt)){
        //info("caught an event...");
        return handleEvent(evt);
    }else{
        if(!app.visible){
            SDL_WaitEvent(NULL);
            return this;
        }else{
            step = nextStep()->draw()
                             ->update();
            return this;
       }
    }
}

Step* Game::nextStep(){
    if(step==NULL){
        step = new Loading(this);
    }
    return step;
}
