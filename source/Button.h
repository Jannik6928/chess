#pragma once

#include <functional>
#include <map>

#include "Event.h"
#include "Element.h"

#include <SFML/Graphics.hpp>

using namespace std;

namespace gui {
	// a simple element which only provides event handling
	class Button : public Element {
	private:
	public:
		Button() {}
		Button(sf::Rect<float> const& bounds) {
			_bounds = bounds;
		}
		Button(sf::Vector2f const& pos) {
			setPosition(pos);
		}
		Button(std::string const& filePath, sf::IntRect const& section) {
			sf::Texture* t = new sf::Texture;
			t->loadFromFile(filePath, section);
			sf::Sprite s(*t);
			_bounds = sf::Rect<float>(sf::Vector2f(0, 0), sf::Vector2f(section.width, section.height));
		}
		Button(std::string const& filePath, sf::IntRect const& section, sf::Vector2f const& pos) {
			sf::Texture* t = new sf::Texture;
			t->loadFromFile(filePath, section);
			sf::Sprite s(*t);
			_sprite = s;
			_bounds = sf::Rect<float>(sf::Vector2f(0, 0), sf::Vector2f(section.width, section.height));
			setPosition(pos);
		}
		Button(sf::Sprite const& sprite, sf::Vector2f const& pos) {
			_sprite = sprite;
			_bounds = sf::Rect<float>(sf::Vector2f(0, 0), sf::Vector2f(sprite.getLocalBounds().width, sprite.getLocalBounds().height));
			setPosition(pos);
		}
		Button(sf::Texture* const& texture, sf::FloatRect const& bounds) {
			sf::Sprite s(*texture);
			_sprite = s;
			_bounds = sf::Rect<float>(sf::Vector2f(0, 0), sf::Vector2f(bounds.width, bounds.height));
			setPosition(bounds.left, bounds.top);
		}
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
			sf::Sprite spr = _sprite;
			spr.setPosition(this->getPosition());
			spr.setRotation(this->getRotation());
			spr.setScale(this->getScale());
			target.draw(spr);
		}
		virtual void handle_event(sf::Vector2f const& mouse_pos, sf::View const& view, Event const& event) {
			if (contains(mouse_pos, view)) retrieve_event(event)();
		}
	};
}