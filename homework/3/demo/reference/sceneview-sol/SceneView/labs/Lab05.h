#pragma once

#include <QGLWidget>
#include <QMenuBar>

#include "LabCanvas.h"
#include "Common.h"

typedef Vector2 GridPoint;


class Lab5Canvas : public LabCanvas {
    Q_OBJECT
signals:
     void _mouseDrag(QMouseEvent *);
public:
    Lab5Canvas(QWidget *parent = 0, QMenuBar *menu = 0);
    virtual ~Lab5Canvas();

    //Methods
    void applyRenderSettings();
    RenderTechnique *getRenderTechnique() { return &m_renderTechnique; }
    ShadeModel *getShadeModel() { return &m_shadingModel; }
    bool *getLightingIsEnabled() { return &m_lightingEnabled; }
    bool *getRenderNormals() { return &m_rendernormals;}
    //Member variables

public slots:
    void toggleRender() ;
protected:


    //HELPER METHODS THAT YOU WILL NEED TO USE


    /*! @brief Converts a grid coordinate(row, column) to a 1-dimensional index into an array */
    inline int getIndex(const GridPoint &c);

    /*! @brief for a specified depth, returns the perturbation of the height */
    double getPerturb(int cur_depth);

    /*! @brief Returns the indices in the grid for the neighbors of the vertex at the given coordinate.  */
    QList<int> getSurroundingVertices(const Vector2 &coordinate);

    // Methods you will have to fill in:
    /*! @brief Subdivides a square recursively to a certain depth. */
    void subdivideSquare(const GridPoint &topLeft, const GridPoint &botRight, GLint curDepth);

    /*! @brief Compute the normals for the points in the terrain. */
    void computeNormals();

    /*! @brief You should know what this does by now... */
    virtual void paintGL();

    // Other Methods
    void updateCamera();

    /*! @brief Loads an image from a filename into OpenGL and returns the texture id. */
    GLuint loadTexture(const QString &filename);

    /*! @brief Releases an OpenGL texture. */
    void releaseTexture(GLuint textureId);

    /*! @brief Populates the initial terrain settings. You must fill this method in for Lab 5. */
    void populateTerrain();

    /*! @brief Draws the normal vectors. */
    void drawNormals();



    // Inherited Methods
    /*! @brief Initalizes the OpenGL canvas. */
    void initializeGL();

    /*! @brief Resizes the OpenGL canvas. */
    void resizeGL(int w, int h);

    /*! @brief Raised when the mouse button is pressed. */
    void mousePressEvent(QMouseEvent *event);

    /*! @brief Raised when the mouse is moved on the canvas. */
    void mouseMoveEvent(QMouseEvent *event);

    /*! @brief Raised when a key is pressed. */
    void keyPressEvent(QKeyEvent *event);

    /*! @brief This function will be called by the UI when the settings have changed */
    virtual void settingsChanged();


    //Member variables
    QTimer     * m_timer;
    bool         m_rendernormals;
    float        m_fps;
    float        m_increment;
    LabCamera  * m_camera;
    GLUquadric * m_quadric;
    Vector3    * m_terrain;
    Vector3    * m_normalmap;
    GLint        m_depth;
    GLint        m_decay;
    GLint        m_gridlength;
    GLfloat      m_roughness;
    Vector2      m_lastmousepos;
    GLuint       m_textureid;

protected slots:
     void mouseDragEvent(QMouseEvent *event);
     void redraw();

};
