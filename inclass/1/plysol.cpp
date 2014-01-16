/*
  File Name: ply.cpp
  Description:
  Author:

  Purpose:
  Examples:
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

	// Parse header 	
  	ifstream myfile (filePath.c_str());
  	if (myfile.is_open())
  	{
  		string line;
  		bool readVertices = false;	// start reading data into our vertex list
  		bool readFaces = false;	// start reading data into our face list
  		int verticesRead = 0;		// How many vertices we have read
  		int facesRead = 0;			// How many faces have we read
  		properties = -2;			// Starts at -1 because it appears 1 additional time in the file
  		// Read in an entire line
    	while (getline (myfile,line) )
    	{
      		// Split the line into space separated tokens
      		char* delimeter_pointer;
      		char* copy = new char[line.length()+1];
      		strcpy(copy,line.c_str());
      		delimeter_pointer = strtok(copy," ");

      		while(delimeter_pointer != NULL){
            			// ====== Parsing Header information ======
            			if(strcmp(delimeter_pointer,"vertex") == 0){	// Set the vertex count
                				delimeter_pointer = strtok(NULL," ");
                				vertexCount = atoi(delimeter_pointer);
                				// Allocate memory for our vertices
                				vertexList = new vertex[vertexCount];
                				if(vertexList==NULL){
                					cout << "Cannot allocate memory for Vertex List: System out of memory" << endl;
                					exit(1);
                				}
            			}
             			else if(strcmp(delimeter_pointer,"face") == 0){	// Set the face count
                				delimeter_pointer = strtok(NULL," ");
                				faceCount = atoi(delimeter_pointer);
                				// Allocate memory for our faces
                				faceList = new face[faceCount];
                				if(faceList==NULL){
                					cout << "Cannot allocate memory for FaceList: System out of memory" << endl;
                					exit(1);
                				}
            			}
             			else if(strcmp(delimeter_pointer,"property") == 0){	// Count the properties
            				    properties++;
            			}
            			// ====== Parsing Header information ======

            			// Switch state so we read in faces
            			if(verticesRead >= vertexCount && readFaces==false && readVertices==true){
                				readVertices=false;
                				readFaces = true;
            			}
            			//
            			if(readVertices==true && verticesRead < vertexCount){
                				if(properties >= 0){  // Read in the x vertex
          	      				   vertexList[verticesRead].x = atof(delimeter_pointer);
          					     }
              					if(properties >= 1){   // Read in the y vertex
              						    delimeter_pointer = strtok(NULL," ");
              	      				vertexList[verticesRead].y = atof(delimeter_pointer);
              					}
                				if(properties >= 2){  // Read in the z vertex
              	      				delimeter_pointer = strtok(NULL," ");
              	      				vertexList[verticesRead].z = atof(delimeter_pointer);
              					}
              					if(properties >= 3){   //
              						    delimeter_pointer = strtok(NULL," ");
              	      				vertexList[verticesRead].confidence = atof(delimeter_pointer);
              					}
              					if(properties >= 4){
              						    delimeter_pointer = strtok(NULL," ");
                    					vertexList[verticesRead].intensity = atof(delimeter_pointer);
                    	    	}
                                if(properties >= 5){
                                    delimeter_pointer = strtok(NULL," ");
                                    vertexList[verticesRead].r = atof(delimeter_pointer);
                                }
                                if(properties >= 6){
                                      delimeter_pointer = strtok(NULL," ");
                                      vertexList[verticesRead].g = atof(delimeter_pointer);
                                }
                                if(properties >=7){
                                      delimeter_pointer = strtok(NULL," ");
                                      vertexList[verticesRead].b = atof(delimeter_pointer);
                                }
                				verticesRead++;
                        }
            			else if(readFaces==true && facesRead < faceCount){
                  				// first number is how many faces to read in
                  				faceList[facesRead].vertexCount = atoi(delimeter_pointer);
                  				// Allocate the vertex list with memory
                  				faceList[facesRead].vertexList = new int[facesRead];
                  				// Then we store the vertices until our delimeter is null
                  				int count = 0;
                  				while(count < faceList[facesRead].vertexCount){
                            					delimeter_pointer = strtok(NULL," ");
                            					faceList[facesRead].vertexList[count] = atoi(delimeter_pointer);
                            					count++;
                  				}
                  				facesRead++;
        			     }
                			// Switch state so we immedietely start reading in vertices the next iteration
                			if(strcmp(delimeter_pointer,"end_header")==0){
                        cout << "found end_header" << endl;
                				readVertices = true;
                			}

                			if(delimeter_pointer != NULL){
                				delimeter_pointer = strtok(NULL," ");
                			}
            }
    	}
    	myfile.close();	

  	}
	else{
		    cout << "Unable to open file: " << filePath << endl; 
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
