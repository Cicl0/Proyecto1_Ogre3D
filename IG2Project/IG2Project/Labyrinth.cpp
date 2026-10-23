#include "Labyrinth.h"

#include <iostream>
#include <cmath>
#include <algorithm>
#include <random>

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
	suelo->setMaterialName(floorMaterial);
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
				wall->setMaterialName(wallMaterial);
				matrix[iRow][iCol] = wall;
			}
			else if (cell == Constant::HERO_BLOCK){
				mHero = new Hero(iniPos, nHeroNode, mSM);
				mHero->setScale(Vector3(Constant::BLOCK_SIZE / 10.0f));

				mHero->InitialPosition() = iniPos;

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

bool Labyrinth::canMoveTo(int row, int col) {
	if (row < 0 || row >= numRows || col < 0 || col >= numCols) return false;
	Block* b = mapa[row][col];
	return b != nullptr && b->canPassThrough();
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

// El movimiento del villano sigue la siguiente logica:
// 1. Se calcula la celda actual y su centro
// 2. Si ha llegado al centro del bloque y ha cambiado de celda, se calcula la nueva dirección:
//   - Si el héroe está cerca, se calcula la dirección hacia él (priorizando el eje con mayor diferencia)
//   - Si el héroe está lejos, se elige una dirección aleatoria que no sea un muro
void Labyrinth::moveVillain(Villain* villain, Ogre::Real time) {
	Vector3 pos = villain->getPosition();
	Vector3 dir = villain->getGridOrientation();
	float halfSize = Constant::BLOCK_SIZE / 2.0f;

	// Calcula la celda actual en la matriz y su centro
	int col = floor((pos.x + halfSize) / Constant::BLOCK_SIZE);
	int row = floor((pos.z + halfSize) / Constant::BLOCK_SIZE);
	col = max(0, min(col, numCols - 1));
	row = max(0, min(row, numRows - 1));

	Vector3 center(col * Constant::BLOCK_SIZE, pos.y, row * Constant::BLOCK_SIZE);

	// Para evitar que calcule durante varios frames la misma celda, solo recalcula la dirección si ha llegado al centro del bloque y ha cambiado de celda
	bool reachedCenter = blockCenterReached(pos - center, dir);
	bool newCell = (row != villain->LastRow() || col != villain->LastCol());

	if (reachedCenter && newCell) {
		villain->LastRow() = row;
		villain->LastCol() = col;

		static std::mt19937 rng(std::random_device{}());

		Vector3 heroPos = mHero->getPosition();
		float difX = heroPos.x - center.x;
		float difZ = heroPos.z - center.z;

		float distance = sqrt(difX * difX + difZ * difZ);
		bool chase = distance < Constant::BLOCK_SIZE * 7;

		auto passable = [&](const Vector3& d) {
			return canMoveTo(row + (int)d.z, col + (int)d.x);
			};

		Vector3 chosen = Vector3::ZERO;

		// ---- PERSECUCIÓN ----

		if (chase) {
			// Entra en el área de persecución: se cancela la patrulla
			villain->PatrolLeft() = 0;

			Vector3 dirX(difX > 0 ? 1 : -1, 0, 0);
			Vector3 dirZ(0, 0, difZ > 0 ? 1 : -1);

			Vector3 primary, secondary;
			if (fabs(difX) >= fabs(difZ)) { primary = dirX; secondary = dirZ; }
			else { primary = dirZ; secondary = dirX; }

			bool secondaryUseful = (primary == dirX) ? (fabs(difZ) > 0.01f)
				: (fabs(difX) > 0.01f);

			if (passable(primary)) {
				chosen = primary;
			}
			else if (secondaryUseful && passable(secondary)) {
				chosen = secondary;
			}
			else {
				std::vector<Vector3> others = {
					Vector3(1, 0, 0), Vector3(-1, 0, 0),
					Vector3(0, 0, 1), Vector3(0, 0, -1)
				};
				others.erase(std::remove_if(others.begin(), others.end(),
					[&](const Vector3& d) { return d == primary || (secondaryUseful && d == secondary); }),
					others.end());

				std::shuffle(others.begin(), others.end(), rng);
				for (const Vector3& d : others) {
					if (passable(d)) { chosen = d; break; }
				}
			}
		}
		// ---- PATRULLA ----
		else {
			// Sigue recto mientras queden bloques y no haya muro de frente
			if (villain->PatrolLeft() > 0 && dir != Vector3::ZERO && passable(dir)) {
				chosen = dir;
				villain->PatrolLeft()--;
			}
			else {
				// Patrulla terminada o muro delante: nueva dirección aleatoria
				std::vector<Vector3> all = {
					Vector3(1, 0, 0), Vector3(-1, 0, 0),
					Vector3(0, 0, 1), Vector3(0, 0, -1)
				};
				std::shuffle(all.begin(), all.end(), rng);

				for (const Vector3& d : all) {
					if (d == -dir) continue;     // evita dar la vuelta si hay otra opción
					if (passable(d)) { chosen = d; break; }
				}

				// Esta decisión ya cuenta como el primer bloque
				villain->PatrolLeft() = Constant::PATROL_BLOCKS - 1;
			}
		}

		// Si todo falla, marcha atrás y se reinicia la patrulla
		if (chosen == Vector3::ZERO) {
			chosen = -dir;
			villain->PatrolLeft() = 0;
		}

		villain->setPosition(center);
		villain->setNextDirection(chosen);
	}

	moveCharacter(villain, time);
}
