#ifndef WORLD_H
#define WORLD_H


#define VAUS_PARTICLES  128  //asplozans!
#define ALIEN_PARTICLES 1024

#define VAUS_COLORS  4

class Vaus;
class Brick;
class Ball;
class Point3f;
class Alien;
class Particle;
class Pill;

class World{
    Vaus * vaus = nullptr;
    std::vector<Brick> bricks;
    std::list<Ball> balls;
    std::list<Alien> aliens;
    std::vector<Particle> particles;
    std::list<Pill> pills;
public:
    Vaus * getVaus() const{
         return vaus;
    }
    void setVaus(Vaus *value){
        vaus = value;
    }
};
#endif // WORLD_H
