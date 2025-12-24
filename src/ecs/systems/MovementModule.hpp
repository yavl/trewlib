#pragma once

#include <flecs.h>
#include "../components/MotionComponents.hpp"
#include "../components/NavigationComponents.hpp"

namespace game::systems {
    struct Movement {
        Movement(flecs::world& world);
    };
}