/*!
   @file   LinearBrush.cpp
   @author Ben Herila (ben@herila.net)
   @author Evan Wallace (evan.exe@gmail.com)
   @date   Fall 2010

   @brief  Implementation of a brush with a linear mask distribution.
   You should fill this file in while completing the Brush assignment.

*/

#include "LinearBrush.h"

LinearBrush::LinearBrush(BGRA color, int flow, int radius) : Brush(color, flow, radius)
{
    // @TODO: [BRUSH] You'll probably want to set up the mask right away.
    makeMask();
}


LinearBrush::~LinearBrush()
{
    // @TODO: [BRUSH] Delete any resources owned by this brush, so you don't leak memory.

}

void LinearBrush::makeMask()
{
    // @TODO: [BRUSH] Set up the mask for your Linear brush here...

    // begin ta code
    {
    int size = 2 * m_radius + 1;
    float *mask_ptr = m_mask = new float[size * size];
    float flow = (float)m_flow / 255.0f;

    for(int y = 0; y < size; y++)
    {
        float fy = map_int_to_float(y, size);
        for(int x = 0; x < size; x++)
        {
            float fx = map_int_to_float(x, size);
            float radius_squared = fx * fx + fy * fy;

            *mask_ptr++ = (radius_squared <= 1.0f) ? flow * (1.0f - sqrtf(radius_squared)) : 0.0f;
        }
    }
    }
    // end ta code
}


