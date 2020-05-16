#include "MoveAction.hpp"
#include <trew/nodes/Node.hpp>
#include <cmath>

using namespace trew;

MoveAction::MoveAction(float x, float y, float time) : dest(x, y) {
	this->time = time;
	finished = false;
	init = false;
	initialDist = 0.f;
}

void MoveAction::update(float dt, Node& node) {
	if (!init) {
		initialDist = Vector2(node.getX(), node.getY()).distance(Vector2(dest.x, dest.y));
		init = true;
	}
	// WIP
	float dx = dest.x - node.getX();
	float dy = dest.y - node.getY();
	float dist = Vector2(dx, dy).length();
	float speed = initialDist / (time * dt * 3600.f);

	if (dist > speed) {
		float ratio = speed / dist;
		node.setXY(node.getX() + ratio * dx, node.getY() + ratio * dy);
	}
	else {
		node.setXY(dest.x, dest.y);
		finished = true;
	}
}

bool MoveAction::hasFinished() const
{
	return finished;
}
