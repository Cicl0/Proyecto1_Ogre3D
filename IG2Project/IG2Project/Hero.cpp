#include "Hero.h"

bool
Hero::keyPressed(const OgreBites::KeyboardEvent& evt) {
    // Identificamos qué tecla se ha pulsado mediante evt.keysym.sym
    switch (evt.keysym.sym)
    {
    case SDLK_w:
    case SDLK_UP:
        // Mover hacia adelante (eje Z negativo en Ogre)
        dir = UP;
        mNode->setOrientation(Quaternion(Degree(180), Vector3::UNIT_Y));
        break;

    case SDLK_s:
    case SDLK_DOWN:
        // Mover hacia atrás (eje Z positivo)
        dir = DOWN;
        mNode->setOrientation(Quaternion(Degree(0), Vector3::UNIT_Y));
        break;

    case SDLK_a:
    case SDLK_LEFT:
        // Mover hacia la izquierda (eje X negativo)
        dir = LEFT;
        mNode->setOrientation(Quaternion(Degree(270), Vector3::UNIT_Y));

        break;

    case SDLK_d:
    case SDLK_RIGHT:
        // Mover hacia la derecha (eje X positivo)
        dir = RIGHT;
        mNode->setOrientation(Quaternion(Degree(90), Vector3::UNIT_Y));
        break;

    default:
        break;
    }

    return true;
}