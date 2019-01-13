#include "GLoid.h"
#include "Step.h"

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
