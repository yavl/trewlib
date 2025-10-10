#pragma once

#include <trew/Asset.hpp>
#include <trew/drawables/Drawable.hpp>

struct SDL_Surface;

namespace trew {
	class Color;
	class ImageSurface : public Asset, public Drawable {
	public:
		ImageSurface(SDL_Surface* sdlSurface, int width, int height);
		SDL_Surface* getSdlSurface() const;
		void draw(float x, float y, float width, float heigh, float rotation, std::optional<glm::mat4> parentModelMatrix, Color color) override;
		int getImageWidth() const override;
		int getImageHeight() const override;
	private:
		SDL_Surface* sdlSurface;
		int width;
		int height;
	};
}