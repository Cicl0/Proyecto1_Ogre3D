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
			// Inserta un bloque vacio (suelo)
			if (cell == Constant::EMPTY_BLOCK) {
				Vector3 iniPos(Constant::BLOCK_SIZE * iCol, 0, Constant::BLOCK_SIZE * iRow);
				Empty* empty = new Empty(iniPos, nLabyrinthFloors, mSM); // En este caso en la constructora no asigna entidad
			}
			// Inserta un bloque lleno (muro)
			else if (cell == Constant::WALL_BLOCK) {
				Vector3 iniPos(Constant::BLOCK_SIZE * iCol, 0, Constant::BLOCK_SIZE * iRow);
				Vector3 scale(Constant::BLOCK_SIZE);
				Wall* wall = new Wall(iniPos, nLabyrinthWalls, mSM); // Internamente en la constructora se asigna la entidad al nodo
				wall->setScale(scale);
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