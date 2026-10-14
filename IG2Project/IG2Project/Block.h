#pragma once

#include "IG2Object.h"

class Block : public IG2Object
{
public:
	Block(Vector3 iniPos, SceneNode* blockNode, SceneManager* sceneManager, string mesh) : // Para inicializar con malla
		IG2Object(iniPos, blockNode, sceneManager, mesh), collision(false)
	{
	}

	Block(Vector3 iniPos, SceneNode* blockNode, SceneManager* sceneManager) : // Para inicializar sin malla
		IG2Object(iniPos, blockNode, sceneManager), collision(false)
	{
	}

	void frameRendered(const Ogre::FrameEvent& evt) override {}

	bool canPassThrough() const { return !collision; }

	bool isVisible() const { return visibility; }

	bool isBreakable() const { return breakable; }

	void breakBlock() { if (breakable) collision = false; visibility = false; }

protected:
	bool collision;
	bool visibility;
	bool breakable;
};

