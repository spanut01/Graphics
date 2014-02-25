#include <CS123Common.h>
#include <CS123Algebra.h>
#include <SupportCanvas3D.h>
#include <cmath>
#include <iostream>
#include <QPushButton>
#include <QFile>
#include <QMenuBar>
#include <QMenu>
#include <QMouseEvent>
#include <QTimer>
#include <qgl.h>
#include <iostream>
#include <cmath>

#ifdef __WIN32
#include <gl/glext.h>
#endif


#undef near
#undef far

#ifndef LABCANVAS_H
#define LABCANVAS_H

struct LabCamera {
    Vector3 eye, center, up;
    float angle, near, far;
};

enum RenderTechnique {
    RENDER_POINT, RENDER_WIREFRAME, RENDER_SHADED
};

enum ShadeModel {
    SHADE_FLAT, SHADE_SMOOTH
};

/*!
  @class    LabCanvas
  @brief    Base class for 3D canvas used in OpenGL labs.
  @author   Ben Herila (ben@herila.net)
**/
class LabCanvas : public SupportCanvas3D
{
Q_OBJECT
public:
    LabCanvas(QWidget *parent = 0, QMenuBar *menu = 0);
    virtual ~LabCanvas();

protected:

    QPushButton* makeButton(QString title);              //! Constructs and adds a button to this LabCanvas
    virtual void applyRenderSettings() = 0;              //! Called to apply render settings to the OpenGL context
    virtual void paintGL() = 0;

    //! This function will be called by the UI when the settings have changed
    virtual void settingsChanged();

    QMenu           * m_menu;
    RenderTechnique   m_renderTechnique;
    ShadeModel        m_shadingModel;
    QList<QObject *>  m_dispose;
    bool              m_lightingEnabled;




protected slots:
    void pickRenderTechnique();
    void toggleLights();
    void pickSMMenu();

private:
    void makeRenderTechniqueButton(QString title, int renderTechnique); //! Constructs a RenderTechnique menu item
    int m_menuOffset;
    void makeSMMenu(QString title, int shademodel);

};

#endif // LABCANVAS_H
