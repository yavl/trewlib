#include "MoveAction.hpp"
#include <trew/nodes/Node.hpp>

using namespace trew;

MoveAction::MoveAction(float x, float y, float time) : dest(x, y) {
	this->time = time;
}

void MoveAction::update(float dt, Node& node) {
	// WIP
	float speed = dt / time;
	float destX = dest.x - node.getX();
	float destY = dest.y - node.getY();
	Vector2 des = Vector2(destX, destY).normalized();
	float dist = Vector2(destX, destY).length();

	float travelX = destX * speed;
	float travelY = destY * speed;
	float distTravel = sqrt(travelX*travelX + travelY*travelY);

	if (distTravel > dist) {
		node.setXY(destX, destY);
	} else {
		node.setX(node.getX() + travelX);
		node.setY(node.getY() + travelY);
	}
}
