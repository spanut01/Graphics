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
    
    double Intersect(Point eyePointP, Vector rayV, Matrix transformMatrix);
    Vector findIsectNormal(Point eyePoint, Vector ray, double dist);
    Point iPointToSquare(Point i, Vector ray, double dist); 
    
private:
    void drawTriangles();
    void drawNormals();
};
#endif
