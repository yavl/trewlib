#pragma once

#include <trew/drawables/ImageSurface.hpp>

using namespace trew;

// Tag types
struct Eats {};
struct Apples {};

struct Position {
	double x;
	double y;
};

struct Velocity {
	double x;
	double y;
};

struct Image {
	ImageSurface* image;
};

struct CharacterIdentity {
	std::string name;
};

struct MapIdentity {
	std::string name;
};