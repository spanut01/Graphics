#include "Torus.h"

void Torus::drawTriangles(){
    int i,j;
    float c,a;
    c = 0.5f;
    a = 0.25f;
    float da = 1.5f;
    for(i=0; i<m_segmentsX; i++){
        float u = i*PI*(2.0f/m_segmentsX);
        float du = (i+1)*PI*(2.0f/m_segmentsX);
        for(j=0; j<m_segmentsY; j++){
            float v = j*PI*(2.0f/m_segmentsY);
            float dv = (j+1)*PI*(2.0f/m_segmentsY);
            glNormal3f((c+da*cosf(v))*cosf(u)-(c+a*cosf(v))*cosf(u),(c+da*cosf(v))*sinf(u)-(c+a*cosf(v))*sinf(u),da*sinf(v)-a*sinf(v));
            glVertex3f((c+a*cosf(v))*cosf(u),(c+a*cosf(v))*sinf(u),a*sinf(v));
            glVertex3f((c+a*cosf(dv))*cosf(u),(c+a*cosf(dv))*sinf(u),a*sinf(dv));
            glVertex3f((c+a*cosf(v))*cosf(du),(c+a*cosf(v))*sinf(du),a*sinf(v));

            glVertex3f((c+a*cosf(dv))*cosf(du),(c+a*cosf(dv))*sinf(du),a*sinf(dv));
            glVertex3f((c+a*cosf(dv))*cosf(u),(c+a*cosf(dv))*sinf(u),a*sinf(dv));
            glVertex3f((c+a*cosf(v))*cosf(du),(c+a*cosf(v))*sinf(du),a*sinf(v));            
        }
    }
}

void Torus::drawNormals(){
    int i,j;
    float c,a;
    c = 0.5f;
    a = 0.25f;
    float da = 0.3f;
    for(i=0; i<m_segmentsX; i++){
        float u = i*PI*(2.0f/m_segmentsX);
        float du = (i+1)*PI*(2.0f/m_segmentsX);
        for(j=0; j<m_segmentsY; j++){
            float v = j*PI*(2.0f/m_segmentsY);
            float dv = (j+1)*PI*(2.0f/m_segmentsY);

            glVertex3f((c+a*cosf(v))*cosf(u),(c+a*cosf(v))*sinf(u),a*sinf(v));
            glVertex3f((c+da*cosf(v))*cosf(u),(c+da*cosf(v))*sinf(u),da*sinf(v));
            glVertex3f((c+a*cosf(dv))*cosf(u),(c+a*cosf(dv))*sinf(u),a*sinf(dv));
            glVertex3f((c+da*cosf(dv))*cosf(u),(c+da*cosf(dv))*sinf(u),da*sinf(dv));
            glVertex3f((c+a*cosf(v))*cosf(du),(c+a*cosf(v))*sinf(du),a*sinf(v));
            glVertex3f((c+da*cosf(v))*cosf(du),(c+da*cosf(v))*sinf(du),da*sinf(v));
        }
    }
}
