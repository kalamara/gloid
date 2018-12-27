#ifndef _LOADING_H
#define _LOADING_H

typedef enum{
    LOAD_SOUNDS,
    LOAD_TEXTURES,
    LOAD_HALLOFAME,
    LOAD_LEVEL,
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

const std::vector<std::string> BmpFiles = {
        "arka1_hires",
        "arka2_hires",
        "arka3_hires",
        "arka4_hires",
        "gloid",
        "vaus",
        "target",
        "crosshairs",
        "warp"
    };

const std::string HalloFame = "hallofame.txt";

class Point3i;
class Loading: public Step{
    static constexpr SDL_Color white = {255, 255, 255, 128};
    static constexpr SDL_Color black = {0, 0, 0, 128};
    std::vector<SDL_Surface *> text;
    void loadSounds();
    void loadTextures();
    void loadHalloFame();
    void loadLevel();

public:
    GLuint * textureIds = nullptr;
    int phase = LOAD_SOUNDS;
    Loading(Game &g);
    ~Loading();
    Loading & next();
    Loading & update();
    Loading & draw();

    std::pair<int,std::string> getScore(std::string line);
    std::vector<float> getElement(std::string line, std::string header);
    std::optional<Point3i> getPos(std::string line);
    std::optional<Point3f> getColor(std::string line);
    std::vector<std::string> getTokens(std::string ase,
                                       std::vector<std::string> tokens);

protected:
    void printText(std::string msg);
};

#endif //_LOADING_H

