#pragma once

#include <string>
#include<fstream>
#include <vector>

#include "IG2Project.h"
#include "Constant.h"
#include "Hero.h"

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
		createLabyrinth(fileName);
	}

	void showBoundingBoxWalls() {
		nLabyrinthWalls->showBoundingBox(true);
	}

	void showBoundingBoxFloors() {
		nLabyrinthFloors->showBoundingBox(true);
	}

	Hero* getHero() { return mHero; }

private:
	Ogre::SceneNode* createLabyrinth(std::string stageFilename);

	Ogre::SceneManager* mSM;
	Ogre::SceneNode* nLabyrinthWalls;
	Ogre::SceneNode* nLabyrinthFloors;
	Ogre::SceneNode* nHeroNode;
	Hero* mHero;

	int numRows;
	int numCols;
};

