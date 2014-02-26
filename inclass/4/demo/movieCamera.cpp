/*  =================== File Information =================
  File Name: movieCamera.cpp
  Description:
  Author: 

  Purpose:
  Examples:


Relevant keywords and OpenGL calls to (understand/complete/create cool effects) for this lab:
-glFrustrum
-glDepthRange
-glMultMatrixf
-glTranslate/glRotate/glScale
-glOrtho
-OpenGL field of view


  ===================================================== */
#include <math.h>
#include "movieCamera.h"

// Static initializers
float movieCamera::translate[] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
float movieCamera::rotate[] = 	{ 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
float movieCamera::scale[] = 	{ 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
float movieCamera::rotate_around = 0;


	/*  ===============================================
	      Desc: Default Constructor
	      Precondition:
	      Postcondition:
    	=============================================== */ 
movieCamera::movieCamera(){
	}

	/*  ===============================================
	      Desc: Default Destructor
	      Precondition:
	      Postcondition:
    	=============================================== */ 
movieCamera::~movieCamera(){

}

/*
	Implement the clipping plane
*/
/*  ===============================================
      Desc: 
      Precondition:
      Postcondition:
	=============================================== */ 
void movieCamera::closeUp(float your_x, float your_y, float your_z, float near, float far){
	// Figure out which direction target is *hint* what mathematical operations can we use?

	// Translate to your position
	glPushMatrix();
		glRotatef(180,0,1,0);
		glTranslatef(-your_x,-your_y+1,-6);
		glDepthRange(0,1);
	glPopMatrix();
}

/*
	Change the field of view
*/
/*  ===============================================
      Desc: 
      Precondition:
      Postcondition:
	=============================================== */ 
void movieCamera::wideAngle(float _fov){
	/*
	xy_aspect = (float)x / (float)y;
	glViewport(0, 0, x, y);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-xy_aspect*.08, xy_aspect*.08, -.08, .08, .1, 15.0);
	*/
	// Call perspective, but change the field of view
	perspective(_fov,0.75, 1, 25);
}

/*
	Set the camera to an orthogonal view
*/
/*  ===============================================
  Desc: 
  Precondition:
  Postcondition:
=============================================== */ 
void movieCamera::orthogonal(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble nearVal, GLdouble farVal){
	glPushMatrix();
		glOrtho(left,right,bottom,top,nearVal,farVal);
//		glPushMatrix();
//		glTranslatef(-2,-1,-9);
//	glPopMatrix();
}

/*
	Setup a perspective camera
	http://steinsoft.net/index.php?site=Programming/Code%20Snippets/OpenGL/gluperspective
	or
	http://www.opengl.org/sdk/docs/man2/xhtml/gluPerspective.xml
*/
/*  ===============================================
      Desc: 
      Precondition:
      Postcondition:
	=============================================== */ 
void movieCamera::perspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar){
  GLdouble xmin, xmax, ymin, ymax;

   ymax = zNear * tan(fovy * 3.14 / 360.0);
   ymin = -ymax;
   xmin = ymin * aspect;
   xmax = ymax * aspect;

   glFrustum(xmin, xmax, ymin, ymax, zNear, zFar);
}
/*  ===============================================
      Desc: 
      Precondition:
      Postcondition:
	=============================================== */ 
void movieCamera::follow(float your_x, float your_y, float your_z,
						float target_x, float target_y, float target_z,
						float up_x, float up_y, float up_z){
	// Figure out which direction target is *hint* what mathematical operations can we use?
	// Rotate 
	glRotatef(90,0,1,0);
	glRotatef(30,0,0,1);
	// Translate to your position
	glTranslatef(-your_x,-your_y-4,-your_z);
	// Set OpenGL up vector

}
/*  ===============================================
      Desc: Spin around a point in space at a distance(i.e. "radius")
      Precondition:
      Postcondition:
	=============================================== */ 
void movieCamera::spinAroundPoint(float your_x, float your_y, float your_z, float distance){
	rotate_around += 1;
	glTranslatef(your_x,your_y-2,your_z-distance);
	glRotatef(rotate_around,0,1,0);

	if(rotate_around>360){ rotate_around = 0; }
}
