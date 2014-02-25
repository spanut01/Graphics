#include "Common.h"
#include "Lab04.h"

#include <CS123Common.h>

#include <qgl.h>
#include <QMouseEvent>
#include <QTimer>
#include <QFile>
#include <cmath>
#include <iostream>

// hack to get around windef.h on Windows
#undef near
#undef far

Lab4Canvas::Lab4Canvas(QWidget *parent, QMenuBar *menu) 
: LabCanvas(parent, menu) , mFPS(30.0f), m_quadric(NULL)
{

    m_increment = 0;
    m_renderTechnique = RENDER_SHADED;
    m_shademodel = SHADE_SMOOTH;
    m_lightingEnabled = true;
    m_camera = new LabCamera();
    m_camera->eye.x = 0.0f, m_camera->eye.y = 0.0f, m_camera->eye.z = 10.0f;
    m_camera->center.x = 0.0f, m_camera->center.y = 0.0f, m_camera->center.z = 0.0f;
    m_camera->up.x = 0.0f, m_camera->up.y = 1.0f, m_camera->up.z = 0.0f;
    m_camera->angle = 45.0f, m_camera->near = 1.0f, m_camera->far = 1000.0f;

    //Feel free to create your own fields


    //Do not change below here
    QObject::connect(this, SIGNAL(_mouseDrag(QMouseEvent*)), this, SLOT(mouseDragEvent(QMouseEvent*)));
    setAutoBufferSwap(false);
    setFocusPolicy(Qt::StrongFocus);
    mTimer = new QTimer(this);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(redraw()));
    mTimer->start(1000.0f / mFPS);

}

Lab4Canvas::~Lab4Canvas() 
{
    safeDelete(mTimer);
    safeDelete(m_camera);
    gluDeleteQuadric(m_quadric);
    makeCurrent();
}

void Lab4Canvas::applyRenderSettings() 
{

    //Change the render technique
    if(m_renderTechnique == RENDER_POINT) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); //Render vertices only
    } else if (m_renderTechnique == RENDER_WIREFRAME) { //Render wireframe only
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else if (m_renderTechnique == RENDER_SHADED) { //Render with shading
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    //Change the shade model
    if(m_shademodel == SHADE_FLAT) {
        glShadeModel(GL_FLAT);
    } else if(m_shademodel == SHADE_SMOOTH) {
        glShadeModel(GL_SMOOTH);
    }

    //Change the lighting
    m_lightingEnabled ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);
}


/**
  Setup OpenGL here.  You should specify your render settings within this method.
  This method is called once when Qt first retreives the OpenGL context during the
  creation of this widget.
  **/
void Lab4Canvas::initializeGL() {
    m_quadric = gluNewQuadric();
    /* Open GL Render Settings */
    applyRenderSettings();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE); //set material properties which will be assigned by glColor
    glCullFace (GL_BACK);
    glEnable (GL_CULL_FACE);


    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

    /* Setup Global Lighting */

    GLfloat global_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f }; //OpenGL defaults to 0.2, 0.2, 0.2, 1.0
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);

    GLfloat ambientLight[] = {0.1f, 0.1f, 0.1f, 1.0f};
    GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0, 1.0f };
    GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat position[] = { 2, 2, 2, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT0, GL_POSITION, position);


    glClearColor(0, 0, 0, 0); //Set the color to clear buffers to

    glEnable(GL_LIGHTING); //Enable lighting
    m_lightingEnabled = true;
    glEnable(GL_LIGHT0);
    //if you add more lights, remember to enable them
}

/**
    The main drawing method which will be called 30 frames per second.  Here
    is where your OpenGL drawing should occur.
**/
void Lab4Canvas::paintGL() {
    // Get the time in seconds
    float time = m_increment++ / (float)mFPS;
    // Clear the color and depth buffers to the current glClearColor
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Swap the buffers to show what we have just drawn onto the screen
    swapBuffers();

    glFlush(); //Usually a good idea to call this
}

/**
    Resets the camera to fit the new viewport when the user resizes
    the window.  (Avoids object distortion)

    You should not need to change this
**/
void Lab4Canvas::resizeGL(int w, int h) {
    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    if(h == 0) h = 1;
    float ratio = 1.0f * w / h;

    // Set the viewport to be the entire window (you probably want to keep this)
    glViewport(0, 0, w, h);
    updateCamera();
}
/**
  Update the camera's specifications.
  You need to fill in this method.
  It gets called in resizeGL which get called automatically on intialization
  and whenever the window is resized.
  **/
void Lab4Canvas::updateCamera() {
    float w = width(), h = height();
    float ratio = 1.0f * w / h;
    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(m_camera->angle, ratio, m_camera->near, m_camera->far);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(m_camera->eye.x, m_camera->eye.y, m_camera->eye.z,
              m_camera->center.x, m_camera->center.y, m_camera->center.z,
              m_camera->up.x, m_camera->up.y, m_camera->up.z);
}



void Lab4Canvas::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton) { //Left click
        //This finds the world space coordinates of you mouse click
        //assuming the film plane is at z=1
        GLint viewport[4];
        GLdouble mvmatrix[16], projmatrix[16];
        GLint realy; /* OpenGL y coordinate position */
        GLdouble wx, wy, wz; /* returned world x, y, z coords     */
        glGetIntegerv (GL_VIEWPORT, viewport);
        glGetDoublev (GL_MODELVIEW_MATRIX, mvmatrix);
        glGetDoublev (GL_PROJECTION_MATRIX, projmatrix);
        realy = viewport[3] - (GLint) event->y() - 1;
        gluUnProject ((GLdouble) event->x(), (GLdouble) realy, 1.0,
                      mvmatrix, projmatrix, viewport, &wx, &wy, &wz);

    } else if (event->button() == Qt::RightButton) { //Right click

    } else if (event->button() == Qt::MidButton) { //Mid click

    }
}

void Lab4Canvas::mouseMoveEvent(QMouseEvent *event) {
    if(event->buttons() & Qt::LeftButton || event->buttons() & Qt::RightButton) //Mouse drag - don't modify this
        emit _mouseDrag(event);
    else if (event->buttons() & Qt::NoButton) {

    }
}


void Lab4Canvas::mouseDragEvent(QMouseEvent *event) {

}

void Lab4Canvas::keyPressEvent(QKeyEvent *event) {
    if(event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Q) {
        exit(0);
    }
}

/**
    Specifies to Qt what to do when the widget needs to be redrawn.  We only
    want to repaint the onscreen objects.

    You should not need to modify this method.

**/
void Lab4Canvas::redraw() {
    repaint();
}

void Lab4Canvas::settingsChanged()
{
    // @TODO: [OPTIONAL] Fill this in if you like...
}

