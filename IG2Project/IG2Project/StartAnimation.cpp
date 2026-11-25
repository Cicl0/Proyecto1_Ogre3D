#include "StartAnimation.h"

StartAnimation::StartAnimation(Ogre::SceneManager* sm, OgreBites::ApplicationContext* appCtx)
	: mSM(sm), mAppCtx(appCtx)
{
	setupStartAnimation();
}

// Todo el montaje de la intro ocurre aquí
void StartAnimation::setupStartAnimation() {

	// ---------- Escena de la intro ----------
	mRoot = mSM->getRootSceneNode()->createChildSceneNode("introRoot");
	mSM->setAmbientLight(Ogre::ColourValue(0.4f, 0.4f, 0.4f));

	mLight = mSM->createLight("introLight");
	mLight->setType(Ogre::Light::LT_DIRECTIONAL);
	mLight->setDiffuseColour(1.0f, 1.0f, 1.0f);
	mLight->setSpecularColour(1.0f, 1.0f, 1.0f);

	mLightNode = mRoot->createChildSceneNode("introLightNode");
	mLightNode->attachObject(mLight);
	mLightNode->setDirection(Ogre::Vector3(0.3f, -1.0f, -0.5f));

	// ---------- Suelo 150x300 (distinto al del laberinto) ----------
	Ogre::MeshManager::getSingleton().createPlane(
		"introPlane", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		Ogre::Plane(Ogre::Vector3::UNIT_Y, 0), 150, 300, 20, 20, true, 1, 5, 5,
		Ogre::Vector3::UNIT_Z);
	mFloor = mSM->createEntity("introFloor", "introPlane");
	mFloor->setMaterialName("Examples/BeachStones");   // pon aquí un material que tengas
	mRoot->createChildSceneNode("introFloorNode")->attachObject(mFloor);

	// ---------- Sinbad ----------
	// Posición y escala BASE de Sinbad (cámbialas aquí). Las traslaciones y escalas de los
	// keyframes son relativas a este estado inicial.
	const Ogre::Vector3 SINBAD_POS(0, 5, 0);   // Y = altura de los pies sobre el suelo
	const Ogre::Real SINBAD_SCALE = 1.0f;

	mSinbadNode = mRoot->createChildSceneNode("introSinbad", SINBAD_POS);
	mSinbadNode->setScale(SINBAD_SCALE, SINBAD_SCALE, SINBAD_SCALE);
	// Imprescindible: una animación de nodo hace resetToInitialState() en cada frame, y
	// sin esto el estado inicial es posición (0,0,0) y escala 1, ignorando lo de arriba.
	mSinbadNode->setInitialState();

	mSinbad = mSM->createEntity("introSinbadEnt", "Sinbad.mesh");
	mSinbadNode->attachObject(mSinbad);

	// Espadas: ya en las manos pero ocultas; se muestran al llegar al extremo derecho
	mSwordL = mSM->createEntity("introSwordL", "Sword.mesh");
	mSwordR = mSM->createEntity("introSwordR", "Sword.mesh");
	mSinbad->attachObjectToBone("Handle.L", mSwordL);
	mSinbad->attachObjectToBone("Handle.R", mSwordR);
	mSwordL->setVisible(false);
	mSwordR->setVisible(false);

	// ---------- Animaciones de esqueleto ----------
	mRunBase = mSinbad->getAnimationState("RunBase");
	mRunTop = mSinbad->getAnimationState("RunTop");
	mDance = mSinbad->getAnimationState("Dance");
	mHandsClosed = mSinbad->getAnimationState("HandsClosed");
	for (auto* s : { mRunBase, mRunTop, mDance, mHandsClosed }) s->setLoop(true);

	// La intro empieza bailando
	mDance->setEnabled(true);
	mRunBase->setEnabled(false);
	mRunTop->setEnabled(false);

	// ---------- Animación de nodo: 9 keyframes, 21 s ----------
	Ogre::Animation* anim = mSM->createAnimation("introAnim", T_TOTAL);
	anim->setInterpolationMode(Ogre::Animation::IM_LINEAR);
	anim->setRotationInterpolationMode(Ogre::Animation::RIM_SPHERICAL);
	Ogre::NodeAnimationTrack* track = anim->createNodeTrack(0, mSinbadNode);

	using Ogre::Vector3; using Ogre::Quaternion; using Ogre::Degree;
	const Vector3 Y = Vector3::UNIT_Y;
	const Quaternion right(Degree(90), Y), left(Degree(-90), Y), front = Quaternion::IDENTITY;

	const Ogre::Real SIDE = 10;   // distancia del centro a cada extremo

	// Recorrido: baila en el centro -> derecha (aparecen espadas) -> izquierda -> centro
	// Caminar: 10, 20 y 10 unidades => 3.25 s, 6.5 s y 3.25 s (misma velocidad); cada giro dura 1 s
	addKeyframe(track, 0, front, Vector3(0, 0, 0));      // 0: centro, de frente: empieza el baile
	addKeyframe(track, 4, front, Vector3(0, 0, 0));      // 1: fin del baile
	addKeyframe(track, 5, right, Vector3(0, 0, 0));      // 2: gira hacia la derecha
	addKeyframe(track, 8.25f, right, Vector3(SIDE, 0, 0));   // 3: extremo derecho (aparecen las espadas)
	addKeyframe(track, 9.25f, left, Vector3(SIDE, 0, 0));   // 4: gira hacia la izquierda
	addKeyframe(track, 15.75f, left, Vector3(-SIDE, 0, 0));  // 5: extremo izquierdo
	addKeyframe(track, 16.75f, right, Vector3(-SIDE, 0, 0)); // 6: gira hacia la derecha
	addKeyframe(track, 20, right, Vector3(0, 0, 0));      // 7: vuelve al centro
	addKeyframe(track, 21, front, Vector3(0, 0, 0));      // 8: se pone de frente; = keyframe 0 (bucle continuo)

	mNodeState = mSM->createAnimationState("introAnim");
	mNodeState->setLoop(true);
	mNodeState->setEnabled(true);

	// ---------- Entrada de teclado ----------
	mAppCtx->addInputListener(this);
}

void StartAnimation::addKeyframe(Ogre::NodeAnimationTrack* track, Ogre::Real time,
	const Ogre::Quaternion& rot, const Ogre::Vector3& pos, const Ogre::Vector3& scale)
{
	Ogre::TransformKeyFrame* kf = track->createNodeKeyFrame(time);
	kf->setRotation(rot);
	kf->setTranslate(pos);
	kf->setScale(scale);
}

// Solo levanta una bandera: no se puede tocar la lista de listeners mientras
// ApplicationContext la está recorriendo para despachar esta misma tecla.
bool StartAnimation::keyPressed(const OgreBites::KeyboardEvent& evt) {
	if (evt.keysym.sym == 's') mStartRequested = true;
	return true;
}

void StartAnimation::frameRendered(const Ogre::FrameEvent& evt) {
	const Ogre::Real dt = evt.timeSinceLastFrame;

	mNodeState->addTime(dt);
	const Ogre::Real t = mNodeState->getTimePosition();

	// Si el tiempo ha retrocedido, el bucle ha dado la vuelta: vuelve el baile y
	// Sinbad pierde las espadas
	if (t < mLastTime) {
		mSwordL->setVisible(false);
		mSwordR->setVisible(false);
		mHandsClosed->setEnabled(false);
	}
	mLastTime = t;

	// Aparecen las espadas al llegar al extremo derecho
	if (t >= T_SWORDS && !mSwordL->getVisible()) {
		mSwordL->setVisible(true);
		mSwordR->setVisible(true);
		mHandsClosed->setEnabled(true);
	}

	// Baile al principio (en el centro); correr el resto del tiempo
	const bool dancing = (t >= T_DANCE_START && t < T_DANCE_END);
	mDance->setEnabled(dancing);
	mRunBase->setEnabled(!dancing);
	mRunTop->setEnabled(!dancing);

	if (dancing) mDance->addTime(dt);
	else { mRunBase->addTime(dt); mRunTop->addTime(dt); }
	if (mHandsClosed->getEnabled()) mHandsClosed->addTime(dt);
}

StartAnimation::~StartAnimation() {

}