#define _CRT_SECURE_NO_WARNINGS

#ifndef CAMERA_H
#define CAMERA_H
#include "Algebra.h"

class Camera {
public:
Camera();
~Camera();
void Reset();
void Orient(Point& eye, Point& focus, Vector& up);
void Orient(Point& eye, Vector& look, Vector& up);
void SetViewAngle (double viewAngle);
void SetNearPlane (double nearPlane);
void SetFarPlane (double farPlane);
void SetScreenSize (int screenWidth, int screenHeight);

Matrix GetProjectionMatrix();
Matrix GetModelViewMatrix();
Matrix GetFilmToWorldMatrix();

void RotateV(double angle);
void RotateU(double angle);
void RotateW(double angle);
void Rotate(Point p, Vector axis, double degree);
void Translate(const Vector &v);

Point GetEyePoint();
Vector GetLookVector();
Vector GetUpVector();
double GetViewAngle();
double GetNearPlane();
double GetFarPlane();
int GetScreenWidth();
int GetScreenHeight();

double GetFilmPlanDepth();
double GetScreenWidthRatio();

private:
Matrix scaling;
Matrix unhinging;
Matrix projection;
Matrix eyeTranslation;
Matrix basisRotation;
Matrix modelView;
Point eyePoint;
Vector lookVector;
Vector upVector;
Vector u;
Vector v;
Vector w;
double thetaH;
double m_near;
double m_far;
int width;
int height;
double rotationV;
double rotationU;
double rotationW;

};
#endif

