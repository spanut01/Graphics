/*!
   @file   CamtransCamera.cpp
   @author Ben Herila (ben@herila.net)
   @author Evan Wallace (evan.exe@gmail.com)
   @date   Fall 2010

   @brief  This is the perspective camera class you will need to fill in for the
   Camtrans assignment.  See the assignment handout for more details.

   For extra credit, you can also create an Orthographic camera. Create another
   class if you want to do that.

*/

#include "CamtransCamera.h"
#include <Settings.h>
#include <qgl.h>

// begin ta code
#include "TACamera.h"

CamtransCamera::CamtransCamera() {}

void CamtransCamera::setAspectRatio(float a) { m_cameraImpl.setAspectRatio(a); }

Matrix4x4 CamtransCamera::getProjectionMatrix() const {
    return m_cameraImpl.getProjectionMatrix(); }

Matrix4x4 CamtransCamera::getModelviewMatrix() const {
    return m_cameraImpl.getModelviewMatrix(); }

Vector4 CamtransCamera::getPosition() const {
    return m_cameraImpl.getPosition();}

Vector4 CamtransCamera::getLook() const {
    return m_cameraImpl.getLook();}

Vector4 CamtransCamera::getUp() const{
    return m_cameraImpl.getUp();}

float CamtransCamera::getAspectRatio() const{
    return m_cameraImpl.getAspectRatio();}

float CamtransCamera::getHeightAngle() const{
    return m_cameraImpl.getHeightAngle();}

void CamtransCamera::orientLook(const Vector4 &eye, const Vector4 &look, const Vector4 &up){
    m_cameraImpl.orientLook(eye, look, up);}

void CamtransCamera::setHeightAngle(float h){
    m_cameraImpl.setHeightAngle(h);}

void CamtransCamera::translate(const Vector4 &v){
    m_cameraImpl.translate(v);}

void CamtransCamera::rotateU(float degrees){
    m_cameraImpl.rotateU(degrees);}

void CamtransCamera::rotateV(float degrees){
    m_cameraImpl.rotateV(degrees);}

void CamtransCamera::rotateN(float degrees){
    m_cameraImpl.rotateN(degrees);}

void CamtransCamera::setClip(float nearPlane, float farPlane){
    m_cameraImpl.setClip(nearPlane, farPlane); }

void CamtransCamera::mouseDown(int x, int y)
{
    m_oldX = x;
    m_oldY = y;
}

void CamtransCamera::mouseDragged(int x, int y)
{
    // @TODO: implement camera pan
    Vector4 up = m_cameraImpl.getUp();
    Vector4 sideways = m_cameraImpl.getLook().cross(up);
    m_cameraImpl.translate(sideways * ((m_oldX - x) * 0.025f) + up * ((y - m_oldY) * 0.025f));
    m_oldX = x;
    m_oldY = y;
}

void CamtransCamera::mouseUp(int x, int y) { }

void CamtransCamera::mouseScrolled(int delta)
{
    // @TODO: implement camera dolly
    m_cameraImpl.translate(m_cameraImpl.getLook() * (delta * 0.005f));
}

#if 0
// end ta code -->

CamtransCamera::CamtransCamera()
{
    // @TODO: [CAMTRANS] Fill this in...
}

void CamtransCamera::setAspectRatio(float a)
{
    // @TODO: [CAMTRANS] Fill this in...

}

Matrix4x4 CamtransCamera::getProjectionMatrix() const
{
    // @TODO: [CAMTRANS] Fill this in...
    return Matrix4x4::identity();
}

Matrix4x4 CamtransCamera::getModelviewMatrix() const
{
    // @TODO: [CAMTRANS] Fill this in...
    return Matrix4x4::identity();
}

Vector4 CamtransCamera::getPosition() const
{
    // @TODO: [CAMTRANS] Fill this in...
    return Vector4::zero();
}

Vector4 CamtransCamera::getLook() const
{
    // @TODO: [CAMTRANS] Fill this in...
    return Vector4::zero();
}

Vector4 CamtransCamera::getUp() const
{
    // @TODO: [CAMTRANS] Fill this in...
    return Vector4::zero();
}

float CamtransCamera::getAspectRatio() const
{
    // @TODO: [CAMTRANS] Fill this in...
    return 0;
}

float CamtransCamera::getHeightAngle() const
{
    // @TODO: [CAMTRANS] Fill this in...
    return 0;
}

void CamtransCamera::orientLook(const Vector4 &eye, const Vector4 &look, const Vector4 &up)
{
    // @TODO: [CAMTRANS] Fill this in...

}

void CamtransCamera::setHeightAngle(float h)
{
    // @TODO: [CAMTRANS] Fill this in...

}

void CamtransCamera::translate(const Vector4 &v)
{
    // @TODO: [CAMTRANS] Fill this in...

}

void CamtransCamera::rotateU(float degrees)
{
    // @TODO: [CAMTRANS] Fill this in...

}

void CamtransCamera::rotateV(float degrees)
{
    // @TODO: [CAMTRANS] Fill this in...

}

void CamtransCamera::rotateN(float degrees)
{
    // @TODO: [CAMTRANS] Fill this in...

}

void CamtransCamera::setClip(float nearPlane, float farPlane)
{
    // @TODO: [CAMTRANS] Fill this in...

}

void CamtransCamera::mouseDown(int x, int y)
{
    // @TODO: Save the old click coordinates. Used for camera
    //       interaction. You will fill this in during lab.

}

void CamtransCamera::mouseDragged(int x, int y)
{
    // @TODO: Implement camera pan. You will do this in lab.

}

void CamtransCamera::mouseUp(int x, int y)
{
    // @TODO: Implement camera dolly. You will do this in lab.

}

void CamtransCamera::mouseScrolled(int delta)
{
    // @TODO: Implement camera dolly. You will do this in lab.

}

// begin ta code
#endif
// end ta code
