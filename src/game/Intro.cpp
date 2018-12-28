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

    return *this;
}

Intro & Intro::draw(){

    for(int i = 0; i < text.size(); i++){
           game->draw2d(text[i], 0, -i*2*game->getFontSize());
    }
    SDL_GL_SwapBuffers();

    return *this;
}




