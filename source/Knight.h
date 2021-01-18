#pragma once 

#include "Piece.h"

class Knight : public Piece {
private:

public:
	Knight() {
		if (color == Color::White) {
			id = Id::W_Knight;
		}
		else {
			id = Id::B_Knight;
		}
	}
	Knight(Point pos, Color const& color) {
		this->color = color;
		this->pos = pos;

		if (color == Color::White) {
			id = Id::W_Knight;
		}
		else {
			id = Id::B_Knight;
		}
	}
	~Knight() {}

	bool checkMove(Piece*** board, int x, int y, int dx, int dy) {
		if (clamp(pos.x + dx, 0, x - 1) != pos.x + dx) return false;
		if (clamp(pos.y + dy, 0, y - 1) != pos.y + dy) return false;
		if (board[pos.x + dx][pos.y + dy] != nullptr) {
			if (board[pos.x + dx][pos.y + dy]->getColor() == color) return false;
		}
		return true;
	}
	vector<Move> getMoves(Piece*** board, int x, int y) override {
		vector<Move> moves;

		if (checkMove(board, x, y, 2, 1)) moves.push_back(Move(pos.x, pos.y, pos.x + 2, pos.y + 1));
		if (checkMove(board, x, y, 2, -1)) moves.push_back(Move(pos.x, pos.y, pos.x + 2, pos.y - 1));
		if (checkMove(board, x, y, -2, 1)) moves.push_back(Move(pos.x, pos.y, pos.x - 2, pos.y + 1));
		if (checkMove(board, x, y, -2, -1)) moves.push_back(Move(pos.x, pos.y, pos.x - 2, pos.y - 1));

		if (checkMove(board, x, y, 1, 2)) moves.push_back(Move(pos.x, pos.y, pos.x + 1, pos.y + 2));
		if (checkMove(board, x, y, 1, -2)) moves.push_back(Move(pos.x, pos.y, pos.x + 1, pos.y - 2));
		if (checkMove(board, x, y, -1, 2)) moves.push_back(Move(pos.x, pos.y, pos.x - 1, pos.y + 2));
		if (checkMove(board, x, y, -1, -2)) moves.push_back(Move(pos.x, pos.y, pos.x - 1, pos.y - 2));

		return moves;
	}
	bool checkThreat(Piece*** board, int x, int y, int dx, int dy) {
		if (clamp(pos.x + dx, 0, x - 1) != pos.x + dx) return false;
		if (clamp(pos.y + dy, 0, y - 1) != pos.y + dy) return false;
		return true;
	}
	vector<Move> getThreats(Piece*** board, int x, int y) override {
		vector<Move> threats;

		if (checkThreat(board, x, y, 2, 1)) threats.push_back(Move(pos.x, pos.y, pos.x + 2, pos.y + 1));
		if (checkThreat(board, x, y, 2, -1)) threats.push_back(Move(pos.x, pos.y, pos.x + 2, pos.y - 1));
		if (checkThreat(board, x, y, -2, 1)) threats.push_back(Move(pos.x, pos.y, pos.x - 2, pos.y + 1));
		if (checkThreat(board, x, y, -2, -1)) threats.push_back(Move(pos.x, pos.y, pos.x - 2, pos.y - 1));

		if (checkThreat(board, x, y, 1, 2)) threats.push_back(Move(pos.x, pos.y, pos.x + 1, pos.y + 2));
		if (checkThreat(board, x, y, 1, -2)) threats.push_back(Move(pos.x, pos.y, pos.x + 1, pos.y - 2));
		if (checkThreat(board, x, y, -1, 2)) threats.push_back(Move(pos.x, pos.y, pos.x - 1, pos.y + 2));
		if (checkThreat(board, x, y, -1, -2)) threats.push_back(Move(pos.x, pos.y, pos.x - 1, pos.y - 2));

		return threats;
	}

	Knight* clone() override {
		Knight* c = new Knight(pos, color);
		c->timesMoved = timesMoved;
		return c;
	}
};

