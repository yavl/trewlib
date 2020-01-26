#pragma once

#include "Action.hpp"
#include <trew/Vector2.hpp>

namespace trew {
	class MoveAction : public Action {
	public:
		MoveAction(float x, float y, float time);
		void update(float dt, Actor& actor) override;
	private:
		Vector2 dest;
		float time;
	};
}