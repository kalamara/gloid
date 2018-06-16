#ifndef _PILL_H_
#define _PILL_H_
// Bonus pills

typedef enum{
    P,       // Grey:    Player life (kanonaki)
    B,       // Pink:    Boost to next level
    E,       // Blue:    Enlarge
    L,       // Red:     Lazer cannon
    G,       // Green:   Glue
    D,       // Magenta: Divide balls
    S,       // Orange:  Slow
    N_PILLS  // Number of pills
} PILLS;
class Vaus;
// Bonus pills
class Pill: public WhatUC<Pill>{
    static constexpr float speedz = -10.0f;
    static constexpr float rotspeed = 2*HALF_CIRCLE;

    // Length and radius of pill
    static constexpr float rad = ONE;
    static constexpr float len = 3.5f;

    class Game * game;
    class Play * gameplay;

public:
    // Pill color palette
    SDL_Color Palette[N_RGB_COLORS] =
    {{198, 198, 198, 128},  // Grey
     {255, 128, 255, 128},  // Pink
     {255,   0,   0, 128},  // Blue
     {  0,   0, 255, 128},  // Red
     {  0, 255,   0, 128},  // Green
     {255, 255,   0, 128},  // Magenta
     {  0, 224, 255, 128},  // Orange
     {  0, 255, 255, 128},  // Yellow
     {255, 255, 255, 128},  // White
     {  0,   0,   0, 128}}; // Black

    const char* PillLabels = "PBELGDS";
    float rotx = ZERO;
    int type;  // Enumerated pill type
    int score = 0;
    Point3f col;     // Pill color
    char label;
    SDL_Surface* surf;
    SDL_Surface* textSurf;
    class text2d * text;

    void display();
    Pill& animate(double secPerFrame);

    Pill(const Point3f& where, class Game *g, class Play *p);
    ~Pill();
private:
    GLUquadricObj * base;
    int roulette(bool hiScore);
    bool collides(float left,
                  float right,
                  float up,
                  float down);
};

#endif //_PILL_H_
