#ifndef CYLINDER_H
#define CYLINDER_H

#include "Shape.h"

class Cylinder : public Shape {
public:
	Cylinder() {};
	~Cylinder() {};

	void draw() {
		glPushMatrix();
		glTranslatef(0, 0.5, 0);
		drawCap();
		glPopMatrix();

		drawBody();

		glPushMatrix();
		glTranslatef(0, -0.5, 0);
		drawCap();
		glPopMatrix();
	};

	void drawNormal() {
		//glPushMatrix();
		//glTranslatef(0, 0.5, 0);
		//drawCap();
		//glPopMatrix();

		drawBodyNormal();

		//glPushMatrix();
		//glTranslatef(0, -0.5, 0);
		//drawCap();
		//glPopMatrix();
	};


private:
	void drawBody() {
		float x = 0.5;
		float z = 0.0;
		float y = -0.5;
		float angle = 2 * PI / (float)m_segmentsX;
		float height = 1.0 / (float)m_segmentsY;

		glBegin(GL_TRIANGLES);
		for (int i = 0; i < m_segmentsY; i++) {
			for (int j = 0; j < m_segmentsX; j++) {
				float newx = x*cos(angle) - z*sin(angle);
				float newz = x*sin(angle) + z*cos(angle);

				normalizeNormal(x, 0, z);
				glVertex3f(x, y, z); 
				normalizeNormal(newx, 0, newz);
				glVertex3f(newx, y, newz);
				normalizeNormal(newx, 0, newz);
				glVertex3f(newx, y + height, newz);

				normalizeNormal(newx, 0, newz);
				glVertex3f(newx, y + height, newz);
				normalizeNormal(x, 0, z);
				glVertex3f(x, y + height, z);
				normalizeNormal(x, 0, z);
				glVertex3f(x, y, z);

				x = newx;
				z = newz;
			}
			y = y + height;
		}
		glEnd();
	};

	void drawCap() {
		float x = 0.5;
		float z = 0.0;
		float angle = 2*PI / (float)m_segmentsX;
		glBegin(GL_TRIANGLES);
		for (int i = 0; i < m_segmentsX; i++) {
			float newx = x*cos(angle) - z*sin(angle);
			float newz = x*sin(angle) + z*cos(angle);

			glNormal3f(0, -1, 0);
			glVertex3f(0, 0, 0);
			glVertex3f(x, 0, z);
			glVertex3f(newx, 0, newz);

			x = newx;
			z = newz;
		}
		glEnd();
	}

	void drawBodyNormal() {
		float x = 0.5;
		float z = 0.0;
		float y = -0.5;
		float angle = 2 * PI / (float)m_segmentsX;
		float height = 1.0 / (float)m_segmentsY;

		glBegin(GL_LINES);
		for (int i = 0; i < m_segmentsY; i++) {
			for (int j = 0; j < m_segmentsX; j++) {
				float newx = x*cos(angle) - z*sin(angle);
				float newz = x*sin(angle) + z*cos(angle);

				glVertex3f(x, y, z); glVertex3f(1.1*x, y, 1.1*z);
				glVertex3f(newx, y, newz); glVertex3f(1.1*newx, y, 1.1*newz); 
				glVertex3f(newx, y + height, newz); glVertex3f(1.1*newx, (y + height), 1.1*newz);

				glVertex3f(newx, y + height, newz); glVertex3f(1.1*newx, (y + height), 1.1*newz);
				glVertex3f(x, y + height, z); glVertex3f(1.1*x, (y + height), 1.1*z);
				glVertex3f(x, y, z); glVertex3f(1.1*x, y, 1.1*z);

				x = newx;
				z = newz;
			}
			y = y + height;
		}
		glEnd();
	};};

#endif