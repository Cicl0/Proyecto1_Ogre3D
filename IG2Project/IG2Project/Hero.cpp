#include "Hero.h"

bool
Hero::keyPressed(const OgreBites::KeyboardEvent& evt) {
    // Identificamos qué tecla se ha pulsado mediante evt.keysym.sym
    switch (evt.keysym.sym)
    {
    case SDLK_w:
    case SDLK_UP:
        // Mover hacia adelante (eje Z negativo en Ogre)
        mNode->translate(Vector3(0, 0, -1 * Constant::BLOCK_SIZE));
        mNode->setOrientation(Quaternion(Degree(180), Vector3::UNIT_Y));
        break;

    case SDLK_s:
    case SDLK_DOWN:
        // Mover hacia atrás (eje Z positivo)
        mNode->translate(Vector3(0, 0, 1 * Constant::BLOCK_SIZE));
        mNode->setOrientation(Quaternion(Degree(0), Vector3::UNIT_Y));
        break;

    case SDLK_a:
    case SDLK_LEFT:
        // Mover hacia la izquierda (eje X negativo)
        mNode->translate(Vector3(-1 * Constant::BLOCK_SIZE, 0, 0));
        mNode->setOrientation(Quaternion(Degree(270), Vector3::UNIT_Y));
        break;

    case SDLK_d:
    case SDLK_RIGHT:
        // Mover hacia la derecha (eje X positivo)
        mNode->translate(Vector3(1 * Constant::BLOCK_SIZE, 0, 0));
        mNode->setOrientation(Quaternion(Degree(90), Vector3::UNIT_Y));
        break;

    default:
        break;
    }

    return true;
}