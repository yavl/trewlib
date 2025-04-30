#pragma once

namespace trew {
	enum class Key {
		ESCAPE,
		W,
		A,
		S,
		D,
		LEFT_MOUSE_BUTTON,
		MIDDLE_MOUSE_BUTTON
	};

	class Input {
	public:
		virtual ~Input() = default;
		virtual bool isKeyPressed(Key key) const = 0;
		virtual bool isMousePressed(Key key) const = 0;
	};
}