#pragma once

#include <vector>
#include <iostream>
#include <algorithm>

#include "Move.h"

using namespace std;

enum class Color { White, Black };
enum class Id {W_Pawn, W_Knight, W_Bishop, W_Rook, W_Queen, W_King, B_Pawn, B_Knight, B_Bishop, B_Rook, B_Queen, B_King, Missing};
class Piece {
protected:
	Color color = Color::White;
	bool key = false;
	Point pos;
	int timesMoved = 0;
	Id id = Id::Missing;
public:
	Piece(){}
	Piece(Color const& color, bool k) {
		this->color = color;
		this->key = k;
	}
	~Piece() {
	}

	Color& getColor() { return color; }
	bool isKey() { return key; }
	int getTimesMoved() { return timesMoved; }
	Point getPos() { return pos; }
	Id getId() { return id; }

	void setColor(Color const& color) { this->color = color; }
	void setKey(bool k) { key = k; }
	void setTimesMoved(int t) { timesMoved = t; }
	void setPos(Point const& p) { pos = p; }
	
	virtual vector<Move> getThreats(Piece*** board, int x, int y) = 0;
	virtual vector<Move> getMoves(Piece*** board, int x, int y) = 0;
	virtual Piece* clone() = 0;
};

