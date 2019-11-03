#pragma once

#include "pm/nodes/Node.hpp"

namespace pm {
	class Texture;
	class Sprite : public Node {
	public:
		Sprite(Texture* tex);
		virtual ~Sprite() = default;
		void draw() override;
	private:
		Texture* texture;
	};
}