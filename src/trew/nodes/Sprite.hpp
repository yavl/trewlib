#pragma once

#include <trew/nodes/Node.hpp>

namespace trew {
	class Drawable;
    /// Sprite class to draw and transform textures.
    /** extends from Node class. */
	class Sprite : public Node {
	public:
		Sprite(Drawable* drawable);
		virtual ~Sprite() = default;
		void draw() const override;
	private:
		Drawable* drawable;
	};
}
