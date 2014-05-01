#define NUM_OPENGL_LIGHTS 8
#define _CRT_SECURE_NO_WARNINGS
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
#include "ppm.h"

using namespace std;

/** These are the live variables passed into GLUI ***/
int  isectOnly = 1;

int     camRotU = 0;
int     camRotV = 0;
int     camRotW = 0;
int  viewAngle = 45;
int  recursion = 0;
float eyeX = 2;
float eyeY = 2;
float eyeZ = 2;
float lookX = -2;
float lookY = -2;
float lookZ = -2;

/** These are GLUI control panel objects ***/
int  main_window;
string filenamePath = ".\\data\\tests\\all_effects.xml";
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

SceneColor getTextureColor(ScenePrimitive* prim, Point hitPoint){
    SceneFileMap* map = prim->material.textureMap;
    if(!map || !map->data->getWidth())return SceneColor();
    int s = (int)(map->repeatU * hitPoint[0] * map->data->getWidth()) % map->data->getWidth();
    int t = (int)(map->repeatV * hitPoint[1] * map->data->getHeight()) % map->data->getHeight();
    int pix = t*map->data->getWidth() + s;

    char* data = map->data->getPixels();
    SceneColor ret;
    ret.r = ((float)((unsigned char)data[pix*3])) / 255.0f;
    ret.g = ((float)((unsigned char)data[pix*3+1])) / 255.0f;
    ret.b = ((float)((unsigned char)data[pix*3+2])) / 255.0f;
    return ret;
}

SceneColor getColorFromRay(Point eyeP, Vector rayV, int depth){
    rayV.normalize();
    //cout << "rayV (" << rayV[0] << "," << rayV[1] << "," << rayV[2] << ")\n";
    float t = 1000000;
    float tempT;
    SceneColor color = SceneColor();//zeroes
    FlatSceneNode* current = parser->headNode;
    FlatSceneNode* closest = NULL;
    while(current != NULL){
        if(current->primitive != NULL){
            setShape(current->primitive->type);
            //current->invMat.print();
            tempT = shape->Intersect(eyeP, rayV, current->invMat);
    
            if((tempT >= 0.0) && (tempT < t)) {
                t = tempT;
                closest = current;
            }
        }
        current = current->next;
    }
    if(t != 1000000){
        SceneGlobalData globals;
        parser->getGlobalData(globals);
            setShape(closest->primitive->type);
            Point objEye = closest->invMat * eyeP;
            Vector objRay = closest->invMat * rayV;
            Vector objNorm = shape->findIsectNormal(objEye, objRay, t);
            //world space normal
            //cout<<"using normal "<<objNorm[0]<<" "<<objNorm[1]<<" "<<objNorm[2]<<"\n";
            Vector iNorm = transpose(closest->invMat) * objNorm;
            iNorm.normalize();
            //cout<<"using normal "<<iNorm[0]<<" "<<iNorm[1]<<" "<<iNorm[2]<<"\n";
            //cout<<"using ray "<<rayV[0]<<" "<<rayV[1]<<" "<<rayV[2]<<"\n\n";
            //ambient belongs here according to presentation
            color = color + (closest->primitive->material.cAmbient * globals.ka);
            //cout << "rgb " << color.r << "," << color.g << "," << color.b << "\n";
            SceneLightData light;
            Vector lightDir;
            float lightDist;
            Vector reflectiveRay;
            SceneColor diffConst = closest->primitive->material.cDiffuse * globals.kd;
            SceneColor diffColor = SceneColor();
            //cout<<"material diffuse "<<closest->primitive->material.cDiffuse.r<<" "<<closest->primitive->material.cDiffuse.g<<" "<<closest->primitive->material.cDiffuse.b<<"\n";
            SceneColor specConst = closest->primitive->material.cSpecular * globals.ks;
            SceneColor specColor = SceneColor();
            //cout<<"material specular "<<closest->primitive->material.cSpecular.r<<" "<<closest->primitive->material.cSpecular.g<<" "<<closest->primitive->material.cSpecular.b<<"\n";
            float specularF = closest->primitive->material.shininess;
            //cout << "rgb " << constant.r << "," << constant.g << "," << constant.b << "\n";
            SceneColor reflectiveConst = closest->primitive->material.cReflective * globals.ks;
            SceneColor reflectiveColor = SceneColor();
            Point hitPoint = eyeP + (rayV * t);
            hitPoint = hitPoint + iNorm * 0.0001;
            for(int k = 0; k < parser->getNumLights(); k++){
                parser->getLightData(k, light);
                if(light.type == LIGHT_DIRECTIONAL){
                    lightDir = -light.dir;//back towards where the light is
                    lightDist = 1e9;
                }
                else if(light.type == LIGHT_POINT){
                    lightDir = light.pos - hitPoint;
                    lightDist = lightDir.normalizeAndReturnDist();
                } else cout << "bad light type "<< light.type <<"\n";
                //see if this light hits
                current = parser->headNode;
                int blocked = 0;
                while(current != NULL && !blocked){
                    if(current->primitive != NULL){
                        setShape(current->primitive->type);
                        //current->invMat.print();
                        tempT = shape->Intersect(hitPoint, lightDir, current->invMat);
                        //cout<<tempT<<"\n";
                        //this fudge factor may actually be too large
                        if(tempT > 0.00001 && tempT < lightDist)blocked = 1;
                    }
                    current = current->next;
                }
                if(!blocked){
                    //diffuse component
                    //cout<<"hitPoint "<<hitPoint[0]<<" "<<hitPoint[1]<<" "<<hitPoint[2]<<"\n";
                    float dotProd = dot(lightDir,iNorm);
                    //cout<<"dot lightDir,iNorm "<<dotProd<<"\n";
                    //cout<<"diffConst "<<diffConst.r<<" "<<diffConst.g<<" "<<diffConst.b<<"\n";
                    if(dotProd < 0.0f)dotProd = 0.0f;
                    SceneColor contrib = (diffConst * dotProd) * light.color;
                    diffColor = diffColor + contrib;
                    //cout <<"diffuse contrib: "<<contrib.r<<" "<<contrib.g<<" "<<contrib.b<<"\n";
                    //color = color + contrib;
                    //specular
                    reflectiveRay = (2 * dotProd * iNorm) - lightDir;
                    reflectiveRay.normalize();
                    //cout<<"reflectiveRay "<<reflectiveRay[0]<<" "<<reflectiveRay[1]<<" "<<reflectiveRay[2]<<"\n";
                    dotProd = dot(reflectiveRay,-rayV);
                    //cout<<"dot reflectiveRay,rayV "<<dotProd<<"\n";
                    if(dotProd < 0.0f)dotProd = 0.0f;
                    contrib = specConst * (light.color * pow(dotProd,specularF));
                    //cout <<"specular contrib: "<<contrib.r<<" "<<contrib.g<<" "<<contrib.b<<"\n";
                    //color = color + contrib;
                    specColor = specColor + contrib;
                }
            }
            if(depth){
                //reflective ray
                float dotProd = dot(rayV,iNorm);
                reflectiveRay = rayV - (2 * dotProd * iNorm);
                //done higher
              //hitPoint = hitPoint + iNorm * 0.0001;
                reflectiveColor = reflectiveConst * getColorFromRay(hitPoint, reflectiveRay, depth-1);
                //cout <<"reflective color: "<<reflectiveColor.r<<" "<<reflectiveColor.g<<" "<<reflectiveColor.b<<"\n";
                //color = color + reflectiveColor;
            }
            if(closest->primitive->material.blend > 1e-10){
                float blend = closest->primitive->material.blend;
                setShape(closest->primitive->type);
                Point squarePoint = shape->iPointToSquare(objEye, objRay, t);
                diffColor = diffColor * (1.0-blend) + getTextureColor(closest->primitive, squarePoint) * (blend);
            }
            color = color + diffColor + specColor + reflectiveColor;
            //cout << "rgb " << color.r << "," << color.g << "," << color.b << "\n\n";
    }
    return color;
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

    //cout << "(w, h): " << pixelWidth << ", " << pixelHeight << endl;

    Point eyeP = camera->GetEyePoint();
    //cout << "eyePoint (" << eyeP[0] << "," << eyeP[1] << "," << eyeP[2] << ")\n";
    Point pointV;
    Vector rayV;    
    Matrix filmToWorld = camera->GetFilmToWorldMatrix();
    //cout << "Film To World\n";
    //filmToWorld.print();
    
    FlatSceneNode* current = parser->headNode;
    while(current != NULL){
        current->invMat = invert(current->matrix);
        current = current->next;
    }
    
    for (int i = 0; i < pixelWidth; i++) {
        for (int j = 0; j < pixelHeight; j++) {
            //setPixel(pixels, i, j, 0, 0, 0);
            
            pointV = Point(-1.0+2.0*i/ (double)pixelWidth,
                           -1.0+2.0*j/ (double)pixelHeight,-1.0);
            //cout<<"camspace "<<pointV[0]<<" "<<pointV[1]<<" "<<pointV[2]<<"\n";
            pointV = filmToWorld * pointV;
            //filmToWorld.print();
            //cout<<"worldspace "<<pointV[0]<<" "<<pointV[1]<<" "<<pointV[2]<<"\n";
            rayV = pointV - camera->GetEyePoint();
            
            SceneColor color = getColorFromRay(eyeP, rayV, recursion);
            color = color * 255.0;
            if(color.r > 255)color.r = 255;
            if(color.g > 255)color.g = 255;
            if(color.b > 255)color.b = 255;
            setPixel(pixels, i, j, color.r, color.g, color.b);
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

    main_window = glutCreateWindow("COMP 175 Assignment 5");
    glutDisplayFunc(myGlutDisplay);
    glutReshapeFunc(myGlutReshape);

    /****************************************/
    /*         Here's the GLUI code         */
    /****************************************/

    GLUI* glui = GLUI_Master.create_glui("GLUI");

    filenameTextField = new GLUI_EditText( glui, "Filename:", filenamePath);
    filenameTextField->set_w(300);
    GLUI_Spinner* recursionSpinner = glui->add_spinner("Recursion:",GLUI_SPINNER_INT,(void*)&recursion); 
    recursionSpinner->set_int_limits(0,11);
    glui->add_button("Load", 0, callback_load);
    glui->add_button("Start!", 0, callback_start);
    //glui->add_checkbox("Isect Only", &isectOnly);

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
    //GLUI_Master.set_glutIdleFunc(myGlutIdle);

    glutMainLoop();

    return EXIT_SUCCESS;
}



