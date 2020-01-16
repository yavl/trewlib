#pragma once

#include "trew/Vector2.hpp"
#include <vector>
#include <string>
#include <functional>
#include <optional>
#include <trew/actions/Actor.hpp>

namespace trew {
	class Node : public Actor {
	public:
		Node();
		Node(std::string name);
		virtual ~Node();
		virtual float getX() const;
		virtual void setX(float x);
		virtual float getY() const;
		virtual void setY(float y);
		virtual void setXY(float x, float y);
		virtual float getWidth() const;
		virtual void setWidth(float width);
		virtual float getHeight() const;
		virtual void setHeight(float height);
		virtual void setSize(float width, float height);
		virtual Node* getParent() const;
		virtual void draw();
		virtual void addChild(Node* node);
		virtual const std::vector<Node*>& getChildren() const;
		virtual std::optional<Node*> findChild(const std::function<bool(Node*)>& condition);
		virtual std::optional<Node*> findChild(std::string name);
		virtual std::string getName() const;

		void act(float dt) override;
		void addAction(Action* action) override;
	private:
		Vector2 pos;
		Vector2 size;
		Node* parent;
		std::vector<Node*> children;
		std::vector<Action*> actions;
		std::string name;
	};
}
