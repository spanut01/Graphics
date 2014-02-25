#ifndef __MESH_H__
#define __MESH_H__

#include "Vector.h"
#include <CS123Algebra.h>

struct Triangle
{
    unsigned int a_vertex, b_vertex, c_vertex;
    unsigned int a_normal, b_normal, c_normal;

    Triangle(unsigned int _a = 0, unsigned int _b = 0, unsigned int _c = 0) :
        a_vertex(_a), b_vertex(_b), c_vertex(_c),
        a_normal(_a), b_normal(_b), c_normal(_c) {}
    Triangle(unsigned int _a_vertex, unsigned int _a_normal, unsigned int _b_vertex,
        unsigned int _b_normal, unsigned int _c_vertex, unsigned int _c_normal) :
        a_vertex(_a_vertex), b_vertex(_b_vertex), c_vertex(_c_vertex),
        a_normal(_a_normal), b_normal(_b_normal), c_normal(_c_normal) {}
};

class Mesh
{
public:
    Mesh();
    virtual ~Mesh();

        virtual void transformByMatrix(const Matrix4x4 &matrix);
        void drawCached();
    virtual void draw();
        virtual void drawNormals(void (*drawNormal)(const Vector4 &, const Vector4 &));

protected:
    std::vector<Vector> vertices;
    std::vector<Vector> normals;
        std::vector<Triangle> triangles;

private:
    unsigned int cachedDisplayList;
};

#endif
