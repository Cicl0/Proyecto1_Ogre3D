#pragma once

#include <string>
#include<fstream>
#include <vector>

#include "IG2Project.h"
#include "Constant.h"

class Labyrinth
{
public:

	Labyrinth(Ogre::SceneManager* sm, std::string fileName) :
		mSM(sm),
		nLabyrinthWalls(nullptr),
		nLabyrinthFloors(nullptr),
		numRows(0),
		numCols(0)
	{
		nLabyrinthWalls = mSM->getRootSceneNode()->createChildSceneNode("nLabyrinthWalls");
		nLabyrinthFloors = mSM->getRootSceneNode()->createChildSceneNode("nLabyrinthFloors");
		createLabyrinth(fileName);
	}

	void showBoundingBoxFloors() {
		nLabyrinthWalls->showBoundingBox(true);
	}

	void showBoundingBoxWalls() {
		nLabyrinthFloors->showBoundingBox(true);
	}

private:
	Ogre::SceneNode* createLabyrinth(std::string stageFilename);

	Ogre::SceneManager* mSM;
	Ogre::SceneNode* nLabyrinthWalls;
	Ogre::SceneNode* nLabyrinthFloors;

	int numRows;
	int numCols;
};

