#include <algorithm>

#include "GLoid.h"
#include "Loading.h"

Loading::Loading(Game & g){
    game = &g;
    type = STEP_LOADING;
}

Loading::~Loading(){
    std::for_each(begin(text),
                  end(text),
                  free);
    text.clear();
    if(textureIds){
        free(textureIds);
        textureIds = nullptr;
    }
}

Loading & Loading::next(){

    return *this;
}

void Loading::printText(std::string msg)
{
    game->info(msg);
    text2d ls(white,black);
    ls.print(msg);
    auto s = game->print2d(ls);
    if(s){
        text.push_back(s);
    }
}

void Loading::loadTextures(){
    printText("Loading textures...");
    int n_bmp = BmpFiles.size();
    textureIds = (GLuint *)malloc(n_bmp*sizeof(GLuint));
    memset(textureIds,0,n_bmp*sizeof(GLuint));
    glGenTextures(n_bmp, textureIds);
    for(int i = 0; i < BmpFiles.size(); i++){
        game->loadBmp(BmpFiles[i], textureIds[i]);
    }
}

void Loading::loadSounds(){
    printText("Loading sounds...");
    //TODO: load stuff asynchronous
    std::for_each(begin(SoundFiles),
                  end(SoundFiles),
                  [this](auto &f){
        game->loadSound(f);
    });
}

Loading & Loading::update(){
    switch (phase) {
    case LOAD_SOUNDS:
        loadSounds();
        break;
    case LOAD_TEXTURES:
        loadTextures();
        break;
    default:
        break;
    }
    phase++;
    return *this;
}

Loading & Loading::draw(){

    for(int i = 0; i < text.size(); i++){
           game->draw2d(text[i], 0, -i*game->getFontSize());
    }
    SDL_GL_SwapBuffers();

    return *this;
}




