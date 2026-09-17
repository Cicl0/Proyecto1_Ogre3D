#pragma once
#include "IG2Object.h"
class Empty : public IG2Object
{
public:
	Empty(Vector3 iniPos, SceneNode* floorNode, SceneManager* sceneManager) :
		IG2Object(iniPos, floorNode, sceneManager)
	{ }
};