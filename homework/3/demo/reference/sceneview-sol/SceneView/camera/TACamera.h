/*!
   @file   TACamera.h
   @author Nong Li
   @author Evan Wallace (evan.exe@gmail.com)
   @date   Fall 2010

   @brief  Contains definitions for the TA's implementation of a perspective camera.
*/

#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <CS123Algebra.h>

class CS123Camera  {
    private:
        Matrix4x4 m_modelviewMatrix;
                Matrix4x4 m_projectionMatrix;
        
                Vector4 m_eye;
        Vector4 m_u; // v cross w
        Vector4 m_v; // up
        Vector4 m_w; // -look
        
        double m_heightAngle;
        double m_aspectRatio;
        double m_near;
        double m_far;

        void rebuildModelviewMatrix();
        void rebuildProjectionMatrix();

    public:
        CS123Camera() : m_heightAngle(60), m_aspectRatio(1), m_near(1), m_far(30) { orientLook(Vector4(2, 2, 2, 0), Vector4(-2, -2, -2, 0), Vector4(0, 1, 0, 0)); }
        ~CS123Camera() {}

        Matrix4x4 getModelviewMatrix() const;
        Matrix4x4 getProjectionMatrix() const;

        Vector4 getPosition() const;
        Vector4 getLook() const;
        Vector4 getUp() const;
        float getAspectRatio() const;
        float getHeightAngle() const;

        void orientLook(const Vector4 &eye, const Vector4 &look, const Vector4 &up);

        void setHeightAngle(float h);
        void setAspectRatio(float a);
        void translate(const Vector4 &v);
        void rotateU(float degrees);
        void rotateV(float degrees);
        void rotateN(float degrees);
        void setClip(float nearPlane, float farPlane);
};

#endif // __CAMERA_H__

