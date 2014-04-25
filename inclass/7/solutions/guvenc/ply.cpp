/*  =================== File Information =================
  File Name: ply.cpp
  Description: Loads a .ply file and renders it on screen.  
        New to this version: also renders the silhouette!
  Author: Paul Nixon
  ===================================================== */
#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <cstdlib>
#include <GL/glui.h>
#include "ply.h"
#include "geometry.h"
#include <math.h>


using namespace std;

/*  ===============================================
      Desc: Default constructor for a ply object
      Precondition: _filePath is set to a valid filesystem location
            which contains a valid .ply file (triangles only)
      Postcondition: vertexList, faceList are filled in
    =============================================== */ 
ply::ply(string _filePath){
        filePath = _filePath;
        vertexList = NULL;
        faceList = NULL;
        edgeList = NULL;
        properties = 0; 
        // Call helper function to load geometry
        loadGeometry();
}

/*  ===============================================
      Desc: Destructor for a ply object
      Precondition: Memory has been already allocated
      =============================================== */ 
ply::~ply(){
  deconstruct();
}
void ply::deconstruct(){
  int i;
  // Delete the allocated arrays
  delete[] vertexList;

  for (i = 0; i < faceCount; i++) {
          delete [] faceList[i].vertexList;
  }

  delete[] faceList;
  
  struct edge* iter;
  struct edge* next;
  for(i=0; i<vertexCount; i++){
      iter = edgeList[i];
      while(iter!=NULL){
          next = iter->next;
          delete iter;
          iter = next;
      }
  }
  delete[] edgeList;
   //Set pointers to NULL
   vertexList = NULL;
   faceList = NULL;
   edgeList = NULL;
}

/*  ===============================================
      Desc: reloads the geometry for a 3D object
            (or loads a different file)
    =============================================== */ 
void ply::reload(string _filePath){
  
  filePath = _filePath;
  deconstruct();
  // Call our function again to load new vertex and face information.
  loadGeometry();
}
/*  ===============================================
      Desc: Loads the data structures (look at geometry.h and ply.h)
      Precondition: filePath is something valid, arrays are NULL
      Postcondition: data structures are filled 
          (including edgeList, this calls scaleAndCenter and findEdges)
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
    findEdges();
    scaleAndCenter();
};

/*  ===============================================
Desc: Moves all the geometry so that the object is centered at 0, 0, 0 and scaled to be between 0.5 and -0.5
Precondition: after all the vetices and faces have been loaded in
Postcondition: points have reasonable values
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
 
    // *******multiply the max by 2.5 to shrink the image to fit it into the 
    // given window dimensions. *******

    // center and scale each vertex 
    for (i = 0; i < vertexCount; i++){
        vertexList[i].x = (vertexList[i].x - avrg_x) / max;
        vertexList[i].y = (vertexList[i].y - avrg_y) / max;
        vertexList[i].z = (vertexList[i].z - avrg_z) / max;
    }
}

/*  ===============================================
      Desc: Draws a filled 3D object
      Precondition: arrays are EITHER valid data OR NULL
      Postcondition: no changes to data
      Error Condition: If we haven't allocated memory for our
      faceList or vertexList then do not attempt to render.
    =============================================== */  
void ply::render(){
    if(vertexList==NULL || faceList==NULL){
                return;
    }

        glPushMatrix();
    glTranslatef(getXPosition(),getYPosition(),getZPosition());
    glScalef(getXScale(),getYScale(),getZScale());
    // For each of our faces
    glBegin(GL_TRIANGLES);
          for(int i = 0; i < faceCount; i++) {
            // All of our faces are actually triangles for PLY files
                        // Get the vertex list from the face list
                        int index0 = faceList[i].vertexList[0];
                        int index1 = faceList[i].vertexList[1];
                        int index2 = faceList[i].vertexList[2];

                        setNormal(i, vertexList[index0].x, vertexList[index0].y, vertexList[index0].z,
                                          vertexList[index1].x, vertexList[index1].y, vertexList[index1].z,
                                          vertexList[index2].x, vertexList[index2].y, vertexList[index2].z);

            for(int j = 0; j < faceList[i].vertexCount; j++){
                                // Get each vertices x,y,z and draw them
                int index = faceList[i].vertexList[j];
                glVertex3f(vertexList[index].x,vertexList[index].y,vertexList[index].z);
            }
        }
        glEnd();        
        glPopMatrix();
}


//loads data structures so edges are known
void ply::findEdges(){
    //printf("A");
    //edges, if you want to use this data structure
    //edgeList = new struct edge*[vertexCount];
    edgeList = new edge*[vertexCount];
    //int vertexMat[vertexCount + 10][vertexCount + 10];
    int* vertexMat  = new int[vertexCount * vertexCount];
    //printf("A");

    for(int k = 0; k < faceCount; k++){
      face f = faceList[k];
      int v1 = f.vertexList[0];
      int v2 = f.vertexList[1];
      int v3 = f.vertexList[2];
      //printf("%d\n %d\n %d\n ",v1, v2, v3);

      if (vertexMat[v1 * vertexCount + v2] == 0){
        vertexMat[v1 * vertexCount + v2] = k + 1;
      }
      else{
        vertexMat[v2 * vertexCount + v1] = k + 1;
      }

      if (vertexMat[v2 * vertexCount + v3] == 0){
        vertexMat[v2 * vertexCount + v3] = k + 1;
      }
      else{
        vertexMat[v3 * vertexCount + v2] = k + 1;
      }

      if (vertexMat[v1 * vertexCount + v3] == 0){
        vertexMat[v1 * vertexCount + v3]  = k + 1;
      }
      else{
        vertexMat[v3 * vertexCount + v1] = k + 1;
      }

    }
    
    //printf("B");

    for(int i = 0; i < vertexCount; i++){
      edge* edgeIHead = edgeList[i];
      for(int k = 0; k <vertexCount; k++){
        edge* newEdge = new edge();
        if ((vertexMat[i * vertexCount + k] != 0) && (vertexMat[k * vertexCount + i] != 0)) {
            int f1 = (vertexMat[i * vertexCount + k]) - 1;
            int f2 = (vertexMat[k * vertexCount + i]) - 1;
            newEdge->vertexes[0] = i;
            newEdge->vertexes[1] = k;
            newEdge->faces[0]    = f1;
            newEdge->faces[1]    = f2;
            newEdge->next        = NULL;
            if (edgeIHead == NULL){
              edgeIHead = newEdge;
              edgeList[i] = edgeIHead;
            }
            else{
              edgeIHead->next = newEdge;
              edgeIHead      = newEdge;
            }
        }
      }

    }

    delete[] vertexMat;
    //TODO add all the edges to the edgeList and make sure they have both faces
}

/* Desc: Renders the silhouette
 * Precondition: Edges are known
 */
void ply::renderSilhouette(){
    glPushMatrix();
    glBegin(GL_LINES);
    
    int i;
    for(i=0; i<faceCount; i++){
        faceList[i].dotProd = (faceList[i].normX * lookX) + (faceList[i].normZ * lookZ);
    }

    for(int i = 0; i < vertexCount; i++){
      edge* edgeI = edgeList[i];
      edge* movingEdge = edgeI;
      while (movingEdge != NULL){
        int f1Ind = movingEdge->faces[0];
        int f2Ind = movingEdge->faces[1];
        face f1 = faceList[f1Ind];
        face f2 = faceList[f2Ind];
        int v1Ind = movingEdge->vertexes[0];
        int v2Ind = movingEdge->vertexes[1];
        vertex v1 = vertexList[v1Ind];
        vertex v2 = vertexList[v2Ind];
        if ((f1.dotProd * f2.dotProd) < 0){
          glVertex3f(v1.x, v1.y, v1.z);
          glVertex3f(v2.x, v2.y, v2.z);
        }
        movingEdge = movingEdge->next;
      }
    }
    
    //TODO Iterate through the edgeList, and if you want to draw an edge,
    //call glVertex3f once for each vertex in that edge.  
    glEnd();
    glPopMatrix();
} 

/*  ===============================================
      Desc: Prints some statistics about the file you have read in
      This is useful for debugging information to see if we parse our file correctly.
    =============================================== */ 
void ply::printAttributes(){
      cout << "==== ply Mesh Attributes=====" << endl;
      cout << "vertex count:" << vertexCount << endl;
      cout << "face count:" << faceCount << endl;       
      cout << "properties:" << properties << endl;
}

/*  ===============================================
      Desc: Iterate through our array and print out each vertex.
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

//makes a GL Call to set the normal but also stores it 
//so it can be accessible for silhouette finding
void ply::setNormal(int facenum, float x1, float y1, float z1,
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

        faceList[facenum].normX = cx;
        faceList[facenum].normY = cy;
        faceList[facenum].normZ = cz;
        
        glNormal3f(cx, cy, cz);
}
