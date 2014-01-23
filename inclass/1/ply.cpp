/*  =================== File Information =================
  File Name: ply.cpp
  Description:
  Author: (You)

  Purpose:
  Examples:
  ===================================================== */
#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <cstdlib>
#include </comp/175/public_html/labs/include/GL/glui.h>
#include "ply.h"
#include "geometry.h"
#include <math.h>


using namespace std;

/*  ===============================================
      Desc: Default constructor for a ply object
      Precondition:
      Postcondition:
    =============================================== */ 
ply::ply(string _filePath){
        filePath = _filePath;
        vertexList = NULL;
        faceList = NULL;
        properties = 0; 
        // Call helper function to load geometry
        loadGeometry();
}

/*  ===============================================
      Desc: Destructor for a ply object
      Precondition: Memory has been already allocated
      Postcondition:  
      =============================================== */ 
ply::~ply(){
  // Delete the allocated arrays
        delete[] vertexList;

        for (int i = 0; i < faceCount; i++) {
                delete [] faceList[i].vertexList;
        }

        delete[] faceList;
  // Set pointers to NULL
  vertexList = NULL;
  faceList = NULL;
}

/*  ===============================================
      Desc: reloads the geometry for a 3D object
      Precondition:
      Postcondition:
    =============================================== */ 
void ply::reload(string _filePath){
  filePath = _filePath;
  // reclaim memory allocated in each array
  delete[] vertexList;
  for (int i = 0; i < faceCount; i++) {
          delete[] faceList[i].vertexList;
  }
  delete[] faceList;
  // Set pointers to array
  vertexList = NULL;
  faceList = NULL;
  // Call our function again to load new vertex and face information.
  loadGeometry();
}
/*  ===============================================
      Desc: You get to implement this
      Precondition:
      Postcondition:  
      =============================================== */ 
void ply::loadGeometry(){

  /* You will implement this section of code
        1. Parse the header
        2.) Update any private or helper variables in the ply.h private section
        3.) allocate memory for the vertexList 
            3a.) Populate vertices
        4.) allocate memory for the faceList
            4a.) Populate faceList
  */


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
    scaleAndCenter();
};

/*  ===============================================
Desc: Moves all the geometry so that the object is centered at 0, 0, 0 and scaled to be between 0.5 and -0.5
Precondition: after all the vetices and faces have been loaded in
Postcondition:
=============================================== */
void ply::scaleAndCenter() {
    float avrg_x = 0.0;
    float avrg_y = 0.0;
    float avrg_z = 0.0;
    float max = 0.0;
    int i; 

    //loop through each vertex in the given image
    for (i = 0; i < vertexCount; i++){
        
        // obtain the total for each property of the vertex
        avrg_x += vertexList[i].x;
        avrg_y += vertexList[i].y;
        avrg_z += vertexList[i].z;

        // obtain the max dimension to find the furthest point from 0,0
        if (max < fabs(vertexList[i].x)) max = fabs(vertexList[i].x);
        if (max < fabs(vertexList[i].y)) max = fabs(vertexList[i].y);
        if (max < fabs(vertexList[i].z)) max = fabs(vertexList[i].z);
    }
    // compute the average for each property
    avrg_x = avrg_x / vertexCount;
    avrg_y = avrg_y / vertexCount;
    avrg_z = avrg_z / vertexCount;
    printf("max : %f \naverages: \n x: %f \n y: %f \n z: %f \n" , max, avrg_x, avrg_y, avrg_z);
 
    // *******multiply the max by 2.5 to shrink the image to fit it into the 
    // given window dimensions. *******
//    max = max * 2.5; 

    // center and scale each vertex 
    for (i = 0; i < vertexCount; i++){
        vertexList[i].x = (vertexList[i].x - avrg_x) / max;
        vertexList[i].y = (vertexList[i].y - avrg_y) / max;
        vertexList[i].z = (vertexList[i].z - avrg_z) / max;
    }
}

/*  ===============================================
      Desc: Draws a filled 3D object
      Precondition:
      Postcondition:
      Error Condition: If we haven't allocated memory for our
      faceList or vertexList then do not attempt to render.
    =============================================== */  
void ply::render(){
    if(vertexList==NULL || faceList==NULL){
                return;
    }

/*        float x1, y1, z1;
        float x2, y2, z2;
        float x3, y3, z3;

        z1 = z2 = z3 = 0;
        x1 = -0.5;
        x2 = 0.5;
        x3 = 0;
        y1 = -0.5;
        y2 = -0.5;
        y3 = 0.5;

        glBegin(GL_TRIANGLES);
        setNormal(x1, y1, z1, x2, y2, z2, x3, y3, z3);
        glVertex3f(x1, y1, z1);
        glVertex3f(x2, y2, z2);
        glVertex3f(x3, y3, z3);
        glEnd();
*/
        glPushMatrix();
    //glTranslatef(getXPosition(),getYPosition(),getZPosition());
    //glScalef(getXScale(),getYScale(),getZScale());
    // For each of our faces
        for(int i = 0; i < faceCount; i++) {
            // All of our faces are actually triangles for PLY files
                        glBegin(GL_TRIANGLES);
                        // Get the vertex list from the face list
                        int index0 = faceList[i].vertexList[0];
                        int index1 = faceList[i].vertexList[1];
                        int index2 = faceList[i].vertexList[2];

                        setNormal(vertexList[index0].x, vertexList[index0].y, vertexList[index0].z,
                                          vertexList[index1].x, vertexList[index1].y, vertexList[index1].z,
                                          vertexList[index2].x, vertexList[index2].y, vertexList[index2].z);

            for(int j = 0; j < faceList[i].vertexCount; j++){
                                // Get each vertices x,y,z and draw them
                int index = faceList[i].vertexList[j];
                glVertex3f(vertexList[index].x,vertexList[index].y,vertexList[index].z);
            }
            glEnd();
        }
        glPopMatrix();
}

/*  ===============================================
      Desc: Prints some statistics about the file you have read in
      This is useful for debugging information to see if we parse our file correctly.

      Precondition:
      Postcondition:  
    =============================================== */ 
void ply::printAttributes(){
      cout << "==== ply Mesh Attributes=====" << endl;
      cout << "vertex count:" << vertexCount << endl;
      cout << "face count:" << faceCount << endl;       
      cout << "properties:" << properties << endl;
}

/*  ===============================================
      Desc: Iterate through our array and print out each vertex.

      Precondition:
      Postcondition:  
    =============================================== */ 
void ply::printVertexList(){
        if(vertexList==NULL){
                  return;
        }else{
                for(int i = 0; i < vertexCount; i++){
                           cout << vertexList[i].x << "," << vertexList[i].y << "," << vertexList[i].z << endl;
                }
        }
}

/*  ===============================================
      Desc: Iterate through our array and print out each face.

      Precondition:
      Postcondition:  
    =============================================== */ 
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

void ply::setNormal(float x1, float y1, float z1,
                                        float x2, float y2, float z2,
                                        float x3, float y3, float z3) {
        
        float v1x, v1y, v1z;
        float v2x, v2y, v2z;
        float cx, cy, cz;

        //find vector between x2 and x1
        v1x = x1 - x2;
        v1y = y1 - y2;
        v1z = z1 - z2;

        //find vector between x3 and x2
        v2x = x2 - x3;
        v2y = y2 - y3;
        v2z = z2 - z3;

        //cross product v1xv2

        cx = v1y * v2z - v1z * v2y;
        cy = v1z * v2x - v1x * v2z;
        cz = v1x * v2y - v1y * v2x;

        //normalize

        float length = sqrt(cx * cx + cy * cy + cz * cz);
        cx = cx / length;
        cy = cy / length;
        cz = cz / length;       

        glNormal3f(cx, cy, cz);
}
