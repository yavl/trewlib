#pragma once

#include <vector>
#include <string>
#include <functional>
#include <optional>
#include <trew/Vector2.hpp>
#include <trew/actions/Actor.hpp>

namespace trew {
	class Node : public Actor {
	public:
		Node();
		Node(std::string name);
		virtual ~Node();
		virtual Node* getParent() const;
		virtual void draw() const;
		virtual void addChild(Node* node);
		virtual std::string getName() const;

		virtual float getX() const override;
		virtual void setX(float x) override;
		virtual float getY() const override;
		virtual void setY(float y) override;
		virtual void setXY(float x, float y) override;
		virtual float getWidth() const override;
		virtual void setWidth(float width) override;
		virtual float getHeight() const override;
		virtual void setHeight(float height) override;
		virtual void setSize(float width, float height) override;

		void act(float dt) override;
		void addAction(Action* action) override;
	private:
		Vector2 pos;
		Vector2 size;
		Node* parent;
		std::vector<std::unique_ptr<Node>> children;
		std::vector<std::unique_ptr<Action>> actions;
		std::string name;
	};
}
