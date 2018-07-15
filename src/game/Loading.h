#ifndef _LOADING_H
#define _LOADING_H

typedef enum{
    LOAD_SOUNDS,
    LOAD_TEXTURES,
    LOAD_LEVEL,
    LOAD_HALLOFAME,
    N_LOAD,
}LOAD_PHASE;

const std::vector<std::string> SoundFiles = {
    "alien",
    "bounce0",
    "bounce1",
    "enlarge",
    "go",
    "intro",
    "kanonaki",
    "launch",
    "lose",
    "shot",
    "warp"
};

class Loading: public Step{
    static constexpr SDL_Color white = {255, 255, 255, 128};
    static constexpr SDL_Color black = {0, 0, 0, 128};

    int phase = LOAD_SOUNDS;
    std::vector<SDL_Surface *> text;
    void loadSounds();
public:
    Loading(class Game  * g);
    ~Loading();
    Loading * next();
    Loading * update();
    Loading * draw();
};

#endif //_LOADING_H

