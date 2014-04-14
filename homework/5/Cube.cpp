#include "Cube.h"
#include <iostream> 
using namespace std;
double Cube::Intersect(Point eyeP, Vector rayV, Matrix worldToObj){
    //Matrix worldToObj = invert(transformMatrix);
    Point p = worldToObj * eyeP;
    Vector d = worldToObj * rayV;
//    cout << " rayV : " << rayV[0] << ", " << rayV[1] << ", " << rayV[2] << "\n" ;
//    cout << " eyeP : " << eyeP[0] << ", " <<  eyeP[1] << ", " << eyeP[2] << "\n" ;
    //cout << " d : " << d[0] << ", " <<  d[1] << ", " << d[2] << "\n" ;
    //cout << " p : " << p[0] << ", " <<  p[1] << ", " << p[2] << "\n\n" ;
    double t = 1000000; 
    double tempT, temp1, temp2;
    int side; 
    for (int i = 0; i < 3; i++){
        tempT = (0.5 - p[i]) / d[i];
        temp1 = p[(i + 1) % 3] + tempT * d[(i + 1) % 3];
        temp2 = p[(i + 2) % 3] + tempT * d[(i + 2) % 3];
        
        if ((tempT > 0.0) && (temp1 > -0.50001) && (temp1 < 0.50001) && (temp2 > -0.50001) && (temp2 < 0.50001))  {
            if (tempT < t){
                t = tempT;
                side = i;
            }
        }
    }
    for (int i = 0; i < 3; i++){
        tempT = (-0.5 - p[i]) / d[i];
        temp1 = p[(i + 1) % 3] + tempT * d[(i + 1) % 3];
        temp2 = p[(i + 2) % 3] + tempT * d[(i + 2) % 3];
        
        if ((tempT > 0.0) && (temp1 > -0.50001) && (temp1 < 0.50001) && (temp2 > -0.50001) && (temp2 < 0.50001))  {
            if (tempT < t){
                t = tempT;
                side = i + 3;
            }
        }
    }

    if (t == 1000000){
        return -1;
    }
    return t;
}
Vector Cube::findIsectNormal(Point eyePoint, Vector ray, double dist){
    Point hit = eyePoint + (ray * dist);
    //cout<<"Cube Hit "<<hit[0]<<" "<<hit[1]<<" "<<hit[2]<<"\n";

    double fudgeFactor = 0.00001;
    /*
    Vector ret = Vector(0.0, 0.0, 0.0);
    if(fabs(hit[0] + 0.5) < fudgeFactor)ret[0] = -1;
    if(fabs(hit[0] - 0.5) < fudgeFactor)ret[0] = 1;
    if(fabs(hit[1] + 0.5) < fudgeFactor)ret[1] = -1;
    if(fabs(hit[1] - 0.5) < fudgeFactor)ret[1] = 1;
    if(fabs(hit[2] + 0.5) < fudgeFactor)ret[2] = -1;
    if(fabs(hit[2] - 0.5) < fudgeFactor)ret[2] = 1;
    */
    if(fabs(hit[0] + 0.5) < fudgeFactor)return Vector(-1.0, 0.0, 0.0);
    if(fabs(hit[0] - 0.5) < fudgeFactor)return Vector(1.0, 0.0, 0.0);
    if(fabs(hit[1] + 0.5) < fudgeFactor)return Vector(0.0, -1.0, 0.0);
    if(fabs(hit[1] - 0.5) < fudgeFactor)return Vector(0.0, 1.0, 0.0);
    if(fabs(hit[2] + 0.5) < fudgeFactor)return Vector(0.0, 0.0, -1.0);
    if(fabs(hit[2] - 0.5) < fudgeFactor)return Vector(0.0, 0.0, 1.0);
    
    cout<<"Cube Normal ERROR\n";
    //return ret;
    return Vector(0.0, 0.0, 0.0);
}

Point Cube::iPointToSquare(Point i, Vector ray, double dist){
    Point hit = i + (ray * dist);

    double fudgeFactor = 0.00001;

    if(fabs(hit[0] + 0.5) < fudgeFactor)return Point(-hit[2] + 0.5, -hit[1] + 0.5, 0.0);
    if(fabs(hit[0] - 0.5) < fudgeFactor)return Point(-hit[2] + 0.5, -hit[1] + 0.5, 0.0);
    if(fabs(hit[1] + 0.5) < fudgeFactor)return Point(hit[0] + 0.5, -hit[2] + 0.5, 0.0);
    if(fabs(hit[1] - 0.5) < fudgeFactor)return Point(hit[0] + 0.5, -hit[2] + 0.5, 0.0);
    if(fabs(hit[2] + 0.5) < fudgeFactor)return Point(hit[0] + 0.5, -hit[1] + 0.5, 0.0);
    if(fabs(hit[2] - 0.5) < fudgeFactor)return Point(hit[0] + 0.5, -hit[1] + 0.5, 0.0);

    return Point(0.0, 0.0, 0.0);
}

void Cube::drawTriangles(){

  int i, j;
  float x,y;
  x = 1.0f / m_segmentsX;
  y = 1.0f / m_segmentsY;

  for(i=0; i<m_segmentsX; i++){
      for(j=0; j<m_segmentsY; j++){
          //front face (+z)
          glNormal3f(0.0f, 0.0f, 1.0f);
          glVertex3f(-0.5f+i*x, -0.5f+j*y, 0.5f);
          glVertex3f(-0.5f+i*x, -0.5f+(j+1)*y, 0.5f);
          glVertex3f(-0.5f+(i+1)*x, -0.5f+j*y, 0.5f);
          
          glVertex3f(-0.5f+i*x, -0.5f+(j+1)*y, 0.5f);
          glVertex3f(-0.5f+(i+1)*x, -0.5f+(j+1)*y, 0.5f);
          glVertex3f(-0.5f+(i+1)*x, -0.5f+j*y, 0.5f);
           
          //back face (-z)
          glNormal3f(0.0f, 0.0f, -1.0f);
          glVertex3f(-0.5f+(i+1)*x, -0.5f+j*y, -0.5f);
          glVertex3f(-0.5f+i*x, -0.5f+(j+1)*y, -0.5f);
          glVertex3f(-0.5f+i*x, -0.5f+j*y, -0.5f);
            
          glVertex3f(-0.5f+i*x, -0.5f+(j+1)*y, -0.5f);
          glVertex3f(-0.5f+(i+1)*x, -0.5f+j*y, -0.5f);
          glVertex3f(-0.5f+(i+1)*x, -0.5f+(j+1)*y, -0.5f);
            
          //right (+x) face
          glNormal3f(1.0f, 0.0f, 0.0f);
          glVertex3f(0.5f, -0.5f+j*y, -0.5f+i*x);
          glVertex3f(0.5f, -0.5f+(j+1)*y, -0.5f+i*x);
          glVertex3f(0.5f, -0.5f+j*y, -0.5f+(i+1)*x);
              
          glVertex3f(0.5f, -0.5f+(j+1)*y, -0.5f+(i+1)*x);
          glVertex3f(0.5f, -0.5f+(j+1)*y, -0.5f+i*x);
          glVertex3f(0.5f, -0.5f+j*y, -0.5f+(i+1)*x);
            
          //left (-x) face
          glNormal3f(-1.0f, 0.0f, 0.0f);
          glVertex3f(-0.5f, -0.5f+j*y, -0.5f+i*x);
          glVertex3f(-0.5f, -0.5f+(j+1)*y, -0.5f+i*x);
          glVertex3f(-0.5f, -0.5f+j*y, -0.5f+(i+1)*x);
            
          glVertex3f(-0.5f, -0.5f+(j+1)*y, -0.5f+(i+1)*x);
          glVertex3f(-0.5f, -0.5f+(j+1)*y, -0.5f+i*x);
          glVertex3f(-0.5f, -0.5f+j*y, -0.5f+(i+1)*x);
             
          //top (+y) face
          glNormal3f(0.0f, 1.0f, 0.0f);
          glVertex3f(-0.5f+j*y, 0.5f, -0.5f+i*x);
          glVertex3f(-0.5f+(j+1)*y, 0.5f, -0.5f+i*x);
          glVertex3f(-0.5f+j*y, 0.5f, -0.5f+(i+1)*x);
            
          glVertex3f(-0.5f+(j+1)*y, 0.5f, -0.5f+(i+1)*x);
          glVertex3f(-0.5f+(j+1)*y, 0.5f, -0.5f+i*x);
          glVertex3f(-0.5f+j*y, 0.5f, -0.5f+(i+1)*x);
             
          //bottom (-y) face
          glNormal3f(0.0f, -1.0f, 0.0f);
          glVertex3f(-0.5f+j*y, -0.5f, -0.5f+i*x);
          glVertex3f(-0.5f+(j+1)*y, -0.5f, -0.5f+i*x);
          glVertex3f(-0.5f+j*y, -0.5f, -0.5f+(i+1)*x);
              
          glVertex3f(-0.5f+(j+1)*y, -0.5f, -0.5f+(i+1)*x);
          glVertex3f(-0.5f+(j+1)*y, -0.5f, -0.5f+i*x);
          glVertex3f(-0.5f+j*y, -0.5f, -0.5f+(i+1)*x);
      }
  }
};

void Cube::drawNormals(){
  int i,j;
  float x,y;
  x = 1.0f / m_segmentsX;
  y = 1.0f / m_segmentsY;


  for(i=0; i<m_segmentsX; i++){
      for(j=0; j<m_segmentsY; j++){
          //front face (+z)
          glVertex3f(-0.5f+i*x, -0.5f+j*y, 0.5f);
          glVertex3f(-0.5f+i*x, -0.5f+j*y, 0.6f);
          glVertex3f(-0.5f+i*x, -0.5f+(j+1)*y, 0.5f);
          glVertex3f(-0.5f+i*x, -0.5f+(j+1)*y, 0.6f);
          glVertex3f(-0.5f+(i+1)*x, -0.5f+j*y, 0.5f);
          glVertex3f(-0.5f+(i+1)*x, -0.5f+j*y, 0.6f);
              
          //back face (-z)
          glVertex3f(-0.5f+(i+1)*x, -0.5f+j*y, -0.5f);
          glVertex3f(-0.5f+(i+1)*x, -0.5f+j*y, -0.6f);
          glVertex3f(-0.5f+i*x, -0.5f+(j+1)*y, -0.5f);
          glVertex3f(-0.5f+i*x, -0.5f+(j+1)*y, -0.6f);
          glVertex3f(-0.5f+i*x, -0.5f+j*y, -0.5f);
          glVertex3f(-0.5f+i*x, -0.5f+j*y, -0.6f);
            
          //right (+x) face
          glVertex3f(0.5f, -0.5f+j*y, -0.5f+i*x);
          glVertex3f(0.6f, -0.5f+j*y, -0.5f+i*x);
          glVertex3f(0.5f, -0.5f+(j+1)*y, -0.5f+i*x);
          glVertex3f(0.6f, -0.5f+(j+1)*y, -0.5f+i*x);
          glVertex3f(0.5f, -0.5f+j*y, -0.5f+(i+1)*x);
          glVertex3f(0.6f, -0.5f+j*y, -0.5f+(i+1)*x);
              
          //left (-x) face
          glVertex3f(-0.5f, -0.5f+j*y, -0.5f+i*x);
          glVertex3f(-0.6f, -0.5f+j*y, -0.5f+i*x);
          glVertex3f(-0.5f, -0.5f+(j+1)*y, -0.5f+i*x);
          glVertex3f(-0.6f, -0.5f+(j+1)*y, -0.5f+i*x);
          glVertex3f(-0.5f, -0.5f+j*y, -0.5f+(i+1)*x);
          glVertex3f(-0.6f, -0.5f+j*y, -0.5f+(i+1)*x);
              
          //top (+y) face
          glVertex3f(-0.5f+j*y, 0.5f, -0.5f+i*x);
          glVertex3f(-0.5f+j*y, 0.6f, -0.5f+i*x);
          glVertex3f(-0.5f+(j+1)*y, 0.5f, -0.5f+i*x);
          glVertex3f(-0.5f+(j+1)*y, 0.6f, -0.5f+i*x);
          glVertex3f(-0.5f+j*y, 0.5f, -0.5f+(i+1)*x);
          glVertex3f(-0.5f+j*y, 0.6f, -0.5f+(i+1)*x);
              
          //bottom (-y) face
          glVertex3f(-0.5f+j*y, -0.5f, -0.5f+i*x);
          glVertex3f(-0.5f+j*y, -0.6f, -0.5f+i*x);
          glVertex3f(-0.5f+(j+1)*y, -0.5f, -0.5f+i*x);
          glVertex3f(-0.5f+(j+1)*y, -0.6f, -0.5f+i*x);
          glVertex3f(-0.5f+j*y, -0.5f, -0.5f+(i+1)*x);
          glVertex3f(-0.5f+j*y, -0.6f, -0.5f+(i+1)*x);
      }
  }
};
