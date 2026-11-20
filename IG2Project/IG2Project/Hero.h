#pragma once

#include "Character.h"
#include "Constant.h"

#include <OgreInput.h>

class Hero : public Character
{
public:
	Hero(Vector3 iniPos, SceneNode* heroNode, SceneManager* sceneManager) :
		Character(iniPos, heroNode, sceneManager, "Sinbad.mesh")
	{
		initAnimations();
	}

	virtual bool keyPressed(const OgreBites::KeyboardEvent& evt);

	void frameRendered(const Ogre::FrameEvent& evt);

	void initAnimations();   // llamar una vez, tras crear la entidad de Sinbad
	void updateAnimation(Ogre::Real dt, bool running);

	int& lives() {
		return liv;
	}

private:
	int liv = 3;

	Ogre::AnimationState* mRunBase = nullptr, * mRunTop = nullptr,
		* mIdleBase = nullptr, * mIdleTop = nullptr;
	bool mRunning = false;
};

