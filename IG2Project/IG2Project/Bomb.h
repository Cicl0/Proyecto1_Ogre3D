#pragma once

#include "IG2Object.h"

class Bomb : public IG2Object
{
public:
	Bomb(Vector3 pos, SceneNode* node, SceneManager* sceneMng, float dur) :
		IG2Object(pos, node, sceneMng),
		duration(dur)
	{ }

	void frameRendered(const Ogre::FrameEvent& evt);

private:
	float duration; // duracion de la mecha de la bomba antes de explotar

	void createMesh();
};

