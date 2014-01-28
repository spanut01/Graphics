#ifndef FUNSHAPE_H
#define FUNSHAPE_H

#include "Shape.h"

class FunShape : public Shape {
public:
	FunShape() {};
	~FunShape() {};

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
