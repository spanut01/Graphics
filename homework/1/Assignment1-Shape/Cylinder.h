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
      int i,j,k;
      
      for(i=0; i<m_segmentsX; i++){
          //top
          glNormal3f(0.0f, 1.0f, 0.0f);
          glNormal3f(0.0f, 0.5f, 0.0f);
          glVertex3f(0.5f*cosf(i*PI*(2.0f/m_segmentsX)), 0.5f, 0.5f*sinf(j*PI*(2.0f/m_segmentsY)));
          glVertex3f(0.5f*cosf((i+1)*PI*(2.0f/m_segmentsX)), 0.5f, 0.5f*sinf((j+1)*PI*(2.0f/m_segmentsY)));
          
          for(j=0; j<m_segmentsY; j++){
          }
      }
      
      glEnd();
      glPopMatrix();
	};

	void drawNormal() {
	};

};
#endif
