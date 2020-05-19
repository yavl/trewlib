#pragma once

namespace trew {
	enum class Key {
		ESCAPE,
		W,
		A,
		S,
		D
	};

	class Input {
	public:
		virtual ~Input() = default;
		virtual bool isKeyPressed(Key key) const = 0;
	};
}