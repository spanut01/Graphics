#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"

class Sphere : public Shape {
public:
	Sphere() {};
	~Sphere() {};

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

    //there's got to be a better way to make this work
    double Intersect(Point eyePointP, Vector rayV, Matrix transformMatrix){
        return intersect(eyePointP, rayV, transformMatrix);
    };

private:
    void drawTriangles();
    void drawNormals();
    double intersect(Point eyePointP, Vector rayV, Matrix transformMatrix);
};

#endif
