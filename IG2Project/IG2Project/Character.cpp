#include "Character.h"

void
Character::rotateToNewDirection() {
	dir = nextDir;

	if (dir == Vector3::UNIT_X) {
		mNode->setOrientation(Quaternion(Degree(90), Vector3::UNIT_Y));
	}
	else if (dir == Vector3::NEGATIVE_UNIT_X) {
		mNode->setOrientation(Quaternion(Degree(270), Vector3::UNIT_Y));
	}
	else if (dir == Vector3::UNIT_Z) {
		mNode->setOrientation(Quaternion(Degree(0), Vector3::UNIT_Y));
	}
	else if (dir == Vector3::NEGATIVE_UNIT_Z) {
		mNode->setOrientation(Quaternion(Degree(180), Vector3::UNIT_Y));
	}
}

bool
Character::is180Turn() {
	if (dir == Vector3::UNIT_X && nextDir == Vector3::NEGATIVE_UNIT_X)
		return true;
	else if (dir == Vector3::NEGATIVE_UNIT_X && nextDir == Vector3::UNIT_X)
		return true;
	else if (dir == Vector3::UNIT_Z && nextDir == Vector3::NEGATIVE_UNIT_Z)
		return true;
	else if (dir == Vector3::NEGATIVE_UNIT_Z && nextDir == Vector3::UNIT_Z)
		return true;
}