#pragma once

#include <Ogre.h>
#include <OgreInput.h>
#include <OgreApplicationContext.h>

// Animación inicial del juego (21 s en bucle): Sinbad baila en el centro, camina a la
// derecha (aparecen las espadas), camina a la izquierda y vuelve al centro.
// Termina al pulsar la tecla 's'.
class StartAnimation : public OgreBites::InputListener
{
public:
	StartAnimation(Ogre::SceneManager* sm, OgreBites::ApplicationContext* appCtx);
	~StartAnimation() override;   // libera todos los recursos de la intro

	// Se llama cada frame desde LabyrinthManager::frameRendered
	void frameRendered(const Ogre::FrameEvent& evt);

	// Devuelve true cuando se ha pulsado 's'
	bool startRequested() const { return mStartRequested; }

	bool keyPressed(const OgreBites::KeyboardEvent& evt) override;

private:
	void setupStartAnimation();   // crea toda la escena y las animaciones

	// Helper pedido en el enunciado
	void addKeyframe(Ogre::NodeAnimationTrack* track, Ogre::Real time,
		const Ogre::Quaternion& rot, const Ogre::Vector3& pos,
		const Ogre::Vector3& scale = Ogre::Vector3::UNIT_SCALE);

	Ogre::SceneManager* mSM;
	OgreBites::ApplicationContext* mAppCtx;

	bool mStartRequested = false;

	// Elementos de la escena
	Ogre::SceneNode* mRoot = nullptr;
	Ogre::SceneNode* mSinbadNode = nullptr;
	Ogre::Entity* mSinbad = nullptr;
	Ogre::Entity* mSwordL = nullptr;
	Ogre::Entity* mSwordR = nullptr;
	Ogre::Entity* mFloor = nullptr;
	Ogre::Light* mLight = nullptr;
	Ogre::SceneNode* mLightNode = nullptr;   // la dirección de la luz la da este nodo

	// Animaciones
	Ogre::AnimationState* mNodeState = nullptr;   // animación de nodo (21 s)
	Ogre::AnimationState* mRunBase = nullptr;
	Ogre::AnimationState* mRunTop = nullptr;
	Ogre::AnimationState* mDance = nullptr;
	Ogre::AnimationState* mHandsClosed = nullptr;

	// Instantes (s) de cada fase
	static constexpr Ogre::Real T_DANCE_START = 0.0f;    // baila al empezar el bucle...
	static constexpr Ogre::Real T_DANCE_END = 4.0f;    // ...hasta el keyframe 1
	static constexpr Ogre::Real T_SWORDS = 8.25f;   // llega al extremo derecho: aparecen espadas
	static constexpr Ogre::Real T_TOTAL = 21.0f;

	Ogre::Real mLastTime = 0;
};