#include "TestPrologue.h"
#include <cal3d/renderer.h>


TEST(CalSubmesh_getFaces_succeeds_if_face_list_is_empty) {
    CalCoreSubmesh csm;

    CalSubmesh sm;
    sm.create(&csm);

    sm.getVectorFace();
}

TEST(CalRenderer_getTextureCoordinates_when_there_are_no_texture_coordinates) {
    CalCoreSubmesh* coreSubmesh = new CalCoreSubmesh;
    coreSubmesh->reserve(0, 1, 0, 0);

    CalCoreMesh* coreMesh = new CalCoreMesh;
    coreMesh->addCoreSubmesh(coreSubmesh);

    CalCoreModel coreModel;
    CHECK(coreModel.createInternal("model"));
    coreModel.addCoreMesh(coreMesh);

    CalCoreSkeleton* coreSkeleton = new CalCoreSkeleton;
    coreModel.setCoreSkeleton(coreSkeleton);

    CalModel model;
    model.create(&coreModel);
    CHECK(model.attachMesh(0));

    CalRenderer renderer;
    renderer.create(&model);

    CalSubmesh* submesh = renderer.selectMeshSubmesh(0, 0);
    renderer.getTextureCoordinates(submesh, 0, 0);

    renderer.destroy();
    model.destroy();
    coreModel.destroy();
}

TEST(CalRenderer_getNormals_when_there_are_no_normals) {
    CalCoreSubmesh* coreSubmesh = new CalCoreSubmesh;
    coreSubmesh->reserve(0, 0, 0, 0);

    CalCoreMesh* coreMesh = new CalCoreMesh;
    coreMesh->addCoreSubmesh(coreSubmesh);

    CalCoreModel coreModel;
    CHECK(coreModel.createInternal("model"));
    coreModel.addCoreMesh(coreMesh);

    CalCoreSkeleton* coreSkeleton = new CalCoreSkeleton;
    coreModel.setCoreSkeleton(coreSkeleton);

    CalModel model;
    model.create(&coreModel);
    CHECK(model.attachMesh(0));

    CalRenderer renderer;
    renderer.create(&model);

    CalSubmesh* submesh = renderer.selectMeshSubmesh(0, 0);
    renderer.getVerticesAndNormals(submesh, 0);

    renderer.destroy();
    model.destroy();
    coreModel.destroy();
}
