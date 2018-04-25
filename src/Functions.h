#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

// SEVERAL FUNCTIONS
namespace Functions {

// Check to see if two rectangles A, B have collided
BOOL collision (float aLeft,
                float aRight,
                float aUp,
                float aDown,
                float bLeft,
                float bRight,
                float bUp,
                float bDown);

// Compute the next power of two: 2^i < x <= 2^(i+1) = y
int nextpoweroftwo(int x);

// Calculate the moving average of <size> samples
double moving_average(double x, double* a, int size);

/*TODO: maybe these are members of the point3f class?*/

// Calculate the norm of a 3-vector
float res3f(float x, float y, float z);

// Calculate the speed components of object A chasing object B at speed U
point3f_t chase(point3f a, point3f b, float U);

// Generate a random vector
point3f_t rand3f(float base);

//convert float to integer coords
point3i_t coords(const point3f_t place);

//convert int to float coords
point3f_t fromcoords(const point3i_t coords);

}
#endif //_FUNCTIONS_H
