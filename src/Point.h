#ifndef _POINT_H
#define _POINT_H

#define ONE 1.0f
#define ZERO 0.0f
#define FLOAT_PRECISION 0.000001f

#include <math.h>
class Point3i;
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
    Point3f(float base);

    //copy ctor
    Point3f(const Point3f& other){
        x = other.x;
        y = other.y;
        z = other.z;
    }

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
    //inner product
    float inner3f(const Point3f& other) const {
        return x * other.x + y * other.y + z * other.z;
    }
    //multiply by number
    Point3f& mul3f(const float lambda){
        x = lambda * x;
        y = lambda * y;
        z = lambda * z;

        return *this;
    }
    //add vectors
    Point3f& add3f(const Point3f& other){
        x += other.x;
        y += other.y;
        z += other.z;

        return *this;
    }
    //subtract vectors
    Point3f& sub3f(const Point3f& other){
        x -= other.x;
        y -= other.y;
        z -= other.z;

        return *this;
    }

    // Calculate the norm of a 3-vector
    float res3f() const {
        return sqrt(x*x + y*y + z*z);
    }

    /**
     * @brief normalize vector
     * @param the norm
     * @return normalized vector, or zeroes
     */
    Point3f norm3f(const float norm);

    /**
     * @brief scale by (x,y,z), keeping the same norm
     * @param scale (x,y,z)
     * @return normalized vector whose coords are scaled
     */
    Point3f scale3f(const Point3f& scale);

    /**
     * @brief projection on vector
     * @param other vector on which to project
     * @return ratio lambda where lambda*other = projection
     */
    float proj3f(const Point3f& other) const;

    /**
     * @brief Calculate the speed components of object A chasing object B at speed U
     * @param object B
     * @param speed normal U
     * @return speed vector
     */
    Point3f chase(const Point3f &other, float U);

    /**
    * @brief find raycasting point on plane where
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
    * @brief direction information in terms of negative or positive sign
    * @return unary vector with negative or positive units
    */
    Point3i signs() const;

    /**
    * @brief euclidean distance from other point
    * @param other
    * @return distance
    */
    float dist(const Point3f other) const;
};

/**
 * @brief int coordinates
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
