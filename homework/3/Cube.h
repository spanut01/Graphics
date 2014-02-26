#ifndef CUBE_H
#define CUBE_H

#include "Shape.h"

class Cube : public Shape {
public:
	Cube() {};
	~Cube() {};

	void draw() {
      glPushMatrix();
      glBegin(GL_TRIANGLES);
      drawTriangles(); 
      
      glEnd();
      glPopMatrix();
	};


    void drawNormal() {
      glPushMatrix();
      glBegin(GL_LINES);
      drawNormals();
      glEnd();
      glPopMatrix();
	};

private:
    void drawTriangles();
    void drawNormals();
};

#endif
