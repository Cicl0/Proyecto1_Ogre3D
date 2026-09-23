#pragma once

#include "IG2Object.h"
#include "Block.h"

class Wall : public Block
{
public:
	Wall(Vector3 iniPos, SceneNode* wallNode, SceneManager* sceneManager, string mesh = "cube.mesh") :
		Block(iniPos, wallNode, sceneManager, mesh)
	{
		collision = true;
	}

	void setScale(Vector3 scale) { IG2Object::setScale(scale); }
};

