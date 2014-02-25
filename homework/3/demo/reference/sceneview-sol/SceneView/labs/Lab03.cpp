#include "Common.h"
#include "Lab03.h"

#include <QMenuBar>
#include <qgl.h>
#include <iostream>
#include <QMouseEvent>
#include <QTimer>
#include <QFile>
#include <CS123Common.h>

// hack to get around windef.h on Windows
#undef near
#undef far


Lab3Canvas::Lab3Canvas(QWidget *parent, QMenuBar *menu)
: LabCanvas(parent, menu) , m_fps(30.0f), m_quadric(NULL)
{


    //You may want to add code here
    m_increment = 0;
    m_camera = new LabCamera();
    m_camera->eye.init((REAL)5, (REAL)0, (REAL)0);
    m_camera->center.init(0,0,0);
    m_camera->up.init(0,1,0);

    assert(EQ(m_camera->eye.data[0],5));
    assert(EQ(m_camera->eye.data[1],0));
    assert(EQ(m_camera->eye.data[2],0));

//    m_camera->eye.data[0] = 5;
//    m_camera->eye.data[1] = 0;
//    m_camera->eye.data[2] = 0;

//    m_camera->center.data[0] = 0;
//    m_camera->center.data[1] = 0;
//    m_camera->center.data[2] = 0;

//    m_camera->up.data[0] = 0;
//    m_camera->up.data[1] = 1;
//    m_camera->up.data[2] = 0;

    m_camera->angle = 45.0f, m_camera->near = 1.0f, m_camera->far = 1000.0f;
    m_renderTechnique = RENDER_SHADED;
    m_shadingModel = SHADE_FLAT;
    m_lightingEnabled = false;

    //Do not change below here
    QObject::connect(this, SIGNAL(_mouseDrag(QMouseEvent*)), this, SLOT(mouseDragEvent(QMouseEvent*)));
    setAutoBufferSwap(false);
    setFocusPolicy(Qt::StrongFocus);
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(redraw()));
    m_timer->start(1000.0f / m_fps);

}

Lab3Canvas::~Lab3Canvas()
{
    safeDelete(m_timer);
    safeDelete(m_camera);
    gluDeleteQuadric(m_quadric);
    makeCurrent();
}


void Lab3Canvas::applyRenderSettings()
{
    /* @TODO: [Lab 3] Change the render techinque */
    switch (m_renderTechnique) {
    case RENDER_POINT:
        break;
    case RENDER_WIREFRAME:
        break;
    case RENDER_SHADED:
        break;
    default:
        break; /* invalid shading model */
    }

    /* @TODO: [Lab 3] Change the shading model */
    switch (m_shadingModel) {
    case SHADE_FLAT:
        break;
    case SHADE_SMOOTH:
        break;
    default:
        break; /* invalid shading model */
    }

    /* @TODO: [Lab 3] Change the lighting model */
    if (m_lightingEnabled) {

    }
    else {

    }
}


/**
  Setup OpenGL here.  You should specify your render settings within this method.
  This method is called once when Qt first retreives the OpenGL context during the
  creation of this widget.
  **/
void Lab3Canvas::initializeGL()
{
    m_quadric = gluNewQuadric();

    /* Open GL Render Settings */
    applyRenderSettings();

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

    /* Setup Local Lighting */

    GLfloat ambientLight[] = {0.1f, 0.1f, 0.1f, 1.0f};
    GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0, 1.0f };
    GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat position[] = { 2, 2, 2, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    /* Material Properties Code Here*/
    GLfloat shiny = 25;
    GLfloat ambientMat[] = {0.0f, 0.0f, 0.0f, 0.0f};
    GLfloat diffuseMat[] = { 0.0f, 0.0f, 0.0, 0.0f };
    GLfloat specularMat[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientMat);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseMat);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularMat);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &shiny);
    glClearColor(0, 0, 0, 0); //Set the color to clear buffers to
}

/**
    The main drawing method which will be called 30 frames per second.  Here
    is where your OpenGL drawing should occur.
**/
void Lab3Canvas::paintGL()
{
    // Get the time in seconds
    float time = m_increment++ / (float)m_fps;

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
void Lab3Canvas::resizeGL(int w, int h)
{
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
void Lab3Canvas::updateCamera()
{
    float w = width(), h = height();
    float ratio = 1.0f * w / h;
    //Fill in camera stuff here
}


void Lab3Canvas::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    { //Left click
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

    }
    else if (event->button() == Qt::RightButton)
    { //Right click

    }
    else if (event->button() == Qt::MidButton)
    { //Mid click

    }
}

void Lab3Canvas::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton || event->buttons() & Qt::RightButton) //Mouse drag - don't modify this
        emit _mouseDrag(event);
    else if (event->buttons() & Qt::NoButton) {

    }
}


void Lab3Canvas::mouseDragEvent(QMouseEvent *event)
{

}

void Lab3Canvas::keyPressEvent(QKeyEvent *event)
{
    if(event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Q) {
        exit(0);
    }
}


/**
    Specifies to Qt what to do when the widget needs to be redrawn.  We only
    want to repaint the onscreen objects.

    You should not need to modify this method.

**/
void Lab3Canvas::redraw()
{
    repaint();
}

void Lab3Canvas::settingsChanged()
{
    // @TODO: [OPTIONAL] Fill this in if you like...
}
