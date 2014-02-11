#ifndef GEOMETRY_H
#define GEOMETRY_H

/*  ============== Vertex ==============
	Purpose: Stores properties of each vertex
	Use: Used in face structure
	==================================== */  
struct vertex{
	float x,y,z;		// position in 3D space
	float confidence;
	float intensity;
	float r,g,b;		// Color values
	
	int faces[3];
};

/*  ============== Face ==============
	Purpose: Store list of vertices that make up a polygon.
			In modern versions of OpenGL this value will always be 3(a triangle)
	Use: Used in Shape data structure.
	==================================== */  
struct face{
	int vertexCount;
	int* vertexList;
  
  int neighbors[3];
	// Default constructor
	face(){
		vertexCount = 0;
 /*   neighbors[0] = -1;
    neighbors[1] = -1;
    neighbors[2] = -1; */
	}

};

#endif