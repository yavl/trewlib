#include "Node.hpp"
#include "pm/Logger.hpp"

using namespace pm;

Node::Node() {
	parent = nullptr;
}

Node::Node(std::string name) : Node() {
	this->name = name;
}

Node::~Node() {
	logDebug("node", fmt::format("{} destructed", name));
	for (auto child : children) {
		delete child;
	}
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

void Node::addChild(Node* node) {
	bool isPresent = std::find(children.begin(), children.end(), node) == children.end();
	assert(isPresent && "can't add child twice");
	node->parent = this;
	children.emplace_back(std::move(node));
}

const std::vector<Node*> Node::getChildren() const {
	return children;
}

std::optional<Node*> Node::findChild(const std::function<bool(Node*)>& condition) {
	auto childIt = std::find_if(children.begin(), children.end(), condition);
	if (childIt != children.end())
		return *childIt;
	return std::nullopt;
}

std::optional<Node*> Node::findChild(std::string name) {
	auto child = findChild([name](Node* node) -> bool { return node->getName() == name; });
	return child;
}

std::string Node::getName() const {
	return name;
}
