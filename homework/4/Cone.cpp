#include "Cone.h"

double Cone::Intersect(Point eyeP, Vector rayV, Matrix worldToObj){
    double a, b, c, det, t1, t2, y1, y2;
    double t = 1000000;
    //Matrix worldToObj = invert(transformMatrix);
    Point p = worldToObj * eyeP;
    Vector d = worldToObj * rayV;

    a = d[0]*d[0] + d[2]*d[2] - 0.25*d[1]*d[1];
    b = 2.0*p[0]*d[0] + 2.0*p[2]*d[2] - 0.5*p[1]*d[1] + 0.25*d[1];
    c = p[0]*p[0] + p[2]*p[2] - 0.25*p[1]*p[1] - 0.0625 + 0.25*p[1];
    det = b*b - 4.0*a*c;
    if(det<0.0)return -1.0;
    
    if(det > 0.0){
        t1 = (- b - sqrt(det)) / (2 * a);
        t2 = (- b + sqrt(det)) / (2 * a);
        y1 = p[1] + t1 * d[1];
        y2 = p[1] + t2 * d[1];
        if(t1 > 0.0 && y1 > -0.5 && y1 < 0.5){
            t = t1;
        }
        if(t2 > 0.0 && y2 > -0.5 && y2 < 0.5 && t2 < t1){
            t = t2;
        }
    }
    t1 = (-0.5 - p[1]) / d[1];
    a = p[0] + t1 * d[0];
    b = p[2] + t1 * d[2];
    if(a > -0.5 && a*a + b*b < 0.25 && t1 < t){
        t = t1;
    }

    if(t == 1000000)return -1;
    return t;
}
Vector Cone::findIsectNormal(Point eyePoint, Vector ray, double dist){
    Point hit = eyePoint + (ray * dist);
    //cap
    float fudgeFactor = 0.00000005;
    if(hit[1] + 0.5 < fudgeFactor)return Vector(0.0, -1.0, 0.0);
    //body
    //unsure of this, "up" should be half? as much as "out"
    Vector ret(hit[0], 0.25, hit[2]);
    ret.normalize();
    return ret; 
}

void Cone::drawTriangles(){
    int i,j;
    float x,y;
    x = PI * (2.0f / m_segmentsX);
    y = 0.5f / m_segmentsY;
    for (i = 0; i < m_segmentsX; i++){    
        //bottom
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(0.0f, -0.5f, 0.0f);
        glVertex3f(0.5f*cosf(i*x), -0.5f, 0.5f*sinf(i*x));
        glVertex3f(0.5f*cosf((i+1)*x), -0.5f, 0.5f*sinf((i+1)*x));
        for (j = 0; j < m_segmentsY; j++){
            // sides
            glNormal3f(-cosf((i+0.5f)*x), 0.0f, -sinf((i+0.5f)*x));

            glVertex3f((0.5f-(y*j))*cosf(i*x), -0.5f+(y*j*2), (-0.5f+(y*j))*sinf(i*x));
            glVertex3f((0.5f-(y*(j+1)))*cosf(i*x), -0.5f+(y*(j+1)*2), (-0.5f+(y*(j+1)))*sinf(i*x));
            glVertex3f((0.5f-(y*j))*cosf((i+1)*x),-0.5f+(y*j*2), (-0.5f+(y*j))*sinf((i+1)*x));

            glVertex3f((0.5f-(y*(j+1)))*cosf((i+1)*x), -0.5f+(y*(j+1)*2), (-0.5f+(y*(j+1)))*sinf((i+1)*x));
            glVertex3f((0.5f-(y*(j+1)))*cosf(i*x), -0.5f+(y*(j+1)*2), (-0.5f+(y*(j+1)))*sinf(i*x));
            glVertex3f((0.5f-(y*j))*cosf((i+1)*x),-0.5f+(y*j*2), (-0.5f+(y*j))*sinf((i+1)*x));

        }
    }
}

void Cone::drawNormals(){
    int i, j;
    float x, y;    
    x = PI * (2.0f / m_segmentsX);
    y = 0.5f / m_segmentsY;
    //sides
    for (i = 0; i < m_segmentsX; i++){
        //sides
        for (j = 0; j < m_segmentsY; j++){
            glVertex3f((0.5f-(y*(j)))*cosf(i*x), -0.5f+(y*(j)*2), (-0.5f+(y*(j)))*sinf(i*x));
            glVertex3f((0.6f-(y*(j)))*cosf(i*x), (-0.5f+(y*(j)*2))+0.05f, (-0.6f+(y*(j)))*sinf(i*x));
        }
        //bottom
        glVertex3f(0.5f*cosf(i*x), -0.5f, 0.5f*sinf(i*x));
        glVertex3f(0.5f*cosf(i*x), -0.6f, 0.5f*sinf(i*x));
        glVertex3f(0.5f*cosf((i+1)*x), -0.5f, 0.5f*sinf((i+1)*x));
        glVertex3f(0.5f*cosf((i+1)*x), -0.6f, 0.5f*sinf((i+1)*x));
    }
    //top
    glVertex3f(0.0f, 0.5f, 0.0f);
    glVertex3f(0.0f, 0.6f, 0.0f);
    //bottom
    glVertex3f(0.0f, -0.5f, 0.0f);
    glVertex3f(0.0f, -0.6f, 0.0f);
    
}
