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

	~Labyrinth() {
		_CrtMemState estadoInicial, estadoFinal, diferencia;

		_CrtMemCheckpoint(&estadoInicial);

		delete mHero; mHero = nullptr;
		delete mVillain; mVillain = nullptr;

		for (auto& i : mapa) {
			for (auto& j : i) {
				delete j; j = nullptr;
			}
			i.clear();
		}
		mapa.clear();

		delete mDirLight; mDirLight = nullptr;
		delete mSpotLight; mSpotLight = nullptr;
		delete mPointLight; mPointLight = nullptr;

		_CrtMemCheckpoint(&estadoFinal);

		if (_CrtMemDifference(&diferencia, &estadoInicial, &estadoFinal)) {
			// _NORMAL_BLOCK corresponde a las reservas hechas por tu aplicación (new / malloc)
			size_t numeroDeLeaks = diferencia.lCounts[_NORMAL_BLOCK];
			size_t bytesPerdidos = diferencia.lSizes[_NORMAL_BLOCK];

			std::cout << "Memory leaks detectados: " << numeroDeLeaks << "--------------------------------------------------------------\n";
			std::cout << "Total de bytes sin liberar: " << bytesPerdidos << " bytes----------------------------------------------------\n";

			// Opcional: imprime en la ventana Output de VS solo los leaks ocurridos entre ambos puntos
			_CrtMemDumpStatistics(&diferencia);
		}
		else {
			std::cout << "0 memory leaks.\n";
		}
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

	bool checkHeroCollision();

	void updateLights();

	int getNumRows() { return numRows; }
	int getNumCols() { return numCols; }

	vector<Block*> getBlocksVector() { return blocks; }
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
};

