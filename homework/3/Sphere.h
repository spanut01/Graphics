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

private:
    void drawTriangles();
    void drawNormals();
};

#endif
