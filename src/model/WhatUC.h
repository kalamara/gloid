#ifndef _WHATUC_H_
#define _WHATUC_H_
template<class T>

class WhatUC{
public:
   BOOL active;
   point3f place;
   point3f size;

   WhatUC(){
      active = TRUE;

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

   virtual void display()=0;
   virtual T& animate(double secPerFrame)=0;
};
#endif //_WHATUC_H_
