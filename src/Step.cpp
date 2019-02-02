#include "GLoid.h"
#include "Step.h"

Step::~Step(){
    std::for_each(begin(text),
                  end(text),
                  free);
    text.clear();
}

void Step::printText(std::string msg, int line)
{
    game->info(msg);
    text2d ls(White,Black);
    ls.print(msg);
    auto s = game->print2d(ls);
    if(s){
        if(line == NEWLINE || line >= text.size()){
            text.push_back(s);
        }else{
            text[line] = s;
        }
    }
}

void Step::clearText(){
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    SDL_GL_SwapBuffers();
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    text.clear();
}

void Step::drawText(int line){
    int pixels = (game->getFontSize() * textSpacingPercent) / 100 ;
    int margin = textMargin;

    switch(textAlignment){
    case ALIGN_CENTER://screen center - text length /2
        margin = ((game->getScreen()->W) - (text[line]->w))/2;
        break;
    case ALIGN_RIGHT://screen edge - text length - margin
        margin = (game->getScreen()->W) - (text[line]->w) - textMargin;
        break;
    default:case ALIGN_LEFT:
        break;
    }
    game->draw2d(text[line],
                 margin,
                 - (textOffset + line) * pixels);

}

