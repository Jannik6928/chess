#pragma once

#include <vector>

#include "Button.h"
#include "Element.h"
#include "Event.h"

#include <SFML/Graphics.hpp>

using namespace std;

namespace gui {
	class Menu : public Element {
	protected:
		std::vector<Element*> _elements;
	public:
		Menu() {}
		Menu(std::vector<Element*> const& elements) {
			_elements = elements;
		}
		Menu(std::vector<Element*> const& elements, sf::FloatRect const& bounds) {
			_elements = elements;
			_bounds = bounds;
		}
		Menu(std::string const& path, sf::IntRect const& section) {
			_bounds = sf::FloatRect(section);
			sf::Texture* t = new sf::Texture;
			t->loadFromFile(path, section);
			_sprite = sf::Sprite(*t);
		}
		Menu(std::string const& path, sf::IntRect const& section, sf::Vector2f const& pos) {
			_bounds = sf::FloatRect(section);
			sf::Texture* t = new sf::Texture;
			t->loadFromFile(path, section);
			_sprite = sf::Sprite(*t);
			setPosition(pos);
		}
		Menu(std::vector<Element*> const& elements, std::string const& path, sf::IntRect const& section) {
			_elements = elements;
			_bounds = sf::FloatRect(section);
			sf::Texture* t = new sf::Texture;
			t->loadFromFile(path, section);
			_sprite = sf::Sprite(*t);
		}

		void set_all_hidden(bool const& h) {
			_hidden = h;
			for (auto& e : _elements) {
				e->set_hidden(h);
			}
		}

		std::vector<Element*>& elements() { return _elements; }
		void set_elements(std::vector<Element*> const& elements) { _elements = elements; }
		void insert_element(Element* element) { _elements.push_back(element); }
		void insert_element(int index, Element* element) { _elements.insert(_elements.begin() + index, element); }
		int element_ct() { return _elements.size(); }
		void clear_elements() { _elements = {}; }
		void remove_element(int index) { _elements.erase(_elements.begin() + index); }

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
			if (_hidden) return;
			sf::Sprite s = _sprite;
			s.setPosition(getPosition());
			target.draw(s);
			for (auto& elem : _elements) {
				if (!elem->hidden()) {
					// should apply interface transformations
					sf::RectangleShape r(sf::Vector2f(elem->bounds().width - 20, elem->bounds().height - 20));
					r.setPosition(elem->getPosition() + this->getPosition() + sf::Vector2f(10,10));
					r.setFillColor(elem->color());
					r.setOutlineThickness(10);
					r.setOutlineColor(sf::Color(220, 220, 220));
					target.draw(r);

					sf::Sprite spr = elem->sprite();
					spr.setPosition(this->getPosition() + elem->getPosition());
					spr.setScale(elem->getScale());
					target.draw(spr);
				}
			}
		}
		virtual void handle_event(sf::Vector2f const& mouse_pos, sf::View const& view, Event const& event) override {
			for (auto& elem : _elements) {
				if (!elem->hidden()) elem->handle_event(mouse_pos, view, event);
			}
			sf::Vector2f global_pos(mouse_pos.x + view.getCenter().x - view.getSize().x / 2, mouse_pos.y + view.getCenter().y - view.getSize().y / 2);
			sf::Rect<float> bounds(sf::Vector2f(0, 0), sf::Vector2f(bounds().width, bounds().height));

			sf::Transform tr = this->getTransform();
			bounds = tr.transformRect(bounds);
			if (bounds.contains(global_pos)) retrieve_event(event)();
		}
	};

}