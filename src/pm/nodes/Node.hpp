#pragma once

#include "pm/pm.hpp"
#include "pm/Vector2.hpp"
#include <vector>
#include <string>
#include <functional>
#include <optional>

namespace pm {
	class Node {
	public:
		Node();
		Node(std::string name);
		virtual ~Node();
		virtual float getX() const;
		virtual void setX(float x);
		virtual float getY() const;
		virtual void setY(float y);
		virtual void setXY(float x, float y);
		virtual void addChild(Node* node);
		virtual const std::vector<Node*> getChildren() const;
		virtual std::optional<Node*> findChild(const std::function<bool(Node*)>& condition);
		virtual std::optional<Node*> findChild(std::string name);
		virtual std::string getName() const;
	private:
		Vector2 pos;
		Node* parent;
		std::vector<Node*> children;
		std::string name;
	};
}