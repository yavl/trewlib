#pragma once

#include <glm/glm.hpp>

namespace pm {
	class Camera {
	private:
		glm::mat4 matrix;
	public:
		Camera();
		~Camera();
		void update();
	};
}