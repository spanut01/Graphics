#include "Sphere.h"
#include <iostream>
#include <math.h>
using namespace std;

double Sphere::Intersect(Point eyeP, Vector rayV, Matrix worldToObj){
    double a, b, c, det, t1, t2;
    //Matrix worldToObj = invert(transformMatrix);
    Point p = worldToObj * eyeP;
    Vector d = worldToObj * rayV;
    //cout<<"worldToObj\n";
    //worldToObj.print();
    a = d[0]*d[0] + d[1]*d[1] + d[2]*d[2];
    b = 2 * (p[0]*d[0] + p[1]*d[1] + p[2]*d[2]);
    c = (p[0]*p[0] + p[1]*p[1] + p[2]*p[2]) - 0.25;
    det = (b*b) - (4*a*c);
    //cout<<"a "<<a<<" b "<<b<<" c "<<c<<" det "<<det<<"\n";
    //cout<<"eyepoint "<<p[0]<<" "<<p[1]<<" "<<p[2]<<"\n";
    //cout<<"ray "<<d[0]<<" "<<d[1]<<" "<<d[2]<<"\n";
    //TODO ACTUALLY SOLVE FOR t
    if (det < -0.000001) {
        return -1;
    }
    t1 = (-b + sqrt(det)) / (2 * a);
    t2 = (-b - sqrt(det)) / (2 * a);
    //cout << "t1 = " << t1 << "  t2 = " << t2 << "\n\n";
    if((t1 > -0.000001) && ((t1 < t2) || (t2 <= -0.000001))){
        //cout << "t1 = " << t1 << "\n\n";
        return t1;
    }
    if((t2 > -0.000001) && ((t2 < t1) || (t1 <= -0.000001))){
        //cout << "t2 = " << t2 << "\n\n";
        return t2;
    }

    return -1;
}
Vector Sphere::findIsectNormal(Point eyePoint, Vector ray, double dist){
    Point hit = eyePoint + (ray * dist);
    //cout << "sphere hit point "<<hit[0]<<" "<<hit[1]<<" "<<hit[2]<<"\n";
    Vector norm;
    norm[0] = hit[0];
    norm[1] = hit[1];
    norm[2] = hit[2];
    
    return norm; 
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
