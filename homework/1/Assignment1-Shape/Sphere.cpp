#include "Sphere.h"

void Sphere::drawTriangles(){
    int i,j;
    float x,y;
    x = PI * (2.0f / m_segmentsX);
    y = PI * (1.0f / m_segmentsY);

    for (i = 0; i < m_segmentsX; i++){
        
        for (j = 0; j < m_segmentsY; j++){

            glNormal3f(0.5f*sinf(j)*cosf(i), 0.5f*cosf(j), 0.5f*sinf(j)*sinf(i));

            glVertex3f(0.5f*sinf(y*j)*cosf(i*x), 0.5f*cosf(y*j), 0.5f*sinf(y*j)*sinf(i*x));
            glVertex3f(0.5f*sinf(y*(j+1))*cosf(i*x), 0.5f*cosf(y*(j+1)), 0.5f*sinf(y*(j+1))*sinf(i*x));
            glVertex3f(0.5f*sinf(y*j)*cosf((i+1)*x), 0.5f*cosf(y*j), 0.5f*sinf(y*j)*sinf((i+1)*x));
  
            glNormal3f(0.5*sinf(j)*cosf(i), 0.5f*cosf(j), 0.5f*sinf(j)*sinf(i));

            glVertex3f(0.5f*sinf(y*(j+1))*cosf((i+1)*x), 0.5f*cosf(y*(j+1)), 0.5f*sinf(y*(j+1))*sinf((i+1)*x));
            glVertex3f(0.5f*sinf(y*(j+1))*cosf(i*x), 0.5f*cosf(y*(j+1)), 0.5f*sinf(y*(j+1))*sinf(i*x));
            glVertex3f(0.5f*sinf(y*j)*cosf((i+1)*x), 0.5f*cosf(y*j), 0.5f*sinf(y*j)*sinf((i+1)*x));
        }
    }
}

void Sphere::drawNormals(){
    int i,j;
    float x,y;
    x = PI * (2.0f / m_segmentsX);
    y = PI * (1.0f / m_segmentsY);

    for (i = 0; i < m_segmentsX; i++){
        for (j = 0; j < m_segmentsY; j++){
            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(0.6f*sinf(y*j)*cosf(i*x), 0.6f*cosf(y*j), 0.6f*sinf(y*j)*sinf(i*x));
        }
    }
}
