#include "Labyrinth.h"

#include <iostream>

#include "Wall.h"
#include "Empty.h"
#include "Hero.h"


SceneNode*
Labyrinth::createLabyrinth(std::string stageFileName) {
	std::ifstream stageFile;
	stageFile.open(stageFileName);

	// Read the number of files and columns
	stageFile >> numRows;
	stageFile >> numCols;

	int iRow = 0, iCol = 0;
	char cell; // siguiente celda, puede ser 'x' u 'o'
	// Tenemos que hacer la clase abstracta "Block" que tendra de hijos "Solid", "Empty" y a futuro "Breakable"
	while (iRow < numRows) {
		iCol = 0;
		while (iCol < numCols) {
			stageFile >> cell;

			Vector3 iniPos(Constant::BLOCK_SIZE * iCol, 0, Constant::BLOCK_SIZE * iRow);

	
			if (cell == Constant::EMPTY_BLOCK) { // Inserta un bloque vacio (suelo)
				SceneNode* floorNode = nLabyrinthFloors->createChildSceneNode();
				Empty* empty = new Empty(iniPos, floorNode, mSM); // En este caso en la constructora no asigna entidad
			}
			else if (cell == Constant::WALL_BLOCK) { // Inserta un bloque lleno (muro)
				SceneNode* wallNode = nLabyrinthWalls->createChildSceneNode();
				Wall* wall = new Wall(iniPos, wallNode, mSM); // Internamente en la constructora se asigna la entidad al nodo
			}
			else if (cell == Constant::HERO_BLOCK){
				mHero = new Hero(iniPos, nHeroNode, mSM);
				mHero->setScale(Vector3(10));
			}
			else{ // Other type of block (por ahora vacio)

			}

			iCol++;
		}
		iRow++;
	}
	stageFile.close();
}