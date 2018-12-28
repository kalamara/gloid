#include <algorithm>

#include "GLoid.h"
#include "Loading.h"
#include "model/Brick.h"

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

int Loading::next(){

   if(phase == N_LOAD){
        return STEP_WAITING;
    }
    return STEP_LOADING;
}

void Loading::printText(std::string msg)
{
    game->info(msg);
    text2d ls(White,Black);
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

void Loading::loadHalloFame(){
    printText("Loading hall of fame...");
    std::ifstream ifs;
    ifs.open(HalloFame, std::ifstream::in);
    int l = 0;
    if(ifs.is_open()){
        std::string line;
        while(getline(ifs,line) && l++ < 10){
            game->hiscore.insert(getScore(line));
        }
        ifs.close();
    }
    while(l++ < 10){
        game->hiscore.insert({50000,"AAA"});
    }
    while(game->hiscore.size() > 10){
        game->hiscore.erase(std::prev(game->hiscore.end()));
    }
}

void Loading::loadLevel(){
    std::stringstream msg;
    msg << "Loading level " << game->level << "...";
    printText(msg.str());
    std::ifstream ifs;
    std::stringstream path;
    path << "levels/level" << game->level << ".ase";
    ifs.open(path.str(),  std::ifstream::in);
    if(ifs.is_open()){
        while(ifs.peek() != EOF){
            auto b = Brick::getBrick(ifs, game);
            if(b){
                game->bricks.push_back(b.value());
            }
        }
        ifs.close();
    }
}

std::pair<int, std::string> Loading::getScore(std::string line){
    std::string initials = "";
    int score = 0;
    try{
        std::stringstream str(line);
        std::string scorestr;
        std::string initstr;
        str >> initstr >> scorestr;

        score = std::stoi(scorestr);
        initials = initstr.substr(0,3);

        std::transform(initials.begin(),
                       initials.end(),
                       initials.begin(),
                       ::toupper);

    }catch(...){
        return {};
    }
    return {score,initials};
}

Loading & Loading::update(){
    switch (phase) {
    case LOAD_SOUNDS:
        loadSounds();
        break;
    case LOAD_TEXTURES:
        loadTextures();
        break;
    case LOAD_HALLOFAME:
        loadHalloFame();
        break;
    case LOAD_LEVEL:
        loadLevel();
        break;
    default:
        return *this;
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




