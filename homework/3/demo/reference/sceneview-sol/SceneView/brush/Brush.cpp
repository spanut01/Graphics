/*!
   @file   Brush.cpp
   @author Ben Herila (ben@herila.net)
   @author Evan Wallace (evan.exe@gmail.com)
   @date   Fall 2010

   @brief  Implementation of common functionality of bitmap brushes.
   You should fill this file in while completing the Brush assignment.

*/

#include "Brush.h"
#include "Canvas2D.h"

#include <iostream>

using namespace std;

unsigned char lerp(unsigned char a, unsigned char b, float percent)
{
    float fa = (float)a / 255.0f;
    float fb = (float)b / 255.0f;
    return (unsigned char)((fa + (fb - fa) * percent) * 255.0f + 0.5f);
}

Brush::Brush(BGRA color, int flow, int radius) {

    // @TODO: [BRUSH] Initialize any memory you are going to use here. Hint - you are going to
    //        need to store the mask in memory. This might (or might not) be a good place to
    //        allocate that memory.

    // Example code: (feel free to use)
    m_color = color;
    m_flow = flow;
    m_radius = radius;
    m_mask = NULL;

}


Brush::~Brush()
{
    // @TODO: [BRUSH] Don't forget to delete any memory you allocate. Use delete[] to delete
    //        a whole array. Otherwise you'll just delete the first element!
    //
    //        i.e. delete[] m_mask;
    //

    delete[] m_mask;
}


void Brush::setGreen(int green)
{
    m_color.g = green;
}


void Brush::setRed(int red)
{
    m_color.r = red;
}


void Brush::setBlue(int blue)
{
    m_color.b = blue;
}


void Brush::setFlow(int flow)
{
    m_flow = flow;
    makeMask();
}


void Brush::setRadius(int radius)
{
    m_radius = radius;
    makeMask();
}


void Brush::paintOnce(int mouse_x, int mouse_y, Canvas2D* canvas)
{
    // @TODO: [BRUSH] You can do any painting on the canvas here. Or, you can
    //        override this method in a subclass and do the painting there.
    //

    int width = canvas->width();
    int height = canvas->height();
    int x_start = qMax(mouse_x - m_radius, 0);
    int y_start = qMax(mouse_y - m_radius, 0);
    int x_end = qMin(mouse_x + m_radius + 1, width);
    int y_end = qMin(mouse_y + m_radius + 1, height);
    int x_offset = x_start - (mouse_x - m_radius);
    int y_offset = y_start - (mouse_y - m_radius);

    for(int y = y_start; y < y_end; y++)
    {
            float *mask_ptr = &m_mask[(y - y_start + y_offset) * (2 * m_radius + 1) + x_offset];

            // paint on the regular canvas
            {
                BGRA *canvas_data = &canvas->data()[y * width + x_start];
                for(int x = x_start; x < x_end; x++)
                {
                        float alpha = *mask_ptr++;
                        canvas_data->r = lerp(canvas_data->r, m_color.r, alpha);
                        canvas_data->g = lerp(canvas_data->g, m_color.g, alpha);
                        canvas_data->b = lerp(canvas_data->b, m_color.b, alpha);
                        canvas_data++;
                }
            }

    }

    canvas->update(x_start, y_start, x_end - x_start, y_end - y_start);
}


