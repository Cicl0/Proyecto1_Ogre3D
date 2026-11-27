#include "Bomb.h"

#include <OgreMaterialManager.h>
#include <OgreLogManager.h>

void
Bomb::frameRendered(const Ogre::FrameEvent& evt) {
	if (duration <= 0) {
		exploded = true;
		return;
	}

	duration -= evt.timeSinceLastFrame;
}

void
Bomb::createMesh() {

	Entity* sphere = mSM->createEntity("sphere.mesh");
	Entity* fuse = mSM->createEntity("column.mesh");

	const Real radius = 1.0;
	const Real sphereScale = radius / 100.0;

	SceneNode* sphereNode = mNode->createChildSceneNode();
	sphereNode->attachObject(sphere);
	sphereNode->setScale(sphereScale, sphereScale, sphereScale);

	const Real fuseHeight = radius * 0.6;   // altura deseada de la mecha

	// Medimos la columna para escalarla a la altura deseada
	const AxisAlignedBox& box = fuse->getBoundingBox();
	const Real fuseScale = fuseHeight / box.getSize().y;

	SceneNode* fuseNode = mNode->createChildSceneNode();
	fuseNode->attachObject(fuse);
	fuseNode->setScale(fuseScale, fuseScale, fuseScale);

	// Colocamos la base de la columna justo en la cima de la esfera.
	const Vector3 center = box.getCenter();
	fuseNode->setPosition(-center.x * fuseScale,
		radius - box.getMinimum().y * fuseScale - 0.05,
		-center.z * fuseScale);

	sphere->setMaterialName("examples/BombBlack");
	fuse->setMaterialName("examples/EnvMappedRustySteel");
}