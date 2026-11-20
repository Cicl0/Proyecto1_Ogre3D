#include "IG2Project.h"
#include "Constant.h"

#include "Labyrinth.h"

using namespace std;
using namespace Ogre;


bool IG2Project::keyPressed(const OgreBites::KeyboardEvent& evt) {

    // ESC key finished the rendering...
    if (evt.keysym.sym == SDLK_ESCAPE) {
        getRoot()->queueEndRendering();
    }

    else if (evt.keysym.sym == SDLK_k) {
        cout << "Position of Sinbad: " << mSinbadNode->getPosition() << endl;
        cout << "Position of the camera: " << mCamNode->getPosition() << endl;
    }

    return true;
}


void IG2Project::shutdown() {

    mShaderGenerator->removeSceneManager(mSM);
    mSM->removeRenderQueueListener(mOverlaySystem);

    mRoot->destroySceneManager(mSM);

    delete mTrayMgr;  mTrayMgr = nullptr;
    delete mCamMgr; mCamMgr = nullptr;

    // do not forget to call the base 
    OgreBites::ApplicationContext::shutdown();
}

void IG2Project::setup(void) {

    // do not forget to call the base first
    OgreBites::ApplicationContext::setup();

    // Create the scene manager
    mSM = mRoot->createSceneManager();

    // Register our scene with the RTSS
    mShaderGenerator->addSceneManager(mSM);
        
    mSM->addRenderQueueListener(mOverlaySystem);
    //mTrayMgr = new OgreBites::TrayManager("TrayGUISystem", mWindow.render);
    mTrayMgr = new OgreBites::TrayManager("TrayGUISystem", getRenderWindow());
    mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
    addInputListener(mTrayMgr);

    // Laberinto
    mLM = new LabyrinthManager(mSM, this, mTrayMgr);

    mLM->setOnGameStart([this]() { setupGameCamera(); });

    // Adds the listener for this object
    addInputListener(this);
    setupScene();
	mLM->setupStartAnimation();
}

void IG2Project::setupScene(void) {

    //------------------------------------------------------------------------
    // Creating the camera

    Camera* cam = mSM->createCamera("Cam");
    cam->setNearClipDistance(1);
    cam->setFarClipDistance(10000);
    cam->setAutoAspectRatio(true);
    //cam->setPolygonMode(Ogre::PM_WIREFRAME);

    mCamNode = mSM->getRootSceneNode()->createChildSceneNode("nCam");
    mCamNode->attachObject(cam);

    // and tell it to render into the main window
    Viewport* vp = getRenderWindow()->addViewport(cam);

    // Perspectiva de la INTRO: cámara fija, de frente al suelo de 150x300.
    // Sinbad se mueve en X entre -50 y 50, y mira a +Z cuando baila (hacia la cámara).
    // Ajusta estos valores hasta que se parezca al vídeo.
    mCamNode->setPosition(0, 5, 30);
    mCamNode->lookAt(Ogre::Vector3(0, 5, 0), Ogre::Node::TS_WORLD);

    // Sombras: se configuran una vez, valen para la intro y para el juego
    mSM->setShadowTechnique(Ogre::ShadowTechnique::SHADOWDETAILTYPE_STENCIL);
    mSM->setShadowTextureSettings(1024, 2);

}

void 
IG2Project::setupGameCamera() {

    Labyrinth* laby = mLM->getLabyrinth();
    const float B = Constant::BLOCK_SIZE;

    // Los bloques están en (B*col, 0, B*row), así que el centro es la mitad de (n-1)
    float cx = (laby->getNumCols() - 1) * B / 2.0f;
    float cz = (laby->getNumRows() - 1) * B / 2.0f;

    mCamNode->setPosition(cx, -B * 40, cz);
    mCamNode->lookAt(Ogre::Vector3(cx, 0, cz), Ogre::Node::TS_WORLD);

    // Ahora sí: control de cámara orbital (en la intro no queremos que se mueva)
    mCamMgr = new OgreBites::CameraMan(mCamNode);
    addInputListener(mCamMgr);
    mCamMgr->setStyle(OgreBites::CS_ORBIT);

    mCamMgr->setPivotOffset(Vector3(cx, -B * 10, cz));
    mCamNode->setOrientation(Ogre::Quaternion(Ogre::Degree(-90), Ogre::Vector3::UNIT_X));

    // La intro dejó luz ambiente 0.4: el laberinto va a oscuras (solo sus luces)
    mSM->setAmbientLight(ColourValue(0.0f, 0.0f, 0.0f));
}