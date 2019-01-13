#include <algorithm>

#include "GLoid.h"
#include "Waiting.h"

Waiting::Waiting(Game & g){
    game = &g;
    type = STEP_WAITING;
}

Waiting::~Waiting(){

}

int Waiting::next(){
    if(game->keyPressed(ANY_KEY_PRESSED)){
        return STEP_INTRO;
    }
    return STEP_WAITING;
}

Waiting & Waiting::update(){
    if(!waiting){
        printText("Press any key...");
    }
    waiting = true;
    return *this;
}

Waiting & Waiting::draw(){
    if(!waiting){
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        SDL_GL_SwapBuffers();
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    if(text.size()){
        game->draw2d(text[0], 0, 0);
    }

    SDL_GL_SwapBuffers();

    return *this;
}




