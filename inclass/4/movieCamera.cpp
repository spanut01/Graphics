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
#include "Algebra.h"
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
    glRotatef(90, 0, 1, 0);
    glTranslatef(-your_x, -your_y, -your_z);
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
  //glPushMatrix();
  //glMatrixMode(GL_PROJECTION);
  //glLoadIdentity();
  glScalef(.1,.1,.1);
  glOrtho(left,right,bottom,top,near,far);
  //glOrtho(left-10,right+10,bottom-10,top+10,nearVal-5,farVal+15);
  //glRotatef(PI,0,1,0);
  //glTranslatef(0,0,-4);
  //glPopMatrix();
}

/*  ===============================================
      Desc: 
      Precondition:
      Postcondition:
	=============================================== */ 
void movieCamera::perspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar){
  gluPerspective(fovy,aspect,zNear,zFar);
}
/*  ===============================================
      Desc: 
      Precondition:
      Postcondition:
	=============================================== */ 
void movieCamera::follow(float your_x, float your_y, float your_z,
						float target_x, float target_y, float target_z,
						float up_x, float up_y, float up_z){
    glRotatef(270, 0, 1, 0);
    glRotatef(22, 0, 0, 1);
    glTranslatef(-your_x + 2, -your_y, -your_z);

}
/*  ===============================================
      Desc: Spin around a point in space at a distance(i.e. "radius")
      Precondition:
      Postcondition:
	=============================================== */ 
void movieCamera::spinAroundPoint(float your_x, float your_y, float your_z, float distance){
	rotate_around += 1;

	glTranslatef(your_x, your_y, your_z);
    glRotatef(rotate_around, 0, 1, 0);

	if(rotate_around>360){ rotate_around = 0; }
}
