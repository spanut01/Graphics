#include "Sphere.h"
#include <iostream>
#include <math.h>
using namespace std;

double Sphere::Intersect(Point eyeP, Vector rayV, Matrix transformMatrix){
    double a, b, c, det, t1, t2;
    Matrix worldToObj = invert(transformMatrix);
    Point p = worldToObj * eyeP;
    Vector d = worldToObj * rayV;
    a = d[0]*d[0] + d[1]*d[1] + d[2]*d[2];
    b = 2.0 * (p[0]*d[0] + p[1]*d[1] + p[2]*d[2]);
    c = (p[0]*p[0] + p[1]*p[1] + p[2]*p[2]) - 0.25;
    det = b*b - 4.0*a*c;
    //TODO ACTUALLY SOLVE FOR t
    //cout << det << "\n";
    if (det < 0.0) {
        return -1;
    }
    t1 = (-b + sqrt(det)) / (2 * a);
    t2 = (-b - sqrt(det)) / (2 * a);
    if ((t1 <= 0.0) && (t2 <= 0.0)){
        return -1;
    }
    if((t1 > 0.0) && ((t1 < t2) || (t2 <= 0.0))){
        return t1;
    }
    return t2;
}
Vector Sphere::findIsectNormal(Point eyePoint, Vector ray, double dist){
    return ray; 
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
