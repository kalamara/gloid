#include "GLoid.h"
#include "Point.h"

Point3f::Point3f(const class Point3i& other){
    x = 5.0f*other.X + SCENE_MIN + 2.5f;
    y = 5.0f*other.Y + SCENE_MIN + 2.5f;
    z = -2.5f*other.Z - 1.25;
}

/**<3<3<3<3<3<3<3******<3<3<3<3<3<3<3<*/

// Calculate the speed components of object A chasing object B at speed U
Point3f Point3f::chase(const Point3f& other, float U){
    float M;
    Point3f finalspeed = Point3f(ZERO, ZERO, ZERO);

    M = Point3f(x - other.x,
                y - other.y,
                z - other.z).res3f();

    if(M > 0){
        finalspeed.x = -(U / M) * (x - other.x);
        finalspeed.y = -(U / M) * (y - other.y);
        finalspeed.z = -(U / M) * (z - other.z); //should this be inverted?
    }
    return finalspeed;
}

Point3i::Point3i(const Point3f& other){
 /*   div_t relative_x; //position in brick coords
    div_t relative_y;
    div_t relative_z;

    relative_x = div((int)round(other.x + SCENE_MAX), INTX-1);
    relative_y = div((int)round(other.y + SCENE_MAX), INTY-1);
    relative_z = div(2*(int)(round(abs(other.z))), (INTZ-1)/2);

    X = relative_x.quot;
    Y = relative_y.quot;
    Z = relative_z.quot;

    if(X < 0)
        X = 0;
    if(Y < 0)
        Y = 0;
    if(Z < 1)
        Z = 1;
    if(X > INTX)
        X = INTX;
    if(Y > INTY)
        Y = INTY;
    if(Z > INTZ)
        Z = INTZ;
        */
    X = (int)round(0.2*other.x) + 3;
    Y = (int)round(0.2*other.y) + 3;
    Z = (int)round(0.4*other.z + 0.5);
}
