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
    printText("NEXT bitches");
    return STEP_WAITING;
}

void Waiting::printText(std::string msg)
{
    game->info(msg);
    text2d ls(White,Black);
    ls.print(msg);
    auto s = game->print2d(ls);
    if(s){
        text.push_back(s);
    }
}

Waiting & Waiting::update(){

    return *this;
}

Waiting & Waiting::draw(){

//    for(int i = 0; i < text.size(); i++){
//           game->draw2d(text[i], 0, -i*2*game->getFontSize());
//    }
    SDL_GL_SwapBuffers();

    return *this;
}




