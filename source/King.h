#pragma once

#include "Piece.h"

using namespace std;

class King : public Piece {
private:

public:
	King() {
		this->key = true;
		if (color == Color::White) {
			id = Id::W_King;
		}
		else {
			id = Id::B_King;
		}
	}
	King(Point pos, Color const& color) {
		this->color = color;
		this->key = true;
		this->pos = pos;

		if (color == Color::White) {
			id = Id::W_King;
		}
		else {
			id = Id::B_King;
		}
	}
	~King(){}

	bool checkMove(Piece*** board, int x, int y, int dx, int dy) {
		if (clamp(pos.x + dx, 0, x - 1) != pos.x + dx) return false;
		if (clamp(pos.y + dy, 0, y - 1) != pos.y + dy) return false;

		if (board[pos.x + dx][pos.y + dy] == nullptr) return true;
		else if (board[pos.x + dx][pos.y + dy]->getColor() != color) return true;

		return false;
	}
	vector<Move> getMoves(Piece*** board, int x, int y) override {
		vector<Move> moves;
		if (checkMove(board, x, y, 1, 0)) moves.push_back(Move(pos.x, pos.y, pos.x + 1, pos.y));
		if (checkMove(board, x, y, 1, 1)) moves.push_back(Move(pos.x, pos.y, pos.x + 1, pos.y + 1));
		if (checkMove(board, x, y, 0, 1)) moves.push_back(Move(pos.x, pos.y, pos.x, pos.y + 1));
		if (checkMove(board, x, y, -1, 1)) moves.push_back(Move(pos.x, pos.y, pos.x - 1, pos.y + 1));
		if (checkMove(board, x, y, -1, 0)) moves.push_back(Move(pos.x, pos.y, pos.x - 1, pos.y));
		if (checkMove(board, x, y, -1, -1)) moves.push_back(Move(pos.x, pos.y, pos.x - 1, pos.y - 1));
		if (checkMove(board, x, y, 0, -1)) moves.push_back(Move(pos.x, pos.y, pos.x, pos.y - 1));
		if (checkMove(board, x, y, 1, -1)) moves.push_back(Move(pos.x, pos.y, pos.x + 1, pos.y - 1));
		return moves;
	}
	bool checkThreat(Piece*** board, int x, int y, int dx, int dy) {
		if (clamp(pos.x + dx, 0, x - 1) != pos.x + dx) return false;
		if (clamp(pos.y + dy, 0, y - 1) != pos.y + dy) return false;
		return true;
	}
	vector<Move> getThreats(Piece*** board, int x, int y) override {
		vector<Move> threats;
		if (checkThreat(board, x, y, 1, 0)) threats.push_back(Move(pos.x, pos.y, pos.x + 1, pos.y));
		if (checkThreat(board, x, y, 1, 1)) threats.push_back(Move(pos.x, pos.y, pos.x + 1, pos.y + 1));
		if (checkThreat(board, x, y, 0, 1)) threats.push_back(Move(pos.x, pos.y, pos.x, pos.y + 1));
		if (checkThreat(board, x, y, -1, 1)) threats.push_back(Move(pos.x, pos.y, pos.x - 1, pos.y + 1));
		if (checkThreat(board, x, y, -1, 0)) threats.push_back(Move(pos.x, pos.y, pos.x - 1, pos.y));
		if (checkThreat(board, x, y, -1, -1)) threats.push_back(Move(pos.x, pos.y, pos.x - 1, pos.y - 1));
		if (checkThreat(board, x, y, 0, -1)) threats.push_back(Move(pos.x, pos.y, pos.x, pos.y - 1));
		if (checkThreat(board, x, y, 1, -1)) threats.push_back(Move(pos.x, pos.y, pos.x + 1, pos.y - 1));
		return threats;
	}

	King* clone() override {
		King* c = new King(pos, color);
		c->timesMoved = timesMoved;
		c->key = true;
		return c;
	}
};

