/*  =================== File Information =================
	File Name: ppm.h
	Description:
	Author: Michael Shah
	Last Modified: 4/2/14

	Purpose: 
	Usage:	

	Further Reading resources: 
	http://antongerdelan.net/opengl/raycasting.html
	===================================================== */
#include <string.h>
#include <iostream>
#include <fstream>
#include <GL/glui.h>
#include <math.h>
#include "object.h"
using namespace std;
/** These are the live variables passed into GLUI ***/
int main_window;

// int top_view_window;
// int left_view_window;
// int bottom_view_window;

int keyframe = 0;

int wireframe = 0;
int filled = 1;
int grid = 1;

// bool drawRay = false;

// Play the scene animation
bool playingAnimation = false;
int totalAnimationFrames = 50;
int currentSceneAnimationFrame = 0;

// Perform linear interpolation for the animation
float ifx,ify,ifz = 0; // The delta for 'interpolated frame step' along the x y and z axis.
int interpolatedFrame = 0;
float maxInterpolationFrame = 24;

// Used for intersection
Vector nearVec;
Vector farVec;
Vector oldFarVec;

// selection handle state
// 0 means nothing selected
// 1 means x-axis selected
// 2 means y-axis selected
// 3 means z-axis selected
int handle_state = 0;	

// Used for navigating the world
float simpleCamera[3] = {0,-2,-4};

// Definition button for keyframe
#define SET_KEYFRAME_ID 101
#define PLAY_ANIMATION_ID 102
#define STOP_ANIMATION_ID 103
#define KEYFRAME_SPINNER_ID 104

float view_rotate[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
float obj_pos[] = { 0.0, 0.0, 0.0 };

// function declarations

// double Intersect(Point eyePointP, Vector rayV, Matrix transformMatrix);
// bool drawRayFunc(int x, int y);

// Window information
double windowXSize = 800;
double windowYSize = 800;

float oldMouseX = 0;
float oldMouseY = 0;

float mouseX = 0;
float mouseY = 0;
float mouseZ = 1.0f;

/* 	======== Our Scene ========
	We are going to create a single sphere that we load a texture onto.

	This object is assigned a unique id.
	=========================== */
object* myObject = new object(175);




/* =======================================================v Mouse and Ray Casting Blocks of Code (below) v=============================== */

/*	===========================================
	Little trick to convert mouse to OpenGL coordinates
	Sources: http://www.opengl.org/sdk/docs/man2/xhtml/gluUnProject.xml
	=========================================== */
Vector ConvertMouseToOGLCoordinate(float mouseX, float mouseY, float _mouseZ)
{
	// Retrive OpenGL Maintained viewports, modelview, and projection matrix
    GLint viewport[4];					glGetIntegerv(GL_VIEWPORT, viewport);
    GLdouble modelMatrix[16];   		glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
    GLdouble projectionMatrix[16];  	glGetDoublev(GL_PROJECTION_MATRIX, projectionMatrix);

    //float winZ;
    float winY = float(viewport[3] - mouseY);
    //glReadPixels(mouseX, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

    double x, y; 
    double z = _mouseZ;
    // gluUnProject maps the specified window coordinates into object coordinates using model, proj, and view. The result is stored in objX, objY, and objZ. A return value of GLU_TRUE indicates success; a return value of GLU_FALSE indicates failure.
    // http://www.opengl.org/sdk/docs/man2/xhtml/gluUnProject.xml
    gluUnProject((double)mouseX, winY, _mouseZ, modelMatrix, projectionMatrix, viewport, &x, &y, &z);

    return Vector(x, y, z);
}


/*	===========================================
	Updates the mouse cooridnates
	This function turns a x and y screen cooridnate into a OpenGL world coordinate
	in object space
	=========================================== */
void updateMouse(int x, int y){
	mouseX = x; mouseY = y;

    nearVec = ConvertMouseToOGLCoordinate(x, y, 0);
    farVec = ConvertMouseToOGLCoordinate(x, y, 1);

    // Figure out the direction of the Ray
    Vector RayDirection = farVec - nearVec;
}


/*	===========================================
	This function is called everytime the mouse moves
	In order to get our ray to draw nicely, we update the mouse coordinates
	where the ray is first cast, and then draw the ray, and then draw the rest
	of our scene.
	=========================================== */
void myGlutMotion(int x, int y )
{
	oldFarVec = farVec;
	// Get the x and y coordinates
	updateMouse(x,y);
	
	//drawRayFunc(mouseX,mouseY);

	// Do some selection
    glutPostRedisplay();
}




/***************************************** myGlutMouse() ***********/
/*	This function is called everytime the mouse is clicked

	Updates the ray that we use for picking
	
*/
void myGlutMouse(int button, int button_state, int x, int y )
{
	updateMouse(x,y);
}





/* =======================================================^ Mouse and Ray Casting Blocks of Code(above) ^=============================== */

/*
	Transformations to perform the animation of the object
*/
void animateScene(){
	// Transform our object based on the animation
	if(playingAnimation==true && currentSceneAnimationFrame < totalAnimationFrames){
		// Output information to the console
		if(interpolatedFrame==1){
			// We have the above conditional so that this debug information is only output
			// once per frame.  We could add another debug function so that
			// we could see the x,y,z values of our object change with 
			// the interpolation.
			myObject->debugTranslationFrame(currentSceneAnimationFrame);
		}
		// Apply the transformations
		glTranslatef(myObject->tx, myObject->ty, myObject->tz);
	}
}


/*	=====================================================
	Perform a linear Interpolation for our simple object

	This block of code plays a simple linear animation between each keyframe, maxed out at 24 frames.
	
	You might choose to do some other interpolation that is better.
	===================================================== */
void linearInterpolation(){
	// If our animation is playing
	if(playingAnimation==true){
		// If we hit the maximum number of frames to interpolate between
		// Then reset the values.
		// i.e. When this value is greater than 24, then increment onto the next 'currentSceneAnimationFrame'
		if(interpolatedFrame >= maxInterpolationFrame){
			currentSceneAnimationFrame++;
			interpolatedFrame = 0;
		}
		
		// Increment the frame we're moving between.
		interpolatedFrame++;

		// Deltas between each keyframe
		if(currentSceneAnimationFrame < totalAnimationFrames-1){
			// Calculate delta values
			// We calculate based on the frame where we were, and the target frame, which is our current frame
			// We calculate these once
			if(interpolatedFrame==1){
				ifx = fabs( (myObject->keyframe[currentSceneAnimationFrame].tx) - (myObject->keyframe[currentSceneAnimationFrame-1].tx) ) / maxInterpolationFrame;
				ify = fabs( (myObject->keyframe[currentSceneAnimationFrame].ty) - (myObject->keyframe[currentSceneAnimationFrame-1].ty) ) / maxInterpolationFrame;
				ifz = fabs( (myObject->keyframe[currentSceneAnimationFrame].tz) - (myObject->keyframe[currentSceneAnimationFrame-1].tz) ) / maxInterpolationFrame;
			}
			// If our current position is less than the previous one(by at least one delta step) do not do anything.
			// Otherwise increment our objects position by at least
			if( fabs(myObject->tx - (myObject->keyframe[currentSceneAnimationFrame].tx)) > ifx+0.01){
				if(myObject->tx <= (myObject->keyframe[currentSceneAnimationFrame].tx))
				{ myObject->tx += ifx; } else { myObject->tx -= ifx; }
			}
			if( fabs(myObject->ty - (myObject->keyframe[currentSceneAnimationFrame].ty)) > ify+0.01){
				if(myObject->ty <= (myObject->keyframe[currentSceneAnimationFrame].ty))
				{ myObject->ty += ify; } else { myObject->ty -= ify; }
			}
			if( fabs(myObject->tz - (myObject->keyframe[currentSceneAnimationFrame].tz)) > ifz+0.01){
				if(myObject->tz <= (myObject->keyframe[currentSceneAnimationFrame].tz))
				{ myObject->tz += ifz; } else { myObject->tz -= ifz; }
			}

		}
	}
}




/*	===================================
	Draw a 10x10 grid under our scene
	=================================== */
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
    	// Draw the final two lines in our scene on the opposite corners
        glBegin(GL_LINES);
            glColor3f( 1.0, 1.0, 1.0 );
            glVertex3f( 0.0, 0.0, grid_size );  glVertex3f( grid_size, 0.0, grid_size ); /* X axis grid */
            glVertex3f( grid_size, 0.0, 0.0 );  glVertex3f( grid_size, 0.0, grid_size ); /* X axis grid */
        glEnd();
    glPopMatrix();
    glEnable( GL_LIGHTING );
}





/*  =====================================
	(1) Simple trick with color buffer
	(2) Or implement ray casting

	You will be finding out which handle you have
	collided with, and then moving the object appropriately.

	You can directly modify the objects positions as follows:

		myObject->tx modifies the x position
		myObject->ty modifies the y position
		myObject->tz modifies the z position

	I recommend you also look at what farVec is doing
	===================================== */
void objectSelection(){
	GLint viewport[4];
	GLubyte pixel[3];

	glGetIntegerv(GL_VIEWPORT,viewport);

	// Read this:
	// https://www.opengl.org/sdk/docs/man2/xhtml/glReadPixels.xml
	glReadPixels(mouseX, viewport[3]-mouseY, 1, 1, GL_RGB,GL_UNSIGNED_BYTE, (void *)pixel);
    cout << "pixel " << (int) pixel[0] << ", " << (int) pixel[1] << ", " << (int) pixel[2] << "\n";
    cout << obj_pos[0] << ", " << obj_pos[1] << ", " << obj_pos[2] << "\n";
    int r, g, b; 
    
    if ((pixel[0] == 255) && (pixel[1] != 255) && (pixel[2] != 255)){
        
    }
    if ((pixel[0] != 255) && (pixel[1] == 255) && (pixel[2] != 255)){
        
    }
    if ((pixel[0] != 255) && (pixel[1] != 255) && (pixel[2] == 255)){
        
    }

	/*


			Fill in the rest
			


	*/

}

/***************************************** myGlutDisplay() *****************/
/* 	============================
	Renders the scene
	============================ */	
void myGlutDisplay(void)
{

	// Clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Translate our camera
	glTranslatef( simpleCamera[0], simpleCamera[1], simpleCamera[2] );



	// Stop playing the animation if we have exceeded the amount of memory
	// we have allocated for keyframes.  In this case, we have allocated
	// 50 totalAnimationFrames by default for this lab assignment.
	if(currentSceneAnimationFrame >= totalAnimationFrames){
		playingAnimation = false;
	}

	// Perform the interpolation of our object
	linearInterpolation();

   

    // Draw the objects filled
	if (filled) {
		glEnable(GL_POLYGON_OFFSET_FILL);
		glColor3f(1.0, 1.0, 1.0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//myObject->render();
		glPushMatrix();
			animateScene();
			glRotatef(90,0,1,0);
				myObject->drawTexturedSphere();
		glPopMatrix();
	}
	

	// Draw a wireframe object
	if (wireframe) {
		glDisable(GL_POLYGON_OFFSET_FILL);
		glColor3f(1.0, 1.0, 1.0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//myObject->render();
		glPushMatrix();
			animateScene();
			glRotatef(90,0,1,0);
				myObject->drawTexturedSphere();
		glPopMatrix();
	}

	// Draw the grid
	// grid is a live variable tied to the GLUI_Checkbox object
	if(grid){
		draw_grid();
	}

	objectSelection();

	glutSwapBuffers();
}




/* 	=========================================================
	If you add any call backs for GLUI, you can put them here
	========================================================= */
void control_cb( int control )
{
	// Sets the keyframe with the current transformations.
	// Automatically overwrites the objects previous co-ordinates
	if ( control == SET_KEYFRAME_ID ) {
        // Set our objects position to the current frame
        myObject->setFrame(keyframe);
    }
    else if(control == PLAY_ANIMATION_ID){
    	playingAnimation = true;
		currentSceneAnimationFrame = 0;
    }
    else if(control == KEYFRAME_SPINNER_ID){
    	myObject->getFrame(keyframe);
    	// Actually make sure the object's tx,ty,tz (translation positions) are
    	// updated to the keyframe that we are on in the spinner.
        myObject->setPosition(myObject->keyframe[keyframe].tx,
							  myObject->keyframe[keyframe].ty,
							  myObject->keyframe[keyframe].tz);
    }
    else if(control == STOP_ANIMATION_ID){
    	// Stops the animation from playing and resets
    	// the current frame and any interpolation that is going on.
    	playingAnimation = false;
    	currentSceneAnimationFrame = 0;
    	interpolatedFrame = 0;
    	// Set our objects position.
        myObject->setPosition(myObject->keyframe[keyframe].tx,
					  		  myObject->keyframe[keyframe].ty,
					  		  myObject->keyframe[keyframe].tz);
    }
}




/*	==========================================
	GlutKeyboard that adds in user interaction
	========================================== */
void myGlutKeyboard(unsigned char Key, int x, int y)
{
    switch(Key)
    {
        case 27:	 // Case '27' maps to the 'ESC' key, this case will fall through into the next body of code.
        case 'q':    exit(0); break;
        case 'w':	 simpleCamera[2] += .1; break;
        case 'a':	 simpleCamera[0] += .1; break;
        case 's':    simpleCamera[2] -= .1; break;
        case 'd':    simpleCamera[0] -= .1; break;
        default:	std::cout << "No callback for key" << std::endl;
    };
    
    glutPostRedisplay();
}




/* =======================
   myGlutIdle() 
   ======================= */
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


/* 	======================
	myGlutReshape() 

	When the window is resized, update aspect ratio to get correct viewing frustrum.
	====================== */
void myGlutReshape(int x, int y)
{
	float xy_aspect;

	xy_aspect = (float)x / (float)y;
	// Pixel coordinate system
	glViewport(0, 0, x, y);

	windowXSize = x;
	windowXSize = y;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-xy_aspect*.08, xy_aspect*.08, -.08, .08, .1, 15.0);

	glutPostRedisplay();
}


/* ==============================
	Reclaim memory we've allocated
   ============================== */
void onExit()
{
	delete myObject;
}

/**************************************** main() ********************/
void glutInitValues(){
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
	/*		Setup textured Objects          */
	/****************************************/

	// Apply all of these functions to our windows.
	glutDisplayFunc(myGlutDisplay);
	glutReshapeFunc(myGlutReshape);
	glutMotionFunc( myGlutMotion );
}


int main(int argc, char* argv[])
{
	atexit(onExit);

	int border = 5;
	/****************************************/
	/*   Initialize GLUT and create window  */
	/****************************************/
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(windowXSize, windowYSize);
	// CREATE THE MAIN WINDOW
	main_window = glutCreateWindow("COMP 175 In Class Assignment 8");
	glutInitValues();
	/*

	// Setup multiple windows that we can render on

	// CREATE THE SECOND SUBWINDOW
	bottom_view_window = glutCreateSubWindow(main_window,0,0,windowXSize/2,windowYSize/2);
	glutInitValues();

	// CREATE THE THIRD SUBWINDOW
	left_view_window = glutCreateSubWindow(main_window,windowXSize/2,0,windowXSize/2,windowYSize/2);
	glutInitValues();

	// CREATE THE FOURTH SUBWINDOW
	top_view_window = glutCreateSubWindow(main_window,0,windowYSize/2,windowXSize/2,windowYSize/2);
	glutInitValues();
	*/

	// Set the base texture of our object
	myObject->setTexture(0,".//data//pink.ppm");
	// Set a second texture layer to our object
	myObject->setTexture(1,".//data//smile.ppm");

	/****************************************/
	/*         Here's the GLUI code         */
	/****************************************/
	GLUI *glui = GLUI_Master.create_glui("GLUI");
	// Animation panel
	GLUI_Panel *animation_panel = glui->add_panel("Animation");
	GLUI_Spinner *keyframe_spinner = new GLUI_Spinner( animation_panel, "keyframe", &keyframe, KEYFRAME_SPINNER_ID, control_cb);
    keyframe_spinner->set_float_limits( 0, 49 );
    glui->add_button("Set Keyframe", SET_KEYFRAME_ID, control_cb);
	glui->add_button("Play Animation", PLAY_ANIMATION_ID, control_cb);
	glui->add_button("Stop Animation", STOP_ANIMATION_ID, control_cb);

	// Render Panel
	GLUI_Panel *render_panel = glui->add_panel("Render");
	new GLUI_Checkbox(render_panel, "Wireframe", &wireframe);
	new GLUI_Checkbox(render_panel, "Filled", &filled);
	new GLUI_Checkbox(render_panel, "Grid", &grid);

	glui->add_button("Quit", 0, (GLUI_Update_CB)exit);

	glui->set_main_gfx_window(main_window);
	/* We register the idle callback with GLUI, *not* with GLUT */
	GLUI_Master.set_glutIdleFunc(myGlutIdle);
	GLUI_Master.set_glutMouseFunc( myGlutMouse );
	GLUI_Master.set_glutKeyboardFunc(myGlutKeyboard);
	
	// Start the GLUT processin loop
	glutMainLoop();

	return EXIT_SUCCESS;
}



