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

	vector<vector<Block*>> matrix = vector<vector<Block*>>(numRows, vector<Block*>(numCols, 0));

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
				matrix[iRow][iCol] = empty;
			}
			else if (cell == Constant::WALL_BLOCK) { // Inserta un bloque lleno (muro)
				SceneNode* wallNode = nLabyrinthWalls->createChildSceneNode();
				Wall* wall = new Wall(iniPos, wallNode, mSM); // Internamente en la constructora se asigna la entidad al nodo
				matrix[iRow][iCol] = wall;
			}
			else if (cell == Constant::HERO_BLOCK){
				mHero = new Hero(iniPos, nHeroNode, mSM);
				mHero->setScale(Vector3(10));

				SceneNode* floorNode = nLabyrinthFloors->createChildSceneNode();
				Empty* empty = new Empty(iniPos, floorNode, mSM);
				matrix[iRow][iCol] = empty;
			}
			else{ // Other type of block (por ahora vacio)

			}

			iCol++;
		}
		iRow++;
	}
	stageFile.close();

	mapa = move(matrix);
}

Block*
Labyrinth::getBlock(Ogre::Vector3 pos) {
	// 1. Obtener la columna (eje X) y la fila (eje Z) dividiendo directamente
	int col = pos.x / Constant::BLOCK_SIZE;
	if (fmod(pos.x, Constant::BLOCK_SIZE) > Constant::BLOCK_SIZE / 2) col++;
	int row = pos.z / Constant::BLOCK_SIZE;
	if (fmod(pos.z, Constant::BLOCK_SIZE) > Constant::BLOCK_SIZE / 2) row++;

	// Control de bordes del mapa
	if (row < 0 || row >= numRows || col < 0 || col >= numCols) {
		return nullptr;
	}

	return mapa[row][col];
}

bool
Labyrinth::blockCenterReached(Ogre::Vector3 difference, Ogre::Vector3 dir) {
	return difference.dotProduct(dir) >= 0.0f;
}

void
Labyrinth::stepForward(Character* character, float speed, Real time) { // Podria hacerse por setPosition() pero he preferido translate
	Ogre::Vector3 displacement = character->getGridOrientation() * speed * time;
	character->move(displacement);
}

void
Labyrinth::moveCharacter(Character* character, Real time) {
	Block* charBlock, * inFrontBlock;

	// Get the block where the character is placed, and the next one
	charBlock = this->getBlock(character->getPosition());
	inFrontBlock = this->getBlock((character->getGridOrientation() * Constant::BLOCK_SIZE) + character->getPosition());

	// Character does not change its direction -> step forward!
	if (!character->isDirectionModified())
		stepForward(character, character->getSpeed(), time);

	// New direction
	else {
		// Check the block in front of the character for the new direction
		Block* newDirBlock = this->getBlock(character->getPosition() + (character->getNexDirVector() * Constant::BLOCK_SIZE));

		// New position of the character after moving... (for checking if the center of the block is reached)
		Vector3 charNewPos = character->getPosition() + (character->getGridOrientation() * character->getSpeed() * time);
		Vector3 difference = Vector3(charNewPos.x - charBlock->getPosition().x, 0, charNewPos.z - charBlock->getPosition().z);

		// Check if the character can rotate for a new VALID direction
		if (newDirBlock->canPassThrough() && blockCenterReached(difference, character->getGridOrientation()))
			character->rotateToNewDirection();

		// 180 turn?
		else if (character->is180Turn())
			character->rotateToNewDirection();

		// Rotation cannot be performed... check if character can step forward
		else
			stepForward(character, character->getSpeed(), time);
	}
}