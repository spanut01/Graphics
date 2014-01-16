/*
	File Name: ply.h
	Description:
	Author:

	Purpose:
	Examples:
*/
#ifndef PLY_H
#define PLY_H

#include <string>
#include "geometry.h"
#include "entity.h"

using namespace std;

/*  ============== ply ==============
	Purpose: Load a PLY File
	Use: 
	==================================== */ 
class ply : public entity{

	public:
		/*	===============================================
			Desc: Default constructor for a ply object
				
			=============================================== */ 
		ply(string _filePath);

		/*	===============================================
			Desc: Destructor for a ply object
				
			=============================================== */ 
		~ply();
		/*	===============================================
			Desc: reloads the geometry for a 3D object
				
		=============================================== */ 
		void reload(string _filePath);

		/*	===============================================
			Desc: Draws the 3D object
				
		=============================================== */  
		void render();
		void renderWireFrame();

		/*	===============================================
			Desc: Draws the 3D object
				
		=============================================== */  
		void printAttributes();
		/*  ===============================================
			Desc: Helper function for you to debug if 
			you are reading in the correct data.
			(Generally these would not be public functions,
			they are here to help you understand the interface)
		=============================================== */
		void printVertexList();
		void printFaceList();


	private:
		/*	===============================================
			Desc: Helper function used in the constructor

			=============================================== */ 
		void loadGeometry();
		string filePath;

		int vertexCount;
		int faceCount;
		int properties;
		vertex* vertexList;
		face* faceList;

};

#endif
