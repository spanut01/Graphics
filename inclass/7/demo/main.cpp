/*  =================== File Information =================
        File Name: main.cpp
        Description:
        Author: Michael Shah, mostly

        Purpose: Driver for 3D program to load .ply models 
        ===================================================== */

#include <string.h>
#include <iostream>
#include <fstream>
#include <GL/glui.h>
#include <math.h>
#include "ply.h"
#include "Algebra.h"
#include <Windows.h>
#include <mmsystem.h>

/** These are the live variables passed into GLUI ***/
int main_window;
int radioInt = 0;
int  rotY = 0;
int  scale = 100;
double distFromEye = 10000;
double chosenD, closeT;
Point x1, x2;
//this is just a global we're using as a flag
int ball = 0;
//one at a time for now
Point ball_location; //where it is
Vector ball_trajectory; //where it's going towards
int hit = 0;
Point ballLoc; //where we actually put it if it hits
//how are these not already stored anywhere
float windowx, windowy;

/* This is a textbox that we can edit, we
        use it to 
*/
GLUI_EditText* filenameTextField = NULL;
string filenamePath = ".\\data\\bunny.ply";

/****************************************/
/*         PLY Object                   */
/****************************************/
ply* myPLY = new ply (filenamePath);

void findIntersection(){
	//x1[0] = -x1[0];
	x1[1] = -x1[1];
	//x2[0] = -x2[0];
	x2[1] = -x2[1];
	
	double angle = DEG_TO_RAD(rotY) + PI;
	if (angle > 2 * PI)angle -= 2 * PI;
	//printf("Angle: %f\n", angle);
	Matrix rotMat = rotY_mat(-angle);
	x1 = rotMat * x1;
	x2 = rotMat * x2;

	Point x0;
	//printf("Line is ");
	//printf("\n first: ");
	//x1.print();
	//printf("\n second: ");
	//x2.print();

	hit = 0; //global flag
	distFromEye = 10000;
	int i;
	for (i = 0; i < myPLY->vertexCount; i++){
		x0[0] = myPLY->vertexList[i].x;
		x0[1] = myPLY->vertexList[i].y;
		x0[2] = myPLY->vertexList[i].z;

		Vector numerator = cross(x2-x1,x1-x0);
		Vector denominator = x2 - x1;
		double d = numerator.length() / denominator.length();
		if(d < 0.05){
			//printf("Hit at ");
			//x0.print();
			//myPLY->vertexList[i].z = 0.0;
			Vector eyeToHit = x0 - x1;
			double dist = eyeToHit.length();
			if (dist < distFromEye){
				distFromEye = dist;
				chosenD = d;
				hit = 1;
			}
		}
	}
	if (hit) {
		closeT = sqrt(pow(distFromEye, 2) - pow(chosenD, 2));
		Vector line = x2 - x1;
		ballLoc = x1 + ((closeT / line.length()) * line);
	}
}

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

void myGlutMouse(int button, int button_state, int x, int y)
{
	//printf("button:%d, buttonstate:%d, x:%d, y:%d\n", button, button_state, x, y);
	
	if (button == 3 && button_state == 1){
		rotY += 5;
		while (rotY<0)rotY += 360;
		while (rotY>360)rotY -= 360;
	}
	if (button == 4 && button_state == 1){
		rotY -= 5;
		while (rotY<0)rotY += 360;
		while (rotY>360)rotY -= 360;
	}
	if (button == 0 && button_state == 1){
		//printf("button:%d, buttonstate:%d, x:%d, y:%d\n", button, button_state, x, y);
		if (!ball) {
			
			//PlaySound((".\\data\\pew-pew.wav"), NULL, SND_FILENAME | SND_ASYNC);
			ball = 1;
			ball_location = Point(0, 0, -0.75);//the camera
			ball_trajectory = Vector(1.5 * (-(float)x / windowx + 0.5), 1.5 * ((float)y / windowy - 0.5), 0.5);//not destination
			x1 = ball_location;
			x2 = ball_location + ball_trajectory;
			findIntersection();
		}
	}


	glutPostRedisplay();
}
/**************************************** myGlutReshape() *************/

/*
        We configure our window so that it correctly displays our objects
        in the correct perspective.
*/
void myGlutReshape(int x, int y)
{
	    windowx = x;
	    windowy = y;
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
        //glFrustum(-xy_aspect*.1, xy_aspect*.1, -.125, .125, .1, 15.0);
        //glOrtho(-xy_aspect*0.6, xy_aspect*0.6, -0.6, 0.6, -.05, 15.0);
        // Since we are in projection mode, here we are setting the camera to the origin (0,0,0)
        glTranslatef(0, 0, -1.0);
        // Call our display function.
        glutPostRedisplay();
}

/***************************************** myGlutDisplay() *****************/

void myGlutDisplay(void)
{
        // Clear the buffer of colors in each bit plane.
        // bit plane - A set of bits that are on or off (Think of a black and white image)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Set the mode so we are modifying our objects.
        glMatrixMode(GL_MODELVIEW);
        // Load the identify matrix which gives us a base for our object transformations
        // (i.e. this is the default state)
        glLoadIdentity();
		if(ball && ball_location[2] >= 0.25) {
			ball = 0; 
		}
		if(ball && hit){
			//printf("HIT\n");
			double angle = DEG_TO_RAD(rotY) + PI;
			if (angle > 2 * PI)angle -= 2 * PI;
			Matrix rotMat = rotY_mat(-angle);
			Point current = rotMat * ball_location;
			current[1] = -current[1];
			Vector fromDest = (current)-ballLoc;

			if (fromDest.length() < .2){
				Vector line = x2 - x1;
				Point x0;
				if (radioInt == 0){

					for (int i = 0; i < myPLY->vertexCount; i++){
						x0[0] = myPLY->vertexList[i].x;
						x0[1] = myPLY->vertexList[i].y;
						x0[2] = myPLY->vertexList[i].z;

						Vector ballToPt = ballLoc - x0;
						double d = ballToPt.length();
						if (d < 0.05){
							Vector numerator = cross(x2 - x1, x1 - x0);
							Vector denominator = x2 - x1;
							double d = numerator.length() / denominator.length();
							Vector tVec = x0 - x1;
							double t = sqrt(pow(tVec.length(), 2) - pow(d, 2)) - closeT;

							Point x0_t = x0 - ((t / line.length()) * line);
							double dist;

							dist = sqrt(0.05*0.05 - pow(d, 2));
							x0_t = x0_t + ((dist / line.length()) * line);
							if (dist > t)x0 = x0_t;
							myPLY->vertexList[i].x = x0[0];
							myPLY->vertexList[i].y = x0[1];
							myPLY->vertexList[i].z = x0[2];

						}
						else if (d < 0.15) {

							Vector numerator = cross(x2 - x1, x1 - x0);
							Vector denominator = x2 - x1;
							double d = numerator.length() / denominator.length();
							Vector tVec = x0 - x1;
							double t = sqrt(pow(tVec.length(), 2) - pow(d, 2)) - closeT;

							//Point x0_t = x0 - ((t / line.length()) * line);
							double dist;

							double x = (d - 0.08)*5;
							dist = ((log(-x) + 4)*exp(-x)) / 350;
							if(dist>0.0) x0 = x0 - ((dist / line.length()) * line);
							
							myPLY->vertexList[i].x = x0[0];
							myPLY->vertexList[i].y = x0[1];
							myPLY->vertexList[i].z = x0[2];
						}
					}
					PlaySound(TEXT(".\\data\\clay.wav"), NULL, SND_FILENAME | SND_ASYNC);
				}
				if (radioInt == 1){
					for (int i = 0; i < myPLY->vertexCount; i++){
					x0[0] = myPLY->vertexList[i].x;
					x0[1] = myPLY->vertexList[i].y;
					x0[2] = myPLY->vertexList[i].z;

					Vector ballToPt = ballLoc - x0;
					double d = ballToPt.length();
					if (d < 0.2){
						//myPLY->vertexList[i].z = 0.0;
						Vector tVec = x0 - x1;
						Vector numerator = cross(x2 - x1, x1 - x0);
						Vector denominator = x2 - x1;
						double d = numerator.length() / denominator.length(); 
						double t = sqrt(pow(tVec.length(), 2) - pow(d, 2)) - closeT;
						//Point x0_t = x0 - ((t / line.length()) * line);
						double dist;
						dist = (0.05 - d/2.0);
						if (dist > 0) x0 = x0 + ((dist / line.length()) * line);
						myPLY->vertexList[i].x = x0[0];
						myPLY->vertexList[i].y = x0[1];
						myPLY->vertexList[i].z = x0[2];
						}
					}
					PlaySound(TEXT(".\\data\\metal.wav"), NULL, SND_FILENAME | SND_ASYNC);

				}
				
				//printf("remove ball \n");
				ball = 0;
			}
		}
		if(ball){
			glPushMatrix();
			glEnable(GL_LIGHTING);
			double location[4];
			ball_location.unpack(location);
			//printf("Ball at %f,%f,%f\n", location[0], location[1], location[2]);
			glTranslated(-location[0], -location[1], -location[2]);
			glColor3f(1.0, 0, 0);
			glutSolidSphere(0.05, 10, 10);
			ball_location = ball_location + 0.1 * ball_trajectory;
			glPopMatrix();
		}


        //allow for user controlled rotation and scaling
        glScalef(scale / 100.0, scale / 100.0, scale / 100.0);
        glRotatef(rotY, 0.0, 1.0, 0.0);
        float rotRad = PI * (rotY / 180.0);
        myPLY->lookX = sinf(-rotRad);
        myPLY->lookZ = cosf(-rotRad);
        //cout<<"lookX: "<<myPLY->lookX<<"  lookZ: "<<myPLY->lookZ<<"\n";
        
        //draw the axes  //why?
		
        glLineWidth(1);
        /*glBegin(GL_LINES);
                glColor3f(1.0, 0.0, 0.0);
                glVertex3f(0, 0, 0); glVertex3f(1.0, 0, 0);
                glColor3f(0.0, 1.0, 0.0);
                glVertex3f(0, 0, 0); glVertex3f(0.0, 1.0, 0);
                glColor3f(0.0, 0.0, 1.0);
                glVertex3f(0, 0, 0); glVertex3f(0, 0, 1.0);
        glEnd();
        */
        glEnable(GL_LIGHTING);
        glEnable(GL_POLYGON_OFFSET_FILL);
        glColor3f(0.6, 0.6, 0.6);
        glPolygonMode(GL_FRONT, GL_FILL);
        myPLY->render();
        
        glutSwapBuffers();
}

/*  ==========================================
        Clean up all dynamically allocated memory
        ========================================== */
void onExit()
{
        delete myPLY;
}

/*   ==========================================
         Callback function
         A callback function is a function that is triggered
         by some operating system event (like clicking a button)
         and then running this function when an action has occurred.
         ========================================== */
void callback_load(int id) {
        
        if (filenameTextField == NULL) {
                return;
        }
        // 
        cout << "Loading new ply file from: " << filenameTextField->get_text() << endl;
        // Reload our model
        myPLY->reload(filenameTextField->get_text());
        // Print out the attributes
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
		windowx = 500;
		windowy = 500;
        glutInitWindowSize(500, 500);

        main_window = glutCreateWindow("AWESOMENESS!!!");
        glutDisplayFunc(myGlutDisplay);
        glutReshapeFunc(myGlutReshape);

        /****************************************/
        /*       Set up OpenGL lighting         */
        /****************************************/

        // Essentially set the background color of the 3D scene.
        //glClearColor(.9f, .9f, .9f, 1.0f);
        glClearColor(0.1, 0.1, 0.1, 1.0);
        glShadeModel(GL_FLAT);

        GLfloat light_pos0[] = { 0.0f, 0.0f, 1.0f, 0.0f };
        GLfloat diffuse[] = { 0.5f, 0.5f, 0.5f, 0.0f };
        GLfloat ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };

        glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
        glLightfv(GL_LIGHT0, GL_POSITION, light_pos0);

        //glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
        glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);

        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);

        /****************************************/
        /*          Enable z-buferring          */
        /****************************************/

        glEnable(GL_DEPTH_TEST);
        glPolygonOffset(1, 1);

        /****************************************/
        /*         Here's the GLUI code         */
        /****************************************/
		
        GLUI *glui = GLUI_Master.create_glui("GLUI");

        //GLUI_Panel *render_panel = glui->add_panel("Render");
        //new GLUI_Checkbox(render_panel, "Wireframe", &wireframe);
        //new GLUI_Checkbox(render_panel, "Filled", &filled);
        //new GLUI_Checkbox(render_panel, "Silhouette", &silhouette);

        //GLUI_Panel *camera_panel = glui->add_panel("Camera");
        //(new GLUI_Spinner(camera_panel, "Rotate Y:", &rotY))->set_int_limits(0, 359);
        //(new GLUI_Spinner(camera_panel, "Scale:", &scale))
        //        ->set_int_limits(1, 1000);
		GLUI_Panel *hit_panel = glui->add_panel("Dent Type");
		GLUI_RadioGroup *dents = glui->add_radiogroup_to_panel(hit_panel, &radioInt);
		glui->add_radiobutton_to_group(dents, "Clay");
		glui->add_radiobutton_to_group(dents, "Metal");

        filenameTextField = new GLUI_EditText( glui, "Filename:", filenamePath);
        filenameTextField->set_w(300);
        glui->add_button("Load PLY", 0, callback_load);


        glui->add_column(true);
        glui->add_button("Quit", 0, (GLUI_Update_CB)exit);
		
        glui->set_main_gfx_window(main_window);
        /* We register the idle callback with GLUI, *not* with GLUT */
        GLUI_Master.set_glutIdleFunc(myGlutIdle);
		GLUI_Master.set_glutMouseFunc(myGlutMouse);


        glutMainLoop();

        return EXIT_SUCCESS;
}



