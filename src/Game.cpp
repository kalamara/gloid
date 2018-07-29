#include "GLoid.h"

#include "game/Loading.h"

Game::Game(){
}

Game::~Game(){
}

Game& Game::loop(){
    SDL_Event evt;    // SDL event
    if(SDL_PollEvent(&evt)){
        //info("caught an event...");
        return handleEvent(evt);
    }else{
        if(!app.visible){
            SDL_WaitEvent(nullptr);
            return *this;
        }else{
            *step = nextStep().draw()
                              .update();
            return *this;
       }
    }
}

Step & Game::nextStep(){
    if(step == nullptr){
        step = std::make_unique<Loading>(*this);
    }
    return *step;
}
