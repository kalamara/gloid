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
    if(game->keyPressed(SDLK_SPACE)
    || game->getMouse()->leftclick){
        return STEP_INTRO;
    }
    return STEP_WAITING;
}

int Waiting::flip(unsigned int millis){
    if((millis % (2 * FLIP_PHASE)) >= FLIP_PHASE){
        return WAIT_HOF;
    }
    return WAIT_RDY;
}

Waiting & Waiting::update(){
    const std::string rdy = "Press fire to play...";
    int newphase = flip(game->toc() - game->tic);
    if(newphase != phase){
        phase = newphase;
        clearText();
    }
    if(text.empty()){
        if(phase == WAIT_RDY){
            printText(rdy);
        }else{
            std::for_each(begin(game->hiscore),
                          end(game->hiscore),
                          [this](auto line){
                std::stringstream str;
                str << line.first << "    " << line.second;
                printText(str.str());
            });
        }
    }
    return *this;
}

Waiting & Waiting::draw(){
    drawText();

    return *this;
}




