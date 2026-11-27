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

	if (mAppCtx) {
		if(laby->getHero()) mAppCtx->addInputListener(laby->getHero());
		if(laby->getVillain()) mAppCtx->addInputListener(laby->getVillain());
		if(laby) mAppCtx->addInputListener(laby);
		for (auto b : laby->getBlocksVector()) {
			mAppCtx->addInputListener(b);
		}
	}
}

void
LabyrinthManager::setupStartAnimation() {
	startAnim = new StartAnimation(mSM, mAppCtx);
}

void 
LabyrinthManager::frameRendered(const Ogre::FrameEvent& evt) {
	if (startAnim) {

		if (startAnim->startRequested()) {
			// Se hace aquí y no en keyPressed: no se puede modificar la lista de
			// listeners mientras ApplicationContext la recorre para despachar la tecla.
			delete startAnim;          // el destructor libera toda la intro
			startAnim = nullptr;

			setup();                   // crea el laberinto
			if (mOnGameStart) mOnGameStart();   // IG2Project coloca la cámara del juego
		}
		return;
	}

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

	vector<Bomb*>& bombs = laby->getBombs();
	for (auto it = bombs.begin(); it != bombs.end(); ) {
		if ((*it)->hasExploded()) {
			mAppCtx->removeInputListener(*it);
			laby->explodeBomb(*it);
			delete* it;
			it = bombs.erase(it);
		}
		else it++;
	}

	laby->updateLights();
	updateInfo();
}

bool
LabyrinthManager::keyPressed(const OgreBites::KeyboardEvent& evt) {
	if (evt.keysym.sym == SDLK_SPACE) {
		spawnBomb();
	}
	return true;
}

void LabyrinthManager::updateInfo() {
	std::stringstream ss;
	ss << "Vidas: " << (mHero ? mHero->lives() : 0) << "\n"
		<< "Puntos: " << laby->Points() << "\n";
	infoText->setText(ss.str());
	ss.str("");
	ss << "Stage: " << currentStage + 1;
	infoLabel->setCaption(ss.str());
}

void
LabyrinthManager::spawnBomb() {
	if (laby->getBombsSpawned() >= laby->getMaxBombs()) return; // No se pueden spawnear mas y hay que esperar a que exploten

	Block* block(laby->getBlock(mHero->getPosition())); // Coje el bloque en el que esta el heroe
	if (!block->canSetBomb()) return; // Si no se puede colocar la bomba no hace nada 

	Vector3 bPos = block->getPosition(); // La posicion de la bomba es el centro del bloque en el que se encuentra
	SceneNode* nBombs = mSM->getSceneNode("nBombs");
	SceneNode* bNode = nBombs->createChildSceneNode();
	Bomb* bomb = new Bomb(bPos, bNode, mSM);
	block->setBomb(true);
	mAppCtx->addInputListener(bomb);

	laby->getBombs().push_back(bomb);
}