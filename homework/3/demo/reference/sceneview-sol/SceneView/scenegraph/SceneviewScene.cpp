#include <qgl.h>

#include "SceneviewScene.h"
#include "CS123SceneData.h"
#include "Camera.h"

// begin ta code
#include "ta_shapes/Mesh.cpp"
#include "ta_shapes/ParameterizedMesh.cpp"
#include "ta_shapes/CubeMesh.cpp"
#include "ta_shapes/ConeMesh.cpp"
#include "ta_shapes/SphereMesh.cpp"
#include "ta_shapes/CylinderMesh.cpp"
#include "ta_shapes/SierpinskiMesh.cpp"
#include "ta_shapes/MobiusStripMesh.cpp"
#include "ta_shapes/ObjMesh.cpp"
// end ta code

SceneviewScene::SceneviewScene()
{
    // TODO: [SCENEVIEW] Set up anything you need for your Sceneview scene here...

}

SceneviewScene::~SceneviewScene()
{
    // TODO: [SCENEVIEW] Don't leak memory!

    // begin ta code
    foreach (Mesh *mesh, m_meshes)
        delete mesh;
    // end ta code
}

void SceneviewScene::setLights(const Camera *follow)
{
    // TODO: [SCENEVIEW] Fill this in...
    //
    // Use OpenGL to set up the lighting for your scene. The lighting information
    // will most likely be stored in CS123SceneLightData structures.
    //
    // begin ta code
    {

        for(int id = 0; id < NUM_OPENGL_LIGHTS; id++)
            glDisable(GL_LIGHT0 + id);

        foreach (CS123SceneLightData light, m_lights)
            setLight(light);

    }
    // end ta code
}

void SceneviewScene::renderGeometry(bool useMaterials)
{
    // TODO: [SCENEVIEW] Fill this in...
    //
    // This is where you should render the geometry of the scene. Use what you
    // know about OpenGL and leverage your Shapes classes to get the job done.
    //
    // <!-- begin ta code
    {
        for (int i = 0; i < m_meshes.count(); i++)
        {
            // NULL meshes indicate meshes that couldn't be loaded
            if (m_meshes[i] != NULL)
            {
                if (useMaterials)
                    applyMaterial(m_primitivesAndMatrices.at(i).primitive.material);
                m_meshes[i]->draw();
            }
        }
    }
    // end ta code -->
}

void SceneviewScene::renderNormals()
{
    // TODO: [SCENEVIEW] Fill this in...
    //
    // Rendering normals is an **optional** part of Sceneview (sorry, not extra credit)
    // You can put all of your renderNormal() calls here if you want to render normals.
    //
    // HINT: Rendering normals can be a helpful debugging tool if things don't look
    // quite right. Your Shapes might be incorrect, and if that's the case, you'll
    // need to go back and fix your buggy shapes code before turning in Sceneview.
    //
    // begin ta code
    foreach (Mesh *mesh, m_meshes)
        if (mesh != NULL)
            renderNormalsForMesh(mesh);
    // end ta code
}

// begin ta code
void SceneviewScene::finishParsing()
{
    OpenGLScene::finishParsing();

    // Start detail level off at 8 for 1 primitive and get down to 1 at 500 primitives
    float detailLevel = 8.0f / (1.0f + (m_primitivesAndMatrices.count() - 1) * 7.0f / 499.0f);

    foreach (const PrimitiveAndMatrix &p, m_primitivesAndMatrices)
    {
        Mesh *mesh = NULL;

        switch (p.primitive.type)
        {
        case PRIMITIVE_CUBE:
            mesh = new CubeMesh(1 + 4 * detailLevel);
            break;

        case PRIMITIVE_CONE:
            mesh = new ConeMesh(1 + 4 * detailLevel, 8 + 4 * detailLevel);
            break;

        case PRIMITIVE_SPHERE:
            mesh = new SphereMesh(4 + 2 * detailLevel, 8 + 4 * detailLevel);
            break;

        case PRIMITIVE_CYLINDER:
            mesh = new CylinderMesh(1 + 4 * detailLevel, 8 + 4 * detailLevel);
            break;

        case PRIMITIVE_MESH:
            mesh = new ObjMesh(p.primitive.meshfile.c_str());
            break;

        default:
            cout << "warning: unknown primitive type " << p.primitive.type << endl;
        }

        if (mesh)
            mesh->transformByMatrix(p.matrix);

        // Always push mesh even when NULL to keep the indices into m_meshes and m_primitivesAndMatrices the same
        m_meshes += mesh;
    }
}
// end ta code
