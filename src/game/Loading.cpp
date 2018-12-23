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

//TODO: load stuff asynchronous

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
    std::for_each(begin(SoundFiles),
                  end(SoundFiles),
                  [this](auto &f){
        game->loadSound(f);
    });
}

//load level
//open file
//tokenize by GEOMOBJECT -> list of strings
//find first occurence of TM_POS
//read 3 floats
//find first occurence of WIREFRAME_COLOR
//read 3 floats
//construct bricks

std::vector<float> Loading::getElement(std::string line, std::string header){
    if(line.compare(0, header.length(), header)){
        return {};
    }
    float x = ZERO;
    float y = ZERO;
    float z = ZERO;
    try{
        std::stringstream str(line.substr(header.length()));
        std::string xstr;
        std::string ystr;
        std::string zstr;
        str >> xstr >> ystr >> zstr;
        x = std::stof(xstr);
        y = std::stof(ystr);
        z = std::stof(zstr);
    }catch(...){
        return {};
    }
    return {x,y,z};
}

std::optional<Point3i> Loading::getPos(std::string line){
    std::vector<float> pos = getElement(line, "TM_POS");
    if(pos.size()!=3){
        return {};
    }
    return Point3i(pos[0],pos[1],pos[2]);
}

std::optional<Point3f> Loading::getColor(std::string line){
    std::vector<float> col = getElement(line, "WIREFRAME_COLOR");
    if(col.size()!=3){
        return {};
    }
    return Point3f(col[0],col[1],col[2]);
}

std::vector<std::string> Loading::getTokens(std::string ase,
                                            std::vector<std::string> tokens){
    std::stringstream check(ase);
    std::string temp;
    while(std::getline(check, temp, '*')){
        if(!temp.empty()){
            tokens.push_back(temp);
        }
    }
    return tokens;
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
           game->draw2d(text[i], 0, -i*2*game->getFontSize());
    }
    SDL_GL_SwapBuffers();

    return *this;
}




