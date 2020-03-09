#pragma once

namespace trew {
	enum class Key {
		ESCAPE
	};

	class Input {
	public:
		virtual ~Input() = default;
		virtual bool isKeyPressed(Key key) const = 0;
	};
}