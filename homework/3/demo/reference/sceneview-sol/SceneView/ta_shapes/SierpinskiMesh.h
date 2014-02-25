#ifndef __SIERPINSKI_MESH_H__
#define __SIERPINSKI_MESH_H__

#include "Mesh.h"

class SierpinskiMesh : public Mesh
{
public:
    SierpinskiMesh(unsigned int depth);
    virtual ~SierpinskiMesh();

private:
    void construct(const Vector &a, const Vector &b, const Vector &c, const Vector &d, unsigned int depth);
};

#endif
