#include <algorithm>

#include "GLoid.h"
#include "Intro.h"

Intro::Intro(Game & g){
    game = &g;
    type = STEP_INTRO;
}

Intro::~Intro(){

}

int Intro::next(){

    return STEP_INTRO;
}

Intro & Intro::update(){
    if(phase == INTRO_STARTING){
        //TODO: print letters one by one
        printText("intro...");
        phase++;
        //start playing audio
    }
    return *this;
}

Intro & Intro::draw(){
    if(phase == INTRO_STARTING){
        clearText();
    }
    if(text.size()){
        game->draw2d(text[0], 0, 0);
    }
    SDL_GL_SwapBuffers();

    return *this;
}




