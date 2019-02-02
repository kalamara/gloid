#ifndef _LOADING_H
#define _LOADING_H

typedef enum{
    LOAD_SOUNDS,
    LOAD_TEXTURES,
    LOAD_HALLOFAME,
    LOAD_LEVEL,
    LOAD_DONE,
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

#define HISCORE 50000 //initial high score
#define HOF_LEN 10 //length of hall of fame
#define INITIALS "AAA" //default initials

const std::string HalloFame = "hallofame.txt";

class Point3i;
class Loading: public Step{

    void loadSounds();
    void loadTextures();
    void loadHalloFame();
    void loadLevel();

public:
    GLuint * textureIds = nullptr;
    int phase = LOAD_SOUNDS;
    Loading(Game &g);
    ~Loading();
    int next();
    Loading & update();
    Loading & draw();

    std::pair<int,std::string> getScore(std::string line);
};

#endif //_LOADING_H

