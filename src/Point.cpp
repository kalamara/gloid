#include "GLoid.h"
#include "Point.h"

Point3f::Point3f(const class Point3i& other){
    x = 5.0f*other.X + SCENE_MIN + 2.5f;
    y = 5.0f*other.Y + SCENE_MIN + 2.5f;
    z = -2.5f*other.Z - 1.25;
}

Point3i::Point3i(const Point3f& other){
    X = (int)round(0.2*other.x) + 3;
    Y = (int)round(0.2*other.y) + 3;
    Z = (int)round(0.4*other.z + 0.5);
}

/**<3<3<3<3<3<3<3******<3<3<3<3<3<3<3<*/

// Calculate the speed components of object A chasing object B at speed U
Point3f Point3f::chase(const Point3f& other, float U){
    Point3f finalspeed = Point3f(ZERO, ZERO, ZERO);

    float M = Point3f(x - other.x,
                y - other.y,
                z - other.z).res3f();

    if(M > 0){
        finalspeed.x = -(U / M) * (x - other.x);
        finalspeed.y = -(U / M) * (y - other.y);
        finalspeed.z = -(U / M) * (z - other.z); //should this be inverted?
    }
    return finalspeed;
}

Point3f Point3f::raycast(const Point3f &speed, int axis, float dist) const{
    Point3f collision(
                (speed.x > ZERO) ? x + dist : x - dist,
                (speed.y > ZERO) ? y + dist : y - dist,
                (speed.z > ZERO) ? z + dist : z - dist
                );

    switch(axis){
        case AXIS_X:
        if(abs(speed.x)>FLOAT_PRECISION){
            collision.y = y + (speed.y * (collision.x - x))/speed.x;
            collision.z = z - (speed.z * (collision.x - x))/speed.x;
        }
        break;

        case AXIS_Y:
        if(abs(speed.y)>FLOAT_PRECISION){
            collision.x = x + (speed.x * (collision.y - y))/speed.y;
            collision.z = z - (speed.z * (collision.y - y))/speed.y;
        }
        break;

        case AXIS_Z:
        if(abs(speed.z)>FLOAT_PRECISION){
            collision.x = x - (speed.x * (collision.z - z))/speed.z;
            collision.y = y - (speed.y * (collision.z - z))/speed.z;
        }
        break;

        default:
           collision.deepcopy(*this);
    }
    return collision;
}

float Point3f::dist(const Point3f other) const {

    return Point3f(abs(x - other.x),
                   abs(y - other.y),
                   abs(z - other.z)).res3f();
}

Point3i Point3f::signs() const {
    int px =  (x < 0) ? -1 : 1;
    int py =  (y < 0) ? -1 : 1;
    int pz =  (z < 0) ? -1 : 1;

    return Point3i(px, py, pz);
}


