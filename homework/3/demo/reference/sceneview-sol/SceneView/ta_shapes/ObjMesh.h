#ifndef __OBJ_MESH_H__
#define __OBJ_MESH_H__

#include "Mesh.h"
#include "glm.h"

class ObjMesh : public Mesh
{
public:
        ObjMesh(const char *file);
        virtual ~ObjMesh();

        virtual void transformByMatrix(const Matrix4x4 &matrix);
        virtual void draw();

private:
        Matrix4x4 m_matrix;
        GLMmodel *m_model;
};

#endif
