#pragma once

#include "IG2Object.h"
class Bomb : public IG2Object
{
public:
	Bomb(Vector3 pos, Ogre::SceneNode* sn, Ogre::SceneManager* sm, float dur = 2, int ran = 3) :
		IG2Object(pos, sn, sm),
		duration(dur),
		range(ran)
	{
		createMesh();
	}

	void frameRendered(const Ogre::FrameEvent& evt);

	bool hasExploded() const { return exploded; }

	int getRange() { return range; }

private:
	float duration;
	bool exploded = false;
	int range;

	Ogre::SceneNode* fuseNode;

	void createMesh();
};

