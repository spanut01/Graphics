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
