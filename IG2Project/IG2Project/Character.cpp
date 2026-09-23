#include "Character.h"

Vector3
Character::getVector(Direction dir) {
	switch (dir) {
	case Direction::UP:    return Vector3::UNIT_X; // O Vector3::UNIT_Y según tu eje
	case Direction::DOWN:  return -Vector3::UNIT_X;
	case Direction::LEFT:  return -Vector3::UNIT_Z;
	case Direction::RIGHT: return Vector3::UNIT_Z;
	case Direction::NONE:  return Vector3::ZERO;
	}
	return Vector3::ZERO;
}

void
Character::rotateToNewDirection() {
	dir = nextDir;

	switch (dir) {
	case(UP):
		mNode->setOrientation(Quaternion(Degree(180), Vector3::UNIT_Y));
		break;
	case(DOWN):
		mNode->setOrientation(Quaternion(Degree(0), Vector3::UNIT_Y));
		break;
	case(RIGHT):
		mNode->setOrientation(Quaternion(Degree(90), Vector3::UNIT_Y));
		break;
	case(LEFT):
		mNode->setOrientation(Quaternion(Degree(270), Vector3::UNIT_Y));
		break;
	}
}

bool
Character::is180Turn() {
	return (dir ^ 1) == nextDir;
}