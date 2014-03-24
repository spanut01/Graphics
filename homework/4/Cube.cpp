#include "Cube.h"

double Cube::intersect(Point eyeP, Vector rayV, Matrix transformMatrix){
    return -1.0;
}

void Cube::drawTriangles(){

  int i, j;
  float x,y;
  x = 1.0f / m_segmentsX;
  y = 1.0f / m_segmentsY;

  for(i=0; i<m_segmentsX; i++){
      for(j=0; j<m_segmentsY; j++){
          //front face (+z)
          glNormal3f(0.0f, 0.0f, 1.0f);
          glVertex3f(-0.5f+i*x, -0.5f+j*y, 0.5f);
          glVertex3f(-0.5f+i*x, -0.5f+(j+1)*y, 0.5f);
          glVertex3f(-0.5f+(i+1)*x, -0.5f+j*y, 0.5f);
          
          glVertex3f(-0.5f+i*x, -0.5f+(j+1)*y, 0.5f);
          glVertex3f(-0.5f+(i+1)*x, -0.5f+(j+1)*y, 0.5f);
          glVertex3f(-0.5f+(i+1)*x, -0.5f+j*y, 0.5f);
           
          //back face (-z)
          glNormal3f(0.0f, 0.0f, -1.0f);
          glVertex3f(-0.5f+(i+1)*x, -0.5f+j*y, -0.5f);
          glVertex3f(-0.5f+i*x, -0.5f+(j+1)*y, -0.5f);
          glVertex3f(-0.5f+i*x, -0.5f+j*y, -0.5f);
            
          glVertex3f(-0.5f+i*x, -0.5f+(j+1)*y, -0.5f);
          glVertex3f(-0.5f+(i+1)*x, -0.5f+j*y, -0.5f);
          glVertex3f(-0.5f+(i+1)*x, -0.5f+(j+1)*y, -0.5f);
            
          //right (+x) face
          glNormal3f(1.0f, 0.0f, 0.0f);
          glVertex3f(0.5f, -0.5f+j*y, -0.5f+i*x);
          glVertex3f(0.5f, -0.5f+(j+1)*y, -0.5f+i*x);
          glVertex3f(0.5f, -0.5f+j*y, -0.5f+(i+1)*x);
              
          glVertex3f(0.5f, -0.5f+(j+1)*y, -0.5f+(i+1)*x);
          glVertex3f(0.5f, -0.5f+(j+1)*y, -0.5f+i*x);
          glVertex3f(0.5f, -0.5f+j*y, -0.5f+(i+1)*x);
            
          //left (-x) face
          glNormal3f(-1.0f, 0.0f, 0.0f);
          glVertex3f(-0.5f, -0.5f+j*y, -0.5f+i*x);
          glVertex3f(-0.5f, -0.5f+(j+1)*y, -0.5f+i*x);
          glVertex3f(-0.5f, -0.5f+j*y, -0.5f+(i+1)*x);
            
          glVertex3f(-0.5f, -0.5f+(j+1)*y, -0.5f+(i+1)*x);
          glVertex3f(-0.5f, -0.5f+(j+1)*y, -0.5f+i*x);
          glVertex3f(-0.5f, -0.5f+j*y, -0.5f+(i+1)*x);
             
          //top (+y) face
          glNormal3f(0.0f, 1.0f, 0.0f);
          glVertex3f(-0.5f+j*y, 0.5f, -0.5f+i*x);
          glVertex3f(-0.5f+(j+1)*y, 0.5f, -0.5f+i*x);
          glVertex3f(-0.5f+j*y, 0.5f, -0.5f+(i+1)*x);
            
          glVertex3f(-0.5f+(j+1)*y, 0.5f, -0.5f+(i+1)*x);
          glVertex3f(-0.5f+(j+1)*y, 0.5f, -0.5f+i*x);
          glVertex3f(-0.5f+j*y, 0.5f, -0.5f+(i+1)*x);
             
          //bottom (-y) face
          glNormal3f(0.0f, -1.0f, 0.0f);
          glVertex3f(-0.5f+j*y, -0.5f, -0.5f+i*x);
          glVertex3f(-0.5f+(j+1)*y, -0.5f, -0.5f+i*x);
          glVertex3f(-0.5f+j*y, -0.5f, -0.5f+(i+1)*x);
              
          glVertex3f(-0.5f+(j+1)*y, -0.5f, -0.5f+(i+1)*x);
          glVertex3f(-0.5f+(j+1)*y, -0.5f, -0.5f+i*x);
          glVertex3f(-0.5f+j*y, -0.5f, -0.5f+(i+1)*x);
      }
  }
};

void Cube::drawNormals(){
  int i,j;
  float x,y;
  x = 1.0f / m_segmentsX;
  y = 1.0f / m_segmentsY;


  for(i=0; i<m_segmentsX; i++){
      for(j=0; j<m_segmentsY; j++){
          //front face (+z)
          glVertex3f(-0.5f+i*x, -0.5f+j*y, 0.5f);
          glVertex3f(-0.5f+i*x, -0.5f+j*y, 0.6f);
          glVertex3f(-0.5f+i*x, -0.5f+(j+1)*y, 0.5f);
          glVertex3f(-0.5f+i*x, -0.5f+(j+1)*y, 0.6f);
          glVertex3f(-0.5f+(i+1)*x, -0.5f+j*y, 0.5f);
          glVertex3f(-0.5f+(i+1)*x, -0.5f+j*y, 0.6f);
              
          //back face (-z)
          glVertex3f(-0.5f+(i+1)*x, -0.5f+j*y, -0.5f);
          glVertex3f(-0.5f+(i+1)*x, -0.5f+j*y, -0.6f);
          glVertex3f(-0.5f+i*x, -0.5f+(j+1)*y, -0.5f);
          glVertex3f(-0.5f+i*x, -0.5f+(j+1)*y, -0.6f);
          glVertex3f(-0.5f+i*x, -0.5f+j*y, -0.5f);
          glVertex3f(-0.5f+i*x, -0.5f+j*y, -0.6f);
            
          //right (+x) face
          glVertex3f(0.5f, -0.5f+j*y, -0.5f+i*x);
          glVertex3f(0.6f, -0.5f+j*y, -0.5f+i*x);
          glVertex3f(0.5f, -0.5f+(j+1)*y, -0.5f+i*x);
          glVertex3f(0.6f, -0.5f+(j+1)*y, -0.5f+i*x);
          glVertex3f(0.5f, -0.5f+j*y, -0.5f+(i+1)*x);
          glVertex3f(0.6f, -0.5f+j*y, -0.5f+(i+1)*x);
              
          //left (-x) face
          glVertex3f(-0.5f, -0.5f+j*y, -0.5f+i*x);
          glVertex3f(-0.6f, -0.5f+j*y, -0.5f+i*x);
          glVertex3f(-0.5f, -0.5f+(j+1)*y, -0.5f+i*x);
          glVertex3f(-0.6f, -0.5f+(j+1)*y, -0.5f+i*x);
          glVertex3f(-0.5f, -0.5f+j*y, -0.5f+(i+1)*x);
          glVertex3f(-0.6f, -0.5f+j*y, -0.5f+(i+1)*x);
              
          //top (+y) face
          glVertex3f(-0.5f+j*y, 0.5f, -0.5f+i*x);
          glVertex3f(-0.5f+j*y, 0.6f, -0.5f+i*x);
          glVertex3f(-0.5f+(j+1)*y, 0.5f, -0.5f+i*x);
          glVertex3f(-0.5f+(j+1)*y, 0.6f, -0.5f+i*x);
          glVertex3f(-0.5f+j*y, 0.5f, -0.5f+(i+1)*x);
          glVertex3f(-0.5f+j*y, 0.6f, -0.5f+(i+1)*x);
              
          //bottom (-y) face
          glVertex3f(-0.5f+j*y, -0.5f, -0.5f+i*x);
          glVertex3f(-0.5f+j*y, -0.6f, -0.5f+i*x);
          glVertex3f(-0.5f+(j+1)*y, -0.5f, -0.5f+i*x);
          glVertex3f(-0.5f+(j+1)*y, -0.6f, -0.5f+i*x);
          glVertex3f(-0.5f+j*y, -0.5f, -0.5f+(i+1)*x);
          glVertex3f(-0.5f+j*y, -0.6f, -0.5f+(i+1)*x);
      }
  }
};
