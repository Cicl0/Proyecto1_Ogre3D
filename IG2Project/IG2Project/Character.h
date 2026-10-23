#pragma once

#include "IG2Object.h"


class Character : public IG2Object
{
public:
	Character(Vector3 iniPos, SceneNode* CharNode, SceneManager* sceneManager, string mesh) :
		IG2Object(iniPos, CharNode, sceneManager, mesh)
	{
	}

	float getSpeed() { return speed; }

	virtual bool keyPressed(const OgreBites::KeyboardEvent& evt) override = 0;

	Vector3 getDirection() { return dir; }

	Vector3 getNexDirVector() { return nextDir; }

	bool isDirectionModified() { return nextDir == Vector3::ZERO ? false : nextDir != dir; }

	bool isStill() { return nextDir == Vector3::ZERO; }

	void rotateToNewDirection();

	bool is180Turn();

	void setNextDirection(Vector3 newDir) { nextDir = newDir; }

protected:
	Vector3 dir = Vector3::ZERO;
	Vector3 nextDir = Vector3::ZERO;

	float speed = 100;
};

