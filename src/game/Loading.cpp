#include <algorithm>

#include "GLoid.h"
#include "Loading.h"

Loading::Loading(class Game * g){
    game = g;
    type = STEP_LOADING;
}
Loading::~Loading(){
    std::for_each(begin(text),
                  end(text),
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
    auto s = game->print2d(ls);
    if(s){
        text.push_back(s);
    }
    std::for_each(begin(SoundFiles),
                  end(SoundFiles),
                  [this](auto &f){
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

    for(int i = 0; i < text.size(); i++){
           game->draw2d(text[i], 0, -i*game->getFontSize());
    }
    SDL_GL_SwapBuffers();
    return this;
}




