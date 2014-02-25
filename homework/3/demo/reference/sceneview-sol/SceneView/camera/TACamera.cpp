/*************************************************************************
 *     NAME: Nong Li
 *     USER: nli
 *     FILE: Camera.cpp
 *     DATE: Fri Aug 31  2:12:46 2007
 *************************************************************************/
#include "TACamera.h"
#include <qgl.h>
#include <iostream>
using namespace std;

void CS123Camera::rebuildModelviewMatrix()
{
    m_modelviewMatrix = Matrix4x4(
        m_u.data[0], m_u.data[1], m_u.data[2], -m_eye.dot(m_u),
        m_v.data[0], m_v.data[1], m_v.data[2], -m_eye.dot(m_v),
        m_w.data[0], m_w.data[1], m_w.data[2], -m_eye.dot(m_w),
        0, 0, 0, 1);
}

void CS123Camera::rebuildProjectionMatrix()
{
    double tan_theta_h = tan(m_heightAngle / 2.0 * M_PI / 180.0);
    double tan_theta_w = tan_theta_h * m_aspectRatio;
    double one_over_far = 1.0 / m_far;
    double c = -1 * m_near * one_over_far;

    m_projectionMatrix = Matrix4x4(
        one_over_far / tan_theta_w, 0, 0, 0,
        0, one_over_far / tan_theta_h, 0, 0,
        0, 0, -one_over_far / (c + 1.0), c / (c + 1.0),
        0, 0, -one_over_far, 0);
}


/*************************************************************************
 * Function Name: CS123Camera::putWorldToFilm
 * Parameters: 
 * Returns: void
 * Effects: 
 *************************************************************************/

Matrix4x4
CS123Camera::getProjectionMatrix() const {
    return m_projectionMatrix;
}

Matrix4x4
CS123Camera::getModelviewMatrix() const {
    return m_modelviewMatrix;
}


/*************************************************************************
 * Function Name: CS123Camera::getPosition
 * Parameters: 
 * Returns: Point3
 * Effects: 
 *************************************************************************/
Vector4
CS123Camera::getPosition() const
{
    return m_eye;
}


/*************************************************************************
 * Function Name: CS123Camera::getLook
 * Parameters: 
 * Returns: Vector4
 * Effects: 
 *************************************************************************/
Vector4
CS123Camera::getLook() const
{
    return -m_w;
}


/*************************************************************************
 * Function Name: CS123Camera::getUp
 * Parameters: 
 * Returns: Vector4
 * Effects: 
 *************************************************************************/
Vector4
CS123Camera::getUp() const
{
    return m_v;
}


/*************************************************************************
 * Function Name: CS123Camera::getAspectRatio
 * Parameters: 
 * Returns: float
 * Effects: 
 *************************************************************************/
float
CS123Camera::getAspectRatio() const
{
    return m_aspectRatio;
}


/*************************************************************************
 * Function Name: CS123Camera::getHeightAngle
 * Parameters: 
 * Returns: float
 * Effects: 
 *************************************************************************/
float
CS123Camera::getHeightAngle() const
{
    return m_heightAngle;
}


/*************************************************************************
 * Function Name: CS123Camera::orientLook
 * Parameters: const Point3 &eye, const Vector4 &look,  const Vector4 &up
 * Returns: void
 * Effects: 
 *************************************************************************/
    void
CS123Camera::orientLook(const Vector4 &eye, const Vector4 &look,  const Vector4 &up)
{
    m_eye = eye;
    m_w = -look.getNormalized();
    m_v = (up - m_w * up.dot(m_w)).getNormalized();
    m_u = m_v.cross(m_w).getNormalized();
    
    rebuildModelviewMatrix();
    rebuildProjectionMatrix();
}


/*************************************************************************
 * Function Name: CS123Camera::setHeightAngle
 * Parameters: float h
 * Returns: void
 * Effects: 
 *************************************************************************/
    void
CS123Camera::setHeightAngle(float h)
{
    m_heightAngle = h;

        rebuildProjectionMatrix();
}


/*************************************************************************
 * Function Name: CS123Camera::setAspectRatio
 * Parameters: float a
 * Returns: void
 * Effects: 
 *************************************************************************/
    void
CS123Camera::setAspectRatio(float a)
{
    m_aspectRatio = a;

        rebuildProjectionMatrix();
}


/*************************************************************************
 * Function Name: CS123Camera::translate
 * Parameters: const Vector4 &v
 * Returns: void
 * Effects: 
 *************************************************************************/
    void
CS123Camera::translate(const Vector4 &v)
{
    m_eye += v;

        rebuildModelviewMatrix();
}


/*************************************************************************
 * Function Name: CS123Camera::rotateX
 * Parameters: float degrees
 * Returns: void
 * Effects: 
 *************************************************************************/
    void
CS123Camera::rotateU(float degrees)
{
    float s = sinf(degrees * M_PI / 180.0f);
    float c = cosf(degrees * M_PI / 180.0f);
    Vector4 v = m_v * c - m_w * s;
    Vector4 w = m_v * s + m_w * c;
    m_v = v;
    m_w = w;
    
        rebuildModelviewMatrix();
}


/*************************************************************************
 * Function Name: CS123Camera::rotateY
 * Parameters: float degrees
 * Returns: void
 * Effects: 
 *************************************************************************/
    void
CS123Camera::rotateV(float degrees)
{
    float s = sinf(degrees * M_PI / 180.0f);
    float c = cosf(degrees * M_PI / 180.0f);
    Vector4 u = m_u * c - m_w * s;
    Vector4 w = m_u * s + m_w * c;
    m_u = u;
    m_w = w;
    
        rebuildModelviewMatrix();
}


/*************************************************************************
 * Function Name: CS123Camera::rotateZ
 * Parameters: float degrees
 * Returns: void
 * Effects: 
 *************************************************************************/
    void
CS123Camera::rotateN(float degrees)
{
    float s = sinf(degrees * M_PI / 180.0f);
    float c = cosf(degrees * M_PI / 180.0f);
    Vector4 v = m_v * c - m_u * s;
    Vector4 u = m_v * s + m_u * c;
    m_v = v;
    m_u = u;
    
        rebuildModelviewMatrix();
}


/*************************************************************************
 * Function Name: CS123Camera::setClip
 * Parameters: float near, float far
 * Returns: void
 * Effects: 
 *************************************************************************/
    void
CS123Camera::setClip(float nearPlane, float farPlane)
{
        m_near = nearPlane;
        m_far = farPlane;
    
        rebuildProjectionMatrix();
}


