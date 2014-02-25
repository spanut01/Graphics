#pragma once

#include "Common.h"
#include "LabCanvas.h"

/*!
    @class Lab3Canvas
    @brief This class will be implemented during Lab 3.
*/
class Lab3Canvas : public LabCanvas {
    Q_OBJECT

signals:
     void _mouseDrag(QMouseEvent *);

public:
    Lab3Canvas(QWidget *parent = 0, QMenuBar *menu = 0);
    virtual ~Lab3Canvas();

    void applyRenderSettings();
    const RenderTechnique *getRenderTechnique () { return &m_renderTechnique; }
    const ShadeModel *getShadeModel () { return &m_shadingModel; }
    bool *getLightingIsEnabled () { return &m_lightingEnabled; }


protected:

    virtual void updateCamera();

//-------------------------------------------------------------------
///@name Inherited Methods
//@{-----------------------------------------------------------------

    virtual void initializeGL ();
    virtual void paintGL ();
    void resizeGL (int w, int h);
    void mousePressEvent (QMouseEvent *event);
    void mouseMoveEvent (QMouseEvent *event);
    void keyPressEvent (QKeyEvent *event);

    //! This function will be called by the UI when the settings have changed
    virtual void settingsChanged();

//@}-----------------------------------------------------------------
///@name Member Variables
//@{-----------------------------------------------------------------

    QTimer         * m_timer;
    float            m_fps;
    float            m_increment;
    LabCamera      * m_camera;
    GLUquadric     * m_quadric;

//@}-----------------------------------------------------------------

protected slots:
    void mouseDragEvent(QMouseEvent *event);
    void redraw();

};
