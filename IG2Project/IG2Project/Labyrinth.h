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

class Labyrinth
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
		createLabyrinth(fileName);
	}

	void showBoundingBoxWalls() {
		nLabyrinthWalls->showBoundingBox(true);
	}

	void showBoundingBoxFloors() {
		nLabyrinthFloors->showBoundingBox(true);
	}

	Hero* getHero() { return mHero; }

	Villain* getVillain() { return mVillain; }

	Block*& getBlock(Ogre::Vector3 pos);

	int& Points() { return points; }

	void moveCharacter(Character* character, Real time);

	void moveVillain(Villain* villain, Ogre::Real time);

private:
	void createLabyrinth(std::string stageFilename);

	bool blockCenterReached(Ogre::Vector3 difference, Ogre::Vector3 dir);

	bool canMoveTo(int row, int col);

	void stepForward(Character* character, float speed, Real time, Block* nextBlock);

	Ogre::SceneManager* mSM;
	Ogre::SceneNode* nLabyrinthWalls;
	Ogre::SceneNode* nLabyrinthFloors;
	Ogre::SceneNode* nHeroNode;
	Ogre::SceneNode* nVillainNode;
	Hero* mHero;
	Villain* mVillain;

	vector<vector<Block*>> mapa;

	int numRows;
	int numCols;

	int points = 0;
};

