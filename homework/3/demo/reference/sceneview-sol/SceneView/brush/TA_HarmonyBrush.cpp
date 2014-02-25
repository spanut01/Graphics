/*!
   @file   TA_HarmonyBrush.cpp
   @author Evan Wallace (evan.exe@gmail.com)
   @date   Fall 2010

   @brief  Implementation of a special brush.
*/

#include "TA_HarmonyBrush.h"


/*************************************************************************
 * Function Name: HarmonyBrush::HarmonyBrush
 * Parameters: int color[3], int flow, int radius
 * Effects: 
 *************************************************************************/

HarmonyBrush::HarmonyBrush(BGRA color, int flow, int radius)
  : Brush(color, flow, radius),
  m_radius(radius),
  m_color(color)
{
    m_color.a = flow;
    m_isFirstPaint = true;
}


/*************************************************************************
 * Function Name: HarmonyBrush::~HarmonyBrush
 * Parameters: 
 * Effects: 
 *************************************************************************/

HarmonyBrush::~HarmonyBrush()
{
}


void
HarmonyBrush::paintOnce(int x, int y, Canvas2D *canvas)
{
    QPoint mouse(x, y);

    if (!m_previousPoints.empty())
    {
        if (!m_isFirstPaint) canvas->drawLine(m_previousPoints.last().x(), m_previousPoints.last().y(), x, y, m_color);
        else m_isFirstPaint = false;

        // from http://mrdoob.com/projects/harmony/
        foreach (QPoint point, m_previousPoints)
        {
            int dx = point.x() - mouse.x();
            int dy = point.y() - mouse.y();
            float d = dx * dx + dy * dy;

            d /= m_radius * m_radius;

            if (d < 1 && (float)rand() / (float)RAND_MAX > (d / 2))
                canvas->drawLine(
                         mouse.x() + (dx * 0.3), mouse.y() + (dy * 0.3),
                         point.x() - (dx * 0.3), point.y() - (dy * 0.3),
                         m_color);
        }
    }

    m_previousPoints += mouse;
}
