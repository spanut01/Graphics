#include "Camera.h"

Camera::Camera() {
}

Camera::~Camera() {
}

void Camera::Reset(){
    scaling.init();
    unhinging.init();
    projection.init();
    eyeTranslation.init();
    basisRotation.init();
    modelView.init();
    eyePoint = Point(0.0,0.0,1.0);
    lookVector = Vector(0.0,0.0,-1.0);
    upVector = Vector(0,1,0);
    Orient(eyePoint, lookVector, upVector);
    thetaH = 60.0;
    m_near = 0.001;
    m_far = 30.0;
    width = 500;
    height = 500;
    rotationV = 0.0;
    rotationU = 0.0;
    rotationW = 0.0;
}

void Camera::Orient(Point& eye, Point& focus, Vector& up) {
    eyePoint = eye;
    Vector temp = focus - eye;
    lookVector = normalize(temp);
    //set change of basis
    u = cross(lookVector, up);
    u = normalize(u);
    upVector = cross(-lookVector, u);
    v = normalize(upVector);
    w = -lookVector;
    
}

//this one is actually called
void Camera::Orient(Point& eye, Vector& look, Vector& up) {
    eyePoint = eye;
    lookVector = normalize(look);
    //set change of basis
    u = cross(lookVector, up);
    u = normalize(u);
    //fprintf(stderr,"\nu:\n");
    //u.print();
    upVector = cross(-lookVector, u);
    v = normalize(upVector);
    //fprintf(stderr,"\nv:\n");
    //v.print();
    w = -lookVector;
    //fprintf(stderr,"\nw:\n");
    //w.print();
}

Matrix Camera::GetProjectionMatrix() {
    //fprintf(stderr,"theta_h: %f\n",PI*thetaH/360.0);//same
    double thetaW = ((double)width/(double)height)*thetaH;
    //double ratio = (double)width/(double)height;
    scaling[0] = 1.0/(tan(PI * thetaW/360.0)*m_far);
    //scaling[0] = 1.0/(tan(PI * thetaH/360.0)*m_m_far*ratio);
    scaling[5] = 1.0/(tan(PI * thetaH/360.0)*m_far);
    scaling[10] = 1.0/m_far;
    //fprintf(stderr,"\nscaling:\n");
    //scaling.print();
    
    float c = -m_near / m_far;
    unhinging[10] = -1.0/(c+1.0);
    unhinging[14] = c/(c+1.0);

    //unhinging[10] = -(m_far + m_near) / (m_far - m_near);
    //unhinging[11] = (-2 * m_far * m_near)/(m_far - m_near);
    unhinging[11] = -1.0;
    unhinging[15] = 0.0;
    //fprintf(stderr,"\nunhinging:\n");
    //unhinging.print();

    projection = scaling * unhinging;
    //projection = transpose(projection); 
    //fprintf(stderr,"\nprojection:\n");
    //projection.print();
    return projection;
}


void Camera::SetViewAngle (double viewAngle) {
    thetaH = viewAngle;
}

void Camera::SetNearPlane (double nearPlane) {
    m_near = nearPlane;
}

void Camera::SetFarPlane (double farPlane) {
    m_far = farPlane;
}

void Camera::SetScreenSize (int screenWidth, int screenHeight) {
    width = screenWidth;
    height = screenHeight;
}

Matrix Camera::GetFilmToWorldMatrix(){
	GetProjectionMatrix();
	//scaling.print();
	GetModelViewMatrix();
	//modelView.print();
        Matrix m = invert(scaling * modelView);
        //m.print();
    return m;
}

Matrix Camera::GetModelViewMatrix() {
    //set change of basis
    basisRotation[0] = u[0];
    basisRotation[4] = u[1];
    basisRotation[8] = u[2];

    basisRotation[1] = v[0];
    basisRotation[5] = v[1];
    basisRotation[9] = v[2];

    basisRotation[2] = w[0];
    basisRotation[6] = w[1];
    basisRotation[10] = w[2];
    //printf("basisRotation\n");
    //basisRotation.print();
    
    Vector eyeTranslateV (-eyePoint[0], -eyePoint[1], -eyePoint[2]);
    Matrix eyeTranslate = trans_mat(eyeTranslateV);
    //printf("eye: \n");
    //eyeTranslate.print();
    modelView = basisRotation * eyeTranslate;
    
    //Point translatedEye = (basisRotation * -eyePoint);
    //fprintf(stderr,"\ntranslatedEye:\n");
    //translatedEye.print();
    
    //set translation
    //modelView = basisRotation;
    //modelView[12] = translatedEye[0];
    //modelView[13] = translatedEye[1];
    //modelView[14] = translatedEye[2];
    
    //fprintf(stderr,"\nmodelView:\n");
    //modelView.print();
    
    return modelView;
}

void Camera::RotateV(double angle) {
    rotationV = PI * angle / 180.0;
    Vector tempu = cos(rotationV) * u + sin(rotationV) * w;
    Vector tempw = cos(rotationV) * w - sin(rotationV) * u;
    u = tempu;
    w = tempw;
}
void Camera::RotateU(double angle) {
    rotationU = PI * angle / 180.0;
    Vector tempw = cos(rotationU) * w + sin(rotationU) * v;
    Vector tempv = cos(rotationU) * v - sin(rotationU) * w;
    w = tempw;
    v = tempv;
}

void Camera::RotateW(double angle) {
    rotationW = PI * angle / 180.0;
    Vector tempu = cos(rotationW)*u - sin(rotationW)*v;
    Vector tempv = sin(rotationW)*u + cos(rotationW)*v;
    u = tempu;
    v = tempv;
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
	return m_near;
}

double Camera::GetFarPlane() {
	return m_far;
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
