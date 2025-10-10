#include "Sprite.hpp"
#include <trew/drawables/Drawable.hpp>
#include <trew/graphics/Renderer.hpp>
#include <trew/drawables/ImageSurface.hpp>
#include <fmt/core.h>

using namespace trew;

Sprite::Sprite(Drawable* drawable) : Node("Sprite") {
	this->drawable = drawable;
	float width = static_cast<float>(drawable->getImageWidth());
	float height = static_cast<float>(drawable->getImageHeight());
	setSize(width, height);
}

void Sprite::draw() const {
    // todo draw via spritebatch instead
	if (getParent()) {
		glm::mat4 parentMatrix(1.f);
		parentMatrix = glm::translate(glm::mat4(1.f), glm::vec3(getParent()->getX(), getParent()->getY(), 0));
		parentMatrix = glm::rotate(parentMatrix, glm::radians(getParent()->getRotation()), glm::vec3(0.f, 0.f, -1.f));
		drawable->draw(getX() + getParent()->getX(), getY() + getParent()->getY(), getWidth(), getHeight(), getRotation(), parentMatrix, color);
	} else {
		drawable->draw(getX(), getY(), getWidth(), getHeight(), getRotation(), std::nullopt, color);
	}
	Node::draw();
}

void Sprite::draw(Renderer* renderer) {
	if (getParent()) {
		glm::mat4 parentMatrix(1.f);
		parentMatrix = glm::translate(glm::mat4(1.f), glm::vec3(getParent()->getX(), getParent()->getY(), 0));
		parentMatrix = glm::rotate(parentMatrix, glm::radians(getParent()->getRotation()), glm::vec3(0.f, 0.f, -1.f));
		auto surface = static_cast<ImageSurface*>(getDrawable());
		renderer->drawTexture(getX(), getY(), surface, getRotation(), parentMatrix);
		//drawable->draw(getX() + getParent()->getX(), getY() + getParent()->getY(), getWidth(), getHeight(), getRotation(), parentMatrix, color);
	}
	else {
		auto surface = static_cast<ImageSurface*>(getDrawable());
		renderer->drawTexture(getX(), getY(), surface, getRotation());
		//drawable->draw(getX(), getY(), getWidth(), getHeight(), getRotation(), std::nullopt, color);
	}
	// дрочилдрен
	drawChildren(renderer);
}

void Sprite::drawChildren(Renderer* renderer) {
	for (const auto& child : children) {
		auto sprite = static_cast<Sprite*>(child.get());
		sprite->draw(renderer);
	}
}

Color Sprite::getColor() const {
	return color;
}

void Sprite::setColor(Color color) {
	this->color = color;
}

Drawable* Sprite::getDrawable() const {
	return drawable;
}
