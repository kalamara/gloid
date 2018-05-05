#ifndef WORLD_H
#define WORLD_H

class Vaus;
class Point3f;
class World{
    Vaus * vaus;
public:
    class Ball * getActiveBall();
    class Brick * getBrickAt(Point3f& where);

    Vaus * getVaus() const{
         return vaus;
    }
    void setVaus(Vaus *value){
        vaus = value;
    }
};
#endif // WORLD_H
