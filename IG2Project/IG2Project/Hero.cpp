#include "Hero.h"

bool
Hero::keyPressed(const OgreBites::KeyboardEvent& evt) {
    // Identificamos qué tecla se ha pulsado mediante evt.keysym.sym
    switch (evt.keysym.sym)
    {
    case SDLK_w:
    case SDLK_UP:
        // Mover hacia adelante (eje Z negativo en Ogre)
        nextDir = Vector3::NEGATIVE_UNIT_Z;
        break;

    case SDLK_s:
    case SDLK_DOWN:
        // Mover hacia atrás (eje Z positivo)
        nextDir = Vector3::UNIT_Z;
        break;

    case SDLK_a:
    case SDLK_LEFT:
        // Mover hacia la izquierda (eje X negativo)
        nextDir = Vector3::NEGATIVE_UNIT_X;
        break;

    case SDLK_d:
    case SDLK_RIGHT:
        // Mover hacia la derecha (eje X positivo)
        nextDir = Vector3::UNIT_X;
        break;

    default:
        break;
    }

    return true;
}

void Hero::initAnimations() {
    mRunBase = entity->getAnimationState("RunBase");
    mRunTop = entity->getAnimationState("RunTop");
    mIdleBase = entity->getAnimationState("IdleBase");
    mIdleTop = entity->getAnimationState("IdleTop");
    for (auto* s : { mRunBase, mRunTop, mIdleBase, mIdleTop }) s->setLoop(true);

    // Al empezar la partida (dir == ZERO) está parado
    mIdleBase->setEnabled(true);  mIdleTop->setEnabled(true);
    mRunBase->setEnabled(false);  mRunTop->setEnabled(false);
}

void Hero::frameRendered(const Ogre::FrameEvent& evt) {
    if (!isStill() != mRunning) {   // solo se conmutan las animaciones cuando cambia el estado
        mRunning = !isStill();
        mRunBase->setEnabled(mRunning);   mRunTop->setEnabled(mRunning);
        mIdleBase->setEnabled(!mRunning); mIdleTop->setEnabled(!mRunning);
    }

    if (mRunning) { mRunBase->addTime(evt.timeSinceLastFrame);  mRunTop->addTime(evt.timeSinceLastFrame); }
    else { mIdleBase->addTime(evt.timeSinceLastFrame); mIdleTop->addTime(evt.timeSinceLastFrame); }
}