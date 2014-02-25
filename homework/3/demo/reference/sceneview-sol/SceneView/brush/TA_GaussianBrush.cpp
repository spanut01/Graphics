/*!
   @file   TA_GaussianBrush.cpp
   @author Evan Wallace (evan.exe@gmail.com)
   @date   Fall 2010

   @brief  Implementation of a brush with a Gaussian mask distribution.
*/

#include "TA_GaussianBrush.h"

/*************************************************************************
 * Function Name: GaussianBrush::GaussianBrush
 * Parameters: int color[3], int flow, int radius
 * Effects: 
 *************************************************************************/

GaussianBrush::GaussianBrush(BGRA color, int flow, int radius)
  : Brush(color, flow, radius)
{
    makeMask();
}


/*************************************************************************
 * Function Name: GaussianBrush::~GaussianBrush
 * Parameters: 
 * Effects: 
 *************************************************************************/

GaussianBrush::~GaussianBrush()
{
}


/*************************************************************************
 * Function Name: GaussianBrush::makeMask
 * Parameters: 
 * Returns: void
 * Effects: create a mask with gaussian distribution
 *************************************************************************/
void
GaussianBrush::makeMask()
{
    int size = 2 * m_radius + 1;
    float *mask_ptr = m_mask = new float[size * size];
    float flow = (float)m_flow / 255.0f;

    float *gaussian_weights = new float[size];
    float sigma = 0.2923f; // 1/255 = e^(-(1^2)/(2*sigma^2))/(sigma*sqrt(2pi))
    float multiplier = -1.0f / (2.0f * sigma * sigma);
    for(int i = 0; i < size; i++)
    {
        float fi = map_int_to_float(i, size);
        gaussian_weights[i] = expf(fi * fi * multiplier);
    }
    
    for(int y = 0; y < size; y++)
    {
        float fy = map_int_to_float(y, size);
        for(int x = 0; x < size; x++)
        {
            float fx = map_int_to_float(x, size);
            float radius_squared = fx * fx + fy * fy;
            
            *mask_ptr++ = (radius_squared <= 1.0f) ? flow * gaussian_weights[x] * gaussian_weights[y] : 0.0f;
        }
    }
    
    delete [] gaussian_weights;
}


