#pragma once

#include <OgreTrays.h>
#include <OgreInput.h>
#include <OgreSceneManager.h>

#include "Labyrinth.h"
#include "Ogre.h"
#include "Hero.h"
#include "Villain.h"

#include "StartAnimation.h"

class Labyrinth;
class Hero;

class LabyrinthManager : public OgreBites::ApplicationContext, OgreBites::InputListener
{
public:
	LabyrinthManager(Ogre::SceneManager* sm, OgreBites::ApplicationContext* appCtx, OgreBites::TrayManager* trayMgr) :
		mSM(sm),
		mAppCtx(appCtx),
		mTrayMgr(trayMgr),
		laby(nullptr)
	{
		mAppCtx->addInputListener(this);
	}

	void setup();                 

	void setupStartAnimation(); // crea la intro

	// Callback que se ejecuta justo después de crear el laberinto
	void setOnGameStart(std::function<void()> cb) { mOnGameStart = std::move(cb); }

	Labyrinth* getLabyrinth() const { return laby; }

	void frameRendered(const Ogre::FrameEvent& evt);

	void updateInfo();

private:
	Ogre::SceneManager* mSM;
	OgreBites::TrayManager* mTrayMgr = nullptr;
	OgreBites::ApplicationContext* mAppCtx;

	StartAnimation* startAnim = nullptr;
	std::function<void()> mOnGameStart;

	std::vector<std::string> stages;
	int currentStage;
	Labyrinth* laby;

	OgreBites::TextBox* infoText;
	OgreBites::Label* infoLabel;

	Hero* mHero;
};

