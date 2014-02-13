#include <GL/glui.h>
#include "solarSystem.h"

solarSystem::solarSystem(){

}

solarSystem::~solarSystem(){

}

// Render this with push and pop operations
// or alternatively implement helper functions
// for each of the planets with parameters.
void solarSystem::render(){
	
	static float rotation = 0.1;
	static float orbitSpeed = 0.1;
	static float moonOrbitX = 0.1;
	static float moonOrbitY = 0.1;
	static float moonOrbitSpeed = 1;

	glPushMatrix();

		// The Sun
		glPushMatrix();
			glRotatef(rotation,0,1,0);

			glColor3f(0.6f,0.4f,0.0f);
			glutSolidSphere(1.2,10,10);
		glPopMatrix();


			// Orbit 1
			glPushMatrix();
				glRotatef(orbitSpeed*2,0,1,0);
						// Planet
						glPushMatrix();
							glTranslatef(2,0,0);
							glRotatef(rotation,0,1,0);
							glColor3f(1,0,0);
							glutSolidSphere(.1,10,10);
									// Moon 1
									glRotatef(moonOrbitY*0.25,0,1,0);
									glPushMatrix();
										glTranslatef(0.2,0,0);
										glRotatef(moonOrbitY*0.25,0,1,0);
										glColor3f(0.5,0.5,0.5);
										glutSolidSphere(.05,10,10);
									glPopMatrix();
						glPopMatrix();
			glPopMatrix();

			// Orbit 2
			glPushMatrix();
				glRotatef(orbitSpeed*1.5,0,1,0);
					// Planet
					glPushMatrix();
						glTranslatef(3,0,0);
						glRotatef(rotation,0,1,0);
						glColor3f(1,0.2,0);
						glutSolidSphere(.1,10,10);
								// Moon 2
								glRotatef(moonOrbitY*0.25,0,1,0);
								glPushMatrix();
									glTranslatef(0,0,0.2);
									glRotatef(moonOrbitX,0,0,1);
									glColor3f(0.5,0.5,0.5);
									glutSolidSphere(.05,10,10);
								glPopMatrix();
					glPopMatrix();
			glPopMatrix();

			// Orbit 3
			glPushMatrix();
				glRotatef(orbitSpeed*1.35,0,1,0);
					// Planet
					glPushMatrix();
						glTranslatef(4,0,0);
						glRotatef(rotation,0,1,0);
						glColor3f(0,0,1);
						glutSolidSphere(.1,10,10);
					glPopMatrix();
			glPopMatrix();

			// Orbit 4
			glPushMatrix();
				glRotatef(orbitSpeed*1.25,0,1,0);
					// Planet
					glPushMatrix();
						glTranslatef(5,0,0);
						glRotatef(rotation,0,1,0);
						glColor3f(0.6f,0.4f,0.0f);
						glutSolidSphere(.1,10,10);
					glPopMatrix();
			glPopMatrix();

			// Orbit 5
			glPushMatrix();
				glRotatef(orbitSpeed*1.15,0,1,0);
					// Planet
					glPushMatrix();
						glTranslatef(7,0,0);
						glRotatef(rotation,0,1,0);
						glColor3f(0.8f,0.4f,0.0f);
						glutSolidSphere(.3,10,10);
								// Ring
								glRotatef(120,1,0,0);
								glPushMatrix();
									glScalef(1,1,.1);
									glTranslatef(0,0,0.2);
									glRotatef(rotation*2,0,0,1);
									glColor3f(0.5,0.5,0.5);
									glutSolidTorus(.4, .6, 10, 10);
								glPopMatrix();
					glPopMatrix();
			glPopMatrix();

			// Orbit 6
			glPushMatrix();
				glRotatef(orbitSpeed*1.05,0,1,0);
					// Planet
					glPushMatrix();
						glTranslatef(9,0,0);
						glRotatef(rotation,0,1,0);
						glColor3f(0.3f,0.2f,1.0f);
						glutSolidSphere(.05,10,10);
					glPopMatrix();
			glPopMatrix();

	glPopMatrix();

	rotation+= 0.01;
	orbitSpeed+=0.05;
	moonOrbitX+=moonOrbitSpeed;
	moonOrbitY+=moonOrbitSpeed;




}