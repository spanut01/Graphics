/*  =================== File Information =================
	File Name: main.cpp
	Description:
	Author: Michael Shah
	Last Modified: 3/04/14

	Purpose: Driver for 3D program to demonstrate splines 
	Usage:	
	===================================================== */
#include <string.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <GL/glui.h>
#include <math.h>
#include "spline.h"

#define PI 3.14

/** These are the live variables passed into GLUI ***/
int main_window;
int renderSpline = 1;
int renderControlPoints = 1;
int grid = 0;
int resolution = 30;

float view_rotate[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
float obj_pos[] = { 0.0, 0.0, 0.0 };

// Pointer to widget controls
GLUI_Button      *spline_btn;
// User ID for callbacks
#define SPLINE_ID	100

// Pointer to widget
GLUI *glui2;


/* 	======== Our Scene ========

	============================== */
spline* mySpline = new spline();
// Add a bunch of points to our spline

// Function from: http://stackoverflow.com/questions/686353/c-random-float-number-generation
float rand_FloatRange(float a, float b)
{
	return ((b-a)*((float)rand()/RAND_MAX))+a;
}

void populatePoints(){
	// Simple spline
/*	
	mySpline->addPoint(0,1,0);
	mySpline->addPoint(2,5,0);
	mySpline->addPoint(4,3,0);
	mySpline->addPoint(6,2,0);
	mySpline->addPoint(8,6,0);
	mySpline->addPoint(10,4,0);
	mySpline->addPoint(12,3,0);
	mySpline->addPoint(14,8,0);
	mySpline->addPoint(16,1,0);
	mySpline->addPoint(18,0,0);
	mySpline->addPoint(20,5,0);
	mySpline->addPoint(22,9,0);
	mySpline->addPoint(24,6,0);
	mySpline->addPoint(26,2,0);
	mySpline->addPoint(28,7,0);
	mySpline->addPoint(30,3,0);
	mySpline->addPoint(32,8,0);

*/
	// Randomly generated pions
	float stepSize = 1.0f; // x coordinate (how spread out are the points along x-axis)
	float maxYHeight = 10.0f;	// y cooridnate (the up and down of our roller coaster)
	float maxZWidth = 10.0f;	// z cooridnate (how spread out is the roller coaster)

	for(int i = 0; i < 101; i++){
		float randomHeight = rand_FloatRange(0,maxYHeight);
		float randomWidth = rand_FloatRange(0,maxZWidth);
		mySpline->addPoint(i*stepSize,randomHeight,0);
	}

	/*
	// More crazy roller coaster
	mySpline->addPoint(-5,0,0);
	mySpline->addPoint(1,1,0);
	mySpline->addPoint(4,4,0);
	mySpline->addPoint(8,8,0);
	mySpline->addPoint(9,8,-5);
	mySpline->addPoint(8,10,-9);
	mySpline->addPoint(8,4,-7);
	mySpline->addPoint(4,1,-7);
	mySpline->addPoint(-5,0,-7);
	*/
}

void generateSpline(){

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
/*
	We configure our window so that it correctly displays our objects
	in the correct perspective.
*/
void myGlutReshape(int x, int y)
{
	float xy_aspect;
	xy_aspect = (float)x / (float)y;

	glViewport(0, 0, x, y);
	// Determine if we are modifying the camera(GL_PROJECITON) matrix(which is our viewing volume)
	// Otherwise we could modify the object transormations in our world with GL_MODELVIEW
	glMatrixMode(GL_PROJECTION);
	// Reset the Projection matrix to an identity matrix
	glLoadIdentity();
	// The frustrum defines the perspective matrix and produces a perspective projection.
	// It works by multiplying the current matrix(in this case, our matrix is the GL_PROJECTION)
	// and multiplies it.
	glFrustum(-xy_aspect*.08, xy_aspect*.08, -.08, .08, .1, 15.0);
	// Since we are in projection mode, here we are setting the camera to the origin (0,0,0)
	glTranslatef(0, 0, 0);
	// Call our display function.
	glutPostRedisplay();
}

/*
	Function to draw the x,y,and z axis
*/
void drawAxis(){
	glBegin(GL_LINES);
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(0, 0, 0); glVertex3f(1.0, 0, 0);
		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(0, 0, 0); glVertex3f(0.0, 1.0, 0);
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(0, 0, 0); glVertex3f(0, 0, 1.0);
	glEnd();
}

/*
	Function to a renderSpline grid in our scene.
*/
void draw_grid(){
	int grid_size = 10;
	glDisable(GL_LIGHTING);
	glPushMatrix();
	glTranslatef(-grid_size/2,0,-grid_size/2);
    // Draw a grid under the object
        for(int i = 0 ; i < grid_size; i=i+1){
            glBegin(GL_LINES);
                glColor3f( 1.0, 1.0, 1.0 );
                glVertex3f( 0.0, 0.0, i );  glVertex3f( grid_size, 0.0, i ); /* X axis grid */
                glVertex3f( i, 0.0, 0.0 );  glVertex3f( i, 0.0, grid_size ); /* X axis grid */
            glEnd();
        }
    
        glBegin(GL_LINES);
            glColor3f( 1.0, 1.0, 1.0 );
            glVertex3f( 0.0, 0.0, grid_size );  glVertex3f( grid_size, 0.0, grid_size ); /* X axis grid */
            glVertex3f( grid_size, 0.0, 0.0 );  glVertex3f( grid_size, 0.0, grid_size ); /* X axis grid */
        glEnd();
    glPopMatrix();
    glEnable( GL_LIGHTING );

}

/***************************************** myGlutDisplay() *****************/

void myGlutDisplay(void)
{
	static float scale = 0.1;
	// Clear the buffer of colors in each bit plane.
    // bit plane - A set of bits that are on or off (Think of a black and white image)
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Set the mode so we are modifying our objects. 
	glMatrixMode(GL_MODELVIEW);
	// Load the identify matrix which gives us a base for our object transformations
    // (i.e. this is the default state) 
	glLoadIdentity();

	glScalef(scale,scale,scale);
	glTranslatef( obj_pos[0], obj_pos[1]-2, -obj_pos[2]-6 );
	glMultMatrixf(view_rotate);

	// Render our scene
		if (renderControlPoints) {
			glEnable(GL_POLYGON_OFFSET_FILL);
			glColor3f(0.5, 0.5, 0.5);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			//mySpline->render();
			mySpline->renderAsRollerCoaster(resolution,0.5);
			mySpline->render();
		}
		
		if (renderSpline) {
			glDisable(GL_POLYGON_OFFSET_FILL);
			glColor3f(0.0, 0.0, 0.0);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glPopMatrix();
			mySpline->cubic_spline(resolution,0);
		}
	// Draw a grid
		if(grid){
			draw_grid();
		}


	glutSwapBuffers();
}

// Callbacks for the scene
void pointer_cb( GLUI_Control* control )
{
  if (control->get_id() == SPLINE_ID ) {
  	std::cout << "Generating Spline" << std::endl;
  	mySpline->cubic_spline(resolution,1);
  }
}

/*
	Reclaim memory we've allocated
*/
void onExit()
{
	delete mySpline;
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

	main_window = glutCreateWindow("COMP 175 In Class Assignment 5");
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

	/*** Create the bottom subwindow ***/
    glui2 = GLUI_Master.create_glui_subwindow( main_window, GLUI_SUBWINDOW_BOTTOM );
    // Create a rotation widget
    GLUI_Rotation *view_rot = new GLUI_Rotation(glui2, "Objects", view_rotate );
    view_rot->set_spin( 1.0 );

    // Navigate our solar system
    new GLUI_Column( glui2, false );
    GLUI_Translation *trans_x =  new GLUI_Translation(glui2, "Objects X", GLUI_TRANSLATION_X, obj_pos );
    trans_x->set_speed( .1 );
    new GLUI_Column( glui2, false );
    GLUI_Translation *trans_y =  new GLUI_Translation( glui2, "Objects Y", GLUI_TRANSLATION_Y, &obj_pos[1] );
    trans_y->set_speed( .1 );
    new GLUI_Column( glui2, false );
    GLUI_Translation *trans_z =  new GLUI_Translation( glui2, "Objects Z", GLUI_TRANSLATION_Z, &obj_pos[2] );
    trans_z->set_speed( .1 );

	glui2->add_column(true);

	GLUI_Panel *render_panel = glui2->add_panel("Render");
	new GLUI_Checkbox(render_panel, "Render Spline", &renderSpline);
	new GLUI_Checkbox(render_panel, "Render ControlPoints", &renderControlPoints);
	new GLUI_Checkbox(render_panel, "Grid", &grid);

	new GLUI_Button(render_panel, "CalculateSpline", SPLINE_ID, pointer_cb);
	glui2->add_button("Quit", 0, (GLUI_Update_CB)exit);



	glui->set_main_gfx_window(main_window);
	/* We register the idle callback with GLUI, *not* with GLUT */
	GLUI_Master.set_glutIdleFunc(myGlutIdle);


	// Setup the points for our spline
	populatePoints();

	glutMainLoop();

	return EXIT_SUCCESS;
}



