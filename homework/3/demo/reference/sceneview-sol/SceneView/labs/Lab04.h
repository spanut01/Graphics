#pragma once

#include <SupportCanvas3D.h>
#include <QGLWidget>
#include <QMenuBar>

#include "LabCanvas.h"
#include "Common.h"

class Camera;


class Lab4Canvas : public LabCanvas {
    Q_OBJECT
signals:
     void _mouseDrag(QMouseEvent *);
public:
    Lab4Canvas(QWidget *parent = 0, QMenuBar *menu = 0);
    virtual ~Lab4Canvas();

    //Methods
    virtual void applyRenderSettings();
    RenderTechnique *getRenderTechnique() { return &m_renderTechnique; }
    ShadeModel *getShadeModel() { return &m_shademodel; }
    bool *getLightingIsEnabled() { return &m_lightingEnabled; }

    //Member variables

public slots:

protected:

     //Methods
     void updateCamera();

     //Inherited Methods
     void initializeGL();
     void paintGL();
     void resizeGL(int w, int h);
     void mousePressEvent(QMouseEvent *event);
     void mouseMoveEvent(QMouseEvent *event);
     void keyPressEvent(QKeyEvent *event);

     //! This function will be called by the UI when the settings have changed
     virtual void settingsChanged();

     //Member variables
     QTimer *mTimer;

     float mFPS, m_increment;
     LabCamera *m_camera;
     GLUquadric *m_quadric;
     ShadeModel m_shademodel;
     int m_shapecount;

protected slots:
     void mouseDragEvent(QMouseEvent *event);
     void redraw();

};
