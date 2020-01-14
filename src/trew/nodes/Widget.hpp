#pragma once
#include "Node.hpp"

namespace trew {
	class Widget : public Node {
	public:
		Widget(std::string name);
		virtual ~Widget() = default;
	private:
	};
}
