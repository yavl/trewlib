#include "Node.hpp"
#include <trew/Logger.hpp>
#include <trew/actions/Action.hpp>

using namespace trew;

const std::string logTag = "Node";

Node::Node() {
	logDebug(logTag, fmt::format("{} constructed", name));
}

Node::Node(std::string name): name(name) {
	logDebug(logTag, fmt::format("{} constructed", name));
}

Node::~Node() {
	logDebug(logTag, fmt::format("{} destructed", name));
}

float Node::getX() const {
	return pos.x;
}

void Node::setX(float x) {
	pos.x = x;
}

float Node::getY() const {
	return pos.y;
}

void Node::setY(float y) {
	pos.y = y;
}

void Node::setXY(float x, float y) {
	pos.x = x;
	pos.y = y;
}

float Node::getWidth() const {
	return size.x;
}

void Node::setWidth(float width) {
	setSize(width, getHeight());
}

float Node::getHeight() const {
	return size.y;
}

void Node::setHeight(float height) {
	setSize(getWidth(), height);
}

void Node::setSize(float width, float height) {
	size.x = width;
	size.y = height;
}

Node* Node::getParent() const {
	return parent;
}

void Node::draw() const {
	for (auto& child : children) {
		child->draw();
	}
}

void Node::addChild(Node* node) {
	std::unique_ptr<Node> nodePtr(node);
	nodePtr->parent = this;
	children.push_back(std::move(nodePtr));
}

std::string Node::getName() const {
	return name;
}

void Node::act(float dt) {
	actions.erase(std::remove_if(actions.begin(), actions.end(), [&](std::unique_ptr<Action>& action) -> bool {
		if (action->hasFinished())
			return true;
		action->update(dt, *this);
		return false;
	}), actions.end());
}

void Node::addAction(Action* action) {
	std::unique_ptr<Action> actionPtr(action);
	actions.push_back(std::move(actionPtr));
}
