#include "Cylinder.h"

void Cylinder::drawTriangles(){
    int i,j;
    float x, y;
    x = PI * (2.0f / m_segmentsX);
    y = 1.0f / m_segmentsY;

    for(i=0; i<m_segmentsX; i++){
        //top
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, 0.5f, 0.0f);
        glVertex3f(0.5f*cosf(i*x), 0.5f, 0.5f*sinf(i*x));
        glVertex3f(0.5f*cosf((i+1)*x), 0.5f, 0.5f*sinf((i+1)*x));
          
        //bottom
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(0.0f, -0.5f, 0.0f);
        glVertex3f(0.5f*cosf(i*x), -0.5f, 0.5f*sinf(i*x));
        glVertex3f(0.5f*cosf((i+1)*x), -0.5f, 0.5f*sinf((i+1)*x));
          
        for(j=0; j<m_segmentsY; j++){
            //sides
            glNormal3f(cosf((i+0.5f)*x), 0.0f, sinf((i+0.5f)*x));
            glVertex3f(0.5f*cosf(i*x), -0.5f+j*y, 0.5f*sinf(i*x));
            glVertex3f(0.5f*cosf((i+1)*x), -0.5f+j*y, 0.5f*sinf((i+1)*x));
            glVertex3f(0.5f*cosf((i+1)*x), -0.5f+(j+1)*y, 0.5f*sinf((i+1)*x));
              
            glVertex3f(0.5f*cosf(i*x), -0.5f+(j+1)*y, 0.5f*sinf(i*x));
            glVertex3f(0.5f*cosf(i*x), -0.5f+j*y, 0.5f*sinf(i*x));
            glVertex3f(0.5f*cosf((i+1)*x), -0.5f+(j+1)*y, 0.5f*sinf((i+1)*x));
        }
    }
};

void Cylinder::drawNormals(){
    int i,j;
    float x,y;
    x =PI * (2.0f / m_segmentsX);
    y = 1.0f / m_segmentsY;

    for(i=0; i<m_segmentsX; i++){
        //top
        glVertex3f(0.0f, 0.5f, 0.0f);
        glVertex3f(0.0f, 0.6f, 0.0f);
        glVertex3f(0.5f*cosf(i*x), 0.5f, 0.5f*sinf(i*x));
        glVertex3f(0.5f*cosf(i*x), 0.6f, 0.5f*sinf(i*x));
        glVertex3f(0.5f*cosf((i+1)*x), 0.5f, 0.5f*sinf((i+1)*x));
        glVertex3f(0.5f*cosf((i+1)*x), 0.6f, 0.5f*sinf((i+1)*x));
          
        //bottom
        glVertex3f(0.0f, -0.5f, 0.0f);
        glVertex3f(0.0f, -0.6f, 0.0f);
        glVertex3f(0.5f*cosf(i*x), -0.5f, 0.5f*sinf(i*x));
        glVertex3f(0.5f*cosf(i*x), -0.6f, 0.5f*sinf(i*x));
        glVertex3f(0.5f*cosf((i+1)*x), -0.5f, 0.5f*sinf((i+1)*x));
        glVertex3f(0.5f*cosf((i+1)*x), -0.6f, 0.5f*sinf((i+1)*x));
          
        for(j=0; j<m_segmentsY; j++){
            //sides
            glVertex3f(0.5f*cosf(i*x), -0.5f+j*y, 0.5f*sinf(i*x));
            glVertex3f(0.6f*cosf(i*x), -0.5f+j*y, 0.6f*sinf(i*x));
            glVertex3f(0.5f*cosf((i+1)*x), -0.5f+j*y, 0.5f*sinf((i+1)*x));
            glVertex3f(0.6f*cosf((i+1)*x), -0.5f+j*y, 0.6f*sinf((i+1)*x));
            glVertex3f(0.5f*cosf((i+1)*x), -0.5f+(j+1)*y, 0.5f*sinf((i+1)*x));
            glVertex3f(0.6f*cosf((i+1)*x), -0.5f+(j+1)*y, 0.6f*sinf((i+1)*x));
              
            glVertex3f(0.5f*cosf(i*x), -0.5f+(j+1)*y, 0.5f*sinf(i*x));
            glVertex3f(0.6f*cosf(i*x), -0.5f+(j+1)*y, 0.6f*sinf(i*x));
            glVertex3f(0.5f*cosf(i*x), -0.5f+j*y, 0.5f*sinf(i*x));
            glVertex3f(0.6f*cosf(i*x), -0.5f+j*y, 0.6f*sinf(i*x));
            glVertex3f(0.5f*cosf((i+1)*x), -0.5f+(j+1)*y, 0.5f*sinf((i+1)*x));
            glVertex3f(0.6f*cosf((i+1)*x), -0.5f+(j+1)*y, 0.6f*sinf((i+1)*x));
          }
      }
};
