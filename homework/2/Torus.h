#ifndef TORUS_H
#define TORUS_H

#include "Shape.h"
#include "Algebra.h"

class Torus : public Shape {
public:
	Torus() {};
	~Torus() {};

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
