
#include "gloid.h"

// SEVERAL FUNCTIONS

// Check to see if two rectangles A, B have collided
BOOL collision (float aLeft, float aRight, float aUp, float aDown, float bLeft, float bRight, float bUp, float bDown)
{
   if((((aLeft <= bLeft) && (bLeft <= aRight)) ||
       ((aLeft <= bRight) && (bRight <= aRight))) &&
      (((aDown <= bDown) && (bDown <= aUp)) ||
       ((aDown <= bUp) && (bUp <= aUp))))
      return TRUE;
   else
   {
      if((((bLeft <= aLeft) && (aLeft <= bRight)) ||
          ((bLeft <= aRight) && (aRight <= bRight))) &&
         (((bDown <= aDown) && (aDown <= bUp)) ||
          ((bDown <= aUp) && (aUp <= bUp))))
         return TRUE;
      else
         return FALSE;
   }
}

// Calculate the norm of a 3-vector
float res3f(float x, float y, float z)
{
   return sqrt(x*x + y*y + z*z);
}

// Calculate the speed components of object A chasing object B at speed U
point3f chase(point3f a, point3f b, float U)
{
   float M;
   point3f finalspeed;

   finalspeed.x = 0.0f;
   finalspeed.y = 0.0f;
   finalspeed.z = 0.0f;

   M = res3f(a.x - b.x, a.y - b.y, a.z - b.z);

   if(M > 0)
   {
      finalspeed.x = -(U / M) * (a.x - b.x);
      finalspeed.y = -(U / M) * (a.y - b.y);
      finalspeed.z =  (U / M) * (a.z - b.z); 
   }
   return finalspeed;
}

// Generate a random vector
point3f rand3f(float base)
{
   point3f final;
   float tempx, tempy, tempz, M;

   tempx = rand() - RAND_MAX / 2;
   tempy = rand() - RAND_MAX / 2;
   tempz = rand() - RAND_MAX / 2;

   M = res3f(tempx, tempy, tempz);

   final.x = (base / M) * tempx;
   final.y = (base / M) * tempy;
   final.z = (base / M) * tempz;

   return final;
}

// Generate a random bonus pill
int roulette()
{
   int i;
   extern unsigned int Totalscore, Hiscore;

   i = rand();

   // A low-scoring game, 83.5% chance of no bonus pill
   if(Totalscore < Hiscore)
   {
      if(i < RAND_MAX / 20)          // 5%
         return D;
      if(i < 3 * (RAND_MAX / 40))    // 2.5%
         return L;
      if(i < RAND_MAX / 10)          // 2.5%
         return G;
      if(i < RAND_MAX / 8)           // 2.5%
         return E;
      if(i < 3 * (RAND_MAX / 20))    // 2.5%
         return S;
      if(i < 4 * (RAND_MAX / 25))    // 1%
         return B;
      if(i < 33 * (RAND_MAX / 200))  // 0.5%
         return P;
   }
   else  // A high-scoring game, 76% chance of no bonus pill
   {
      if(i < RAND_MAX / 20)          // 5%
         return L;
      if(i < RAND_MAX / 10)          // 5%
         return E;
      if(i < 3 * (RAND_MAX / 20))    // 5%
         return G;
      if(i < RAND_MAX / 5)           // 5%
         return S;
      if(i < 9 * (RAND_MAX / 40))    // 2.5%
         return D;
      if(i < 47 * (RAND_MAX / 200))  // 1%
         return P;
      if(i < 6 * (RAND_MAX / 25))    // 0.5%
         return B;
   }
   return -1;
}

// Compute the next power of two: 2^i < x <= 2^(i+1) = y
int nextpoweroftwo(int x)
{
   double y;

   y = pow(2, ceil(log(x) / log(2)));
   return (int)y;
}

// Find the first active element in the list
int find_first(WhatUC* list, int size)
{
   int i;

   for(i = 0; i < size; i++)
   {	
      if(!list[i].active)
         return i;
   }
   return -1;
}

void cleanup(void)
{
//delete[] brick;
//delete[] vaus;
//delete[] texture;
//delete[] pill;
//delete[] alien;
//delete[] ball;
//delete[] shot;
}

// Print a line to the log file
int Log(const char *szFormat, ...)
{
   extern FILE *ErrorLog;
   va_list Arg;

   va_start(Arg, szFormat);

   if(ErrorLog)
   {
      vfprintf(ErrorLog, szFormat, Arg);
      fflush(ErrorLog);
   }
   va_end(Arg);

   return 0;
}

// Render text to an SDL_Surface
void printText(BOOL option, text2d* text, SDL_Color fg, SDL_Color bg, int x, int y, const char* buf, ...)
{
   extern TTF_Font *DejaVuSans;
   va_list Arg;

   va_start(Arg, buf);
   vsprintf(text->msg,buf, Arg);
   va_end(Arg);

   if(option)
      text->T = TTF_RenderText_Shaded(DejaVuSans, text->msg, fg, bg);
   else
      text->T = TTF_RenderText_Blended(DejaVuSans, text->msg, fg);

   text->src.w = text->T->w;
   text->src.h = text->T->h;
   text->src.x = x;
   text->src.y = y;
}

// Load text into a buffer
int loadText(const char* path, char** buffer)
{
   int i = 0;
   FILE* f;
   char fullpath[MAX_PATH];
   char line[MAXLINE];
   char text[TEXTLINES][MAXLINE];

   memset(fullpath, 0, MAX_PATH);
   memset(line, 0, MAXLINE);
   memset(text, 0, TEXTLINES * MAXLINE);

   snprintf(fullpath, MAX_PATH, "%s/%s", WORKPATH, path);

   f = fopen(fullpath, "r");

   if(!f)
      return FALSE;

   while(fgets(line, MAXLINE, f) && (i < TEXTLINES))
   {
        line[strlen(line) - 1] = 0;
        snprintf(text[i++], MAXLINE, "%s", line);
   }

   memcpy(buffer, text, TEXTLINES * MAXLINE);
   return TRUE;
}

// Calculate the moving average of <size> samples
double moving_average(double x, double* a, int size)
{
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

// Draw a solid cube
void mySolidCube(float side)
{
   side = side / 2.0f;

   glBegin(GL_QUADS);

   // Front Face
   glNormal3f(0.0f, 0.0f, 1.0f);     // Normal Facing Forward
   glVertex3f(-side, -side,  side);  // Bottom Left Of The Texture and Quad
   glVertex3f( side, -side,  side);  // Bottom Right Of The Texture and Quad
   glVertex3f( side,  side,  side);  // Top Right Of The Texture and Quad
   glVertex3f(-side,  side,  side);  // Top Left Of The Texture and Quad

   // Back Face
   glNormal3f(0.0f, 0.0f,-1.0f);     // Normal Facing Away
   glVertex3f(-side, -side, -side);  // Bottom Right Of The Texture and Quad
   glVertex3f(-side,  side, -side);  // Top Right Of The Texture and Quad
   glVertex3f( side,  side, -side);  // Top Left Of The Texture and Quad
   glVertex3f( side, -side, -side);  // Bottom Left Of The Texture and Quad

   // Top Face
   glNormal3f(0.0f, 1.0f, 0.0f);     // Normal Facing Up
   glVertex3f(-side,  side, -side);  // Top Left Of The Texture and Quad
   glVertex3f(-side,  side,  side);  // Bottom Left Of The Texture and Quad
   glVertex3f( side,  side,  side);  // Bottom Right Of The Texture and Quad
   glVertex3f( side,  side, -side);  // Top Right Of The Texture and Quad

   // Bottom Face
   glNormal3f(0.0f,-1.0f, 0.0f);     // Normal Facing Down
   glVertex3f(-side, -side, -side);  // Top Right Of The Texture and Quad
   glVertex3f( side, -side, -side);  // Top Left Of The Texture and Quad
   glVertex3f( side, -side,  side);  // Bottom Left Of The Texture and Quad
   glVertex3f(-side, -side,  side);  // Bottom Right Of The Texture and Quad

   // Right face
   glNormal3f(1.0f, 0.0f, 0.0f);     // Normal Facing Right
   glVertex3f(side, -side, -side);   // Bottom Right Of The Texture and Quad
   glVertex3f(side,  side, -side);   // Top Right Of The Texture and Quad
   glVertex3f(side,  side,  side);   // Top Left Of The Texture and Quad
   glVertex3f(side, -side,  side);   // Bottom Left Of The Texture and Quad

   // Left Face
   glNormal3f(-1.0f, 0.0f, 0.0f);    // Normal Facing Left
   glVertex3f(-side, -side, -side);  // Bottom Left Of The Texture and Quad
   glVertex3f(-side, -side,  side);  // Bottom Right Of The Texture and Quad
   glVertex3f(-side,  side,  side);  // Top Right Of The Texture and Quad
   glVertex3f(-side,  side, -side);  // Top Left Of The Texture and Quad

   glEnd();
}

// Draw a Rhombicuboctahedron
void mySolidRhombik(float side)
{
		float unary, big;
		int dim, dir, neg, tog, i,j;
		unary = side/2.0f;	//unit
		//big	 = (1.0f+sqrt(2.0f))*unary;//unit * (1+v2)
		big = unary*1.5f;
		point3f vertice[24], face[6];
		int path[18]  = {3,0,2,1,18,19,17,16,6,7,5,4,23,20,22,21,3,0};
		int toppath[19] = {17,11,18,10, 2,10,3,9,22, 8,23, 5,6, 8,17,11,10, 8, 9};
		int botpath[19] = {15,12,14,13,16,12,7,4,20,12,21,15,0,14,1 ,14,19,13,16};		
		int facepath[4] = {0,4,1,5};
	/*	//FYI
		// Front Face
		vertice[0] = {-unary, -unary,  big};
		vertice[1] = {unary, -unary,  big};
		vertice[2] = {unary,  unary,  big};
		vertice[3] = {-unary,  unary,  big};
		// Back Face
		vertice[4] = {-unary, -unary, -big};
		vertice[5] = {-unary,  unary, -big};
		vertice[6] = {unary,  unary, -big};
		vertice[7] = { unary, -unary, -big};
		// Top Face
		vertice[8] = {-unary,  big, -unary};
		vertice[9] = {-unary,  big,  unary};
		vertice[10] = { unary,  big,  unary};
		vertice[11] = {unary,  big, -unary};
		// Bottom Face
		vertice[12] = {-unary, -big, -unary};
		vertice[13] = {unary, -big, -unary};
		vertice[14] = {unary, -big,  unary};
		vertice[15] = {-unary, -big,  unary};
		// Right face
		vertice[16] = {big, -unary, -unary};
		vertice[17] = {big,  unary, -unary};
		vertice[18] = {big,  unary,  unary};
		vertice[19] = {big, -unary,  unary};
		// Left Face
		vertice[20] = {-big, -unary, -unary};
		vertice[21] = {-big, -unary,  unary};
		vertice[22] = {-big,  unary,  unary};
		vertice[23] = {-big,  unary, -unary};

		face[0] = {0.0f, 0.0f, 1.0f};
		face[1] = {0.0f, 0.0f,-1.0f};
		face[2] = {0.0f, 1.0f, 0.0f};
		face[3] = {0.0f,-1.0f, 0.0f};
		face[4] = {1.0f, 0.0f, 0.0f};
		face[5] = {-1.0f, 0.0f, 0.0f};*/

		for(dim = 0; dim <3; dim++)
			for(dir = 0; dir<2;dir++)
			{
				i = 2*dim+dir;
				
				switch(dim)
				{
							case 0:	face[i].z = 1.0f-2*dir;
								break;		
							case 1: face[i].y = 1.0f-2*dir;
								break;
							case 2: face[i].x = 1.0f-2*dir;
								break;
				}
				glNormal3f( face[i].x, face[i].y, face[i].z);		// Normal Facing 
				for(neg = 0; neg <2; neg ++)
					for(tog = 0; tog <2; tog++)
					{
						j = 4*i+2*neg+tog;	
						switch(dim)
						{
							case 0:	
									vertice[j].z = (1-2*dir)*big;
									if(dir==0)
									{
										vertice[j].y = (2*neg-1)*unary;
										vertice[j].x = -vertice[j].y*(2*tog-1);
									}
									else
									{
										vertice[j].x = (2*neg-1)*unary;
										vertice[j].y = -vertice[j].x*(2*tog-1);
									}
									break;
							case 1: 
									vertice[j].y = (1-2*dir)*big;
									if(dir==0)
									{
										vertice[j].x = (2*neg-1)*unary;
										vertice[j].z = -vertice[j].x*(2*tog-1);
									}
									else
									{
										vertice[j].z = (2*neg-1)*unary;
										vertice[j].x = -vertice[j].z*(2*tog-1);
									}
									break;
							case 2: 
									vertice[j].x =(1-2*dir)*big;
									if(dir==0)
									{
										vertice[j].z = (2*neg-1)*unary;
										vertice[j].y = -vertice[j].z*(2*tog-1);
									}
									else
									{
										vertice[j].y = (2*neg-1)*unary;
										vertice[j].z = -vertice[j].y*(2*tog-1);
									}
									break;
						}
						//glVertex3f(vertice[j].x, vertice[j].y,  vertice[j].z);
						//Log(" E[%d] = (%g,%g,%g) V[%d] = (%g,%g,%g)\n", i, face[i].x, face[i].y, face[i].z, j, vertice[j].x, vertice[j].y, vertice[j].z ); 
					}	
			}
			//glEnd();					// Done Drawing Quads

glBegin(GL_TRIANGLE_STRIP);			// Start Drawing Strips
	for(j = 0; j <18; j++)
	{//front, left, back, right faces
				glNormal3f(face[facepath[j%4]].x,face[facepath[j%4]].y,face[facepath[j%4]].z);		// Normal Facing
				glVertex3f(vertice[path[j]].x,  vertice[path[j]].y,  vertice[path[j]].z);	
				//Log(" E[%d] = (%g,%g,%g) V[%d] = (%g,%g,%g)\n", i, face[i].x, face[i].y, face[i].z, j, vertice[j].x, vertice[j].y, vertice[j].z ); 
	}
glEnd();					// Done Drawing Quads

// Top Face

glBegin(GL_TRIANGLE_STRIP);			// Start Drawing Strips
glNormal3f( 0.0f, 1.0f, 0.0f);		// Normal Facing Up	
	for(j = 0; j <19; j++)
	{
				glVertex3f(vertice[toppath[j]].x,  vertice[toppath[j]].y,  vertice[toppath[j]].z);	
				//Log(" E[%d] = (%g,%g,%g) V[%d] = (%g,%g,%g)\n", i, face[i].x, face[i].y, face[i].z, j, vertice[j].x, vertice[j].y, vertice[j].z ); 
	}
glEnd();					// Done Drawing Strips

glBegin(GL_TRIANGLE_STRIP);			// Start Drawing Strips

// Bottom Face

glNormal3f( 0.0f,-1.0f, 0.0f);		// Normal Facing Down

	for(j = 0; j <19; j++)
	{
				glVertex3f(vertice[botpath[j]].x,  vertice[botpath[j]].y,  vertice[botpath[j]].z);	
				//Log(" E[%d] = (%g,%g,%g) V[%d] = (%g,%g,%g)\n", i, face[i].x, face[i].y, face[i].z, j, vertice[j].x, vertice[j].y, vertice[j].z ); 
	}
glEnd();					// Done Drawing Strips
}

// Draw a Tetrahedron
// FIXME: Not geometrically correct, normals are not normalized
void mySolidTetrahedron(float side)
{
   side = side / 2.0f;

   glBegin(GL_TRIANGLES);

   // Back Face
   glNormal3f(0.0f, -0.5f, -1.0f);   // Normal Facing Away and 1/2 down
   glVertex3f( 0.0f,  side,  side);  // Top front 
   glVertex3f(-side, -side,  0.0f);  // bottom Left
   glVertex3f( side, -side,  0.0f);  // Bottom Right 

   // Front Face
   glNormal3f(0.0f, -0.5f, 1.0f);    // Normal Facing Forward and 1/2 down
   glVertex3f( 0.0f,  side, -side);  // Top Back 
   glVertex3f( side, -side,  0.0f);  // Bottom Right 
   glVertex3f(-side, -side,  0.0f);  // Bottom Left 

   // Left Face
   glNormal3f(-1.0f, 0.5f, 0.0f);    // Normal Facing Left and 1/2 up
   glVertex3f( 0.0f,  side,  side);  // Top Front 
   glVertex3f( side, -side,  0.0f);  // Bottom Right 
   glVertex3f( 0.0f,  side, -side);  // Top Back 

   // Right face
   glNormal3f(1.0f, 0.5f, 0.0f);     // Normal Facing Right and 1/2 up
   glVertex3f( 0.0f,  side, -side);  // Top Back 
   glVertex3f(-side, -side,  0.0f);  // Bottom Left 
   glVertex3f( 0.0f,  side,  side);  // Top Front 

   glEnd();
}

int get_hiscore(void)
{
   extern char Initials[MAXLINE];
   extern int Hiscore;
   char textbuffer[TEXTLINES][MAXLINE];//a buffer
   char score_str[MAXLINE], init_str[MAXLINE];
   int i, j, k;

   memset(score_str, 0, MAXLINE);
	
   if(!loadText("hallofame.txt", (char**)textbuffer))
   {
      Log("Loading file %s failed!\n", "hallofame.txt");
      return -1;
   }

   for(i = 0, j = 0, k = 0; (i < MAXLINE) && (textbuffer[0][i] != 0); i++)
   {
      if(isdigit(textbuffer[0][i]))
      {
         score_str[j] = textbuffer[0][i];
         j++;
      }
      else
      {
         if(isalpha(textbuffer[0][i]))
            init_str[k] = textbuffer[0][i];
      }
   }
   Hiscore = atoi(score_str);
   snprintf(Initials, MAXLINE, "%s", init_str);

   return 0;
}

// Set high score when a record is made
void set_hiscore(char* initials)
{
   extern int Totalscore;
   char textbuffer[TEXTLINES][MAXLINE];
   FILE* f;
   int i;
   char path[MAX_PATH];

   memset(path, 0, MAX_PATH);
   snprintf(path, MAX_PATH, "%s/hallofame.txt", WORKPATH);

   // Read current high score file into text buffer
   if(!loadText("hallofame.txt", (char**)textbuffer))
      Log("Loading file %s failed!\n", "hallofame.txt");

   f = fopen(path, "w");

   if(!f)
      return;

   fprintf(f,"%s %d\n", initials, Totalscore);

   for(i = 0; i < TEXTLINES-1;i++)
      fprintf(f,"%s\n", textbuffer[i]);

   fclose(f);
}

point3i brickcoords(point3f place)
{
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
