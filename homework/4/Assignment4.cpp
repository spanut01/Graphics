#define NUM_OPENGL_LIGHTS 8

#include <iostream>
#include <fstream>
#include <string>
#include <GL/glui.h>
#include "Shape.h"
#include "Cube.h"
#include "Cylinder.h"
#include "Cone.h"
#include "Sphere.h"
#include "SceneParser.h"
#include "Camera.h"

using namespace std;

/** These are the live variables passed into GLUI ***/
int  isectOnly = 1;

int	 camRotU = 0;
int	 camRotV = 0;
int	 camRotW = 0;
int  viewAngle = 45;
float eyeX = 2;
float eyeY = 2;
float eyeZ = 2;
float lookX = -2;
float lookY = -2;
float lookZ = -2;

/** These are GLUI control panel objects ***/
int  main_window;
string filenamePath = "./data/general/test.xml";
GLUI_EditText* filenameTextField = NULL;
GLubyte* pixels = NULL;
int pixelWidth = 0, pixelHeight = 0;
int screenWidth = 0, screenHeight = 0;

/** these are the global variables used for rendering **/
Shape* shape = NULL;
Cube* cube = new Cube();
Cylinder* cylinder = new Cylinder();
Cone* cone = new Cone();
Sphere* sphere = new Sphere();
SceneParser* parser = NULL;
Camera* camera = new Camera();

void setupCamera();
void updateCamera();

void setPixel(GLubyte* buf, int x, int y, int r, int g, int b) {
	buf[(y*pixelWidth + x) * 3 + 0] = (GLubyte)r;
	buf[(y*pixelWidth + x) * 3 + 1] = (GLubyte)g;
	buf[(y*pixelWidth + x) * 3 + 2] = (GLubyte)b;
}

void setShape(int shapeType){
	switch (shapeType) {
	case SHAPE_CUBE:
		shape = cube;
		break;
	case SHAPE_CYLINDER:
		shape = cylinder;
		break;
	case SHAPE_CONE:
		shape = cone;
		break;
	case SHAPE_SPHERE:
		shape = sphere;
		break;
	case SHAPE_SPECIAL1:
		shape = cube;
		break;
	default:
		shape = sphere;
	}
}

void callback_start(int id) {
    cout << "start button clicked!" << endl;

    if (parser == NULL) {
        cout << "no scene loaded yet" << endl;
        return;
    }

    pixelWidth = screenWidth;
    pixelHeight = screenHeight;

    updateCamera();

    if (pixels != NULL) {
        delete pixels;
    }
    pixels = new GLubyte[pixelWidth  * pixelHeight * 3];
    memset(pixels, 0, pixelWidth  * pixelHeight * 3);

    cout << "(w, h): " << pixelWidth << ", " << pixelHeight << endl;

    Point eyeP = camera->GetEyePoint();
    cout << "eyePoint (" << eyeP[0] << "," << eyeP[1] << "," << eyeP[2] << ")\n";
    Point pointV;
    Vector rayV;    
    Matrix filmToWorld = camera->GetFilmToWorldMatrix();
    cout << "Film To World\n";
    filmToWorld.print();
    double t;// = 1000000;
    double tempT;
    FlatSceneNode* closest;
    
    FlatSceneNode* current = parser->headNode;
    while(current != NULL){
        current->invMat = invert(current->matrix);
        current = current->next;
    }
    SceneGlobalData globals;
    parser->getGlobalData(globals);
    
    for (int i = 0; i < pixelWidth; i++) {
        for (int j = 0; j < pixelHeight; j++) {
            setPixel(pixels, i, j, 0, 0, 0);
            
            pointV = Point(-1.0+2.0*((double)i/(double)pixelWidth),-1.0+2.0*((double)j/(double)pixelHeight),-1.0);
            pointV = filmToWorld * pointV;
            rayV = pointV - camera->GetEyePoint();
            rayV.normalize();
            //cout << "rayVector (" << rayV[0] << "," << rayV[1] << "," << rayV[2] << ")\n";
            t = 1000000;
            current = parser->headNode;
            while(current != NULL){
                if(current->primitive != NULL){
                    setShape(current->primitive->type);
                    tempT = shape->Intersect(eyeP, rayV, current->invMat);
            
                    if((tempT >= 0.0) && (tempT < t)) {
                        t = tempT;
                        closest = current;
                    }
                }
                current = current->next;
            }
            if(t != 1000000){
                if (isectOnly){
                    setPixel(pixels, i, j, 255, 255, 255);
                }
                else{
                    setShape(closest->primitive->type);
                    Point objEye = closest->invMat * eyeP;
                    Vector objRay = closest->invMat * rayV;
                    Vector iNorm = shape->findIsectNormal(objEye, objRay, t);
                    iNorm = transpose(closest->invMat) * iNorm;
                    
                    SceneColor color = SceneColor();
                    color = color + (closest->primitive->material.cAmbient * globals.ka);
                    //cout << "rgb " << color.r << "," << color.g << "," << color.b << "\n";
                    SceneLightData light;
                    Vector lightDir;
                    SceneColor constant = closest->primitive->material.cDiffuse * globals.kd;
                    //cout << "rgb " << constant.r << "," << constant.g << "," << constant.b << "\n";
                    for(int k = 0; k < parser->getNumLights(); k++){
                        parser->getLightData(k, light);
                        if(light.type == LIGHT_DIRECTIONAL)lightDir = light.dir;
                        else if(light.type == LIGHT_POINT){
                            lightDir = light.pos - (eyeP + (rayV * t));
                            lightDir.normalize();
                        } else cout << "bad light type\n";
                        float dotProd = dot(lightDir,iNorm);
                        if(dotProd > 0.0)color = color + (constant * light.color * dotProd);
                    }
                    color = color * 255.0;
                    if(color.r > 255)color.r = 255;
                    if(color.g > 255)color.g = 255;
                    if(color.b > 255)color.b = 255;
                    //cout << "rgb " << color.r << "," << color.g << "," << color.b << "\n";
                    setPixel(pixels, i, j, color.r, color.g, color.b);
                }
            }
        }
    }
    glutPostRedisplay();
}



void callback_load(int id) {
	char curDirName [2048];
	if (filenameTextField == NULL) {
		return;
	}
	printf ("%s\n", filenameTextField->get_text());

	if (parser != NULL) {
		delete parser;
	}
	parser = new SceneParser (filenamePath);
	cout << "success? " << parser->parse() << endl;

	setupCamera();
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
	camera->SetScreenSize(x, y);

	screenWidth = x;
	screenHeight = y;

	glutPostRedisplay();
}


/***************************************** setupCamera() *****************/
void setupCamera()
{
	SceneCameraData cameraData;
	parser->getCameraData(cameraData);

	camera->Reset();
	camera->SetViewAngle(cameraData.heightAngle);
    Point pos = cameraData.pos;
    Vector look = cameraData.look;
    Point lookAt = cameraData.lookAt;
    Vector up = cameraData.up;
	if (cameraData.isDir == true) {
		camera->Orient(pos, look, up);
	}
	else {
		camera->Orient(pos, lookAt, up);
	}

	viewAngle = camera->GetViewAngle();
	Point eyeP = camera->GetEyePoint();
	Vector lookV = camera->GetLookVector();
	eyeX = eyeP[0];
	eyeY = eyeP[1];
	eyeZ = eyeP[2];
	lookX = lookV[0];
	lookY = lookV[1];
	lookZ = lookV[2];
	camRotU = 0;
	camRotV = 0;
	camRotW = 0;
	GLUI_Master.sync_live_all();
}

void updateCamera()
{
	camera->Reset();

	Point guiEye (eyeX, eyeY, eyeZ);
	Point guiLook(lookX, lookY, lookZ);//NOTE SHOULD THIS BE A VECTOR???
    Vector guiUp = camera->GetUpVector();
	camera->SetViewAngle(viewAngle);
	camera->Orient(guiEye, guiLook, guiUp);
	camera->RotateU(camRotU);
	camera->RotateV(camRotV);
	camera->RotateW(camRotW);
}

/***************************************** myGlutDisplay() *****************/

void myGlutDisplay(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	if (parser == NULL) {
		return;
	}

	if (pixels == NULL) {
		return;
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glDrawPixels(pixelWidth, pixelHeight, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	glutSwapBuffers();
}

void onExit()
{
	delete cube;
	delete cylinder;
	delete cone;
	delete sphere;
	delete camera;
	if (parser != NULL) {
		delete parser;
	}
	if (pixels != NULL) {
		delete pixels;
	}
}

/**************************************** main() ********************/

int main(int argc, char* argv[])
{
	atexit(onExit);

	/****************************************/
	/*   Initialize GLUT and create window  */
	/****************************************/

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(500, 500);

	main_window = glutCreateWindow("COMP 175 Assignment 4");
	glutDisplayFunc(myGlutDisplay);
	glutReshapeFunc(myGlutReshape);

	/****************************************/
	/*         Here's the GLUI code         */
	/****************************************/

	GLUI* glui = GLUI_Master.create_glui("GLUI");

	filenameTextField = new GLUI_EditText( glui, "Filename:", filenamePath);
	filenameTextField->set_w(300);
	glui->add_button("Load", 0, callback_load);
	glui->add_button("Start!", 0, callback_start);
	glui->add_checkbox("Isect Only", &isectOnly);
	
	GLUI_Panel *camera_panel = glui->add_panel("Camera");
	(new GLUI_Spinner(camera_panel, "RotateV:", &camRotV))
		->set_int_limits(-179, 179);
	(new GLUI_Spinner(camera_panel, "RotateU:", &camRotU))
		->set_int_limits(-179, 179);
	(new GLUI_Spinner(camera_panel, "RotateW:", &camRotW))
		->set_int_limits(-179, 179);
	(new GLUI_Spinner(camera_panel, "Angle:", &viewAngle))
		->set_int_limits(1, 179);

	glui->add_column_to_panel(camera_panel, true);

	GLUI_Spinner* eyex_widget = glui->add_spinner_to_panel(camera_panel, "EyeX:", GLUI_SPINNER_FLOAT, &eyeX);
	eyex_widget->set_float_limits(-10, 10);
	GLUI_Spinner* eyey_widget = glui->add_spinner_to_panel(camera_panel, "EyeY:", GLUI_SPINNER_FLOAT, &eyeY);
	eyey_widget->set_float_limits(-10, 10);
	GLUI_Spinner* eyez_widget = glui->add_spinner_to_panel(camera_panel, "EyeZ:", GLUI_SPINNER_FLOAT, &eyeZ);
	eyez_widget->set_float_limits(-10, 10);

	GLUI_Spinner* lookx_widget = glui->add_spinner_to_panel(camera_panel, "LookX:", GLUI_SPINNER_FLOAT, &lookX);
	lookx_widget->set_float_limits(-10, 10);
	GLUI_Spinner* looky_widget = glui->add_spinner_to_panel(camera_panel, "LookY:", GLUI_SPINNER_FLOAT, &lookY);
	looky_widget->set_float_limits(-10, 10);
	GLUI_Spinner* lookz_widget = glui->add_spinner_to_panel(camera_panel, "LookZ:", GLUI_SPINNER_FLOAT, &lookZ);
	lookz_widget->set_float_limits(-10, 10);

	glui->add_button("Quit", 0, (GLUI_Update_CB)exit);

	glui->set_main_gfx_window(main_window);

	/* We register the idle callback with GLUI, *not* with GLUT */
	GLUI_Master.set_glutIdleFunc(myGlutIdle);

	glutMainLoop();

	return EXIT_SUCCESS;
}



