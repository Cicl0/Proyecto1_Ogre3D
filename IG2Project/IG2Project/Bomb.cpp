#include "Bomb.h"

#include <OgreMaterialManager.h>
#include <OgreLogManager.h>

#include <algorithm>

void Bomb::createMesh()
{
	radius = Constant::BLOCK_SIZE * 0.4f;

	// ---- Esfera (sphere.mesh tiene radio 100). Solo este nodo crece. ----
	sphere = mSM->createEntity("sphere.mesh");
	sphere->setMaterialName("examples/BombBlack");
	sphere->setCastShadows(false);

	sphereBase = radius / 100.0f;
	sphereNode = mNode->createChildSceneNode();
	sphereNode->setScale(sphereBase, sphereBase, sphereBase);
	sphereNode->attachObject(sphere);

	// ---- Mecha (column.mesh): escala constante, hermana de la esfera ----
	fuse = mSM->createEntity("column.mesh");
	fuse->setCastShadows(false);

	const AxisAlignedBox& box = fuse->getBoundingBox();
	fuseLength = radius * 0.6f;
	const float fuseScale = fuseLength / box.getSize().y;
	const float thin = 0.5f;   // mecha mas fina que alta

	fuseNode = mNode->createChildSceneNode();
	fuseNode->setScale(fuseScale * thin, fuseScale, fuseScale * thin);
	fuseNode->attachObject(fuse);

	// Base de la mecha pegada a la cima de la esfera (con un pequeno solape)
	const Vector3 c = box.getCenter();
	fuseBaseY = radius - box.getMinimum().y * fuseScale - radius * 0.05f;
	fuseOffset = fuseBaseY - radius;
	fuseNode->setPosition(-c.x * fuseScale * thin, fuseBaseY, -c.z * fuseScale * thin);

	// ---- Humo: plantilla "Bomb/Fuse" (Bomb.particle). Nombre unico por bomba. ----
	static int bombId = 0;
	mFuseFx = mSM->createParticleSystem("BombFuse" + std::to_string(bombId++), "Bomb/Fuse");
	mFuseFx->setCastShadows(false);

	// El script esta pensado para radius = 4. Escalamos tamano y velocidad al radio real.
	const float k = radius / 4.0f;
	mFuseFx->setDefaultDimensions(3.0f * k, 3.0f * k);
	if (ParticleEmitter* em = mFuseFx->getEmitter(0))
		em->setParticleVelocity(3.0f * k, 6.0f * k);   // hacia arriba (direction 0 1 0)

	mSmokeNode = mNode->createChildSceneNode();
	mSmokeNode->attachObject(mFuseFx);

	arm();
}

// Reinicia la bomba
void Bomb::arm()
{
	elapsed = 0.0f;
	exploded = false;

	sphereNode->setScale(sphereBase * minScale, sphereBase * minScale, sphereBase * minScale);

	const float baseY = fuseFollowsSphere ? radius * minScale + fuseOffset : fuseBaseY;
	mSmokeNode->setPosition(0, baseY + fuseLength, 0);

	mFuseFx->clear();              // quita las particulas vivas
	mFuseFx->setEmitting(true);
}

void Bomb::frameRendered(const FrameEvent& evt)
{
	if (exploded) return;

	// Limita el delta: un breakpoint haria explotar la bomba de golpe
	elapsed += std::min(evt.timeSinceLastFrame, 0.05f);
	const float t = std::min(elapsed / fuseTime, 1.0f);   // 0 -> 1

	// ---- Tamano de la esfera: oscila entre minScale y maxScale ----
	const float osc = 0.5f - 0.5f * std::cos(elapsed * growSpeed * Math::TWO_PI);   // 0..1
	const float factor = minScale + (maxScale - minScale) * osc;
	const float s = sphereBase * factor;
	sphereNode->setScale(s, s, s);          // la mecha NO cambia de tamano

	// ---- Mecha: misma escala siempre; solo sigue la cima de la esfera ----
	const float baseY = radius * factor + fuseOffset;
	Vector3 fp = fuseNode->getPosition();
	fp.y = baseY;
	fuseNode->setPosition(fp);

	// ---- Emisor de humo: baja de la punta al final de la mecha (o al centro) ----
	// Llega justo cuando acaba fuseTime: velocidad = distancia / fuseTime.
	// Las particulas ya emitidas quedan en el mundo y suben solas.
	const float tipY = baseY + fuseLength;
	const float endY =  0.0f;
	mSmokeNode->setPosition(0, tipY + (endY - tipY) * t, 0);

	if (t >= 1.0f) {
		exploded = true;
		mFuseFx->setEmitting(false);
	}
}

// ADAPTA: si la clase base ya destruye mNode o entidades, quita esa parte de aqui.
Bomb::~Bomb()
{
	try {
		if (mFuseFx) { mSM->destroyParticleSystem(mFuseFx); mFuseFx = nullptr; }
		if (sphere) { mSM->destroyEntity(sphere);          sphere = nullptr; }
		if (fuse) { mSM->destroyEntity(fuse);            fuse = nullptr; }
		if (mNode) {
			mNode->removeAndDestroyAllChildren();
			mSM->destroySceneNode(mNode);
			mNode = nullptr;
		}
	}
	catch (const Ogre::Exception& e) {
		LogManager::getSingleton().logMessage("~Bomb: " + e.getFullDescription());
	}
}
