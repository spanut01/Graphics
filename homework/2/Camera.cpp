#include "Camera.h"

Camera::Camera() {
}

Camera::~Camera() {
}

void Camera::Orient(Point& eye, Point& focus, Vector& up) {
    eyePoint = eye;
    Vector temp = focus - eye;
    lookVector = normalize(temp);
    //set change of basis
    u = cross(lookVector, up);
    upVector = cross(u, lookVector);
    v = upVector;
    w = -lookVector;
    
}


void Camera::Orient(Point& eye, Vector& look, Vector& up) {
    eyePoint = eye;
    lookVector = normalize(look);
    //set change of basis
    u = cross(lookVector, up);
    upVector = cross(u, lookVector);
    v = upVector;
    w = -lookVector;
}

Matrix Camera::GetProjectionMatrix() {
    scaling[0] = 1.0/(tan(thetaH/2.0)*far);//TODO this uses the wrong theta
    scaling[5] = 1.0/(tan(thetaH/2.0)*far);
    scaling[10] = 1.0/far;

    unhinging[10] = 1.0/(near+1.0);
    unhinging[11] = -near/(near+1.0);
    unhinging[14] = -1.0;
    unhinging[15] = 0.0;

    //projection = unhinging * scaling;
    return projection;
}


void Camera::SetViewAngle (double viewAngle) {
    thetaH = viewAngle;
}

void Camera::SetNearPlane (double nearPlane) {
    near = nearPlane;
}

void Camera::SetFarPlane (double farPlane) {
    far = farPlane;
}

void Camera::SetScreenSize (int screenWidth, int screenHeight) {
    width = screenWidth;
    height = screenHeight;
}

Matrix Camera::GetModelViewMatrix() {
    //set translation
    eyeTranslation[3] = -eyePoint[0];
    eyeTranslation[7] = -eyePoint[1];
    eyeTranslation[11] = -eyePoint[2];

    //set change of basis
    basisRotation[0] = u[0];
    basisRotation[1] = u[1];
    basisRotation[2] = u[2];

    basisRotation[4] = v[0];
    basisRotation[5] = v[1];
    basisRotation[6] = v[2];

    basisRotation[8] = w[0];
    basisRotation[9] = w[1];
    basisRotation[10] = w[2];

    modelView = basisRotation * eyeTranslation;
    modelView.print();
    return modelView;
}

void Camera::RotateV(double angle) {
}

void Camera::RotateU(double angle) {
}

void Camera::RotateW(double angle) {
}

//unused
void Camera::Translate(const Vector &v) {
}


void Camera::Rotate(Point p, Vector axis, double degrees) {
}


Point Camera::GetEyePoint() {
	return eyePoint;
}

Vector Camera::GetLookVector() {
	return lookVector;
}

Vector Camera::GetUpVector() {
	return upVector;
}

double Camera::GetViewAngle() {
	return thetaH;
}

double Camera::GetNearPlane() {
	return near;
}

double Camera::GetFarPlane() {
	return far;
}

int Camera::GetScreenWidth() {
	return width;
}

int Camera::GetScreenHeight() {
	return height;
}

//unused
double Camera::GetFilmPlanDepth() {
	return 0;
}

//unused
double Camera::GetScreenWidthRatio() {
	return 0;
}
