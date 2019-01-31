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

//calculate margin = screen center - text length /2

void Step::drawText(){
    int pixels = (game->getFontSize() * textSpacingPercent) / 100 ;
    int margin = textMargin;

    for(int i = 0; i < text.size(); i++){
        switch(align){
            case ALIGN_CENTER:
                margin = ((game->getScreen()->W) - (text[i]->w))/2;
            break;
            case ALIGN_RIGHT:
                margin = (game->getScreen()->W) - (text[i]->w);
            break;
            default:case ALIGN_LEFT:
            break;
        }
        game->draw2d(text[i],
                     margin,
                     - (textOffset + i) * pixels);
    }
    SDL_GL_SwapBuffers();
}
