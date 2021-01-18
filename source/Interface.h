#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>

#include "Menu.h"
#include "Position.h"

using namespace std;

// implements chess position as a drawable object for sfml

class Interface : public sf::Drawable {
private:
	Position* position = nullptr;
	map<Id, sf::Texture*>* textures = nullptr;

	bool selected = false;
	Point selectedPos;

	bool doFlip = false;
	bool flipped = true;

	gui::Menu* promotionMenu = nullptr;
	Id promotion = Id::Missing;
	bool paused = false;
	
public:
	Interface() {}
	Interface(Position* p, map<Id, sf::Texture*>* t) {
		position = p;
		textures = t;

		gui::Button* knight = new gui::Button(textures->at(Id::W_Knight), sf::FloatRect(0, 0, 100, 100));
		knight->setScale(1.5, 1.5);
		knight->set_color(sf::Color(250, 218, 94));
		gui::Button* bishop = new gui::Button(textures->at(Id::W_Bishop), sf::FloatRect(0, 100, 100, 100));
		bishop->setScale(1.5, 1.5);
		bishop->set_color(sf::Color(250, 218, 94));
		gui::Button* rook = new gui::Button(textures->at(Id::W_Rook), sf::FloatRect(0, 200, 100, 100));
		rook->setScale(1.5, 1.5);
		rook->set_color(sf::Color(250, 218, 94));
		gui::Button* queen = new gui::Button(textures->at(Id::W_Queen), sf::FloatRect(0, 300, 100, 100));
		queen->setScale(1.5, 1.5);
		queen->set_color(sf::Color(250, 218, 94));

		promotionMenu = new gui::Menu({ knight, bishop, rook, queen });
		promotionMenu->setPosition(p->getX() * 100 + 50, 200);
		promotionMenu->set_hidden(true);
	}
	~Interface() {
		delete position;
		for (auto& [key, val] : *textures) {
			delete val;
		}
		delete textures;
		delete promotionMenu;
	}

	void toggleSelected(Point s) {
		if (!(clamp(s.x, 0, position->getX() - 1) == s.x) || !(clamp(s.y, 0, position->getY() - 1) == s.y)) return;
		if (selected == false) {
			selected = true;
			selectedPos = s;
		}
		else {
			if (s == selectedPos) selected = false;
			else selectedPos = s;
		}
	}
	void toggleFlip() {
		if (doFlip) flipped = !flipped;
	}
	void tryMove(Point s) {
		if (position == nullptr) return;
		if (selected) {

			if (position->at(selectedPos) == nullptr) return;
			if (position->at(selectedPos)->getColor() != position->getTurn()) return;

			vector<Move> moves;

			for (auto& move : position->getLegalMoves()) {
				if (s == move.second && selectedPos == move.first) {
					moves.push_back(Move(selectedPos, s, move.type));
				}
			}

			// normal move
			if (moves.size() == 1) {
				Position* np = position->makeMove(Move(selectedPos, s, moves[0].type));
				delete position;
				position = np;

				toggleSelected(s);
				toggleFlip();
			}
			// brings up interface (for promotions)
			else if (moves.size() > 1) {
				//promotionMenu->set_hidden(false);
				paused = true;

				Position* np = position->makeMove(Move(selectedPos, s, moves[0].type));
				delete position;
				position = np;

				toggleSelected(s);
				toggleFlip();
			}

		}
	}
	void tryInterface(sf::Vector2f const& mousePos) {

	}

	map<Id, sf::Texture*>*& getTextures() { return textures; }
	Position* getPosition() { return position; }
	bool isFlipped() { return flipped; }

	void setPosition(Position* const& p) { position = p; }
	void setDoFlip(bool f) { doFlip = f; }

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
		bool dark = 1;
		for (int i = 0; i < position->getX(); i++) {
			for (int j = 0; j < position->getY(); j++) {
				int newY = (flipped) ? position->getY() - j - 1 : j;

				sf::RectangleShape box(sf::Vector2f(100, 100));
				if (dark) {
					box.setFillColor(sf::Color::Red);
				}
				else {
					box.setFillColor(sf::Color::White);
				}
				box.setPosition(i * 100, newY * 100);
				target.draw(box);

				Piece* p = position->at(Point(i, j));
				if (p != nullptr) {
					sf::Sprite s;
					s.setTexture(*textures->at(p->getId()));

					s.setScale(1.5, 1.5);
					s.setPosition(100 * i, 100 * newY);
					target.draw(s);
				}

				dark = !dark;
			}
			dark = !dark;
		}

		if (selected) {
			sf::RectangleShape selectedHighlight(sf::Vector2f(80, 80));
			selectedHighlight.setFillColor(sf::Color(0, 0, 0, 0));
			selectedHighlight.setOutlineThickness(10);
			selectedHighlight.setOutlineColor(sf::Color::Blue);

			if (flipped)
				selectedHighlight.setPosition(100 * selectedPos.x + 10, 100 * (position->getY() - selectedPos.y - 1) + 10);
			else 
				selectedHighlight.setPosition(100 * selectedPos.x + 10, 100 * selectedPos.y + 10);

			target.draw(selectedHighlight);
			for (auto& move : position->getLegalMoves()) {
				// checks that only the highlighted position's moves are shown
				if (move.first == selectedPos) {
					// checks that the turn is the highlighted position's color
					if (position->at(selectedPos)->getColor() == position->getTurn()) {
						if (position->at(move.second) == nullptr)
							selectedHighlight.setOutlineColor(sf::Color(173, 216, 230));
						else
							selectedHighlight.setOutlineColor(sf::Color(102, 51, 153));
						if (move.type == Type::Castle) selectedHighlight.setOutlineColor(sf::Color(128, 128, 0));
						else if (move.type == Type::N_Promotion) selectedHighlight.setOutlineColor(sf::Color(250, 218, 94));

						if (flipped)
							selectedHighlight.setPosition(100 * move.second.x + 10, 100 * (position->getY() - move.second.y - 1) + 10);
						else 
							selectedHighlight.setPosition(100 * move.second.x + 10, 100 * move.second.y + 10);

						target.draw(selectedHighlight);
					}
				}
				
			}
		}

		target.draw(*promotionMenu);
	}
};

static map<Id, sf::Texture*>* standardTextures() {
	map<Id, sf::Texture*>* st = new map<Id, sf::Texture*>;

	sf::Texture* W_P = new sf::Texture;
	W_P->loadFromFile("./sprites/pieces/w_pawn.png", sf::IntRect(0, 0, 100, 100));
	st->insert(pair<Id, sf::Texture*>(Id::W_Pawn, W_P));
	sf::Texture* W_N = new sf::Texture;
	W_N->loadFromFile("./sprites/pieces/w_knight.png", sf::IntRect(0, 0, 100, 100));
	st->insert(pair<Id, sf::Texture*>(Id::W_Knight, W_N));
	sf::Texture* W_B = new sf::Texture;
	W_B->loadFromFile("./sprites/pieces/w_bishop.png", sf::IntRect(0, 0, 100, 100));
	st->insert(pair<Id, sf::Texture*>(Id::W_Bishop, W_B));
	sf::Texture* W_R = new sf::Texture;
	W_R->loadFromFile("./sprites/pieces/w_rook.png", sf::IntRect(0, 0, 100, 100));
	st->insert(pair<Id, sf::Texture*>(Id::W_Rook, W_R));
	sf::Texture* W_Q = new sf::Texture;
	W_Q->loadFromFile("./sprites/pieces/w_queen.png", sf::IntRect(0, 0, 100, 100));
	st->insert(pair<Id, sf::Texture*>(Id::W_Queen, W_Q));
	sf::Texture* W_K = new sf::Texture;
	W_K->loadFromFile("./sprites/pieces/w_king.png", sf::IntRect(0, 0, 100, 100));
	st->insert(pair<Id, sf::Texture*>(Id::W_King, W_K));

	sf::Texture* B_P = new sf::Texture;
	B_P->loadFromFile("./sprites/pieces/b_pawn.png", sf::IntRect(0, 0, 100, 100));
	st->insert(pair<Id, sf::Texture*>(Id::B_Pawn, B_P));
	sf::Texture* B_N = new sf::Texture;
	B_N->loadFromFile("./sprites/pieces/b_knight.png", sf::IntRect(0, 0, 100, 100));
	st->insert(pair<Id, sf::Texture*>(Id::B_Knight, B_N));
	sf::Texture* B_B = new sf::Texture;
	B_B->loadFromFile("./sprites/pieces/b_bishop.png", sf::IntRect(0, 0, 100, 100));
	st->insert(pair<Id, sf::Texture*>(Id::B_Bishop, B_B));
	sf::Texture* B_R = new sf::Texture;
	B_R->loadFromFile("./sprites/pieces/b_rook.png", sf::IntRect(0, 0, 100, 100));
	st->insert(pair<Id, sf::Texture*>(Id::B_Rook, B_R));
	sf::Texture* B_Q = new sf::Texture;
	B_Q->loadFromFile("./sprites/pieces/b_queen.png", sf::IntRect(0, 0, 100, 100));
	st->insert(pair<Id, sf::Texture*>(Id::B_Queen, B_Q));
	sf::Texture* B_K = new sf::Texture;
	B_K->loadFromFile("./sprites/pieces/b_king.png", sf::IntRect(0, 0, 100, 100));
	st->insert(pair<Id, sf::Texture*>(Id::B_King, B_K));

	return st;
}