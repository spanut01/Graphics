#include "Common.h"
#include "Lab05.h"

#include <qgl.h>
#include <GL/glext.h>
#include <iostream>

#include <QMouseEvent>
#include <QTimer>
#include <QFile>

#include <cmath>

using namespace std;

// hack to get around windef.h on Windows
#undef near
#undef far

Lab5Canvas::Lab5Canvas(QWidget *parent, QMenuBar *menu) : LabCanvas(parent, menu) , m_fps(30.0f) {

    //You may want to add code here
    m_textureid = 0;
    m_quadric = gluNewQuadric();
    m_rendernormals = false;
    m_renderTechnique = RENDER_SHADED;
    m_shadingModel = SHADE_SMOOTH;
    m_lightingEnabled = true;
    m_camera = new LabCamera();
    m_camera->eye.x = 0.0f, m_camera->eye.y = -10, m_camera->eye.z = 20.0f;
    m_camera->center.x = 0.0f, m_camera->center.y = 0.0f, m_camera->center.z = 0.0f;
    m_camera->up.x = 0.0f, m_camera->up.y = 1.0f, m_camera->up.z = 0.0f;
    m_camera->angle = 45.0f, m_camera->near = 1.0f, m_camera->far = 1000.0f;

    //Do not change below here
    QObject::connect(this, SIGNAL(_mouseDrag(QMouseEvent*)), this, SLOT(mouseDragEvent(QMouseEvent*)));
    setAutoBufferSwap(false);
    setFocusPolicy(Qt::StrongFocus);
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(redraw()));
    m_timer->start(1000.0f / m_fps);

    //A variable that gets used in determining how much a vertex gets perturbed by. The larger the value, the more the perturbation will decrease per recursive value
    m_decay = 2;

    //the number of levels of recursion your terrain uses (can be considered the level of detail of your terrain)
    m_depth = 8;

    //The roughness of your terrain. Higher roughnesses ental taller mountains and deepers valleys. Lower values entail hills and shallow valleys
    m_roughness = 6;

    //seed a random number, your terrain will change based on what number you seed
    srand(0);


    //imagining your terrain as a square grid of points, gridLength is the number of elements on 1 side, or the length of a side of the grid
    m_gridlength = (1 << m_depth)+1;

    //knowing the length of a side of the grid we know the total number of elements in the grid is just
    int terrain_array_size = m_gridlength * m_gridlength;

    //an array representing all of our vertices, picture this as a grid of vertices
    m_terrain = new Vector3[terrain_array_size];

    //an array representing all the normals at each vertex
    m_normalmap = new Vector3[terrain_array_size];


    //Render normals button
    QPushButton* b2 = makeButton("Rend. Normal");
    b2->setCheckable(true);
    connect(b2, SIGNAL(clicked()), this, SLOT(toggleRender()));
}


//destructor
Lab5Canvas::~Lab5Canvas() {
    safeDelete(m_timer);
    safeDelete(m_camera);
    delete[] m_terrain;
    delete[] m_normalmap;
    gluDeleteQuadric(m_quadric);
    if (m_textureid != 0)
        releaseTexture(m_textureid);
    makeCurrent();
}

//YOU ONLY HAVE TO DEAL WITH THESE NEXT 3 (maybe 4) METHODS

/**
  You need to fill this in...
  Takes in the grid coordinates of the top left corner and the bottom right corner of your square
  Finds the vertices at the midpoints of each side and the center (as the algorithm describes) and then recurs on each
  of the four subsquares created
  **/
void Lab5Canvas::subdivideSquare(const GridPoint &topleft, const GridPoint &botright, GLint curDepth){

    // TL--TM--TR    +---> x
    // |   |   |     |
    // ML--MM--MR    V
    // |   |   |     y
    // BL--BM--BR

    // corner coordinates (in the grid space [x,y])
    GridPoint TL = GridPoint(topleft.x, topleft.y);
    GridPoint TR = GridPoint(botright.x, topleft.y);
    GridPoint BL = GridPoint(topleft.x, botright.y);
    GridPoint BR = GridPoint(botright.x, botright.y);


    // corner vertices on the terrain (in the grid space [x,y,z])
    Vector3 &v1 = m_terrain[getIndex(TL)];
    Vector3 &v2 = m_terrain[getIndex(TR)];
    Vector3 &v3 = m_terrain[getIndex(BL)];
    Vector3 &v4 = m_terrain[getIndex(BR)];

    //
    // @TODO: [Lab 5] Above, we demonstrate how to find
    //                  a) the coordinates in the grid of the corner vertices (in grid space)
    //                  b) the actual (X,Y,Z) points in the terrain map
    //
    //         Now, you just need to compute the midpoints in grid space, average the
    //         heights of the neighboring vertices, and write the new points to the
    //         terrain map. Remember to perturb the new center vertex by a random amount.
    //         Scale this perturbation by curDepth so that the perturbation factor is less
    //         as you recur deeper.
    //
    //         Remember that [X,Y] grid points map to [X,Z] in world space -- the Y
    //         coordinate in world space will be used for the height.
    //
    //         Once you compute these remaining 5 points, you will need to recur on the
    //         four sub-squares you've just created. To do this, you'll call recurOnSquare
    //         recursively, decrementing curDepth by one.
    //

}


/**
  populates all the normals...you need to fill this in
  **/
void Lab5Canvas::computeNormals()
{
    // For each vertex in the 2D grid...
    for (int a = 0; a < m_gridlength; a++) {
        for (int b = 0; b < m_gridlength; b++) {

            const GridPoint grid_position(a, b);                        // grid position
            const int       terrain_index   = getIndex(grid_position);  // index into the 1D array
            const Vector3   &vert_position  = m_terrain[terrain_index]; // vertex position

            // Get the neighbors of the vertex at (a,b)
            const QList<int> &neighbors    = getSurroundingVertices(grid_position); // returns indices into m_terrain
            const int        n_neighbors   = neighbors.size();

            // @TODO: [Lab 5] Compute a list of vectors from vert_position to each neighbor in neighbors
            Vector3 *vectors = new Vector3[n_neighbors];
            for (int i = 0; i < n_neighbors; ++i)
                vectors[i] = Vector3::zero();

            // @TODO: [Lab 5] Compute cross products for each neighbor
            Vector3 *normals = new Vector3[n_neighbors];
            for (int i = 0; i < n_neighbors; ++i)
                normals[i] = Vector3::zero();



            // Average the normals and store this final normal in the normal map
            Vector3 sum = Vector3::zero();
            for (int i = 0; i < n_neighbors; ++i)
                sum += normals[i];
            m_normalmap[terrain_index] = sum.getNormalized();

            delete[] vectors;
            delete[] normals;

        }
    }
    //end ta code
}




/**
    The main drawing method which will be called 30 frames per second.  Here
    is where your OpenGL drawing should occur.
**/
void Lab5Canvas::paintGL() {
    // Get the time in seconds

    float time = m_increment++ / (float)m_fps;
    // Clear the color and depth buffers to the current glClearColor
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Push a new matrix onto the stack for modelling transformations
    glPushMatrix();

    //TESSELATE YOUR TERRAIN HERE

    //draw normals
    drawNormals();
    // Discard the modelling transformations (leaving only camera settings)
    glPopMatrix();
    // Swap the buffers to show what we have just drawn onto the screen
    swapBuffers();
    glFlush(); //Usually a good idea to call this

}

/**
  returns a random value to perturb a vertex by based on an inputed level of depth
  Feel free to modify this
  **/
double Lab5Canvas::getPerturb(int cur_depth){
    double toreturn = m_roughness*pow(((double)cur_depth/m_depth), m_decay)*((rand()%200-100)/100.0);
    return toreturn;
}


/****************************************************************************************************************/
/**********************************DO NOT MODIFY ANY OF THE FOLLOWING CODE***************************************/
/****************************************************************************************************************/
/**
  initializes the height map's corner values (use mSize), fills the map...you need to fill this in

  Starts by intializing the four corners of the terrain, calls the recurOnSquare recursive method that performs the actual populating
  of the terrain array
  **/
void Lab5Canvas::populateTerrain()
{
    Vector3 tl(-10, 10, 0);
    Vector3 tr(10, 10, 0);
    Vector3 bl(-10, -10, 0);
    Vector3 br(10, -10, 0);
    GridPoint tlg(0,0);
    GridPoint trg(0,m_gridlength-1);
    GridPoint blg(m_gridlength-1, 0);
    GridPoint brg(m_gridlength-1, m_gridlength-1);
    m_terrain[getIndex(tlg)] = tl;
    m_terrain[getIndex(trg)] = tr;
    m_terrain[getIndex(blg)] = bl;
    m_terrain[getIndex(brg)] = br;
    subdivideSquare(tlg, brg, m_depth);
}


/**
  takes in a coordinate, (row, col),
  outputs -1 if the coordinate is not valid,
  the corresponding index in an array otherwise, give the width of
  each row, mSize

  If you prefer to work in using (row,col) coordinate you will find this method useful for converting such a pair
  into the corresponding index in a 1D array
  **/
inline int Lab5Canvas::getIndex(const GridPoint &c){
    if (c.x < 0 || c.x >= m_gridlength || c.y < 0 || c.y >= m_gridlength){
        return -1;
    }
    return c.x*m_gridlength+c.y;
}






/** All of the following methods are to be used in finding the normals**/

/**
    Finds all vectors from vertex at grid coordinate (i,j) to surrounding vectors.
    These vectors are stored in vecs.
    Returns a the number of resulting vectors that aren't(0,0,0)(edge cases)
**/
QList<int> Lab5Canvas::getSurroundingVertices(const GridPoint &coordinate){
    int index;
    QList<int> vecs;
    GridPoint coords[8];
    coords[0] = GridPoint(coordinate.x, coordinate.y-1);
    coords[1] = GridPoint(coordinate.x+1, coordinate.y-1);
    coords[2] = GridPoint(coordinate.x+1, coordinate.y);
    coords[3] = GridPoint(coordinate.x+1, coordinate.y+1);
    coords[4] = GridPoint(coordinate.x,   coordinate.y+1);
    coords[5] = GridPoint(coordinate.x-1, coordinate.y+1);
    coords[6] = GridPoint(coordinate.x-1, coordinate.y);
    coords[7] = GridPoint(coordinate.x-1, coordinate.y-1);
    for (int i = 0; i < 8; i++){
        index = getIndex(coords[i]);
        if (index != -1) {
            vecs.push_back(index);
        }
    }
    return vecs;
}


/**
  draws normals...
**/
void Lab5Canvas::drawNormals(){
    if (m_rendernormals){
        glColor3f(1,0,0);
        for (int row = 0; row < m_gridlength;row++){
            for (int column = 0; column < m_gridlength;column++){
                glBegin(GL_LINES);
                Vector3 curVert = m_terrain[row*m_gridlength + column];
                Vector3 curNorm = m_normalmap[row*m_gridlength + column];
                glVertex3f(curVert.x, curVert.y, curVert.z);
                glVertex3f(curVert.x +curNorm.x,
                           curVert.y + curNorm.y,
                           curVert.z + curNorm.z);
                glEnd();
            }
        }
    }

}



void Lab5Canvas::applyRenderSettings() {

    //Change the render technique
    if(m_renderTechnique == RENDER_POINT) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); //Render vertices only
    } else if (m_renderTechnique == RENDER_WIREFRAME) { //Render wireframe only
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else if (m_renderTechnique == RENDER_SHADED) { //Render with shading
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    //Change the shade model
    if(m_shadingModel == SHADE_FLAT) {
        glShadeModel(GL_FLAT);
    } else if(m_shadingModel == SHADE_SMOOTH) {
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
void Lab5Canvas::initializeGL() {
    /* Open GL Render Settings */
    applyRenderSettings();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE); //set material properties which will be assigned by glColor
    glCullFace (GL_BACK);
    glEnable (GL_CULL_FACE);

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    /* Setup Global Lighting */

    GLfloat global_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f }; //OpenGL defaults to 0.2, 0.2, 0.2, 1.0
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);

    /* Setup Local Lighting */

    GLfloat ambientLight[] = {0.1f, 0.1f, 0.1f, 1.0f};
    GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0, 1.0f };
    GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat position[] = { 0, 0, 10, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    glClearColor(0, 0, 0, 0); //Set the color to clear buffers to


    glEnable(GL_LIGHT0);

    //Here we make a call to loadTexture (defined at the bottom of Lab05.cpp) to load an image, feel free to choose your own image
    m_textureid = loadTexture("/course/cs123/data/image/BoneHead.jpg");

    populateTerrain();
    computeNormals();

}




/**
    Resets the camera to fit the new viewport when the user resizes
    the window.  (Avoids object distortion)

    You should not need to change this
**/
void Lab5Canvas::resizeGL(int w, int h) {
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
void Lab5Canvas::updateCamera() {
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

void Lab5Canvas::toggleRender() {
    this->m_rendernormals = !this->m_rendernormals;
    ((QPushButton*)QObject::sender())->setChecked(this->m_rendernormals);
}


void Lab5Canvas::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton||event->button() == Qt::RightButton) { //Left click
        m_lastmousepos.x = event->x(); m_lastmousepos.y = event->y();
    }
}

void Lab5Canvas::mouseMoveEvent(QMouseEvent *event) {
    if(event->buttons() & Qt::LeftButton || event->buttons() & Qt::RightButton){ //Mouse drag - don't modify this
        emit _mouseDrag(event);
          m_lastmousepos.x = event->x(); m_lastmousepos.y = event->y();
      }
    else if (event->buttons() & Qt::NoButton) {

    }
}


void Lab5Canvas::mouseDragEvent(QMouseEvent *event) {
    if(event->buttons() & Qt::RightButton || event->buttons() & Qt::LeftButton) {
        float dx = event->x() - m_lastmousepos.x, dy = event->y() - m_lastmousepos.y;
        double x = m_camera->eye.x, y = m_camera->eye.y, z = m_camera->eye.z,
               r = sqrt(m_camera->eye.x * m_camera->eye.x +
                        m_camera->eye.y * m_camera->eye.y +
                        m_camera->eye.z * m_camera->eye.z),
               theta = acos(y / r) - dy * 0.05f, phi = atan2(z, x) + dx * 0.05f;
        if (theta > M_PI-.1) theta =M_PI-.1;
        if (theta < .1) theta = .1;
        m_camera->eye.x = r * sin(theta) * cos(phi), m_camera->eye.z = r * sin(theta) * sin(phi),
        m_camera->eye.y = r * cos(theta);
        updateCamera();
    }
}

void Lab5Canvas::keyPressEvent(QKeyEvent *event) {
    if(event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Q) {
        exit(0);
    }
}

/**
    Specifies to Qt what to do when the widget needs to be redrawn.  We only
    want to repaint the onscreen objects.

    You should not need to modify this method.

**/
void Lab5Canvas::redraw() {
    repaint();
}

void Lab5Canvas::settingsChanged()
{
    // @TODO: [OPTIONAL] Fill this in if you like...
}

GLuint Lab5Canvas::loadTexture(const QString &filename)
{
    const QFile file (filename);
    assert(file.exists());

    // Load the texture from the file.
    QImage image, texture;
    image.load(file.fileName());
    image = image.mirrored(false, true);
    texture = QGLWidget::convertToGLFormat(image); // How convenient! Qt will do all the work for us.

    // Feed the texture into OpenGL.
    GLuint textureId = -1;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texture.width(), texture.height(), GL_RGBA, GL_UNSIGNED_BYTE, texture.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    return textureId;
}

void Lab5Canvas::releaseTexture(GLuint textureId)
{
    glDeleteTextures(1, &textureId);
}
