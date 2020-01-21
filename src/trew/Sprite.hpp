#pragma once

#include "trew/nodes/Node.hpp"

namespace trew {
	class Texture;
	class Sprite : public Node {
	public:
		Sprite(Texture* tex);
		virtual ~Sprite() = default;
		void draw() const override;
	private:
		Texture* texture;
	};
}