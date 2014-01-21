/****************************************************************************

Last Modified: 12/23/13


****************************************************************************/

#include <string.h>
#include <iostream>
#include <fstream>
#include </comp/175/public_html/labs/include/GL/glui.h>
#include <math.h>
#include "ply.h"

/** These are the live variables passed into GLUI ***/
int main_window;
int  wireframe = 1;
int  filled = 1;
int	 rotY = 0;
int  scale = 100;

GLUI_EditText* filenameTextField = NULL;
string filenamePath = "./data/happy.ply";

/****************************************/
/*         PLY Object                   */
/****************************************/
ply* myPLY = new ply (filenamePath);

/***************************************** myGlutIdle() ***********/

void myGlutIdle(void)
{
	/* According to the GLUT specification, the current window is
	undefined during an idle callback.  So we need to explicitly change
	it if necessary */
	if (glutGetWindow() != main_window){
		glutSetWindow(main_window);
	}

	glutPostRedisplay();
}


/**************************************** myGlutReshape() *************/

void myGlutReshape(int x, int y)
{
	float xy_aspect;

	xy_aspect = (float)x / (float)y;
	glViewport(0, 0, x, y);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-xy_aspect*.08, xy_aspect*.08, -.08, .08, .1, 15.0);
	glTranslatef(0, 0, 0);

	glutPostRedisplay();
}

/***************************************** myGlutDisplay() *****************/

void myGlutDisplay(void)
{
	static float rotationX = 0.0, rotationY = 0.0;

	glClearColor(.9f, .9f, .9f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();
		glTranslatef(0.0, -0.125, -0.75);
		//drawing the axes
		glBegin(GL_LINES);
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(0, 0, 0); glVertex3f(1.0, 0, 0);
		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(0, 0, 0); glVertex3f(0.0, 1.0, 0);
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(0, 0, 0); glVertex3f(0, 0, 1.0);
		glEnd();
	glPopMatrix();

			
	glPushMatrix();
			glScalef(scale / 50.0, scale / 50.0, scale / 50.0);
			glTranslatef(0.0, -0.125, -0.25);
			glRotatef(rotY, 0.0, 1.0, 0.0);

		if (filled) {
			glEnable(GL_POLYGON_OFFSET_FILL);
			glColor3f(0.5, 0.5, 0.5);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			myPLY->render();
		}
		
		if (wireframe) {
			glDisable(GL_POLYGON_OFFSET_FILL);
			glColor3f(0.0, 0.0, 0.0);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			myPLY->render();
			glPopMatrix();
		}
	glPopMatrix();

	glutSwapBuffers();
}

void onExit()
{
	delete myPLY;
}


void callback_load(int id) {
	
	if (filenameTextField == NULL) {
		return;
	}
	cout << "loading: " << filenameTextField->get_text() << endl;
	myPLY->reload(filenameTextField->get_text());
	myPLY->printAttributes();

}


/**************************************** main() ********************/

int main(int argc, char* argv[])
{
	
	atexit(onExit);

	/****************************************/
	/*   Initialize GLUT and create window  */
	/****************************************/

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(500, 500);

	main_window = glutCreateWindow("COMP 175 In Class Assignment 1");
	glutDisplayFunc(myGlutDisplay);
	glutReshapeFunc(myGlutReshape);

	/****************************************/
	/*       Set up OpenGL lighting         */
	/****************************************/

	glShadeModel(GL_SMOOTH);

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);

	static float one[] = { 1, 1, 1, 1 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, one);


	/****************************************/
	/*          Enable z-buferring          */
	/****************************************/

	glEnable(GL_DEPTH_TEST);
	glPolygonOffset(1, 1);

	/****************************************/
	/*         Here's the GLUI code         */
	/****************************************/

	GLUI *glui = GLUI_Master.create_glui("GLUI");

	GLUI_Panel *render_panel = glui->add_panel("Render");
	new GLUI_Checkbox(render_panel, "Wireframe", &wireframe);
	new GLUI_Checkbox(render_panel, "Filled", &filled);

	GLUI_Panel *camera_panel = glui->add_panel("Camera");
	(new GLUI_Spinner(camera_panel, "Rotate Y:", &rotY))
		->set_int_limits(0, 359);
	(new GLUI_Spinner(camera_panel, "Scale:", &scale))
		->set_int_limits(1, 1000);


	filenameTextField = new GLUI_EditText( glui, "Filename:", filenamePath);
	filenameTextField->set_w(300);
	glui->add_button("Load PLY", 0, callback_load);


	glui->add_column(true);
	glui->add_button("Quit", 0, (GLUI_Update_CB)exit);

	glui->set_main_gfx_window(main_window);
	/* We register the idle callback with GLUI, *not* with GLUT */
	GLUI_Master.set_glutIdleFunc(myGlutIdle);



	glutMainLoop();

	return EXIT_SUCCESS;
}



