/**

  CS123 2-dimensional canvas. Contains support code necessary for Brush,
  Filter, Intersect, and Ray.

  YOU WILL NEED TO FILL THIS IN!

  Please contact the authors if there are any bugs.

  @date   2010-08-24
  @author Evan Wallace (edwallac)
  @author Ben Herila (ben@herila.net)

**/

// For your convenience, a few headers are included for you.
#include <math.h>
#include <assert.h>
#include "Canvas2D.h"
#include "Canvas3D.h"
#include "Settings.h"
#include "RayScene.h"

// begin ta code
#include "brush/TA_GaussianBrush.cpp"
#include "brush/TA_HarmonyBrush.cpp"
// end ta code

#include <QPainter>
#include "brush/Brush.h"
#include "brush/ConstantBrush.h"
#include "brush/LinearBrush.h"
#include "brush/QuadraticBrush.h"
#include "brush/SmudgeBrush.h"

Canvas2D::Canvas2D()
{
    // @TODO: Initialize any pointers in this class here.
    m_scene = NULL;

    // begin ta code
    _heightMap = NULL;
    m_brush = NULL;
    m_brushType = -1;
    notifySizeChanged(width(), height());
    m_smooth = true;
    setMouseTracking(true);
    // end ta code
}

Canvas2D::~Canvas2D()
{
    // @TODO: Be sure to release all memory that you allocate.
    delete m_scene;

    // begin ta code
    m_timer.stop();
    delete m_brush;
    delete m_specialImage;
    delete[] _heightMap;
    // end ta code
}

// This is called when the canvas size is changed. You can change the canvas size
// by calling setSize(...). You probably won't need to fill this in, but you can
// if you want to.
void Canvas2D::notifySizeChanged(int w, int h) {
    //  begin ta code
    int m_width = width();
    int m_height = height();
    bool active = m_timer.isActive();
    m_timer.stop();

    delete [] _heightMap;
    _heightMap = new int[2 * w * h];
    _currentMap = _heightMap;
    _oldMap = _heightMap + w * h;

    memset(_currentMap, 0, sizeof(int) * m_width * m_height);
    memset(_oldMap, 0, sizeof(int) * m_width * m_height);

    m_specialImage = NULL;//new QImage(*m_image);

    if (active)
       m_timer.start(20, this);

    m_specialDirty = true;

    // Remake the brush every time the size is set, this will clear the point history of the HarmonyBrush
    m_brushType = -1;
    // end ta code
}

void Canvas2D::paintEvent(QPaintEvent *e) {
    // You probably won't need to fill this in, but you can if you want to override
    // any painting events for the 2D canvas. For now, we simply call the superclass.

    SupportCanvas2D::paintEvent(e);

    // begin ta code
    if (settings.brushType == BRUSH_SPECIAL_2 && m_specialImage) {
        QPainter painter(this);
        painter.drawImage(QPoint(0,0), *m_specialImage);
    }
    // end ta code

}

// *****************************************************************************************
// ** BRUSH
// *****************************************************************************************

void Canvas2D::mouseDown(int x, int y)
{
    // @TODO: [BRUSH] Mouse interaction for brush. You will probably want to create a
    //        separate class for each of your brushes. Remember that you can use the
    //        static Settings object to get the currently selected brush and its parameters.

    int currentBrush = settings.brushType;
    int currentRadius = settings.brushRadius;

    BGRA currentColor;
    currentColor.b = settings.brushBlue;
    currentColor.g = settings.brushGreen;
    currentColor.r = settings.brushRed;
    currentColor.a = 255;

    int currentFlow = settings.brushAlpha;

    // you're going to need to leave the alpha value on the canvas itself at 255, but
    // you will need to use the actual alpha value to compute the new color of the pixel
    // begin ta code
    m_lastPoint = QPoint(x,y);
    if(m_brushType != settings.brushType || m_brushRadius != settings.brushRadius || memcmp(&m_brushColor, &currentColor, 4))
    {
        m_brushType = settings.brushType;
        m_brushRadius = settings.brushRadius;
        m_brushColor = currentColor;

        delete m_brush;
        delete m_specialImage;
        m_brush = NULL;
        m_specialImage = NULL;

        switch (settings.brushType)
        {
        case BRUSH_SOLID:
            m_brush = new ConstantBrush(currentColor, settings.brushAlpha, settings.brushRadius);
            break;

        case BRUSH_LINEAR:
            m_brush = new LinearBrush(currentColor, settings.brushAlpha, settings.brushRadius);
            break;

        case BRUSH_QUADRATIC:
            m_brush = new QuadraticBrush(currentColor, settings.brushAlpha, settings.brushRadius);
            break;

        case BRUSH_SMUDGE:
            m_brush = new SmudgeBrush(currentColor, settings.brushAlpha, settings.brushRadius);
            break;

        case BRUSH_SPECIAL_1:
            m_brush = new HarmonyBrush(currentColor, settings.brushAlpha, settings.brushRadius);
            break;

        case BRUSH_SPECIAL_2:
            m_brush = new LinearBrush(currentColor, settings.brushAlpha, settings.brushRadius);

            m_specialDirty = true;
            m_specialImage = new QImage(*m_image);

            _rippleRadius = 3; // 2 to 3 works well
            _strength     = 4 + (settings.brushAlpha * 7) / 255; // 3 to 9 works well
            if (_strength > 9)
                _strength  = 9;
            memset(_currentMap, 0, sizeof(int) * width() * height());
            memset(_oldMap, 0, sizeof(int) * width() * height());
            m_timer.start(20, this);
            createDisturbance(x, y);
            break;

        }

        if (m_brush)
            m_brush->paintOnce(x, y, this);


    }
    // end ta code
}

void Canvas2D::mouseDragged(int x, int y)
{
    // TODO: [BRUSH] Mouse interaction for Brush.

    // begin ta code *************************************
    if (m_brush)
    {
        m_brush->paintOnce(x,y,this);
    }
    if (settings.brushType == BRUSH_SPECIAL_2)
        createDisturbance(x, y);
    // end ta code
}

void Canvas2D::mouseUp(int x, int y)
{
    // TODO: [BRUSH] Mouse interaction for Brush.

    // begin ta code *************************************
    if (m_brush)
        m_brush->resetOnMouseUp();
    // end ta code
}

// *****************************************************************************************
// ** Special stuff for the TA Special Brush
// *****************************************************************************************

int randNum(int limit) {
   if (limit <= 0)
      return 0;

   return rand() % limit;
}

int randNo(int low, int high) { // inclusive
   return (low + randNum(high - low));
}

void Canvas2D::createDisturbance(int x, int y) {
   _rippleRadius = randNo(1, 3);

   for(int j = -_rippleRadius; j <= _rippleRadius; j++) {
      int val = (8 - _rippleRadius - abs(j));

      int yOff = y + j;
      if (yOff >= 0 && yOff < height()) {
         yOff *= width();

         for(int i = -_rippleRadius; i <= _rippleRadius; i++) {
            val += (i <= 0 ? 1 : -1);

            int xOff = x + i;
            if (xOff >= 0 && xOff < width())
               _oldMap[yOff + xOff] = (1 << val);
         }
      }
   }

   m_specialDirty = true;
}

// update waves  (special brush wave animation by Travis Fischer - tfischer)
void Canvas2D::timerEvent(QTimerEvent *event) {
   if (!m_specialDirty) {
      if (m_specialImage == NULL)
         m_specialImage = new QImage(*m_image);

      return; // only update waves if there are any
   }

   int *temp = _currentMap;
   _currentMap = _oldMap;
   _oldMap = temp;
   if (m_specialImage == NULL)
      m_specialImage = new QImage(*m_image);
   int *data = (int*)m_specialImage->bits(); // distorted
   int *back = (int*)(this->data()); // original image

   int index = 0;
   bool dirty = false;

   int m_height = height();
   int m_width = width();

   for(int j = 0; j < m_height; ++j) {
      int a = (j <= 0 ? 0 : m_width);
      int b = (j >= m_height - 1 ? 0 : m_width);

      for(int i = 0; i < m_width; ++i, ++index) {
         int old = _currentMap[index];

         int avg = (int)(((_oldMap[index - a] + _oldMap[index + b] +
                     _oldMap[index - (i <= 0 ? 0 : 1)] +
                     _oldMap[index + (i >= m_width - 1 ? 0 : 1)]) >> 1) - old);


         // dampen the strength of the wave over time
         avg -= (avg >> _strength);

         if (old != avg) {
            _currentMap[index] = avg;
            dirty = true;

            int x = i, y = j;

            if (avg > 0 && avg < 1024) {
               avg = (1024 - avg);

               // reflect (i, j) incides based on current magnitude of wave
               x = (i * avg) >> 10;
               y = (j * avg) >> 10;

               // Boundary checks
               x = (x <= 0 ? 0 : (x >= m_width  ? m_width - 1  : x));
               y = (y <= 0 ? 0 : (y >= m_height ? m_height - 1 : y));
            }

            data[j * m_width + i] = back[y * m_width + x];
         }
      }
   }

   m_specialDirty = dirty;
   update();
}

// end ta code


// *****************************************************************************************
// ** FILTER
// *****************************************************************************************

void Canvas2D::filterImage()
{
    // TODO: [FILTER] Filter the image. Some example code to get the filter type is provided below.

    switch (settings.filterType) {
    case FILTER_BLUR:
        // ...
        break;
        // fill in the rest
    }

}

void Canvas2D::setScene(RayScene *scene)
{
    delete m_scene;
    m_scene = scene;
}


void Canvas2D::renderImage(Camera *camera, int width, int height)
{
    if (m_scene)
    {
        // @TODO: raytrace the scene based on settings
        //        YOU MUST FILL THIS IN FOR INERSECT/RAY

        // If you want the interface to stay responsive, make sure to call
        // QCoreApplication::processEvents() periodically during the rendering

    }
}

void Canvas2D::cancelRender()
{
    // TODO: cancel the raytracer (optional)
}



void Canvas2D::settingsChanged() {

    // TODO: Process changes to the application settings.

}
