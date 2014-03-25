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
