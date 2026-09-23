#pragma once

#include "IG2Object.h"

class Block : public IG2Object
{
public:
	Block(Vector3 iniPos, SceneNode* blockNode, SceneManager* sceneManager, string mesh) : // Para inicializar con malla
		IG2Object(iniPos, blockNode, sceneManager, mesh)
	{
	}

	Block(Vector3 iniPos, SceneNode* blockNode, SceneManager* sceneManager) : // Para inicializar sin malla
		IG2Object(iniPos, blockNode, sceneManager)
	{
	}

	bool canPassThrough() { return collision; }

protected:
	bool collision;
};

