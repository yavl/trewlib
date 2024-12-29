#pragma once

#include <trew/nodes/Node.hpp>
#include <trew/Color.hpp>

namespace trew {
	class Drawable;
    /// Sprite class to draw and transform textures.
    /** extends from Node class. */
	class Sprite : public Node {
	public:
		Sprite(Drawable* drawable);
		virtual ~Sprite() = default;
		/// <summary>
		/// draws sprite
		/// </summary>
		void draw() const override;

		Color getColor() const;
		void setColor(Color color);
	private:
		Drawable* drawable;
		Color color = Color(1, 1, 1);
	};
}
