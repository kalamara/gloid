#include "GLoid.h"
#include "Step.h"

Step::~Step(){
    std::for_each(begin(text),
                  end(text),
                  free);
    text.clear();
}

void Step::printText(std::string msg)
{
    game->info(msg);
    text2d ls(White,Black);
    ls.print(msg);
    auto s = game->print2d(ls);
    if(s){
        text.push_back(s);
    }
}

void Step::clearText(){
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    SDL_GL_SwapBuffers();
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    text.clear();
}

void Step::drawText(){
    for(int i = 0; i < text.size(); i++){
           game->draw2d(text[i], 0, -i*2*game->getFontSize());
    }
    SDL_GL_SwapBuffers();
}
