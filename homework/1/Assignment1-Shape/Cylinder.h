#ifndef CYLINDER_H
#define CYLINDER_H

#include "Shape.h"

class Cylinder : public Shape {
public:
	Cylinder() {};
	~Cylinder() {};

	void draw() {
      glPushMatrix();
      glBegin(GL_TRIANGLES);
      int i,j;
      
      for(i=0; i<m_segmentsX; i++){
          //top
          glNormal3f(0.0f, 1.0f, 0.0f);
          glVertex3f(0.0f, 0.5f, 0.0f);
          glVertex3f(0.5f*cosf(i*PI*(2.0f/m_segmentsX)), 0.5f, 0.5f*sinf(i*PI*(2.0f/m_segmentsX)));
          glVertex3f(0.5f*cosf((i+1)*PI*(2.0f/m_segmentsX)), 0.5f, 0.5f*sinf((i+1)*PI*(2.0f/m_segmentsX)));
          
          //bottom
          glNormal3f(0.0f, -1.0f, 0.0f);
          glVertex3f(0.0f, -0.5f, 0.0f);
          glVertex3f(0.5f*cosf(i*PI*(2.0f/m_segmentsX)), -0.5f, 0.5f*sinf(i*PI*(2.0f/m_segmentsX)));
          glVertex3f(0.5f*cosf((i+1)*PI*(2.0f/m_segmentsX)), -0.5f, 0.5f*sinf((i+1)*PI*(2.0f/m_segmentsX)));
          
          for(j=0; j<m_segmentsY; j++){
              //sides
              glNormal3f(cosf((i+0.5f)*PI*(2.0f/m_segmentsX)), 0.0f, sinf((i+0.5f)*PI*(2.0f/m_segmentsX)));
              glVertex3f(0.5f*cosf(i*PI*(2.0f/m_segmentsX)), -0.5f+j*(1.0f/m_segmentsY), 0.5f*sinf(i*PI*(2.0f/m_segmentsX)));
              glVertex3f(0.5f*cosf((i+1.0f)*PI*(2.0f/m_segmentsX)), -0.5f+j*(1.0f/m_segmentsY), 0.5f*sinf((i+1.0f)*PI*(2.0f/m_segmentsX)));
              glVertex3f(0.5f*cosf((i+1.0f)*PI*(2.0f/m_segmentsX)), -0.5f+(j+1)*(1.0f/m_segmentsY), 0.5f*sinf((i+1.0f)*PI*(2.0f/m_segmentsX)));
              
              glVertex3f(0.5f*cosf(i*PI*(2.0f/m_segmentsX)), -0.5f+(j+1)*(1.0f/m_segmentsY), 0.5f*sinf(i*PI*(2.0f/m_segmentsX)));
              glVertex3f(0.5f*cosf(i*PI*(2.0f/m_segmentsX)), -0.5f+j*(1.0f/m_segmentsY), 0.5f*sinf(i*PI*(2.0f/m_segmentsX)));
              glVertex3f(0.5f*cosf((i+1.0f)*PI*(2.0f/m_segmentsX)), -0.5f+(j+1)*(1.0f/m_segmentsY), 0.5f*sinf((i+1.0f)*PI*(2.0f/m_segmentsX)));
          }
      }
      
      glEnd();
      glPopMatrix();
	};

	void drawNormal() {
      glPushMatrix();
      glBegin(GL_LINES);
      int i,j;
      
      for(i=0; i<m_segmentsX; i++){
          //top
          glVertex3f(0.0f, 0.5f, 0.0f);
          glVertex3f(0.0f, 0.6f, 0.0f);
          glVertex3f(0.5f*cosf(i*PI*(2.0f/m_segmentsX)), 0.5f, 0.5f*sinf(i*PI*(2.0f/m_segmentsX)));
          glVertex3f(0.5f*cosf(i*PI*(2.0f/m_segmentsX)), 0.6f, 0.5f*sinf(i*PI*(2.0f/m_segmentsX)));
          glVertex3f(0.5f*cosf((i+1)*PI*(2.0f/m_segmentsX)), 0.5f, 0.5f*sinf((i+1)*PI*(2.0f/m_segmentsX)));
          glVertex3f(0.5f*cosf((i+1)*PI*(2.0f/m_segmentsX)), 0.6f, 0.5f*sinf((i+1)*PI*(2.0f/m_segmentsX)));
          
          //bottom
          glVertex3f(0.0f, -0.5f, 0.0f);
          glVertex3f(0.0f, -0.6f, 0.0f);
          glVertex3f(0.5f*cosf(i*PI*(2.0f/m_segmentsX)), -0.5f, 0.5f*sinf(i*PI*(2.0f/m_segmentsX)));
          glVertex3f(0.5f*cosf(i*PI*(2.0f/m_segmentsX)), -0.6f, 0.5f*sinf(i*PI*(2.0f/m_segmentsX)));
          glVertex3f(0.5f*cosf((i+1)*PI*(2.0f/m_segmentsX)), -0.5f, 0.5f*sinf((i+1)*PI*(2.0f/m_segmentsX)));
          glVertex3f(0.5f*cosf((i+1)*PI*(2.0f/m_segmentsX)), -0.6f, 0.5f*sinf((i+1)*PI*(2.0f/m_segmentsX)));
          
          for(j=0; j<m_segmentsY; j++){
              //sides
              glVertex3f(0.5f*cosf(i*PI*(2.0f/m_segmentsX)), -0.5f+j*(1.0f/m_segmentsY), 0.5f*sinf(i*PI*(2.0f/m_segmentsX)));
              glVertex3f(0.6f*cosf(i*PI*(2.0f/m_segmentsX)), -0.5f+j*(1.0f/m_segmentsY), 0.6f*sinf(i*PI*(2.0f/m_segmentsX)));
              glVertex3f(0.5f*cosf((i+1.0f)*PI*(2.0f/m_segmentsX)), -0.5f+j*(1.0f/m_segmentsY), 0.5f*sinf((i+1.0f)*PI*(2.0f/m_segmentsX)));
              glVertex3f(0.6f*cosf((i+1.0f)*PI*(2.0f/m_segmentsX)), -0.5f+j*(1.0f/m_segmentsY), 0.6f*sinf((i+1.0f)*PI*(2.0f/m_segmentsX)));
              glVertex3f(0.5f*cosf((i+1.0f)*PI*(2.0f/m_segmentsX)), -0.5f+(j+1)*(1.0f/m_segmentsY), 0.5f*sinf((i+1.0f)*PI*(2.0f/m_segmentsX)));
              glVertex3f(0.6f*cosf((i+1.0f)*PI*(2.0f/m_segmentsX)), -0.5f+(j+1)*(1.0f/m_segmentsY), 0.6f*sinf((i+1.0f)*PI*(2.0f/m_segmentsX)));
              
              glVertex3f(0.5f*cosf(i*PI*(2.0f/m_segmentsX)), -0.5f+(j+1)*(1.0f/m_segmentsY), 0.5f*sinf(i*PI*(2.0f/m_segmentsX)));
              glVertex3f(0.6f*cosf(i*PI*(2.0f/m_segmentsX)), -0.5f+(j+1)*(1.0f/m_segmentsY), 0.6f*sinf(i*PI*(2.0f/m_segmentsX)));
              glVertex3f(0.5f*cosf(i*PI*(2.0f/m_segmentsX)), -0.5f+j*(1.0f/m_segmentsY), 0.5f*sinf(i*PI*(2.0f/m_segmentsX)));
              glVertex3f(0.6f*cosf(i*PI*(2.0f/m_segmentsX)), -0.5f+j*(1.0f/m_segmentsY), 0.6f*sinf(i*PI*(2.0f/m_segmentsX)));
              glVertex3f(0.5f*cosf((i+1.0f)*PI*(2.0f/m_segmentsX)), -0.5f+(j+1)*(1.0f/m_segmentsY), 0.5f*sinf((i+1.0f)*PI*(2.0f/m_segmentsX)));
              glVertex3f(0.6f*cosf((i+1.0f)*PI*(2.0f/m_segmentsX)), -0.5f+(j+1)*(1.0f/m_segmentsY), 0.6f*sinf((i+1.0f)*PI*(2.0f/m_segmentsX)));
          }
      }
      
      glEnd();
      glPopMatrix();
	};

};
#endif
