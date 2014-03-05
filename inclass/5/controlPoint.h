/*  =================== File Information =================
	File Name: ControlPoint.h
	Description:
	Author: Michael Shah
	Last Modified: 3/04/14

	Purpose: A 3D point in space that can be used with the spline class
	Usage:	

	Further Reading resources: 
	N/A
	===================================================== */
#ifndef CONTROL_POINT_H
#define CONTROL_POINT_H

/*	Struct
	Desc: A point in 3D space
	Purpose: 
	Usage:
*/
struct ControlPoint{
	// Public members
	float x,y,z;
	ControlPoint* next;

	/*	===============================================
	Desc: Default constructor.
	Precondition:
	Postcondition:
	=============================================== */ 
	ControlPoint(){
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		next = NULL;
	}

	/*	===============================================
	Desc: Simple setter function
	Precondition:
	Postcondition:
	=============================================== */ 
	void setValues(float _x,float _y, float _z){
		x = _x;
		y = _y;
		z = _z;
	}
};


#endif
