#pragma once

#include "IG2Object.h"
#include "Block.h"

class FakeBlock : public Block
{
public:
	FakeBlock(Vector3 iniPos, SceneNode* wallNode, SceneManager* sceneManager, string mesh = "cube.mesh") :
		Block(iniPos, wallNode, sceneManager, mesh)
	{
		collision = false;
		visibility = true;
		breakable = false;
	}

	void frameRendered(const Ogre::FrameEvent& evt) override {}
};

