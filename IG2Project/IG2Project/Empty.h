#pragma once

#include "Block.h"

class Empty : public Block
{
public:
	Empty(Vector3 iniPos, SceneNode* floorNode, SceneManager* sceneManager) :
		Block(iniPos, floorNode, sceneManager)
	{
		collision = false;
		for (auto& path : paths) {
			path = false;
		}
	}

private:
	bool paths[4]; // 0 = UP, 1 = RIGHT, 2 = DOWN, 3 = LEFT    
};