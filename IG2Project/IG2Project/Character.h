#pragma once

#include "IG2Object.h"

enum Direction {
	UP = 0,
	DOWN = 1,
	LEFT = 2,
	RIGHT = 3,
	NONE = 4
};

class Character : public IG2Object
{
public:
	Character(Vector3 iniPos, SceneNode* CharNode, SceneManager* sceneManager, string mesh) :
		IG2Object(iniPos, CharNode, sceneManager, mesh)
	{
	}

	float getSpeed() { return speed; }

	virtual bool keyPressed(const OgreBites::KeyboardEvent& evt) override = 0;

	Vector3 getVector(Direction dir);

	Vector3 getNexDirVector() { return getVector(nextDir); }

	bool isDirectionModified() { return nextDir != NONE; }

	void rotateToNewDirection();

	bool is180Turn();

protected:
	Direction dir = NONE;
	Direction nextDir = NONE;

	float speed = 1;
};

