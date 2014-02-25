#ifndef __MOBIUS_STRIP_MESH_H__
#define __MOBIUS_STRIP_MESH_H__

#include "ParameterizedMesh.h"

class MobiusStripMesh : public ParameterizedMesh
{
public:
    MobiusStripMesh(unsigned int stacks, unsigned int slices);
    virtual ~MobiusStripMesh();
};

#endif
