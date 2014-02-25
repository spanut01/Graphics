/*!
   @file   TA_HarmonyBrush.h
   @author Evan Wallace (evan.exe@gmail.com)
   @date   Fall 2010
*/

#ifndef HarmonyBrush_Header
#define HarmonyBrush_Header

#include "Brush.h"
#include <QList>
#include <QPoint>

//! This is Evan Wallace's implementation of a special brush.
class HarmonyBrush : public Brush {
    public:
        HarmonyBrush(BGRA color, int flow, int radius);
        virtual ~HarmonyBrush();

        virtual void paintOnce(int x, int y, Canvas2D *canvas);
        virtual void makeMask() {}
        virtual void resetOnMouseUp() { m_isFirstPaint = true; }

    private:
        int m_radius;
        BGRA m_color;
        bool m_isFirstPaint;
        QList<QPoint> m_previousPoints;
};

#endif
