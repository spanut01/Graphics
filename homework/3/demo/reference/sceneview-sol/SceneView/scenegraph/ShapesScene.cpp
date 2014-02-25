#include "ShapesScene.h"
#include "Settings.h"
#include "Camera.h"
#include <qgl.h>
#include <SupportCanvas3D.h>

// begin ta code
#include "ta_shapes/CubeMesh.h"
#include "ta_shapes/ConeMesh.h"
#include "ta_shapes/SphereMesh.h"
#include "ta_shapes/CylinderMesh.h"
#include "ta_shapes/SierpinskiMesh.h"
#include "ta_shapes/MobiusStripMesh.h"
#include "ta_shapes/ObjMesh.h"
#include "ta_shapes/teapot.cpp"
// end ta code

Vector4 lightDirection = Vector4(1, -1, -1, 0).getNormalized();

ShapesScene::ShapesScene()
{
    // Use a shiny orange material
    memset(&m_material, 0, sizeof(m_material));
    m_material.cAmbient.r = 0.2f;
    m_material.cAmbient.g = 0.1f;
    m_material.cDiffuse.r = 1;
    m_material.cDiffuse.g = 0.5f;
    m_material.cSpecular.r = m_material.cSpecular.g = m_material.cSpecular.b = 1;
    m_material.shininess = 64;

    // Use a white directional light from the upper left corner
    memset(&m_light, 0, sizeof(m_light));
    m_light.type = LIGHT_DIRECTIONAL;
    m_light.dir = lightDirection;
    m_light.color.r = m_light.color.g = m_light.color.b = 1;
    m_light.id = 0;

    //TODO: [SHAPES] Allocate any additional memory you need...

    // begin ta code
    m_mesh = NULL;
    // end ta code

}

ShapesScene::~ShapesScene()
{
    //TODO: [SHAPES] Don't leak memory!

    // begin ta code
    delete m_mesh;
    // end ta code
}

void ShapesScene::setLights(const Camera *follow)
{
    // YOU DON'T NEED TO TOUCH THIS METHOD,
    // unless you want to do fancy lighting...

    // Disable all OpenGL lights so the lights from the previous scene
    // don't appear in the next one.  There are 8 lights in OpenGL,
    // GL_LIGHT0 to GL_LIGHT7, and each GL_LIGHT* is one greater than
    // the previous one.  This means the nth light is GL_LIGHT0 + n.
    for (int id = 0; id < NUM_OPENGL_LIGHTS; ++id)
        glDisable(GL_LIGHT0 + id);

    m_light.dir = lightDirection * follow->getModelviewMatrix().getInverse().getTranspose();

    // Load one light in the upper left (subclasses will have to load lights
    // for sceneview anyway, so they will overwrite this light).
    setLight(m_light);
}

void ShapesScene::renderGeometry(bool useMaterials)
{
    // TODO: [SHAPES] Render a shape based on settings.shapeType.
    //       The sample code provided draws a single triangle to
    //       the screen using OpenGL. If you've chosen to implement
    //       subclasses for your shapes, this would be a good time
    //       to call upon the subclass to draw itself as well.
    //

    // begin ta code
#if 0
    // SAMPLE CODE FOR STUDENTS ============>>
    // end ta code

    // Example shape:
    if (useMaterials)
        applyMaterial(m_material);

    // Draw the polygon
    glBegin(GL_TRIANGLES);
    glNormal3f(0, 0, 1); // glNormal*() applies to all glVertex*() below it
    glVertex3f(-1, -1, 0);
    glVertex3f(+1, -1, 0);
    glVertex3f(0, +1, 0);
    glEnd();

    // begin ta code
    // <<===================== END SAMPLE CODE
#endif
    if (useMaterials)
        applyMaterial(m_material);

    updateMeshIfChanged();
    if (m_mesh)
    {
        m_mesh->draw();
    }
    else if (settings.shapeType == SHAPE_SPECIAL_3)
    {
        glCullFace(GL_FRONT);
        teapot(settings.shapeParameter1, 0.5f, GL_FILL);
        glCullFace(GL_BACK);
    }
    // end ta code

}

void ShapesScene::renderNormals()
{
    //TODO: [SHAPES] Rendering normals is OPTIONAL in shapes, and it's not
    //      extra credit. However, we believe that rendering normals will
    //      greatly assist you with visual debugging - both in this and
    //      future assignments. Therefore, we highly recommend you fill
    //      this method in.
    //

    // begin ta code
#if 0
    // end ta code
    Vector4 normal = Vector4(0, 0, 1, 0);
    renderNormal(Vector4(-1, -1, 0, 1), normal);
    renderNormal(Vector4(+1, -1, 0, 1), normal);
    renderNormal(Vector4(0, +1, 0, 1), normal);
    // begin ta code
#endif
    updateMeshIfChanged();
    if (m_mesh)
        renderNormalsForMesh(m_mesh);
    // end ta code
}

// begin ta code
void ShapesScene::updateMeshIfChanged()
{
    CS123ScenePrimitive primitive;
    primitive.material = m_material;

    delete m_mesh;
    m_mesh = NULL;

    // Reset the scene contents (we'll add objects to m_primitivesAndMatrices below so they can be raytraced)
    m_primitivesAndMatrices.clear();

    switch (settings.shapeType)
    {
    case SHAPE_CUBE:
        m_mesh = new CubeMesh(settings.shapeParameter1);
        primitive.type = PRIMITIVE_CUBE;
        m_primitivesAndMatrices += PrimitiveAndMatrix(primitive, Matrix4x4::identity());
        break;

    case SHAPE_CONE:
        m_mesh = new ConeMesh(settings.shapeParameter1, settings.shapeParameter2);
        primitive.type = PRIMITIVE_CONE;
        m_primitivesAndMatrices += PrimitiveAndMatrix(primitive, Matrix4x4::identity());
        break;

    case SHAPE_SPHERE:
        m_mesh = new SphereMesh(settings.shapeParameter1, settings.shapeParameter2);
        primitive.type = PRIMITIVE_SPHERE;
        m_primitivesAndMatrices += PrimitiveAndMatrix(primitive, Matrix4x4::identity());
        break;

    case SHAPE_CYLINDER:
        m_mesh = new CylinderMesh(settings.shapeParameter1, settings.shapeParameter2);
        primitive.type = PRIMITIVE_CYLINDER;
        m_primitivesAndMatrices += PrimitiveAndMatrix(primitive, Matrix4x4::identity());
        break;

    case SHAPE_SPECIAL_1:
        m_mesh = new MobiusStripMesh(settings.shapeParameter1, settings.shapeParameter2);
        break;

    case SHAPE_SPECIAL_2:
        m_mesh = new SierpinskiMesh(1 + settings.shapeParameter1 / 10);
        break;
    }
}
// end ta code

