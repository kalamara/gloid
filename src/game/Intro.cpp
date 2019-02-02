#include <algorithm>

#include "GLoid.h"
#include "Intro.h"
#define INTRO_DURATION 7000 //ms

static const std::string introStr ="\
THE ERA AND TIME OF \n\
THIS STORY IS UNKNOWN.\n\
AFTER THE MOTHERSHIP \n\
\"GLOID\" WAS DESTROYED,\n\
A SPACECRAFT \"VAUSDL\" \n\
SCRAMBLED AWAY FROM IT.\n\
BUT ONLY TO BE \n\
TRAPPED IN SPACE WARPED \n\
BY SOMEONE...";

Intro::Intro(Game & g){
    game = &g;
    type = STEP_INTRO;
}

Intro::~Intro(){

}

int Intro::next(){
    if(game->keyPressed(SDLK_SPACE)
    || game->getMouse()->leftclick){
        if (phase == INTRO_DONE){
            return STEP_GO;
        }
    }
    return STEP_INTRO;
}

Intro & Intro::update(){
    int char_life = INTRO_DURATION / introStr.size();
    if(phase == INTRO_STARTING){
        tic = game->toc();
        game->playSound("intro");
        //play audio
        phase++;
        //start playing audio
    }else if (phase == INTRO_STARTED){
        unsigned int toc = game->toc() - tic;
        auto read = readLine(toc/char_life);
        printText(read.second, read.first);
        if(toc > INTRO_DURATION){
            phase++;
        }
    }
    return *this;
}

Intro & Intro::draw(){
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int lines = text.size();
    for(int i  = 0; i < lines; i++){
        drawText(i);
    }
    if(phase == INTRO_DONE
    || phase == INTRO_STARTING){
        clearText();
    }
    SDL_GL_SwapBuffers();
    return *this;
}

std::pair<int, std::string> Intro::readLine(unsigned int cursor){
    std::stringstream istr(introStr);
    unsigned int lineno = 0;
    int remains = cursor;
    std::string line = "";
    std::string subs = "";
    unsigned int count = 0;
    while(remains > 0 && std::getline(istr, line)){
        lineno = count++;
        subs = line.substr(0,remains);
        remains -= line.size();
    }
    return {lineno, subs};
}




