#pragma once

#include "IG2Object.h"
#include "Constant.h"

#include <OgreInput.h>

class Hero : public IG2Object
{
public:
	Hero(Vector3 iniPos, SceneNode* wallNode, SceneManager* sceneManager) :
		IG2Object(iniPos, wallNode, sceneManager, "Sinbad.mesh")
	{ }

	virtual bool keyPressed(const OgreBites::KeyboardEvent& evt) override;
};

