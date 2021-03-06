#ifndef SHAPE_H
#define SHAPE_H

//#include <GL/glui.h>
#include <GL/glui.h>
#include "Algebra.h"


#define RAD 0.0174532925199432954743716805978692718782
#define PI 3.1415926535897932384626433832795028841971693993751058209749445923
#define DEG_TO_RAD(a)	(a*RAD)

class Shape {
public:
	Shape() {};
	~Shape() {};

	void setSegments(int x, int y) {
		m_segmentsX = x;
		m_segmentsY = y;
	}

	virtual void draw() {};
	virtual void drawNormal() {};

protected:
	void normalizeNormal (float x, float y, float z) {
		normalizeNormal (Vector(x, y, z));
	};

	void normalizeNormal (Vector& v) {
    v.normalize();
    glNormal3dv(v.unpack());
  };

void normalizeNormal (Vector v) {
    v.normalize();
    glNormal3dv(v.unpack());
  };


	int m_segmentsX, m_segmentsY;
};

#endif