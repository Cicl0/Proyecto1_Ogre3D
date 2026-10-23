#include "Labyrinth.h"

#include <iostream>
#include <cmath>
#include <algorithm>

#include "Wall.h"
#include "Villain.h"


void
Labyrinth::createLabyrinth(std::string stageFileName) {
	std::ifstream stageFile;
	stageFile.open(stageFileName);

	// Read the number of files and columns
	stageFile >> numRows;
	stageFile >> numCols;

	vector<vector<Block*>> matrix = vector<vector<Block*>>(numRows, vector<Block*>(numCols, 0));

	// --- Suelo del laberinto ---
	float anchoX = Constant::BLOCK_SIZE * numCols - Constant::BLOCK_SIZE;   // eje X = columnas
	float largoZ = Constant::BLOCK_SIZE * numRows - Constant::BLOCK_SIZE;   // eje Z = filas

	Plane plano(Vector3::UNIT_Y, 0);                 // normal hacia arriba
	MeshManager::getSingleton().createPlane(
		"LabyrinthFloorMesh",
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		plano,
		anchoX, largoZ,        // tamaño
		1, 1,                  // segmentos
		true, 1,               // normales, nº de juegos de UV
		numCols / 4, numRows / 4,      // la textura se repite una vez por bloque
		Vector3::UNIT_Z);      // dirección "arriba" de la textura

	Entity* suelo = mSM->createEntity("LabyrinthFloorMesh");
	suelo->setMaterialName("examples/Grass");
	suelo->setCastShadows(false);

	SceneNode* nSuelo = mSM->getRootSceneNode()->createChildSceneNode();
	nSuelo->attachObject(suelo);

	nSuelo->setPosition(numCols * Constant::BLOCK_SIZE / 2.0f,   // mitad de columnas (X)
		-Constant::BLOCK_SIZE / 2.0f,            // altura (Y), sin cambios
		numRows * Constant::BLOCK_SIZE / 2.0f);  // mitad de filas (Z)

	// --- Construcción del laberinto ---
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
				wall->setMaterialName("examples/Wall");
				matrix[iRow][iCol] = wall;
			}
			else if (cell == Constant::HERO_BLOCK){
				mHero = new Hero(iniPos, nHeroNode, mSM);
				mHero->setScale(Vector3(Constant::BLOCK_SIZE / 10.0f));

				SceneNode* floorNode = nLabyrinthFloors->createChildSceneNode();
				Empty* empty = new Empty(iniPos, floorNode, mSM);
				matrix[iRow][iCol] = empty;
			}
			else if (cell == Constant::VILLAIN_BLOCK){
				mVillain = new Villain(iniPos, nVillainNode, mSM);
				mVillain->setScale(Vector3(Constant::BLOCK_SIZE / 250.0f));

				SceneNode* floorNode = nLabyrinthFloors->createChildSceneNode();
				Empty* empty = new Empty(iniPos, floorNode, mSM);
				matrix[iRow][iCol] = empty;
			}
			else { // Other type of block (por ahora vacio)

			}

			iCol++;
		}
		iRow++;
	}

	stageFile.close();

	mapa = move(matrix);
}

Block*&
Labyrinth::getBlock(Ogre::Vector3 pos) {
	float halfSize = Constant::BLOCK_SIZE / 2.0f;

	float posX = pos.x;
	float posZ = pos.z;

	int col = floor((pos.x + halfSize) / Constant::BLOCK_SIZE);
	int row = floor((pos.z + halfSize) / Constant::BLOCK_SIZE);

	return mapa[row][col];
}

bool
Labyrinth::blockCenterReached(Ogre::Vector3 difference, Ogre::Vector3 dir) {
	return difference.dotProduct(dir) >= 0.0f;
}

void 
Labyrinth::stepForward(Character* character, float speed, Real time, Block* nextBlock) {
	if (character->isStill())
		return;

	Vector3 currentPos = character->getPosition();
	Vector3 dir = character->getGridOrientation();
	float halfSize = Constant::BLOCK_SIZE / 2.0f;

	// Calcular en qué celda está actualmente
	int col = floor((currentPos.x + halfSize) / Constant::BLOCK_SIZE);
	int row = floor((currentPos.z + halfSize) / Constant::BLOCK_SIZE);

	// El centro del bloque [row][col] está EXACTAMENTE en estas coordenadas:
	Vector3 blockCenter(
		col * Constant::BLOCK_SIZE,
		currentPos.y,
		row * Constant::BLOCK_SIZE
	);

	// Siguiente posición si se moviera
	Vector3 charNewPos = currentPos + (dir * speed * time);
	Vector3 difference = charNewPos - blockCenter;

	// Si enfrente hay una pared/bloque no traspasable Y con este movimiento al menos se alcanza el centro:
	if (!nextBlock->canPassThrough() && blockCenterReached(difference, dir)) {
		// Se coloca exactamente en el centro del bloque actual y no se mueve más
		character->setPosition(blockCenter);
		return;
	}

	// Si el camino está libre o aún no ha llegado al centro, avanza libremente
	character->move(dir * speed * time);
	cout << "Char posx: " << character->getPosition().x << ", posz: " << character->getPosition().z << endl;
}

void
Labyrinth::moveCharacter(Character* character, Real time) {
	Vector3 charPos = character->getPosition();
	float halfSize = Constant::BLOCK_SIZE / 2.0f;

	// 1. Obtener la celda actual en la matriz
	// X -> Columnas, Z -> Filas
	int currentCol = floor((charPos.x + halfSize) / Constant::BLOCK_SIZE);
	int currentRow = floor((charPos.z + halfSize) / Constant::BLOCK_SIZE);

	currentCol = max(0, min(currentCol, numCols - 1));
	currentRow = max(0, min(currentRow, numRows - 1));

	Block* charBlock = mapa[currentRow][currentCol];

	// 2. Obtener el bloque de ENFRENTE según la dirección actual
	Vector3 gridDir = character->getGridOrientation();

	int frontCol = currentCol + gridDir.x;
	int frontRow = currentRow + gridDir.z;

	Block* inFrontBlock = mapa[frontRow][frontCol];

	// Centro exacto del bloque actual en coordenadas 3D
	Vector3 blockCenter(
		currentCol * Constant::BLOCK_SIZE,
		charPos.y,
		currentRow * Constant::BLOCK_SIZE
	);

	if (!character->isDirectionModified()) {
		stepForward(character, character->getSpeed(), time, inFrontBlock);
	}
	else {
		// Obtener el bloque de la nueva dirección deseada (nextDir)
		Vector3 nextDir = character->getNexDirVector();

		int newCol = currentCol + nextDir.x;
		int newRow = currentRow + nextDir.z;

		Block* newDirBlock = mapa[newRow][newCol];

		// calcula la diferencia entre la nueva posicion y el centro del bloque actual
		Vector3 charNewPos = charPos + (gridDir * character->getSpeed() * time);
		Vector3 difference = charNewPos - blockCenter;

		// Situaciones potenciales:
		// Giro de 180°
		if (character->is180Turn()) {
			character->rotateToNewDirection();
			stepForward(character, character->getSpeed(), time, inFrontBlock);
		}
		// Giro válido a 90°
		else if (newDirBlock->canPassThrough() && blockCenterReached(difference, gridDir)) {
			character->setPosition(blockCenter); // Clavar en el centro exacto antes de rotar, para evitar solapamientos entre las paredes
			character->rotateToNewDirection();
		}
		// No se puede girar (es un muro o no ha llegado al centro)
		else {
			stepForward(character, character->getSpeed(), time, inFrontBlock);
		}
	}
}
