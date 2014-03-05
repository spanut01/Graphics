/*  =================== File Information =================
	File Name: spline.cpp
	Description:
	Author: Michael Shah
	Last Modified: 3/04/14

	Purpose: Create a cubic spline 
	Usage:	

	Further Reading resources: 
	http://mathworld.wolfram.com/CubicSpline.html
	http://www.geos.ed.ac.uk/~yliu23/docs/lect_spline.pdf
	===================================================== */
#include <GL/glui.h>
#include <iostream>
#include <cmath>
#include "spline.h"

/*	===============================================
Desc:
Precondition:
Postcondition:
=============================================== */ 
spline::spline(){
	// Set to Null
	head = NULL;
	tail = NULL;

	frames = 60;
	points = 0;
}

/*	===============================================
Desc:
Precondition:
Postcondition:
=============================================== */ 
spline::~spline(){
	// Delete all of our control points
	controlPoint* iter;
	iter = head;
	while(iter != NULL){
		delete iter;
		iter = iter->next;
	}
}

/*	===============================================
Desc:
Precondition:
Postcondition:
=============================================== */ 
void spline::addPoint(float x, float y, float z){
	// Create a new point
	controlPoint* temp = new controlPoint();
	// Did the memory allocate?
	if(temp!=NULL){
		temp->setValues(x,y,z);
		temp->next = NULL;	// remember to set this to NULL since this is our last point in our list.
		std::cout << "Roller Coaster Route: (" << x << "," << y << "," << z << ")" << std::endl;
		points++;			// enumerate our point
	}
	else{
		std::cerr << "addPoint out of memory!" << std::endl;
	}

	// If this is our first point, then it is our head and tail
	if(head==NULL && tail==NULL){
		head = temp;
		head->next = tail;
		tail = temp;
		tail->next = NULL;
	}else
	{
		tail->next = temp;
		tail = temp;
	}
}

/*	===============================================
Desc:			Implement this function
Precondition:
Postcondition:
=============================================== */ 
void spline::cubic_spline(int resolution, int output){
	// Color each segment a different color
	

	controlPoint* iter = head;
	controlPoint* iter2 = head->next;

	glPointSize(1.0);		// Set our point size
		glDisable(GL_LIGHTING);	// Remove lighting so that the points render correctly
			glPushMatrix();
				float intervalColor = 0.0;
				glBegin(GL_POINTS);
					while(iter->next != NULL && iter2->next != NULL){
						float deltaSteps = resolution;
						float stepSize = ((iter2->x)-(iter->x)) / deltaSteps;
						// Color our intervals different colors
						
						
						for(float t = 0; t < (iter2->x)-(iter->x) ; t+=stepSize)
						{
							float yStart = iter->y;
							float yGoal = iter2->y;

							float zStart = iter->z;
							float zGoal = iter2->z;

							// See if we've reached a new interval, and update our points
							// accordingly
							if((iter->x) + t > (iter2->x)){
								yGoal = iter2->next->y;
								zGoal = iter2->next->z;
							}
								glColor3f(intervalColor,1-intervalColor,0);
								float yCoordinate = calculate_Spline(t, yStart, yGoal, 5, 5);//cubic_equation(S,G,Vs,Vg,t);
								float zCoordinate = calculate_Spline(t,zStart,zGoal,5,5);
								if(output==1){
									std::cout << (iter->x)+t << "," << yCoordinate << "," << zCoordinate << std::endl;
								}
								glVertex3f((iter->x)+t,yCoordinate,zCoordinate);
						}
								intervalColor+=.1;
								if(intervalColor>1){
									intervalColor=0.0;
								}
						iter = iter->next;
						iter2 = iter->next;
					}
				glEnd();
			glPopMatrix();
		glEnable(GL_LIGHTING);
}

/*	===============================================
Desc:	Fill this in 

	S = starting position of your interval
	G = ending(or "goal") position of your interval
	Vs = velocity your points grow from from your starting point
	Vg = velocity your point ends on from your goal position
Precondition: 
Postcondition:
=============================================== */ 
float spline::calculate_Spline (float t, float S, float G, float Vs, float Vg) {
  return t*t*t*( 2.0*S - 2.0*G + 1.0*Vs + 1.0*	Vg) +
                  t*t*(-3.0*S + 3.0*G - 2.0*Vs - 1.0*	Vg) +
                    t*(                           		Vs) +
                      (     S);
}

/*	===============================================
Desc:
Precondition:
Postcondition:
=============================================== */ 
void spline::render(){
	// Set our point sizes, these serve to highlight
	// the points that we added in our program.
	glPointSize(20.0);

	// Create an iterator to move through all of our control points
	controlPoint* iter;
	// Set the iterator to the head of our list.
	iter = head;

	// Disable lighting, so that lines and points show up.
	glDisable(GL_LIGHTING);
	// Iterate through each of the points until our iter reaches the end.
	while(iter != NULL){
		// Set the draw mode so that each vertex is drawn as a point.
		glBegin(GL_POINTS);
			// Our points will be drawn as red boxes
			glColor3f(1,1,1);
			// Color our start and stop positions different colors.
			if(iter==head){ glColor3f(0,0,1); }
			if(iter->next==NULL){ glColor3f(0,1,1); }
			// Finally draw a regular control point.
			glVertex3f(iter->x,iter->y,iter->z);
		glEnd();
		// Continue iterating through our list.
		iter = iter->next;
	}

	// Reset our iter to the head
	// The purpose of this loop is to
	// draw lines between each of the control points.
	iter = head;
	while(iter->next != NULL){
		glBegin(GL_LINES);
			glColor3f(0,1,0);
			glVertex3f(iter->x,iter->y,iter->z);
			glVertex3f(iter->next->x,iter->next->y,iter->next->z);
		glEnd();
		iter = iter->next;
	}

	// Renable lighting now that we are done drawing lines and points.
	glEnable(GL_LIGHTING);
}


void spline::renderAsRollerCoaster(int resolution, float width){
	cubic_spline(resolution,0);
	glPushMatrix();
		glTranslatef(0,0,width);
		cubic_spline(resolution,0);
	glPopMatrix();

}

