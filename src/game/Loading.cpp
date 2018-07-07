#include <algorithm>

#include "GLoid.h"
#include "Loading.h"

Loading::Loading(class Game * g){
    game = g;
    type = STEP_LOADING;
}
Loading::~Loading(){
    std::for_each(text.begin(),
                  text.end(),
                  free);
    text.clear();
}

Loading * Loading::next(){

    return this;
}

void Loading::loadSounds(){

    std::string msg = "Loading sounds...";
    game->info(msg);
    text2d ls(white,black);
    ls.print(msg);
    SDL_Surface * s = game->print2d(ls);
    if(s){
        text.push_back(s);
    }

    std::for_each(SoundFiles.begin(),
                  SoundFiles.end(),
                  [this](const std::string &f){
        game = game->loadSound(f);
    });
}

Loading * Loading::update(){
    switch (phase) {
    case LOAD_SOUNDS:
        loadSounds();
        break;
    default:
        break;
    }
    phase++;
    return this;
}

Loading * Loading::draw(){

    return this;
}




