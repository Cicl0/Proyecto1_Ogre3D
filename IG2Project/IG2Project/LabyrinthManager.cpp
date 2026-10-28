#include "LabyrinthManager.h"

void
LabyrinthManager::setup() {

	stages = {
		"Labyrinths/stage1.txt",
		"Labyrinths/stage2.txt",
		"Labyrinths/stage3.txt"
	};

	currentStage = 2;

	// Información del juego (primero la etiqueta, luego el cuadro de texto)
	infoLabel = mTrayMgr->createLabel(
		OgreBites::TL_BOTTOMRIGHT, "infoLabel", "Stage: ", 200);
	infoText = mTrayMgr->createTextBox(
		OgreBites::TL_BOTTOMRIGHT, "infoText", "Game Info:", 200, 100);


	laby = new Labyrinth(mSM, stages[currentStage]); // en la constructora se genera todo el laberinto a partir del "file"

	mHero = laby->getHero();

	if (mAppCtx && laby && laby->getHero() && laby->getVillain()) {
		mAppCtx->addInputListener(this);
		mAppCtx->addInputListener(laby->getHero());
		mAppCtx->addInputListener(laby->getVillain());
	}
}

void 
LabyrinthManager::frameRendered(const Ogre::FrameEvent& evt) {
	if (!laby) return;

	if (laby->getHero()) {
		laby->moveCharacter(laby->getHero(), evt.timeSinceLastFrame);
	}
	if (laby->getVillain()) {
		laby->moveVillain(laby->getVillain(), evt.timeSinceLastFrame);
	}
	if (laby->checkHeroCollision() && laby->getHero() && laby->getHero()->lives() <= 0) {
		// fin de juego: mostrar mensaje, parar el movimiento, etc.
	}

	laby->updateLights();
	updateInfo();
}

void LabyrinthManager::updateInfo() {
	std::stringstream ss;
	ss << "Vidas: " << mHero->lives() << "\n"
		<< "Puntos: " << laby->Points() << "\n";
	infoText->setText(ss.str());
	ss.str("");
	ss << "Stage: " << currentStage + 1;
	infoLabel->setCaption(ss.str());
}