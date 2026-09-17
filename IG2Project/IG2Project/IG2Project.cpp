#include "IG2Project.h"
#include "Constant.h"

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

    // Adds the listener for this object
    addInputListener(this);
    setupScene();
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

    mCamNode->setPosition(0, 0, 1000);
    mCamNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_WORLD);

    // and tell it to render into the main window
    Viewport* vp = getRenderWindow()->addViewport(cam);

    mCamMgr = new OgreBites::CameraMan(mCamNode);
    addInputListener(mCamMgr);
    mCamMgr->setStyle(OgreBites::CS_ORBIT);


    //------------------------------------------------------------------------
    // Creating the light

    mSM->setAmbientLight(ColourValue(0.5, 0.5, 0.5));
    
    Light* luz = mSM->createLight("Luz");
    luz->setType(Ogre::Light::LT_DIRECTIONAL);
    luz->setDiffuseColour(0.75, 0.75, 0.75);

    mLightNode = mSM->getRootSceneNode()->createChildSceneNode("nLuz");
    mLightNode->attachObject(luz);
    mLightNode->setDirection(Ogre::Vector3(-1, -1, -1));
 

    //------------------------------------------------------------------------
    // Creating Sinbad

    //Ogre::Entity* ent = mSM->createEntity("Sinbad.mesh");
    //mSinbadNode = mSM->getRootSceneNode()->createChildSceneNode("nSinbad");
    //mSinbadNode->attachObject(ent);

    // Show bounding box
    //mSinbadNode->showBoundingBox(true);

    // Set position of Sinbad
    //mSinbadNode->setPosition(x, y, z);

    // Set scale of Sinbad
    //mSinbadNode->setScale(20, 20, 20);

    //mSinbadNode->yaw(Ogre::Degree(-45));
    //mSinbadNode->setVisible(false);    


    //------------------------------------------------------------------------
    // Creating the floor

    //MeshManager::getSingleton().createPlane("floor", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
    //    Plane(Vector3::UNIT_Y, 0),
    //    1500, 1500, 50, 50, true, 1, 5, 5,
    //    Vector3::UNIT_Z);

    //Entity* entFloor = mSM->createEntity("exampleFloor", "floor");
    //entFloor->setMaterialName("example/stonesFloor");
    //SceneNode* floorNode = mSM->getRootSceneNode()->createChildSceneNode();
    //floorNode->attachObject(entFloor);


    //------------------------------------------------------------------------
   // Creating the dragon

    //Ogre::Entity* entDragon = mSM->createEntity("dragon.mesh");
    //Ogre::SceneNode* mDragonNode = mSM->getRootSceneNode()->createChildSceneNode("nDragon");
    //mDragonNode->attachObject(entDragon);

    // Show bounding box
    //mDragonNode->showBoundingBox(true);
    //mDragonNode->yaw(Ogre::Degree(-45));
    //mDragonNode->setVisible(false);

    //Ogre::Entity* entCube = mSM->createEntity("cube.mesh");
    //Ogre::SceneNode* mCubeNode = mSM->getRootSceneNode()->createChildSceneNode("nCube");
    //mCubeNode->attachObject(entCube);


    // Set position of the dragon
    //mDragonNode->setPosition(x, y, z);

    // Set scale of the dragon
    //mCubeNode->setScale(Constant::BLOCK_SIZE, Constant::BLOCK_SIZE, Constant::BLOCK_SIZE);
    //mCubeNode->showBoundingBox(true);

     //-----------------------------------------------------------------------------
    // Create Labyrinth

    std::string file = "Labyrinths/stage1.txt"; // Asignar a posteriori el nombre del archivo del laberinto

    Labyrinth* labyrinth = new Labyrinth(mSM, file); // en la constructora se genera todo el laberinto a partir del "file"

    labyrinth->showBoundingBoxWalls();
}

