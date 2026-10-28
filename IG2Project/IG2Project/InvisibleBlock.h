#pragma once

#include "IG2Object.h"
#include "Block.h"

class InvisibleBlock : public Block
{
public:
	InvisibleBlock(Vector3 iniPos, SceneNode* wallNode, SceneManager* sceneManager, string mesh = "cube.mesh") :
		Block(iniPos, wallNode, sceneManager, mesh)
	{
		collision = true;
		breakable = false;
		visibility = false;
	}

	void frameRendered(const Ogre::FrameEvent& evt) override;

private:
	float invisibleTime = 0.0f;
	float invisibleDuration = 5.0f; // Duracion de la invisibilidad en segundos
	float visibleDuration = 2.0f; // Duracion de la visibilidad
};

