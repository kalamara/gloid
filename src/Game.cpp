#include "GLoid.h"
#include "Hud.h"

#include "model/WhatUC.h"
#include "model/Particle.h"
#include "model/Crosshair.h"
#include "model/Ball.h"
#include "model/Vaus.h"
#include "model/Brick.h"
#include "model/Pill.h"
#include "model/Alien.h"
#include "model/Shot.h"
#include "game/Loading.h"
#include "game/Waiting.h"
#include "game/Intro.h"
#include "game/Play.h"

Game::Game(){
    vaus = std::make_unique<Vaus>(*this);
    steps.insert({STEP_LOADING, std::make_unique<Loading>(*this)});
    steps.insert({STEP_WAITING, std::make_unique<Waiting>(*this)});
    steps.insert({STEP_INTRO, std::make_unique<Intro>(*this)});
    steps.insert({STEP_PLAY, std::make_unique<Play>(*this)});
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
            if(keyPressed(SDLK_ESCAPE)){
                terminate();
            }
            step->draw().update();
            step = nextStep();
            return *this;
       }
    }
}

Step *Game::nextStep(){
    return steps[step->next()].get();
}

void Game::handleKeys(){
    // Arrow keys change the camera angle
    if(keys[SDLK_LEFT]){
        phi += CAMERA_ROTATION;
        cameraPos.x  = sin(phi) * (SCENE_AIR + 2 * SCENE_MAX);
        cameraPos.z = cos(phi) * (SCENE_AIR + 2 * SCENE_MAX) - 2 * SCENE_MAX;
    }
    if(keys[SDLK_RIGHT]){
        phi -= CAMERA_ROTATION;
        cameraPos.x  = sin(phi) * (SCENE_AIR + 2 * SCENE_MAX);
        cameraPos.z = cos(phi) * (SCENE_AIR + 2 * SCENE_MAX) - 2 * SCENE_MAX;
    }
    if(keys[SDLK_UP]){
        theta += CAMERA_ROTATION;
        cameraPos.y  = sin(theta) * (SCENE_AIR + 2 * SCENE_MAX);
        cameraPos.z = cos(theta) * (SCENE_AIR + 2 * SCENE_MAX) - 2 * SCENE_MAX;
    }
    if(keys[SDLK_DOWN]){
        theta -= CAMERA_ROTATION;
        cameraPos.y  = sin(theta) * (SCENE_AIR + 2 * SCENE_MAX);
        cameraPos.z = cos(theta) * (SCENE_AIR + 2 * SCENE_MAX) - 2 * SCENE_MAX;
    }
    // Force a reshape
    if(keys[SDLK_r]){
        keys[SDLK_r] = false;
        reshape(sdlScreen->W, sdlScreen->H);
    }
    // Toggle between full screen and windowed display modes
    if(keys[SDLK_f]){
        keys[SDLK_f] = false;
        //TODO ToggleFullscreen();
    }
    // Pause the game
    if(keys[SDLK_p]){
        keys[SDLK_p] = false;
        //TODO paused = TogglePause(paused);
    }
    // Decrease the frame rate
    if(keys[SDLK_KP_PLUS]){
        keys[SDLK_KP_PLUS] = false;
        if(minmsPerFrame>10)
            minmsPerFrame--;
    }
    // Increase the frame rate
    if(keys[SDLK_KP_MINUS]){
        keys[SDLK_KP_MINUS] = false;
        if(minmsPerFrame<100)
            minmsPerFrame++;
    }
}


