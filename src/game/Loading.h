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
class Point3i;
class Loading: public Step{
    static constexpr SDL_Color white = {255, 255, 255, 128};
    static constexpr SDL_Color black = {0, 0, 0, 128};
    std::vector<SDL_Surface *> text;
    void loadSounds();
    void loadTextures();
    std::vector<float> getElement(std::string line, std::string header);
public:
    GLuint * textureIds = nullptr;
    int phase = LOAD_SOUNDS;
    Loading(Game &g);
    ~Loading();
    Loading & next();
    Loading & update();
    Loading & draw();
    std::optional<Point3i> getPos(std::string line);
    std::optional<Point3f> getColor(std::string line);
    std::vector<std::string> getTokens(std::string ase,
                                       std::vector<std::string> tokens);

protected:
    void printText(std::string msg);
};

#endif //_LOADING_H

