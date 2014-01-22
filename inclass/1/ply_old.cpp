/*
  File Name: ply.cpp
  Description: a file to load and display an image represented in 
        .ply file format.
  Author: Sophie Panuthos, Paul Nixon   

*/

#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <cstdlib>
#include </comp/175/public_html/labs/include/GL/glui.h>
#include "ply.h"
#include "geometry.h"
#include "entity.h"
#include <cmath>

using namespace std;

ply::ply(string _filePath){
	filePath = _filePath;
	vertexList = NULL;
	faceList = NULL;
	properties = 0;	
	// Call helper function to load geometry
	loadGeometry();
}

void ply::reload(string _filePath){
	filePath = _filePath;
	// reclaim memory allocated
	delete vertexList;
	delete faceList;
	loadGeometry();
}

// Destructor
ply::~ply(){
	delete vertexList;
	delete faceList;
}

void ply::loadGeometry(){

    ifstream myfile (filePath.c_str()); // load the file
    if ( myfile.is_open()) { // if the file is accessable
        properties = -2; // set the properties because there are extras labeled
        
        string line;
        char * token_pointer; 
        char * lineCopy = new char[80]; 
        int count;
        bool reading_header = true;
        // loop for reading the header 
        while (reading_header && getline ( myfile, line)) {

            // get the first token in the line, this will determine which
            // action to take. 
            strcpy(lineCopy, line.c_str());
            token_pointer = strtok(lineCopy, " ");
            // case when the element label is spotted:
            if (strcmp(token_pointer, "element") == 0){
                token_pointer = strtok(NULL, " ");

                // When the vertex token is spotted read in the next token
                // and use it to set the vertexCount and initialize vertexList
                if (strcmp(token_pointer, "vertex") == 0){
                    token_pointer = strtok(NULL, " ");
                    vertexCount = atoi(token_pointer);
                    vertexList = new vertex[vertexCount];
                }

                // When the face label is spotted read in the next token and 
                // use it to set the faceCount and initialize faceList.
                if (strcmp(token_pointer, "face") == 0){
                    token_pointer = strtok(NULL, " ");
                    faceCount = atoi(token_pointer);
                    faceList = new face[faceCount];
                }
            }
            // if property label increment the number of properties.
            if (strcmp(token_pointer, "property") == 0) {properties++;} 
            // if end_header break the header loop and move to reading vertices.
            if (strcmp(token_pointer, "end_header") == 0) {reading_header = false; }
        }
        
        // Read in exactly vertexCount number of lines after reading the header
        // and set the appropriate vertex in the vertexList.
        for (int i = 0; i < vertexCount; i++){

            getline ( myfile, line); 
            strcpy(lineCopy, line.c_str());
            
            // depending on how many properties there are set that number of 
            // elements (x, y, z, confidence, intensity, r, g, b) (max 7) with
            // the input given
            if ( properties >= 0){
                vertexList[i].x = atof(strtok(lineCopy, " "));
            }
            if ( properties >= 1){
                vertexList[i].y = atof(strtok(NULL, " "));
            }
            if (properties >= 2){
                vertexList[i].z = atof(strtok(NULL, " "));
            }
            if (properties >= 3){
                vertexList[i].confidence = atof(strtok(NULL, " "));
            }
            if (properties >= 4){
                vertexList[i].intensity = atof(strtok(NULL, " "));
            }
            if (properties >= 5){
                vertexList[i].r = atof(strtok(NULL, " "));
            }
            if (properties >= 6) {
                vertexList[i].g = atof(strtok(NULL, " "));
            }
            if (properties >= 7) {
                vertexList[i].b = atof(strtok(NULL, " "));
            }
        } 
        
        // Read in the faces (exactly faceCount number of lines) and set the 
        // appropriate face in the faceList
        for (int i = 0; i < faceCount; i++){

            getline ( myfile, line);

            strcpy(lineCopy, line.c_str());
            count = atoi(strtok(lineCopy, " "));
            faceList[i].vertexCount = count; // number of vertices stored 
            faceList[i].vertexList = new int[count]; // initialize the vertices
            
            // set the vertices from the input, reading only the number of 
            // vertices that are specified
            for (int j = 0; j < count; j++){
                faceList[i].vertexList[j] = atoi(strtok(NULL, " "));
            }
        }
        delete(lineCopy);
    }
    // if the path is invalid, report then exit.
    else {
        cout << "cannot open file " << myfile << "\n";
        exit(1);
    }
    myfile.close();
    normalize();
};

void ply::normalize(){

    float avrg_x;
    float avrg_y;
    float avrg_z;
    float max = 0.0;
    int i; 

    for (i = 0; i < vertexCount; i++){
        
        avrg_x += vertexList[i].x;
        avrg_y += vertexList[i].y;
        avrg_z += vertexList[i].z;

        if (max < abs(vertexList[i].x)) max = abs(vertexList[i].x);
        if (max < abs(vertexList[i].y)) max = abs(vertexList[i].y);
        if (max < abs(vertexList[i].z)) max = abs(vertexList[i].z);
    }

    avrg_x = avrg_x / vertexCount;
    avrg_y = avrg_y / vertexCount;
    avrg_z = avrg_z / vertexCount;
    
    for (i = 0; i < vertexCount; i++){
        vertexList[i].x = (vertexList[i].x - avrg_x) / max;
        vertexList[i].y = (vertexList[i].y - avrg_y) / max;
        vertexList[i].z = (vertexList[i].z - avrg_z) / max;
    }
};

void ply::render(){
    	if(vertexList==NULL || faceList==NULL){
    		    return;
    	}
  		
      glPushMatrix();
            glTranslatef(getXPosition(),getYPosition(),getZPosition());
            glScalef(getXScale(),getYScale(),getZScale());
            // For each of our faces
        		for(int i = 0; i < faceCount; i++)
        		{
            			glBegin(GL_TRIANGLES);
            			// Get the vertex list from the face list
            			for(int j = 0; j < faceList[i].vertexCount; j++){
              				// Get each vertices x,y,z and draw them
              				int index = faceList[i].vertexList[j];
              				glVertex3f(vertexList[index].x,vertexList[index].y,vertexList[index].z);
            			}
            			glEnd();
        		}
      glPopMatrix();
}

void ply::renderWireFrame(){
      if(vertexList==NULL || faceList==NULL){
            return;
      }
      glPushMatrix();
            glTranslatef(getXPosition(),getYPosition(),getZPosition());
            glScalef(getXScale(),getYScale(),getZScale());
            // For each of our faces
            for(int i = 0; i < faceCount; i++)
            {
                  glBegin(GL_LINES);
                  // Get the vertex list from the face list
                  for(int j = 0; j < faceList[i].vertexCount; j++){
                      // Get each vertices x,y,z and draw them
                      int index = faceList[i].vertexList[j];
                      glVertex3f(vertexList[index].x,vertexList[index].y,vertexList[index].z);
                  }
                  glEnd();
            }
      glPopMatrix();
}

void ply::printAttributes(){
      cout << "==== ply Mesh Attributes=====" << endl;
      cout << "vertex count:" << vertexCount << endl;
      cout << "face count:" << faceCount << endl;	
      cout << "properties:" << properties << endl;
}


void ply::printVertexList(){
  	if(vertexList==NULL){
  		  return;
  	}else{
    		for(int i = 0; i < vertexCount; i++){
    			   cout << vertexList[i].x << "," << vertexList[i].y << "," << vertexList[i].z << endl;
    		}
  	}
}

void ply::printFaceList(){
	if(faceList==NULL){
		return;
	}else{
		// For each of our faces
		for(int i = 0; i < faceCount; i++){
			// Get the vertices that make up each face from the face list
			for(int j = 0; j < faceList[i].vertexCount; j++){
				// Print out the vertex
				int index = faceList[i].vertexList[j];
				cout << vertexList[index].x << "," << vertexList[index].y << "," << vertexList[index].z << endl;
			}
		}
	}
}
