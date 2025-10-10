#pragma once

#include <trew/nodes/Node.hpp>
#include <trew/Color.hpp>

namespace trew {
	class Drawable;
	class SpriteBatch;
	class Renderer;
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
		void draw(Renderer* renderer);
		void drawChildren(Renderer* renderer);

		Color getColor() const;
		void setColor(Color color);

		Drawable* getDrawable() const;
	private:
		Drawable* drawable;
		Color color = Color();
	};
}
