#pragma once

#include "Action.hpp"
#include <trew/Vector2.hpp>

namespace trew {
	class MoveAction : public Action {
	public:
		MoveAction(float x, float y, float time);
		void update(float dt, Node& node) override;
		bool hasFinished() const override;
	private:
		Vector2 dest;
		float initialDist;
		float time;
		bool finished;
		bool init;
	};
}