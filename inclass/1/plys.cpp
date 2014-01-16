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

void ply::readHeader(){
    string line;
    char * token_pointer; 
    char * lineCopy; 
    while ( getline ( myfile, line) ) {
        lineCopy = new char [line.length()];
        strcpy(lineCopy, line.c_str());
        token_pointer = strtok(lineCopy, " ");
        cout << lineCopy << "\n";
        if (strcmp(token_pointer, "element")){
            token_pointer = strtok(NULL, " ");
            if (strcmp(token_pointer, "vertex")){
                token_pointer = strtok(NULL, " ");
                vertexCount = atoi(token_pointer);
                vertexList = new vertex[vertexCount];
            }
            if (strcmp(token_pointer, "face")){
                token_pointer = strtok(NULL, " ");
                faceCount = atoi(token_pointer);
                faceList = new face[faceCount];
            }
        }
        if (strcmp(token_pointer, "property")) {properties++;} 
        if (strcmp(token_pointer, "end_header")) {return; }
    }
}

void ply::readFaces(){

    string line; 
    char * lineCopy;
    int count;

    for (int i = 0; i < faceCount; i++){
        getline ( myfile, line);
        lineCopy = new char[line.length()];
        strcpy(lineCopy, line.c_str());
        count = atoi(strtok(lineCopy, " "));
        faceList[i].vertexCount = count;
        for (int j = 0; j < count; j++){
            faceList[i].vertexList[j] = atoi(strtok(lineCopy, " "));
        }
    }
        
}

void ply::readVertices(){

    string line;
    char * lineCopy;
    char * vertices;

    for (int i = 0; i < vertexCount; i++){
        getline ( myfile, line); 
        lineCopy = new char[line.length()];
        strcpy(lineCopy, line.c_str());

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
}

void ply::loadGeometry(){
    ifstream myfile (filePath.c_str());
    if ( myfile.is_open()) {
        properties = -2; 
        readHeader();
        readVertices();
        readFaces();
        myfile.close();
    }
    else {
        cout << "cannot open file " << myfile << "\n";
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
