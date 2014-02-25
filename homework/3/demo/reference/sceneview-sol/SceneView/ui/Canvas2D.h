/*!
   @file   Canvas2D.h
   @author Ben Herila (ben@herila.net)
   @author Evan Wallace (evan.exe@gmail.com)
   @date   Fall 2010
*/

#ifndef CANVAS2D_H
#define CANVAS2D_H

#include "SupportCanvas2D.h"

class RayScene;

// <!-- begin ta code

#include <QBasicTimer>

class Brush;

class Raytracer;

// end ta code -->

class Camera;

/*!
   @class Canvas2D

   @brief 2D canvas that students will implement in the Brush and Filter assignments.
   The same canvas will be used to display raytraced images in the Intersect and Ray
   assignments.

**/
class Canvas2D : public SupportCanvas2D
{
    Q_OBJECT
public:
    Canvas2D();
    virtual ~Canvas2D();

    // begin ta code
    BGRA* specialData() { return (m_specialImage ? (BGRA *)m_specialImage->bits() : NULL); };
    // end ta code

    void setScene(RayScene *scene);

    //! UI will call this from the button on the "Ray" dock
    void renderImage(Camera *camera, int width, int height);

    //! This will be called when the settings have changed
    virtual void settingsChanged();


public slots:
    //! UI will call this from the button on the "Ray" dock
    void cancelRender();

    //! UI will call this from the button on the "Filter" dock
    void filterImage();

protected:
    virtual void paintEvent(QPaintEvent *);  /// overridden from SupportCanvas2D
    virtual void mouseDown(int x, int y);    /// called when the left mouse button is pressed down on the canvas
    virtual void mouseDragged(int x, int y); /// called when the left mouse button is dragged on the canvas
    virtual void mouseUp(int x, int y);      /// called when the left mouse button is released

    //! This will be called when the size of the canvas has been changed
    virtual void notifySizeChanged(int w, int h);

    // begin ta code
    void timerEvent(QTimerEvent *event);
    // end ta code

private:
    RayScene *m_scene;

    //TODO: [BRUSH, INTERSECT, RAY] Put your member variables here.

    // begin ta code
    Brush *m_brush;
    int m_brushType;
    int m_brushRadius;
    BGRA m_brushColor;
    bool m_smooth;
    QPoint m_lastPoint;

    void createDisturbance(int x, int y);
    void copyRenderBufferToCanvas(Raytracer *raytracer, int x1, int y1, int x2, int y2);

    QBasicTimer m_timer;
    QImage *m_specialImage;

    int *_heightMap;
    int *_currentMap;
    int *_oldMap;
    int _rippleRadius, _strength;
    bool m_specialDirty;

    // end ta code

};

#endif // CANVAS2D_H
