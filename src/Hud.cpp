#include "GLoid.h"
#include "Hud.h"

Hud::~Hud(){}

void Hud::printText(std::string msg,
                    class TextBody & text,
                    const SDL_Color &fg,
                    int line)
{
    if(!_game){
        return;
    }
    _game->debug(msg);

    auto ls = text2d(fg ,Black);
    ls.print(msg);

    auto surf = _game->print2d(ls);
    if(surf!=nullptr){
        if(line == NEWLINE || line >= text.body.size()){
            text.body.push_back(surf);
        } else {
            SDL_FreeSurface(text.body[line]);
            text.body.at(line) = surf;
        }
    }
    text.timestamp = (text.lifetime > 0) ? _game->toc() : 0;
}

void Hud::clearText(){
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    SDL_GL_SwapBuffers();
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Hud::drawText(class TextBody & text){
    int line = 0;
    if(text.timestamp != 0 && _game->toc() - text.timestamp > text.lifetime){
        for(auto s : text.body){
            SDL_FreeSurface(s);
        }
        text.body.clear();
        text.timestamp = 0;
    }
    for(auto surf : text.body){

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

