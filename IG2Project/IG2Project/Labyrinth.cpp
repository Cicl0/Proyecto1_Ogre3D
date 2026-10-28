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
	stageFile >> numRows >> numCols;

	string wallMaterial, floorMaterial;
	stageFile >> wallMaterial >> floorMaterial >> lightType;

	vector<vector<Block*>> matrix = vector<vector<Block*>>(numRows, vector<Block*>(numCols, 0));

	// --- Suelo del laberinto ---
	float anchoX = Constant::BLOCK_SIZE * numCols - Constant::BLOCK_SIZE;   // eje X = columnas
	float largoZ = Constant::BLOCK_SIZE * numRows - Constant::BLOCK_SIZE;   // eje Z = filas

	MeshManager::getSingleton().createPlane(
		"FloorTileMesh", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		Plane(Vector3::UNIT_Y, 0),
		Constant::BLOCK_SIZE, Constant::BLOCK_SIZE,
		20, 20,              // segmentos por bloque
		true, 1, 1, 1, Vector3::UNIT_Z);

	for (int r = 0; r < numRows; ++r) {
		for (int c = 0; c < numCols; ++c) {
			Entity* e = mSM->createEntity("FloorTileMesh");
			e->setMaterialName(floorMaterial);
			e->setCastShadows(true);
			SceneNode* n = mSM->getRootSceneNode()->createChildSceneNode(
				Vector3(c * Constant::BLOCK_SIZE, -Constant::BLOCK_SIZE / 2.0f,
					r * Constant::BLOCK_SIZE));
			n->attachObject(e);
		}
	}

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

				wall->setScale(Vector3(Constant::MESH_TO_BLOCK));

				wall->setMaterialName(wallMaterial);
				matrix[iRow][iCol] = wall;
			}
			else if (cell == Constant::HERO_BLOCK){
				mHero = new Hero(iniPos, nHeroNode, mSM);
				nHeroNode->setScale(Vector3(Constant::BLOCK_SIZE / 10.0f));
				mHero->setInitialPosition(iniPos);

				SceneNode* floorNode = nLabyrinthFloors->createChildSceneNode();
				Empty* empty = new Empty(iniPos, floorNode, mSM);
				matrix[iRow][iCol] = empty;
			}
			else if (cell == Constant::VILLAIN_BLOCK){
				mVillain = new Villain(iniPos, nVillainNode, mSM);
				mVillain->setScale(Vector3(Constant::BLOCK_SIZE / 250.0f));

				mVillain->setInitialPosition(iniPos);

				SceneNode* floorNode = nLabyrinthFloors->createChildSceneNode();
				Empty* empty = new Empty(iniPos, floorNode, mSM);
				matrix[iRow][iCol] = empty;
			}
			else if (cell == Constant::FAKE_BLOCK) { // Inserta un bloque lleno (muro)
				SceneNode* fakeNode = nLabyrinthWalls->createChildSceneNode();
				FakeBlock* fake = new FakeBlock(iniPos, fakeNode, mSM); // Internamente en la constructora se asigna la entidad al nodo

				fake->setScale(Vector3(Constant::MESH_TO_BLOCK));

				fake->setMaterialName(wallMaterial);
				matrix[iRow][iCol] = fake;
			}
			else if (cell == Constant::INVISIBLE_BLOCK) { // Inserta un bloque lleno (muro)
				SceneNode* inviNode = nLabyrinthWalls->createChildSceneNode();
				InvisibleBlock* invisible = new InvisibleBlock(iniPos, inviNode, mSM); // Internamente en la constructora se asigna la entidad al nodo

				invisible->setScale(Vector3(Constant::MESH_TO_BLOCK));
				blocks.push_back(invisible);

				invisible->setMaterialName(wallMaterial);
				matrix[iRow][iCol] = invisible;
			}

			iCol++;
		}
		iRow++;
	}

	stageFile.close();

	mapa = move(matrix);

	// Crea las luces
	createLights();
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

		mVillain->setChasing(chase);

		// ---- PERSECUCIÓN ----
		if (chase) {
			Vector3 dirX(difX > 0 ? 1 : -1, 0, 0);
			Vector3 dirZ(0, 0, difZ > 0 ? 1 : -1);

			Vector3 primary, secondary;
			if (fabs(difX) >= fabs(difZ)) { primary = dirX; secondary = dirZ; }
			else { primary = dirZ; secondary = dirX; }

			bool secondaryUseful = (primary == dirX) ? (fabs(difZ) > 0.01f) : (fabs(difX) > 0.01f);

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
			std::vector<Vector3> options;
			const Vector3 all[4] = {
				Vector3(1, 0, 0), Vector3(-1, 0, 0),
				Vector3(0, 0, 1), Vector3(0, 0, -1)
			};

			// Direcciones transitables que no sean volver por donde ha venido
			for (const Vector3& d : all) {
				if (dir != Vector3::ZERO && d == -dir) continue;
				if (passable(d)) options.push_back(d);
			}

			if (!options.empty()) {
				std::uniform_int_distribution<int> pick(0, (int)options.size() - 1);
				chosen = options[pick(rng)];
			}
			// Si options está vacía (callejón sin salida), chosen sigue en ZERO
			// y el fallback de abajo lo hace volver atrás.
		}

		// Si todo falla, marcha atrás y se reinicia la patrulla
		if (chosen == Vector3::ZERO) {
			chosen = -dir;
		}

		villain->setPosition(center);
		villain->setNextDirection(chosen);
	}

	moveCharacter(villain, time);
}

bool Labyrinth::heroTouchesVillain() {
	if (!mHero || !mVillain) return false;
	return mHero->getAABB().intersects(mVillain->getAABB());
}

bool Labyrinth::checkHeroCollision() {
	if (!heroTouchesVillain())
		return true;

	mHero->lives()--; // pierde una vida

	if (mHero->lives() > 0) { 
		mHero->restartPosition(); // reinicia las posiciones del heroe y del villano
		mVillain->restartPosition();

		return true;
	}

	return false; // fin del juego
}

void Labyrinth::createLights() {
	const float B = Constant::BLOCK_SIZE;

	if (lightType == "directional") {
		// --- DIRECTIONAL ---
		mDirLight = mSM->createLight("DirLight");
		mDirLight->setType(Light::LT_DIRECTIONAL);
		mPointLight->setDiffuseColour(1.0f, 1.0f, 1.0f);
		mPointLight->setSpecularColour(1.0f, 1.0f, 1.0f);
		mDirLight->setCastShadows(true);

		SceneNode* nDir = mSM->getRootSceneNode()->createChildSceneNode();
		nDir->attachObject(mDirLight);
		nDir->setDirection(Vector3(-1, -1, -0.5f));
	}
	else if (lightType == "spotlight") {
		// --- SPOTLIGHT ---
		mSpotLight = mSM->createLight("SpotLight");
		mSpotLight->setType(Light::LT_SPOTLIGHT);
		mSpotLight->setDiffuseColour(1.0f, 1.0f, 1.0f);
		mSpotLight->setSpecularColour(1.0f, 1.0f, 1.0f);
		mSpotLight->setSpotlightFalloff(1.0f);
		mSpotLight->setSpotlightRange(Degree(B * 6), Degree(B * 7));   // cono interior y exterior
		mSpotLight->setAttenuation(B * 20, 1.0f, 0.0f, 0.0f);    // alcance suficiente
		mSpotLight->setCastShadows(true);

		nSpot = mSM->getRootSceneNode()->createChildSceneNode();
		nSpot->attachObject(mSpotLight);
		nSpot->setDirection(Vector3(0, -1, 0));
	}
	else if (lightType == "point") {
		// --- POINT ---
		mPointLight = mSM->createLight("PointLight");
		mPointLight->setType(Light::LT_POINT);
		mPointLight->setDiffuseColour(1.0f, 1.0f, 1.0f);
		mPointLight->setSpecularColour(1.0f, 1.0f, 1.0f);
		mPointLight->setAttenuation(B * 6, 1.0f, 0.0f, 0.0f);
		mPointLight->setCastShadows(true);

		nPoint = mSM->getRootSceneNode()->createChildSceneNode();
		nPoint->attachObject(mPointLight);
	}

	updateLights();
}

void Labyrinth::updateLights() {
	if (!mHero) return;
	Vector3 h = mHero->getPosition();
	const float B = Constant::BLOCK_SIZE;

	if(lightType == "spotlight") nSpot->setPosition(h.x, B * 6, h.z);    // misma x y z que el héroe, por encima
	else if (lightType == "point") nPoint->setPosition(h.x, B * 0.75f, h.z + B); // sobre su cabeza
}