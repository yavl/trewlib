#pragma once

namespace game::components {
	struct MoveTarget {
		double x, y;
	};

	struct Next {};

	struct ActiveRoute {
		flecs::entity head;
	};
}