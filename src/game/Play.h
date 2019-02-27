#ifndef _PLAY_H
#define _PLAY_H

typedef enum{
    PLAY_GO,
    PLAY_PLAYING,
    PLAY_DONE,
}PLAY_PHASE;

// Messages
typedef enum
{
   HUD_GO,
   HUD_1UP,
   HUD_SCORE,
   HUD_HILABEL,
   HUD_HISCORE,
   HUD_LEVEL,
   HUD_PAWS,
   HUD_FPS,
   N_HUD
}PLAY_HUD;

#define BGD_SIZE 4.0f
#define WARP_SIZE 5.0f

class Play: public Step,  public Hud{
    class TextBody go;
    class TextBody up1;
    class TextBody score;
    class TextBody debug;
    float lightPos[3] = {-20.0, 20.0, -10.0};
    Point3f cameraPos = {-20.0, 20.0, -10.0};
    std::pair<GLuint, SDL_Surface> background;
    std::pair<GLuint, SDL_Surface> warp;
    bool flipWarp = false;
    // Minimum number of milliseconds per frame
    double minmspf = 40.0f;
    //average number of milliseconds per frame over last ten frames
    double ms10 =  50.0f;
public:
    int phase = PLAY_GO;
    //int old_phase = PLAY_GO;
    Play(Game & g);
    ~Play();
    Play & draw();
    Play & update();
    int next();
    void displayBackground();
    void displayWarp();
};
#endif //_GAME_H
