#pragma once

namespace trew {
	class Action;
	class Actor {
	public:
		virtual void act(float dt) = 0;
		virtual void addAction(Action* action) = 0;
	};
}