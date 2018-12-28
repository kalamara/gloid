#include "GLoid.h"

#include "game/Loading.h"
#include "game/Waiting.h"
#include "model/WhatUC.h"
#include "model/Particle.h"
#include "model/Ball.h"
#include "model/Vaus.h"
#include "model/Brick.h"
#include "model/Pill.h"
#include "model/Alien.h"
#include "model/Shot.h"

Game::Game(){
    steps.insert({STEP_LOADING, std::make_unique<Loading>(*this)});
    steps.insert({STEP_WAITING, std::make_unique<Waiting>(*this)});
    step = steps[STEP_LOADING].get();
}

Game::~Game(){
}

Game& Game::loop(){
    SDL_Event evt;    // SDL event
    memset(&evt,0,sizeof(SDL_Event));
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
    return *steps[step->next()];
}

void Game::divideBalls(){}
void Game::killVaus(){}
class Ball * Game::getActiveBall(){return nullptr;}
class Brick * Game::getBrickAt(Point3f& where){return nullptr;}
