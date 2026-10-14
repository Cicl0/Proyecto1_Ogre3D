#pragma once

#include "Block.h"

class Empty : public Block
{
public:
	Empty(Vector3 iniPos, SceneNode* floorNode, SceneManager* sceneManager) :
		Block(iniPos, floorNode, sceneManager)
	{
		collision = false;
		breakable = false;
		visibility = true;
	}

private:
};