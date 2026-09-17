#pragma once
#include "IG2Object.h"
class Wall : public IG2Object
{
public:
	Wall(Vector3 iniPos, SceneNode* wallNode, SceneManager* sceneManager) :
		IG2Object(iniPos, wallNode, sceneManager, "cube.mesh")
	{ }

	void setScale(Vector3 scale) { IG2Object::setScale(scale); }
};

