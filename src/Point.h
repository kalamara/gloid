#ifndef _POINT_H
#define _POINT_H

#define ONE 1.0f
#define ZERO 0.0f
#define FLOAT_PRECISION 0.000001f

#include <math.h>

// 3 dimensional vectors
class Point3f {
public:
    float x = ZERO;
    float y = ZERO;
    float z = ZERO;
    //ctor
    Point3f(float a, float b, float c){
        x = a;
        y = b;
        z = c;
    }
    //default ctor returns {0,0,0} - see above
    Point3f(){}

    //construct random point with base magnitude
    Point3f(float base){

        float tempx = rand() - RAND_MAX / 2;
        float tempy = rand() - RAND_MAX / 2;
        float tempz = rand() - RAND_MAX / 2;

        float M = Point3f(tempx, tempy, tempz).res3f();

        if(M > 0){
            x = (base / M) * tempx;
            y = (base / M) * tempy;
            z = (base / M) * tempz;
        } else {
            x = ZERO;
            y = ZERO;
            z = ZERO;
        }
    }    
    //copy ctor
    Point3f(const Point3f& other){
        x = other.x;
        y = other.y;
        z = other.z;
    }
    //convertor from integers
    Point3f(const class Point3i& other);
    //equals
    bool eq(const Point3f other){
        return abs(x - other.x) <= FLOAT_PRECISION
                && abs(y - other.y) <= FLOAT_PRECISION
                && abs(z - other.z) <= FLOAT_PRECISION;
    }
    //copy values from other
    Point3f& deepcopy(const Point3f& other){
        x = other.x;
        y = other.y;
        z = other.z;

        return *this;
    }
    // Calculate the norm of a 3-vector
    float res3f(){
        return sqrt(x*x + y*y + z*z);
    }
    // Calculate the speed components of object A chasing object B at speed U
    Point3f chase(const Point3f &other, float U);

/**  @abstract find raycasting point on plane where
 * P(axis = c) where c is this(axis) (+-) dist eg. P(x = c)
 * sign is the same as speed component on axis, value is iqual to dist
 *
 * @param speed given speed vector U
 * @param axis axis of movement
 * @param distance of plane from start on axis of movement
 * @return point of collision on plane
 * */
    Point3f raycast(const Point3f &speed, int axis, float dist) const;

/**
 * @brief prefix
 * @return unary vector with negative or positive ones
 */
    Point3i prefix() const;

/**
  * @brief euclidean distance form other point
  * @param other
  * @return distance
  */
    float dist(const Point3f other) const;
};

/**
 * @brief Brick coordinates (maybe should move to Brick class)
 *
 */
class Point3i{
public:
  int X = 0;
  int Y = 0;
  int Z = 0;
  //ctor
  Point3i(int a, int b, int c){
      X = a;
      Y = b;
      Z = c;
  }
  //default ctor returns {0,0,0} - see above
  Point3i(){}
  //copy ctor
  Point3i(const Point3i& other){
      X = other.X;
      Y = other.Y;
      Z = other.Z;
  }
  //convertor from float
  Point3i(const Point3f& other);
  //equals
  bool eq(const Point3i& other) const{
      return X == other.X
               && Y == other.Y
               && Z == other.Z;
  }
  //greater than
  bool gt(const Point3i& other) const{
      //Z is most important, followed by Y, X
      return  Z > other.Z
                || (Z == other.Z && Y > other.Y)
                || (Z == other.Z && Y == other.Y) && X > other.X;
  }
  bool operator ==(const Point3i& other) const{

      return eq(other);
  }
  bool operator !=(const Point3i& other) const{

      return !eq(other);
  }
  bool operator <=(const Point3i& other) const{

      return !gt(other);
  }
  bool operator >=(const Point3i& other) const{

      return eq(other) || gt(other);
  }
  bool operator >(const Point3i& other) const{

      return gt(other);
  }
  bool operator <(const Point3i& other) const{

      return !gt(other) && !eq(other);
  }
};

#endif //_POINT_H
