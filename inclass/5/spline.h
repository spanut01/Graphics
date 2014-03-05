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
#ifndef SPLINE_H
#define SPLINE_H

#include "controlPoint.h"


/*
	
*/
class spline{
	public:
		/*	===============================================
		Desc:
		Precondition:
		Postcondition:
		=============================================== */ 
		spline();
		/*	===============================================
		Desc:
		Precondition:
		Postcondition:
		=============================================== */ 
		~spline();

		/*	===============================================
		Desc:
		Precondition:
		Postcondition:
		=============================================== */ 
		void addPoint(float x, float y, float z);
		/*	===============================================
		Desc: Called once to calculate a table of values once we have
			added all of our points.
		Precondition: 	resolution(in my implementation), specifies how many points to draw between
						each control point.

						output is used as a flag to output the points along the spline to the console.
						A value of 1 will output each interpolated point, while a value of 0 will output
						nothing to standard IO.
						You may remove this is parameter if you think it is unnecessary.
		Postcondition:
		=============================================== */ 
		void cubic_spline(int resolution, int output);
		/*	===============================================
		Desc: Function for returning an interpolated point between and sPos(starting position) and
		an gPos (goal/ending) position
		Precondition:
		Postcondition:
		=============================================== */ 
		float calculate_Spline (float t, float S, float G, float Vs, float Vg);
		/*	===============================================
		Desc:
		Precondition:
		Postcondition:
		=============================================== */ 
		void render();
		/*	===============================================
		Desc:
		Precondition:
		Postcondition:
		=============================================== */ 
		void renderAsRollerCoaster(int resolution, float width);
	private:
		// Each frame is a time step
		int frames;
		// Number of points
		int points;

		// Linked list of points
		ControlPoint* head;
		ControlPoint* tail;

};

#endif
