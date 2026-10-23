#include "LabyrinthManager.h"

void
LabyrinthManager::setup() {
	std::string file = "Labyrinths/stage3.txt"; // Asignar a posteriori el nombre del archivo del laberinto

	laby = new Labyrinth(mSM, file); // en la constructora se genera todo el laberinto a partir del "file"

	if (mAppCtx && laby && laby->getHero() && laby->getVillain()) {
		mAppCtx->addInputListener(this);
		mAppCtx->addInputListener(laby->getHero());
		mAppCtx->addInputListener(laby->getVillain());
	}
}

void 
LabyrinthManager::frameRendered(const Ogre::FrameEvent& evt) {
	if (laby->getHero()) {
		laby->moveCharacter(laby->getHero(), evt.timeSinceLastFrame);
	}
	if (laby->getVillain()) {
		laby->moveVillain(laby->getVillain(), evt.timeSinceLastFrame);
	}
}