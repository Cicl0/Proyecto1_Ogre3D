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
		
	}

	virtual bool keyPressed(const OgreBites::KeyboardEvent& evt);

	int& lives() {
		return liv;
	}

private:
	int liv = 3;
};

