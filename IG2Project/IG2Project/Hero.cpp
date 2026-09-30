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