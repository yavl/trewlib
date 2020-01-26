#pragma once

namespace trew {
	class Actor;
	class Action {
	public:
		virtual ~Action() = default;
		virtual void update(float dt, Actor& actor) = 0;
	};
}