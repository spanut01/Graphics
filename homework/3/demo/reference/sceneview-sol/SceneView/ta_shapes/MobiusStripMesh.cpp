#include "MobiusStripMesh.h"
#include <qgl.h>

void mobiusStripMeshCallback(float s, float t, Vector *position, Vector *normal)
{
    s = s * 2.0f - 1.0f;
    t *= 4.0f * M_PI;

    // temporary variables to not repeat computations
    float temp1 = 0.5f * cosf(0.5f * t);
    float temp2 = 0.5f * s * -sinf(0.5f * t) * 0.5f;
    float temp3 = 0.5f + 0.25f * s * cosf(0.5f * t);

    Vector position_ds = Vector
    (
        temp1 * cosf(t),
        temp1 * sinf(t),
        0.5f * sinf(0.5f * t)
    );
    Vector position_dt = Vector
    (
        temp2 * cosf(t) - temp3 * sinf(t),
        temp2 * sinf(t) + temp3 * cosf(t),
        0.5f * s * cosf(0.5f * t) * 0.5f
    );

    *position = Vector
    (
        temp3 * cosf(t),
        temp3 * sinf(t),
        0.25f * s * sinf(0.5f * t)
    );
    *normal = position_dt.Cross(position_ds).Unit();
}

MobiusStripMesh::MobiusStripMesh(unsigned int stacks, unsigned int slices)
{
    if(stacks < 1) stacks = 1;
    if(slices < 3) slices = 3;

    slices *= 2;
    generateParameterizedSurface(mobiusStripMeshCallback, stacks, slices, 0, 0, stacks, slices);
}

MobiusStripMesh::~MobiusStripMesh() {}
