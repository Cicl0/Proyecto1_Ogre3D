#pragma once

#include "Character.h"
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <OgreFrameListener.h>
#include <OgreTimer.h>

class Villain : public Character
{
public:
    Villain(Vector3 iniPos, SceneNode* heroNode, SceneManager* sceneManager);

    void frameRendered(const Ogre::FrameEvent& evt) override;

    bool keyPressed(const OgreBites::KeyboardEvent& evt) override { return true; }

    SceneNode* crearHelice(Vector3 pos, Degree roll, Degree yaw, Degree pitch);

	int& LastRow() { return lastRow; }

	int& LastCol() { return lastCol; }

	int& RangeVision() { return rangeVision; }

	int& PatrolLeft() { return patrolLeft; }


private:
    Ogre::SceneNode* mNodoVillano;
    Ogre::SceneNode* mHeliceIzq;
    Ogre::SceneNode* mHeliceDer;

    SceneNode* mCabeza = nullptr;
    float mAnguloCabeza = -60.0f;   // ángulo actual (grados)
    float mAnguloMax = 40.0f;       // barrido: de -60º a +60º
    float mVelCabeza = 40.0f;       // grados/s (120º / 3 s)

    Ogre::Timer mTimer;          // controla el tiempo en cada sentido
    unsigned long mTiempoSentido = 3000;  // ms por sentido
    int mSentido = 1;            // +1 / -1
    float mVelHelices = 260.0f;   // grados por segundo

    int lastRow = -1;
    int lastCol = -1;

	int rangeVision = 7; // rango de visión del villano (en bloques)

    int patrolLeft = 0;   // bloques que le quedan a la patrulla actual
};

