#ifndef _WHATUC_H_
#define _WHATUC_H_

//base class for any visible models.
//it's a Curiously Recurring Template Pattern:
template<class T>

class WhatUC{
public:
   bool active = true;
   point3f place;
   point3f size;

   WhatUC(){
      memset(&place, 0, sizeof(point3f));
      memset(&size, 0, sizeof(point3f));
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
   //pure virtual but also we want to be able to have each sublasses'
   //implementation of animate to return self so we can implement
   //Builder patterns with animate and display
   virtual void display()=0;
   virtual T& animate(double secPerFrame)=0;
   //if object collides with a rectangle
   virtual bool collides(float left,
                         float right,
                         float up,
                         float down){
       return false;
   }

};
#endif //_WHATUC_H_
