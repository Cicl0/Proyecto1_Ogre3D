#pragma once

#include "IG2Object.h"
#include "Block.h"

class BreakableBlock : public Block
{
public:
	BreakableBlock(Vector3 iniPos, SceneNode* wallNode, SceneManager* sceneManager, string mesh = "cube.mesh") :
		Block(iniPos, wallNode, sceneManager, mesh)
	{
		collision = true;
		visibility = true;
		breakable = true;
	}

	void frameRendered(const Ogre::FrameEvent& evt) override {}
};

