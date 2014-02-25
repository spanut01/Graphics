/*!
   @file   Brush.cpp
   @author Ben Herila (ben@herila.net)
   @author Evan Wallace (evan.exe@gmail.com)
   @date   Fall 2010

   @brief
     Implementation of a brush that smudges the image on the canvas as
   the mouse is dragged. You should fill this file in for the Brush
   assignment.

*/

#include <iostream>
#include <CS123Common.h>

#include "SmudgeBrush.h"
#include "Canvas2D.h"

using namespace std;

// begin ta code
unsigned char lerp(unsigned char, unsigned char, float);
// end ta code

SmudgeBrush::SmudgeBrush(BGRA color, int flow, int radius) : QuadraticBrush(color, flow, radius)
{
    // @TODO: [BRUSH] Initialize any memory you are going to use here. Hint - you are going to
    //       need to store temporary image data in memory. This might be a good place to
    //       allocate that memory.

    // begin ta code
    m_isStartOfStroke = true;
    m_paint = NULL;
    // end ta code

    makeMask();
}


SmudgeBrush::~SmudgeBrush()
{
    // @TODO: [BRUSH] Be sure not to leak memory!

    // begin ta code
    delete [] m_paint;
    // end ta code
}

// begin ta code
void SmudgeBrush::resetOnMouseUp()
{
    m_isStartOfStroke = true;
}
// end ta code


//! create a mask with a distribution of your choice (probably want to use quadratic for best results)
void SmudgeBrush::makeMask()
{
    // @TODO: [BRUSH] Set up the mask for your brush here. For this brush you will probably want
    //        to use a quadratic distribution for the best results. Linear or Gaussian would
    //        work too, however. Feel free to paste your code from the Linear or Quadratic brushes
    //        or modify the class inheritance to be able to take advantage of one of those class's
    //        existing implementations. The choice is yours!
    //
    // begin ta code (Quadratic distribution)
    {
        QuadraticBrush::makeMask();

        int size = 2 * m_radius + 1;
        delete [] m_paint;
        m_paint = new BGRA[size * size];
    }
    // end ta code
}

//! Picks up paint from the canvas before drawing begins.
void SmudgeBrush::pickUpPaint(int x, int y, Canvas2D* canvas)
{
    int width = canvas->width();
    int height = canvas->height();

    // @TODO: [BRUSH] Perform the "pick up paint" step described in the assignment handout here.
    //        In other words, you should store the "paint" under the brush mask in a temporary
    //        buffer (which you'll also have to figure out where and how to allocate). Then,
    //        in the paintOnce() method, you'll paste down the paint that you picked up here.
    //

    // <!-- begin ta code
    {
        int size = 2 * m_radius + 1;
        int x_start = qMax(x - m_radius, 0);
        int y_start = qMax(y - m_radius, 0);
        int x_end = qMin(x + m_radius + 1, width);
        int y_end = qMin(y + m_radius + 1, height);
        int x_offset = x_start - (x - m_radius);
        int y_offset = y_start - (y - m_radius);

        memset(m_paint, 0, size * size * sizeof(BGRA));

        // need to check bounds for memcpy here
        if(x_start < x_end)
        {
                for(int y = y_start; y < y_end; y++)
                {
                        int index = (y - y_start + y_offset) * size + x_offset;
                        BGRA *to = &m_paint[index];
                        BGRA *from = &canvas->data()[y * width + x_start];
                        memcpy(to, from, (x_end - x_start) * sizeof(BGRA));
                }
        }
    }
    // end ta code -->
}

void SmudgeBrush::paintOnce(int mouse_x, int mouse_y, Canvas2D* canvas)
{
    // @TODO: [BRUSH] Here, you'll need to paste down the paint you picked up in
    //        the previous method. Be sure to take the mask into account! You can
    //        ignore the alpha parameter, but you can also use it (smartly) if you
    //        would like to.

    // <!-- begin ta code
    {
        if(!m_isStartOfStroke)
        {
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
                        int index = (y - y_start + y_offset) * (2 * m_radius + 1) + x_offset;
                        float *mask_ptr = &m_mask[index];
                        BGRA *color = &m_paint[index];
                        BGRA *canvas_data = &canvas->data()[y * width + x_start];
                        for(int x = x_start; x < x_end; x++)
                        {
                                float alpha = *mask_ptr++;
                                canvas_data->r = lerp(canvas_data->r, color->r, alpha);
                                canvas_data->g = lerp(canvas_data->g, color->g, alpha);
                                canvas_data->b = lerp(canvas_data->b, color->b, alpha);
                                canvas_data++;
                                color++;
                        }
                }

                canvas->update(x_start, y_start, x_end - x_start, y_end - y_start);
        }

        m_isStartOfStroke = false;
    }
    // end ta code -->

    // now pick up paint again...
    pickUpPaint(mouse_x, mouse_y, canvas);

}


