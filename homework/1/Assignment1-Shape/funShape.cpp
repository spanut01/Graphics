#include "funShape.h"

void FunShape::drawTriangles(){
    int i,j;
    float x,xr,y,yr;
    xr = PI * (2.0f / m_segmentsX);
    yr = PI * (1.0f / m_segmentsY);
    y = 0.5f / m_segmentsY;
    x = 0.5f / m_segmentsX;

    for (i = 0; i < m_segmentsX; i++){
        
        for (j = 0; j < m_segmentsY; j++){
            glVertex3f(-0.5+x*i*cosf(xr*i), -0.5+(y*i)*sinf(yr*j), -0.5+x*i*sinf(xr*i));
            glVertex3f(-0.5+x*(i+1)*cosf(xr*(i+1)), -0.5+(y*i)*sinf(yr*(j+1)), -0.5+x*(i+1)*sinf(xr*(i+1)));
            glVertex3f(-0.5+x*(i+1)*cosf(xr*i), -0.5+y*(i+1)*sinf(yr*j), -0.5+x*(i+1)*sinf(xr*(i+1)));
        }
    }
}

void FunShape::drawNormals(){
    int i,j;
    float x,y;
    x = PI * (2.0f / m_segmentsX);
    y = 0.5f / m_segmentsY;

}
