#pragma once

#include <trew/nodes/Node.hpp>

namespace trew {
	class Drawable;
	class Sprite : public Node {
	public:
		Sprite(Drawable* drawable);
		virtual ~Sprite() = default;
		void draw() const override;
	private:
		Drawable* drawable;
	};
}