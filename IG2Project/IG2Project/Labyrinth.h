#pragma once

#include <string>
#include<fstream>
#include <vector>

#include "IG2Project.h"
#include "Constant.h"
#include "Hero.h"
#include "Villain.h"
#include "Block.h"
#include "Wall.h"
#include "Empty.h"
#include "FakeBlock.h"
#include "InvisibleBlock.h"
#include "BreakableBlock.h"
#include "Bomb.h"

#include <OgreInput.h>

class Labyrinth : public OgreBites::InputListener
{
public:

	Labyrinth(Ogre::SceneManager* sm, std::string fileName) :
		mSM(sm),
		numRows(0),
		numCols(0)
	{
		nLabyrinthWalls = mSM->getRootSceneNode()->createChildSceneNode("nLabyrinthWalls");
		nLabyrinthFloors = mSM->getRootSceneNode()->createChildSceneNode("nLabyrinthFloors");
		nHeroNode = mSM->getRootSceneNode()->createChildSceneNode("nHero");
		nVillainNode = mSM->getRootSceneNode()->createChildSceneNode("nVillain");
		nBombs = mSM->getRootSceneNode()->createChildSceneNode("nBombs");
		createLabyrinth(fileName);
	}

	~Labyrinth() {
		for (auto& fila : mapa) {
			for (Block*& b : fila) {
				delete b;       // delete sobre nullptr es seguro
				b = nullptr;
			}
		}
		mapa.clear();

		if (mHero) delete mHero; mHero = nullptr;
		if (mVillain) delete mVillain; mVillain = nullptr;
	}

	void showBoundingBoxWalls() {
		nLabyrinthWalls->showBoundingBox(true);
	}

	void showBoundingBoxFloors() {
		nLabyrinthFloors->showBoundingBox(true);
	}

	Hero* getHero() { return mHero; }
	Villain* getVillain() { return mVillain; }
	Block*& getBlock(Ogre::Vector3 pos); // Busqueda por posicion absoluta
	Block* blockAt(int row, int col); // Busqueda por fila y columna

	int& Points() { return points; }

	void moveCharacter(Character* character, Real time);
	void moveVillain(Villain* villain, Ogre::Real time);

	bool checkHeroCollision();

	void updateLights();

	int getNumRows() { return numRows; }
	int getNumCols() { return numCols; }

	vector<Block*>& getBlocksVector() { return blocks; }


	// --- Bombs ---
	vector<Bomb*>& getBombs() { return bombs; }

	void setMaxBombs(int max) { maxBombs = max; }
	int getMaxBombs() { return maxBombs; }

	int getBombsSpawned() { return bombs.size(); }

	void explodeBomb(Bomb* bomb);


private:

	void createLabyrinth(std::string stageFilename);


	bool blockCenterReached(Ogre::Vector3 difference, Ogre::Vector3 dir);
	bool canMoveTo(int row, int col);
	void stepForward(Character* character, float speed, Real time, Block* nextBlock);

	bool heroTouchesVillain();

	void createLights();

	Ogre::SceneManager* mSM;

	Ogre::SceneNode* nLabyrinthWalls;
	Ogre::SceneNode* nLabyrinthFloors;
	Ogre::SceneNode* nHeroNode;
	Ogre::SceneNode* nVillainNode;
	Ogre::SceneNode* nBombs;

	Hero* mHero;
	Villain* mVillain;

	vector<vector<Block*>> mapa;

	int numRows;
	int numCols;

	string wallMaterial = "examples/Wall";
	string floorMaterial = "examples/Grass";

	int points = 0;

	Ogre::Light* mDirLight = nullptr;
	Ogre::Light* mSpotLight = nullptr;
	Ogre::Light* mPointLight = nullptr;
	Ogre::SceneNode* nSpot = nullptr;
	Ogre::SceneNode* nPoint = nullptr;

	string lightType = "";

	vector<Block*> blocks;

	vector<Bomb*> bombs;
	int maxBombs = 10;
};

