#pragma once

#include "Constant.h"
#include <OgreParticleSystem.h>
#include <OgreParticleSystemManager.h>
#include <OgreBillboard.h>
#include <OgreBillboardSet.h>
#include <OgreParticleEmitter.h>

#include "IG2Object.h"

class Bomb : public IG2Object
{
public:
	Bomb(Vector3 initPos, SceneNode* node, SceneManager* sceneMng)
		: IG2Object(initPos, node, sceneMng)
	{
		createMesh();
	}
	~Bomb();

	void frameRendered(const Ogre::FrameEvent& evt);

	bool hasExploded() const { return exploded; }

	int getRange() { return range; }

	float fuseTime = 3.0f;   // segundos desde que se coloca hasta que explota
	float minScale = 0.9f;   // tamanio minimo
	float maxScale = 1.2f;   // tamanio maximo
	float growSpeed = 2.0f;   // ciclos minimo->maximo->minimo por segundo
	float frameTime = 0.08f;  // segundos por fotograma del humo
	int   range = 2;      // alcance de la explosion en bloques

	bool fuseFollowsSphere = true;

private:
	std::vector<ColourValue> sGradient;

	// Partes visuales
	Ogre::Entity* sphere = nullptr;
	Ogre::Entity* fuse = nullptr;
	Ogre::SceneNode* sphereNode = nullptr;   // solo este nodo cambia de escala
	Ogre::SceneNode* fuseNode = nullptr;   // escala constante
	Ogre::ParticleSystem* mFuseFx = nullptr;   // humo
	Ogre::SceneNode* mSmokeNode = nullptr;   // nodo del emisor (baja por la mecha)

	// Geometria (locales a mNode, con la esfera en su tamano base)
	float radius = 0.0f;
	float sphereBase = 0.0f;   // escala base de la esfera (radius / 100)
	float fuseBaseY = 0.0f;   // altura de la base de la mecha con la esfera a escala 1
	float fuseOffset = 0.0f;   // fuseBaseY - radius (distancia a la cima de la esfera)
	float fuseLength = 0.0f;   // altura de la mecha

	// Estado
	float elapsed = 0.0f;
	bool  exploded = false;

	void createMesh();
	void arm();
};

