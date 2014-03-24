#include "Sphere.h"

double Sphere::intersect(Point eyeP, Vector rayV, Matrix transformMatrix){
    double a, b, c, det, t;
    Point p = transformMatrix * eyeP;
    Vector d = transformMatrix * rayV;
    a = d[0]*d[0] + d[1]*d[1] + d[2]*d[2];
    b = 2.0 * (p[0]*d[0] + p[1]*d[1] + p[2]*d[2]);
    c = (p[0]*p[0] + p[1]*p[1] + p[2]*p[2]) - 0.25;
    det = b*b - 4.0*a*c;
    //TODO ACTUALLY SOLVE FOR t
    return det;
}

void Sphere::drawTriangles(){
    int i,j;
    float x,y;
    x = PI * (2.0f / m_segmentsX);
    y = PI * (1.0f / m_segmentsY);

    for (i = 0; i < m_segmentsX; i++){
        
        for (j = 0; j < m_segmentsY; j++){

            glNormal3f(sinf(y*j)*cosf(i*x), cosf(y*j), sinf(y*j)*sinf(i*x));

            glVertex3f(0.5f*sinf(y*j)*cosf(i*x), 0.5f*cosf(y*j), 0.5f*sinf(y*j)*sinf(i*x));
            glVertex3f(0.5f*sinf(y*(j+1))*cosf(i*x), 0.5f*cosf(y*(j+1)), 0.5f*sinf(y*(j+1))*sinf(i*x));
            glVertex3f(0.5f*sinf(y*j)*cosf((i+1)*x), 0.5f*cosf(y*j), 0.5f*sinf(y*j)*sinf((i+1)*x));
  
            glNormal3f(sinf(y*j)*cosf(i*x), cosf(y*j), sinf(y*j)*sinf(i*x));

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
