#pragma once

#include "IG2Object.h"
#include "Block.h"

#include <OgreManualObject.h>
#include <OgreMeshManager.h>

class Wall : public Block
{
public:

	Wall(Vector3 iniPos, SceneNode* wallNode, SceneManager* sceneManager, string mesh = "cube.mesh") :
		Block(iniPos, wallNode, sceneManager,
			mesh.empty() ? mesh : mesh)
	{
		collision = true;
		breakable = false;
		visibility = true;
	}

	void frameRendered(const Ogre::FrameEvent& evt) override {}

	void setScale(Vector3 scale) { IG2Object::setScale(scale); }
};

