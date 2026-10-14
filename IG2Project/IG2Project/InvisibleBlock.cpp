#include "InvisibleBlock.h"

void 
InvisibleBlock::frameRendered(const Ogre::FrameEvent& evt) {
	if (visibility) { // Calcula si ha pasado el tiempo de visibilidad y cambia el estado del bloque a invisible
		invisibleTime += evt.timeSinceLastFrame;
		if (invisibleTime >= visibleDuration) {
			visibility = false;
			invisibleTime = 0.0f;
			setVisible(false);
		}
	}
	else { // Lo mismo pero para el tiempo de invisisibilidad
		invisibleTime += evt.timeSinceLastFrame;
		if (invisibleTime >= invisibleDuration) {
			visibility = true;
			invisibleTime = 0.0f;
			setVisible(true);
		}
	}
}