#pragma once

#include <functional>
#include <map>
#include <iostream>

#include "Event.h"

#include <SFML/Graphics.hpp>

using namespace std;

namespace gui {
	// has graphical properties: sprite, texture, and color
	// provides event handling
	// executes bound functions when a specific event occurs
	class Element : public sf::Transformable, public sf::Drawable {
	protected:
		sf::Sprite _sprite;
		sf::Rect<float> _bounds;

		sf::Color _color;

		std::map<Event, std::function<void()>> _event_executions;
		std::function<void()> _default_execution = []() {};

		bool _hidden = 0;
	public:
		Element() {}
		Element(sf::Rect<float> const& bounds) {
			_bounds = bounds;
		}
		~Element() {

		}

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;

		bool contains(sf::Vector2f const& mouse_pos, sf::View const& view) {
			sf::Vector2f global_pos(mouse_pos.x + view.getCenter().x - view.getSize().x / 2, mouse_pos.y + view.getCenter().y - view.getSize().y / 2);
			sf::Rect<float> bounds(sf::Vector2f(0, 0), sf::Vector2f(_bounds.width, _bounds.height));
			bounds = this->getTransform().transformRect(bounds);
			return bounds.contains(global_pos);
		}
		virtual void handle_event(sf::Vector2f const& mouse_pos, sf::View const& view, Event const& event) = 0;
		void set_event_executions(std::map<Event, std::function<void()>> const& events) { _event_executions = events; }
		std::map<Event, std::function<void()>>& event_executions() { return _event_executions; }
		void assign_event(Event const& event, std::function<void()> const& func) { _event_executions.insert_or_assign(event, func); };
		std::function<void()>& retrieve_event(Event const& event) {
			if (_event_executions.count(event) != 0) return _event_executions.at(event);
			return _default_execution;
		}

		void set_sprite(sf::Sprite const& spr) { _sprite = spr; }
		void set_bounds(sf::Rect<float> const& bounds) { _bounds = bounds; }

		void set_texture(sf::Texture* const& texture) {
			sf::Texture* t = texture;
			_sprite.setTexture(*t);
		}
		void set_color(sf::Color const& color) { _color = color; }

		sf::Sprite& sprite() { return _sprite; }
		sf::Rect<float> const& bounds() const { return _bounds; }

		const sf::Texture* texture() { return _sprite.getTexture(); }
		sf::Color& color() { return _color; }

		bool hidden() { return _hidden; }
		void set_hidden(bool const& hidden) { _hidden = hidden; }
	};
	class Text : public Element {
	private:
		sf::Text text;
	public:
		Text(std::string const& filePath, sf::IntRect const& section, sf::Vector2f const& pos) {
			sf::Texture* t = new sf::Texture;
			t->loadFromFile(filePath, section);
			sf::Sprite s(*t);
			_sprite = s;
			_bounds = sf::Rect<float>(sf::Vector2f(0, 0), sf::Vector2f(section.width, section.height));
			setPosition(pos);
		}
		void setText(string& text) {
			this->text.setString(text);
		}
		sf::Text& getText() { return text; }
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
			sf::Sprite spr = _sprite;
			spr.setPosition(this->getPosition());
			spr.setRotation(this->getRotation());
			spr.setScale(this->getScale());
			target.draw(spr);

			sf::Text t = text;
			t.setPosition(this->getPosition().x + t.getPosition().x, this->getPosition().y + t.getPosition().y);
			t.setRotation(this->getRotation());
			t.setScale(this->getScale());
			target.draw(t);
		}
		virtual void handle_event(sf::Vector2f const& mouse_pos, sf::View const& view, Event const& event) {
			if (contains(mouse_pos, view)) retrieve_event(event)();
		}
	};
}

