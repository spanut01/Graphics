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
	
	// Some ideas for constants that can be used for
	// rotating the planets.
	static float rotation = 0.1;
	static float orbitSpeed = 0.1;
	static float moonOrbitX = 0.1;
	static float moonOrbitY = 0.1;
	static float moonOrbitSpeed = 1;
        static float rotations[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        int i;
        
	glPushMatrix();

		// The Sun
		glPushMatrix();
			//glRotatef(rotation,0,1,0);
			glColor3f(0.8f,0.8f,0.0f);
			glutSolidSphere(1.2,10,10);
		glPopMatrix();


	// Add more planets, moons, and rings here!	

        glPushMatrix();
            glRotatef(rotations[0],0,1,0);
            glTranslatef(1.3,0.0,1.5);
            glColor3f(0.7,0.7,0.7);
            glutSolidSphere(0.05f,10,10);
        glPopMatrix();

        glPushMatrix();
            glRotatef(rotations[1],0,1,0);
            glTranslatef(2.1,0.0,2.1);
            glColor3f(0.6,0.4,0.0);
            glutSolidSphere(0.1f,10,10);
        glPopMatrix();

        glPushMatrix();
            glRotatef(rotations[2],0,1,0);
            glTranslatef(2.7,0.0,3.0);
            glColor3f(0.0,0.6,0.6);
            glutSolidSphere(0.15f,10,10);
        glPopMatrix();

        glPushMatrix();
            glRotatef(rotations[2],0,1,0);
            glTranslatef(3.9,0.0,3.0);
            glColor3f(0.5,0.0,0.0);
            glutSolidSphere(0.15f,10,10);
        glPopMatrix();

        for(i=0; i<10; i++)rotations[i]+=((float)(i+1))/3.0;
        
	rotation+= 0.1;
	orbitSpeed+=0.05;
	moonOrbitX+=moonOrbitSpeed;
	moonOrbitY+=moonOrbitSpeed;

}
