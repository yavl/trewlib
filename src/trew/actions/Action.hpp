#pragma once

namespace trew {
	class Node;
	class Action {
	public:
		virtual ~Action() = default;
		virtual void update(float dt, Node& node) = 0;
		virtual bool hasFinished() const = 0;
	};
}