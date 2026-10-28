#include "Villain.h"

Villain::Villain(Vector3 iniPos, SceneNode* heroNode, SceneManager* sceneManager) :
	Character(iniPos, heroNode, sceneManager, "sphere.mesh")
{
    entity->setMaterialName("examples/Rockwall"); // Aplica un material al cuerpo

    // Cabeza (malla 2: ogrehead)
    mCabeza = mNode->createChildSceneNode();
    mCabeza->setPosition(0, 150, 0);
    mCabeza->setScale(2.5f, 2.5f, 2.5f);
    mCabeza->attachObject(mSM->createEntity("ogrehead.mesh"));

    // Helices (moviles)
    mHeliceIzq = crearHelice(Vector3(-100.0f, 0.0f, 0.0f), Degree(0), Degree(90), Degree(0));
    mHeliceDer = crearHelice(Vector3(100.0f, 0.0f, 0.0f), Degree(0), Degree(-90), Degree(0));
    mTimer.reset();
}

SceneNode* Villain::crearHelice(Vector3 pos, Degree roll, Degree yaw, Degree pitch)
{

    SceneNode* helice = mNode->createChildSceneNode();
    helice->setPosition(pos);
    helice->yaw(yaw);
    helice->roll(roll);
    helice->pitch(pitch);

    // Eje (malla 3: knot)
    Entity* eje = mSM->createEntity("knot.mesh");
    eje->setMaterialName("examples/EnvMappedRustySteel");
    SceneNode* ejeNode = helice->createChildSceneNode();
    ejeNode->setScale(0.15f, 0.15f, 0.15f);
    ejeNode->attachObject(eje);

    // 3 aspas (cube)
    for (int i = 0; i < 3; ++i) {
        SceneNode* orient = helice->createChildSceneNode();
        orient->roll(Degree(i * 120));

        Entity* aspa = mSM->createEntity("cube.mesh");
        aspa->setMaterialName("examples/BumpyMetal");
        SceneNode* aspaNode = orient->createChildSceneNode();
        aspaNode->setPosition(0, 55, 0);
        aspaNode->setScale(0.3f, 1.2f, 0.1f);
        aspaNode->attachObject(aspa);
    }
    return helice;
}

void 
Villain::frameRendered(const Ogre::FrameEvent& evt) {

    // --- Animacion ---
    if (mTimer.getMilliseconds() >= mTiempoSentido) {
        mSentido = -mSentido;
        mTimer.reset();
    }

    // Hélices
    float giro = mVelHelices * evt.timeSinceLastFrame;
    mHeliceIzq->roll(Degree(giro));
    mHeliceDer->roll(Degree(-giro));

    // Cabeza: barrido izquierda-derecha limitado
    if (!chasing) {
        mAnguloCabeza += mSentido * mVelCabeza * evt.timeSinceLastFrame;
        if (mAnguloCabeza > mAnguloMax)  mAnguloCabeza = mAnguloMax;
        if (mAnguloCabeza < -mAnguloMax) mAnguloCabeza = -mAnguloMax;
    }
    else {
        mAnguloCabeza = 0.0f;
    }

    mCabeza->setOrientation(Quaternion(Degree(mAnguloCabeza), Vector3::UNIT_Y));

    // --- Movimiento ---
}