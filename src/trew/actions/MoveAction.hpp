#pragma once

#include "Action.hpp"
#include <trew/Vector2.hpp>

namespace trew {
	class MoveAction : public Action {
	public:
		MoveAction(float x, float y, float time);
		void update(float dt, Node& node) override;
	private:
		Vector2 dest;
		float time;
	};
}