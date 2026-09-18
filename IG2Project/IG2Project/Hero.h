#pragma once

#include "IG2Object.h"

class Hero : public IG2Object
{
public:
	Hero(Vector3 iniPos, SceneNode* wallNode, SceneManager* sceneManager) :
		IG2Object(iniPos, wallNode, sceneManager, "Sinbad.mesh")
	{ }
};

