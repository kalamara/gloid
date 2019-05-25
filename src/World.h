#ifndef WORLD_H
#define WORLD_H


#define VAUS_PARTICLES  128  //asplozans!
#define ALIEN_PARTICLES 1024

#define VAUS_COLORS  4

class Vaus;
class Brick;
class Ball;
class Point3f;
class Point3i;
class Alien;
class Particle;
class Pill;
class Shot;

class World{

public:
    std::unique_ptr<Vaus> vaus;
    std::map<Point3i, Brick> bricks;
    std::list<Ball> balls;
    std::list<Alien> aliens;

    Vaus & getVaus() const{
         return *vaus;
    }
//    void setVaus( value){
//        vaus = value;
//    }

};
#endif // WORLD_H
