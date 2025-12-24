#include "MovementModule.hpp"
#include "../components/MotionComponents.hpp"
#include "../components/NavigationComponents.hpp"
#include <cmath>

using namespace game::systems;
using namespace game::components;

Movement::Movement(flecs::world& world) {
	world.module<Movement>();
	world.system<Position, Velocity, ActiveRoute>("MoveToTarget").each([](flecs::entity e, Position& p, Velocity& v, ActiveRoute& route) {
		if (!route.head || !route.head.is_alive()) {
			v.x = 0; v.y = 0;
			return;
		}

		const MoveTarget& target = route.head.get<MoveTarget>();
		double dx = target.x - p.x;
		double dy = target.y - p.y;
		double dist_sq = dx * dx + dy * dy;

		auto epsilon = 10.f;
		if (dist_sq < epsilon) {
			route.head.destruct();
			route.head = route.head.target<Next>();

			v.x = 0; v.y = 0;
		}
		else {
			double dist = std::sqrt(dist_sq);
			double speed = 100.0;
			v.x = (dx / dist) * speed;
			v.y = (dy / dist) * speed;
		}
		});

	world.system<Position, Velocity>("ApplyVelocity")
		.each([](flecs::iter& it, size_t, Position& p, Velocity& v) {
		p.x += v.x * it.delta_time();
		p.y += v.y * it.delta_time();
			});
}
