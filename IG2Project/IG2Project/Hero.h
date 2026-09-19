#pragma once

#include "IG2Object.h"
#include <OgreInput.h>

class Hero : public IG2Object
{
public:
	Hero(Vector3 iniPos, SceneNode* wallNode, SceneManager* sceneManager) :
		IG2Object(iniPos, wallNode, sceneManager, "Sinbad.mesh")
	{ }

    // Sobrescribimos el método keyPressed para reaccionar al teclado
    virtual bool keyPressed(const OgreBites::KeyboardEvent& evt) override
    {
        // Identificamos qué tecla se ha pulsado mediante evt.keysym.sym
        switch (evt.keysym.sym)
        {
        case SDLK_w:
        case SDLK_UP:
            // Mover hacia adelante (eje Z negativo en Ogre)
            mNode->translate(Vector3(0, 0, -10));
            break;

        case SDLK_s:
        case SDLK_DOWN:
            // Mover hacia atrás (eje Z positivo)
            mNode->translate(Vector3(0, 0, 10));
            break;

        case SDLK_a:
        case SDLK_LEFT:
            // Mover hacia la izquierda (eje X negativo)
            mNode->translate(Vector3(-10, 0, 0));
            break;

        case SDLK_d:
        case SDLK_RIGHT:
            // Mover hacia la derecha (eje X positivo)
            mNode->translate(Vector3(10, 0, 0));
            break;

        default:
            break;
        }

        return true; // Indica que el evento ha sido procesado
    }
};

