#include "LabyrinthManager.h"

void
LabyrinthManager::setup() {
	std::string file = "Labyrinths/stage3.txt"; // Asignar a posteriori el nombre del archivo del laberinto

	laby = new Labyrinth(mSM, file); // en la constructora se genera todo el laberinto a partir del "file"

	if (mAppCtx && laby && laby->getHero()) {
		mAppCtx->addInputListener(this);
		mAppCtx->addInputListener(laby->getHero());
	}
}

void 
LabyrinthManager::frameRendered(const Ogre::FrameEvent& evt) {
	if (laby->getHero()) {
		laby->moveCharacter(laby->getHero(), evt.timeSinceLastFrame);
	}
}