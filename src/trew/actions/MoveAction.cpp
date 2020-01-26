#include "MoveAction.hpp"
#include <trew/nodes/Node.hpp>

using namespace trew;

MoveAction::MoveAction(float x, float y, float time) : dest(x, y) {
	this->time = time;
}

void MoveAction::update(float dt, Actor& actor) {
	// WIP
	float speed = dt / time;
	float destX = dest.x - actor.getX();
	float destY = dest.y - actor.getY();
	Vector2 des = Vector2(destX, destY).normalized();
	float dist = Vector2(destX, destY).length();

	float travelX = destX * speed;
	float travelY = destY * speed;
	float distTravel = sqrt(travelX*travelX + travelY*travelY);

	if (distTravel > dist) {
		actor.setXY(destX, destY);
	} else {
		actor.setX(actor.getX() + travelX);
		actor.setY(actor.getY() + travelY);
	}
}
