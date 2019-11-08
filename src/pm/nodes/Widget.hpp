#pragma once
#include "Node.hpp"

namespace pm {
	class Widget : public Node {
	public:
		Widget(std::string name);
		virtual ~Widget() = default;
	private:
	};
}
