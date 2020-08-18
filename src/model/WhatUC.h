#ifndef _WHATUC_H_
#define _WHATUC_H_

// Colors
typedef enum{
   RGB_GREY,
   RGB_PINK,
   RGB_BLUE,
   RGB_RED,
   RGB_GREEN,
   RGB_MAGENTA,
   RGB_ORANGE,
   RGB_YELLOW,
   RGB_WHITE,
   RGB_BLACK,
   N_RGB_COLORS
}RGB_COLORS;

//axes of motion
typedef enum {
    NO_AXIS = 0,
    AXIS_X = 0x1,//left to right
    AXIS_Y = 0x2,//up to down
    AXIS_Z = 0x4,//front to back
} AXES;//minus sign inverts motion direction

class Point3f;
//base class for any visible models.
//it's a Curiously Recurring Template Pattern:
template<class T> class WhatUC{
public:
   bool active = true;
   Point3f place;
   Point3f size;
   Point3f speed;

   WhatUC(){
      place = Point3f();
      size = Point3f();
   }
   T& setPlace(float X, float Y, float Z){
        place.x = X;
        place.y = Y;
        place.z = Z;
        return *dynamic_cast<T*>(this);
   }

   T& setSize(float X, float Y, float Z){
        size.x = X;
        size.y = Y;
        size.z = Z;
        return *dynamic_cast<T*>(this);
   }

   ~WhatUC(){}
   //pure virtual but also we want to be able to have each sublasses'
   //implementation of animate to return self so we can implement
   //Builder patterns with animate and display
   virtual void display()=0;
   virtual T& animate(double secPerFrame)=0;
   //if object collides with a rectangle

   /**
    * @brief if it collides with
    * @param other model object
    * @return true otherwise false
    */
   virtual bool collides(const WhatUC<class O> & other){
       return (abs(place.x - other.place.x) < size.x/2 + other.size.x/2)
            ||(abs(place.y - other.place.y) < size.y/2 + other.size.y/2)
            ||(abs(place.z - other.place.z) < size.z/2 + other.size.z/2);
   }
};
#endif //_WHATUC_H_
