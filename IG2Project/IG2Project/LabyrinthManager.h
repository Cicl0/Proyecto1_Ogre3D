#pragma once

#include "Labyrinth.h"
#include "Ogre.h"

#include <OgreInput.h>
#include <OgreSceneManager.h>

class Labyrinth;

class LabyrinthManager : public OgreBites::ApplicationContext, OgreBites::InputListener
{
public:
	LabyrinthManager(Ogre::SceneManager* sm, OgreBites::ApplicationContext* appCtx) :
		mSM(sm),
		mAppCtx(appCtx),
		laby(nullptr)
	{
		setup();
	}

	void setup();

	Labyrinth* getLabyrinth() const { return laby; }

	void frameRendered(const Ogre::FrameEvent& evt);

private:
	Ogre::SceneManager* mSM;
	OgreBites::ApplicationContext* mAppCtx;
	Labyrinth* laby;
};

