#include "Labyrinth.h"

#include <iostream>

#include "Wall.h"
#include "Empty.h"


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

			// Inserta un bloque vacio (suelo)
			if (cell == Constant::EMPTY_BLOCK) {
				SceneNode* floorNode = nLabyrinthFloors->createChildSceneNode();
				Empty* empty = new Empty(iniPos, floorNode, mSM); // En este caso en la constructora no asigna entidad
			}
			// Inserta un bloque lleno (muro)
			else if (cell == Constant::WALL_BLOCK) {
				SceneNode* wallNode = nLabyrinthWalls->createChildSceneNode();
				std::cout << "Pos Muro: " << iniPos.x << ", " << iniPos.y << ", " << iniPos.z << std::endl;
				Wall* wall = new Wall(iniPos, wallNode, mSM); // Internamente en la constructora se asigna la entidad al nodo
			}
			// Other type of block (por ahora vacio)
			else {

			}
			iCol++;
		}
		iRow++;
	}
	stageFile.close();
}