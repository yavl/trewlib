#include "ImageSurface.hpp"

#include <SDL3/SDL_surface.h>
#include <trew/Color.hpp>

using namespace trew;

ImageSurface::ImageSurface(SDL_Surface* sdlSurface, int width, int height) :
	sdlSurface(sdlSurface),
	width(width),
	height(height)
{
}

SDL_Surface* ImageSurface::getSdlSurface() const {
	return sdlSurface;
}

void ImageSurface::draw(float x, float y, float width, float heigh, float rotation, std::optional<glm::mat4> parentModelMatrix, Color color) {
}

int ImageSurface::getImageWidth() const {
	return width;
}

int ImageSurface::getImageHeight() const {
	return height;
}
