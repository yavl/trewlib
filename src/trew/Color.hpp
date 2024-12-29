#pragma once

namespace trew {
	class Color {
	public:
		Color();
		Color(float r, float g, float b);
		virtual ~Color() = default;
		virtual Color operator+(const Color&) const;
		virtual bool operator==(const Color&) const;
		virtual bool operator!=(const Color&) const;
		virtual void operator=(const Color&);

		float r, g, b;
	private:
	};
}
