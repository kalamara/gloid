#include "GLoid.h"
#include "Hud.h"

Hud::~Hud(){}

void Hud::printText(std::string msg, class TextBody & text, int line)
{
    if(!_game){
        return;
    }
    _game->debug(msg);
    auto ls = text2d(White,Black);
    ls.print(msg);
    auto s = _game->print2d(ls);
    if(line == NEWLINE || line >= text.body.size()){
        text.body.push_back(s);
    }else{
        auto begin = text.body.begin();
        text.body.at(line) = s;
    }
}

void Hud::clearText(){
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    SDL_GL_SwapBuffers();
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Hud::drawText(class TextBody & text){
    int line = 0;
    for(auto surf: text.body){
        if(!surf){
            return;
        }
        int pixels = (_game->getFontSize() * text.spacingPercent) / 100 ;
        int margin = text.margin;

        switch(text.alignment){
        case ALIGN_CENTER://screen center - text length /2
            margin = ((_game->getScreen()->W) - (surf->w))/2;
            break;
        case ALIGN_RIGHT://screen edge - text length - margin
            margin = (_game->getScreen()->W) - (surf->w) - text.margin;
            break;
        default:case ALIGN_LEFT:
            break;
        }
        if(!text.splash){
            _game->draw2d(surf,margin, - (text.offset + line) * pixels);
        }else{
            _game->draw2d(surf,margin, - _game->getScreen()->H/2);
        }
        line++;
    }
}

