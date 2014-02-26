#ifndef COHE_H
#define CONE_H

#include "Shape.h"

#define NORMAL_Y 0.447213595499958f

class Cone : public Shape {
public:
	Cone() {};
	~Cone() {};

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
