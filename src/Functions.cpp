#include <list>

#include "GLoid.h"
#include "Functions.h"
// SEVERAL FUNCTIONS

// Check to see if two rectangles A, B have collided
BOOL Functions::collision (float aLeft,
                float aRight,
                float aUp,
                float aDown,
                float bLeft,
                float bRight,
                float bUp,
                float bDown){
   if((((aLeft <= bLeft) && (bLeft <= aRight)) ||
       ((aLeft <= bRight) && (bRight <= aRight))) &&
      (((aDown <= bDown) && (bDown <= aUp)) ||
       ((aDown <= bUp) && (bUp <= aUp)))){
      return TRUE;
   }else{
      if((((bLeft <= aLeft) && (aLeft <= bRight)) ||
          ((bLeft <= aRight) && (aRight <= bRight))) &&
         (((bDown <= aDown) && (aDown <= bUp)) ||
          ((bDown <= aUp) && (aUp <= bUp)))){
            return TRUE;
      }else{
        return FALSE;
      }
   }
}

// Calculate the norm of a 3-vector
float Functions::res3f(float x, float y, float z){
   return sqrt(x*x + y*y + z*z);
}

// Calculate the speed components of object A chasing object B at speed U
point3f_t Functions::chase(point3f a, point3f b, float U){
   float M;
   point3f_t finalspeed = new point3f(ZERO, ZERO, ZERO);

   M = res3f(a.x - b.x, a.y - b.y, a.z - b.z);

   if(M > 0)
   {
      finalspeed->x = -(U / M) * (a.x - b.x);
      finalspeed->y = -(U / M) * (a.y - b.y);
      finalspeed->z =  (U / M) * (a.z - b.z);
   }
   return finalspeed;
}

// Generate a random vector
point3f_t Functions::rand3f(float base){

   float tempx = rand() - RAND_MAX / 2;
   float tempy = rand() - RAND_MAX / 2;
   float tempz = rand() - RAND_MAX / 2;

   float M = res3f(tempx, tempy, tempz);

   point3f_t final = new point3f((base / M) * tempx,
                                (base / M) * tempy,
                                (base / M) * tempz);

   return final;
}

/**<3<3<3<3<3<3<3******<3<3<3<3<3<3<3<*
//template <typename T>
T *Functions::randItem(std::list<T> l){
    return &*l.end();
}
*/
// Compute the next power of two: 2^i < x <= 2^(i+1) = y
int Functions::nextpoweroftwo(int x){
   double y;

   y = pow(2, ceil(log(x) / log(2)));
   return (int)y;
}

// Calculate the moving average of <size> samples
double Functions::moving_average(double x, double* a, int size){
   int i;
   double sum = x;

   for(i = size - 1; i > 0; i--)
   {
      a[i] = a[i-1];
      sum += a[i];
   }
   a[0] = x;

   return sum / size;
}

point3i Functions::coords(point3f place){
	point3i axis;
	div_t relative_x; //position in brick coords 
	div_t relative_y;
	div_t relative_z;
	relative_x = div((int)(place.x + SCENE_MAX), INTX-1); //ball position in brick coords
	relative_y = div((int)(place.y + SCENE_MAX), INTY-1); 
	relative_z = div(2*(int)(abs(place.z)), (INTZ-1)/2); 
	axis.X = relative_x.quot;
	axis.Y = relative_y.quot;
	axis.Z = relative_z.quot;
	
	if(axis.X <0)
		axis.X = 0;
	if(axis.Y <0)
		axis.Y = 0;
	if(axis.Z <1)
		axis.Z = 1;
	if(axis.X > INTX)
		axis.X = INTX;
	if(axis.Y > INTY)
		axis.Y = INTY;
	if(axis.Z > INTZ)
		axis.Z = INTZ;
	return axis;
}

