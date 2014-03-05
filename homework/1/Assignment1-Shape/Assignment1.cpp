/****************************************************************************

A simple GLUT program using the GLUI User Interface Library

This program sets up a checkbox and a spinner, both with live variables.
No callbacks are used.

-----------------------------------------------------------------------

9/9/98 Paul Rademacher (rademach@cs.unc.edu)

****************************************************************************/

//#include <string.h>
#include <GL/glui.h>
#include "Shape.h"
#include "Cube.h"
#include "Cylinder.h"
#include "Cone.h"
#include "Sphere.h"
#include "Torus.h"
#include "funShape.h"

enum OBJ_TYPE {
	CUBE = 0,
	CYLINDER = 1,
	CONE = 2,
	SPHERE = 3,
	TORUS = 4,
    FUN   = 5
};

/** These are the live variables passed into GLUI ***/
int  wireframe = 1;
int  fill = 1;
int  normal = 0;
int  segmentsX = 10;
int  segmentsY = 10;
int	 rotX = 0;
int	 rotY = 0;
int	 rotZ = 0;
int  scale = 50;
int  main_window;


/** these are the global variables used for rendering **/
OBJ_TYPE objType = CUBE;
Cube* cube = new Cube();
Cylinder* cylinder = new Cylinder();
Cone* cone = new Cone();
Sphere* sphere = new Sphere();
Torus* torus = new Torus();
FunShape* fun = new FunShape();
Shape* shape = cube;

/***************************************** callback_obj() ***********/

void callback_obj(int id) {
	switch (objType) {
	case CUBE:
		shape = cube;
		break;
	case CYLINDER:
		shape = cylinder;
		break;
	case CONE:
		shape = cone;
		break;
	case SPHERE:
		shape = sphere;
		break;
	case TORUS:
		shape = torus;
		break;
    case FUN:
        shape = fun;
        break;
	default:
		shape = cube;
	}
}

/***************************************** myGlutIdle() ***********/

void myGlutIdle(void)
{
	/* According to the GLUT specification, the current window is
	undefined during an idle callback.  So we need to explicitly change
	it if necessary */
	if (glutGetWindow() != main_window)
		glutSetWindow(main_window);

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
	glTranslatef(0, 0, -0.5);

	glutPostRedisplay();
}

/***************************************** myGlutDisplay() *****************/

void myGlutDisplay(void)
{
	glClearColor(.9f, .9f, .9f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//rotate object
	glTranslatef(0.0, 0.0, -1.0);
	glRotatef(rotX, 1.0, 0.0, 0.0);
	glRotatef(rotY, 0.0, 1.0, 0.0);
	glRotatef(rotZ, 0.0, 0.0, 1.0);

	//drawing the axes
    glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0, 0, 0); glVertex3f(1.0, 0, 0);
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0, 0, 0); glVertex3f(0.0, 1.0, 0);
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0, 0, 0); glVertex3f(0, 0, 1.0);
	glEnd();

	//scale object
	glScalef(scale / 50.0, scale / 50.0, scale / 50.0);

	shape->setSegments(segmentsX, segmentsY);
	
	if (normal) {
		glColor3f(1.0, 0.0, 0.0);
		shape->drawNormal();
	}

    glEnable(GL_LIGHTING);
	if (fill) {
		glEnable(GL_POLYGON_OFFSET_FILL);
		glColor3f(0.5, 0.5, 0.5);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		shape->draw();
	}
	
	if (wireframe) {
		glDisable(GL_POLYGON_OFFSET_FILL);
		glColor3f(0.0, 0.0, 0.0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		shape->draw();
	}

	glutSwapBuffers();
}

void onExit()
{
	delete cube;
	delete cylinder;
	delete cone;
	delete sphere;
	delete torus;
    delete fun;
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

	main_window = glutCreateWindow("COMP 175 Assignment 1");
	glutDisplayFunc(myGlutDisplay);
	glutReshapeFunc(myGlutReshape);

	/****************************************/
	/*       Set up OpenGL lighting         */
	/****************************************/


	        glClearColor (0.38, 0.38, 0.38, 0.0);
        glShadeModel (GL_SMOOTH);

        GLfloat light_pos0[] = {0.0f, 0.0f, 1.0f, 0.0f};
        GLfloat diffuse[] = {0.5f, 0.5f, 0.5f, 0.0f};
        GLfloat ambient[] = {0.1f, 0.1f, 0.1f, 1.0f};

        glLightfv (GL_LIGHT0, GL_AMBIENT, ambient);
        glLightfv (GL_LIGHT0, GL_DIFFUSE, diffuse);
        glLightfv (GL_LIGHT0, GL_POSITION, light_pos0);

        glColorMaterial (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
        glEnable (GL_COLOR_MATERIAL);

        glEnable(GL_LIGHTING);
        glEnable (GL_LIGHT0);
        glEnable (GL_DEPTH_TEST);

	//glShadeModel(GL_FLAT);

	//glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	//glEnable(GL_COLOR_MATERIAL);
	//
	//glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);

	//static float one[] = { 1, 1, 1, 1 };
	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, one);


	///****************************************/
	///*          Enable z-buferring          */
	///****************************************/


	//glEnable(GL_DEPTH_TEST);
	glPolygonOffset(1, 1);



	/****************************************/
	/*         Here's the GLUI code         */
	/****************************************/

	GLUI *glui = GLUI_Master.create_glui("GLUI");

	GLUI_Panel *render_panel = glui->add_panel("Render");
	new GLUI_Checkbox(render_panel, "Wireframe", &wireframe);
	new GLUI_Checkbox(render_panel, "Fill", &fill);
	new GLUI_Checkbox(render_panel, "Normal", &normal);
	//new GLUI_Checkbox(glui, "Rotate", &rotate);
	(new GLUI_Spinner(render_panel, "Segments X:", &segmentsX))
		->set_int_limits(3, 60);
	(new GLUI_Spinner(render_panel, "Segments Y:", &segmentsY))
		->set_int_limits(3, 60);


	GLUI_Panel *camera_panel = glui->add_panel("Camera");
	(new GLUI_Spinner(camera_panel, "Rotate X:", &rotX))
		->set_int_limits(0, 359);
	(new GLUI_Spinner(camera_panel, "Rotate Y:", &rotY))
		->set_int_limits(0, 359);
	(new GLUI_Spinner(camera_panel, "Rotate Z:", &rotZ))
		->set_int_limits(0, 359);
	(new GLUI_Spinner(camera_panel, "Scale:", &scale))
		->set_int_limits(1, 100);


	glui->add_column(true);

	GLUI_Panel *obj_panel = glui->add_panel("Object Type");
	GLUI_RadioGroup *group1 =
		glui->add_radiogroup_to_panel(obj_panel, (int*)(&objType), 3, callback_obj);
	glui->add_radiobutton_to_group(group1, "Cube");
	glui->add_radiobutton_to_group(group1, "Cylinder");
	glui->add_radiobutton_to_group(group1, "Cone");
	glui->add_radiobutton_to_group(group1, "Sphere");
	glui->add_radiobutton_to_group(group1, "Torus");
//    glui->add_radiobutton_to_group(group1, "funShape");
	glui->add_button("Quit", 0, (GLUI_Update_CB)exit);

	glui->set_main_gfx_window(main_window);

	/* We register the idle callback with GLUI, *not* with GLUT */
	GLUI_Master.set_glutIdleFunc(myGlutIdle);

	glutMainLoop();

	return EXIT_SUCCESS;
}



