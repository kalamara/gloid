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

class Play:
        public Step,  public Hud{
    class TextBody go;
    class TextBody up1;
    class TextBody score;
public:
    int phase = PLAY_GO;
    Play(Game & g);
    ~Play();
    Play & draw();
    Play & update();
    int next();

};
#endif //_GAME_H
