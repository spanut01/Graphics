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

    double Intersect(Point eyePointP, Vector rayV, Matrix transformMatrix){
        return intersect(eyePointP, rayV, transformMatrix);
    };

private:
    void drawTriangles();
    void drawNormals();
    double intersect(Point eyePointP, Vector rayV, Matrix transformMatrix);
};

#endif
