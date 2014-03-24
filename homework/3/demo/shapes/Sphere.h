#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"

class Sphere : public Shape {
public:
	Sphere() {};
	~Sphere() {};

	void draw() {
		glPushMatrix();
		drawBody();
		glPopMatrix();
	};

	void drawNormal() {
		glPushMatrix();
		drawBodyNormal();
		glPopMatrix();
	};

private:
	void drawBody() {
		float radius = 0.5;
		float angle = 0;
		float angleH = -PI/(float)2.0;

		float angle_delta = 2.0 * PI / (float)m_segmentsX;
		float angleH_delta = PI / (float)m_segmentsY;

		glBegin(GL_TRIANGLES);
		for (int i = 0; i < m_segmentsY; i++) {
			angle = 0;
			for (int j = 0; j < m_segmentsX; j++) {
				float x = radius * cos(angleH) * cos(angle);
				float z = radius * cos(angleH) * sin(angle);
				float y = radius * sin(angleH);

				float newx = radius * cos(angleH) * cos(angle + angle_delta);
				float newz = radius * cos(angleH) * sin(angle + angle_delta);
				float newy = radius * sin(angleH);

				float x_next = radius * cos(angleH + angleH_delta) * cos(angle);
				float z_next = radius * cos(angleH + angleH_delta) * sin(angle);
				float y_next = radius * sin(angleH + angleH_delta);

				float newx_next = radius * cos(angleH + angleH_delta) * cos(angle + angle_delta);
				float newz_next = radius * cos(angleH + angleH_delta) * sin(angle + angle_delta);
				float newy_next = radius * sin(angleH + angleH_delta);

                                Vector normV = Vector(x,y,z);
				normalizeNormal(normV);
                                //glNormal3f(x, y, z);
				glVertex3f(x, y, z);
                                normV = Vector(newx,newy,newz);
				normalizeNormal(normV);
				//glNormal3f(newx, newy, newz);
				glVertex3f(newx, newy, newz);
                                normV = Vector(newx_next,newy_next,newz_next);
				normalizeNormal(normV);
				//glNormal3f(newx_next, newy_next, newz_next);
				glVertex3f(newx_next, newy_next, newz_next);

				normV = Vector(newx_next, newy_next, newz_next);
				normalizeNormal(normV);
				//glNormal3f(newx_next, newy_next, newz_next);
				glVertex3f(newx_next, newy_next, newz_next);
				normV = Vector(x_next, y_next, z_next);
				normalizeNormal(normV);
				//glNormal3f(x_next, y_next, z_next);
				glVertex3f(x_next, y_next, z_next);
				normV = Vector(x,y,z);
				normalizeNormal(normV);
				//glNormal3f(x, y, z);
				glVertex3f(x, y, z);

				angle = angle + angle_delta;
			}
			angleH = angleH + angleH_delta;
		}
		glEnd();
	};


	void drawBodyNormal() {
		float radius = 0.5;
		float angle = 0;
		float angleH = -PI/(float)2.0;

		float angle_delta = 2.0 * PI / (float)m_segmentsX;
		float angleH_delta = PI / (float)m_segmentsY;

		glBegin(GL_LINES);
		for (int i = 0; i < m_segmentsY; i++) {
			angle = 0;
			for (int j = 0; j < m_segmentsX; j++) {
				float x = radius * cos(angleH) * cos(angle);
				float z = radius * cos(angleH) * sin(angle);
				float y = radius * sin(angleH);

				float newx = radius * cos(angleH) * cos(angle + angle_delta);
				float newz = radius * cos(angleH) * sin(angle + angle_delta);
				float newy = radius * sin(angleH);

				float x_next = radius * cos(angleH + angleH_delta) * cos(angle);
				float z_next = radius * cos(angleH + angleH_delta) * sin(angle);
				float y_next = radius * sin(angleH + angleH_delta);

				float newx_next = radius * cos(angleH + angleH_delta) * cos(angle + angle_delta);
				float newz_next = radius * cos(angleH + angleH_delta) * sin(angle + angle_delta);
				float newy_next = radius * sin(angleH + angleH_delta);

				glVertex3f(x, y, z); glVertex3f(1.1*x, 1.1*y, 1.1*z);
				glVertex3f(newx, newy, newz); glVertex3f(1.1*newx, 1.1*newy, 1.1*newz);
				glVertex3f(newx_next, newy_next, newz_next); glVertex3f(1.1*newx_next, 1.1*newy_next, 1.1*newz_next);

				glVertex3f(newx_next, newy_next, newz_next); glVertex3f(1.1*newx_next, 1.1*newy_next, 1.1*newz_next);
				glVertex3f(x_next, y_next, z_next); glVertex3f(1.1*x_next, 1.1*y_next, 1.1*z_next);
				glVertex3f(x, y, z); glVertex3f(1.1*x, 1.1*y, 1.1*z);

				angle = angle + angle_delta;
			}
			angleH = angleH + angleH_delta;
		}
		glEnd();
	};
};

#endif
