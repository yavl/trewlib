#pragma once

namespace trew {
	class Application {
	public:
		virtual ~Application() = default;
		virtual void create() = 0;
		virtual void update(float dt) = 0;
		virtual void render() = 0;
		virtual void resize(int width, int height) = 0;
	};
}