#pragma once

#include "Piece.h"

class Pawn : public Piece {
private:

public:
	Pawn() {
		if (color == Color::White) {
			id = Id::W_Pawn;
		}
		else {
			id = Id::B_Pawn;
		}
	}
	Pawn(Point pos, Color const& color) {
		this->color = color;
		this->pos = pos;

		if (color == Color::White) {
			id = Id::W_Pawn;
		}
		else {
			id = Id::B_Pawn;
		}
	}
	~Pawn() {}

	bool checkMove(Piece*** board, int x, int y, int dx, int dy) {
		if (clamp(pos.x + dx, 0, x - 1) != pos.x + dx) return false;
		if (clamp(pos.y + dy, 0, y - 1) != pos.y + dy) return false;

		if (dx == 0) {
			if (board[pos.x + dx][pos.y + dy] != nullptr) return false;
		}
		else {
			if (board[pos.x + dx][pos.y + dy] == nullptr) return false;
			if (board[pos.x + dx][pos.y + dy]->getColor() == color) return false;
		}
		return true;
	}
	vector<Move> getMoves(Piece*** board, int x, int y) override {
		vector<Move> moves;

		if (color == Color::White) {
			if (pos.y == y - 2) {
				if (checkMove(board, x, y, 0, 1)) {
					moves.push_back(Move(pos.x, pos.y, pos.x, pos.y + 1, Type::Q_Promotion));
					moves.push_back(Move(pos.x, pos.y, pos.x, pos.y + 1, Type::R_Promotion));
					moves.push_back(Move(pos.x, pos.y, pos.x, pos.y + 1, Type::B_Promotion));
					moves.push_back(Move(pos.x, pos.y, pos.x, pos.y + 1, Type::N_Promotion));
				}
				if (checkMove(board, x, y, 1, 1)) {
					moves.push_back(Move(pos.x, pos.y, pos.x + 1, pos.y + 1, Type::Q_Promotion));
					moves.push_back(Move(pos.x, pos.y, pos.x + 1, pos.y + 1, Type::R_Promotion));
					moves.push_back(Move(pos.x, pos.y, pos.x + 1, pos.y + 1, Type::B_Promotion));
					moves.push_back(Move(pos.x, pos.y, pos.x + 1, pos.y + 1, Type::N_Promotion));
				}
				if (checkMove(board, x, y, -1, 1)) {
					moves.push_back(Move(pos.x, pos.y, pos.x - 1, pos.y + 1, Type::Q_Promotion));
					moves.push_back(Move(pos.x, pos.y, pos.x - 1, pos.y + 1, Type::R_Promotion));
					moves.push_back(Move(pos.x, pos.y, pos.x - 1, pos.y + 1, Type::B_Promotion));
					moves.push_back(Move(pos.x, pos.y, pos.x - 1, pos.y + 1, Type::N_Promotion));
				}
			}
			else {
				if (checkMove(board, x, y, 0, 1)) {
					moves.push_back(Move(pos.x, pos.y, pos.x, pos.y + 1));
					if (timesMoved == 0 && checkMove(board, x, y, 0, 2)) {
						moves.push_back(Move(pos.x, pos.y, pos.x, pos.y + 2));
					}
				}
				if (checkMove(board, x, y, 1, 1)) moves.push_back(Move(pos.x, pos.y, pos.x + 1, pos.y + 1));
				if (checkMove(board, x, y, -1, 1)) moves.push_back(Move(pos.x, pos.y, pos.x - 1, pos.y + 1));
			}
		}
		else {
			if (pos.y == 1) {
				if (checkMove(board, x, y, 0, -1)) {
					moves.push_back(Move(pos.x, pos.y, pos.x, pos.y - 1, Type::Q_Promotion));
					moves.push_back(Move(pos.x, pos.y, pos.x, pos.y - 1, Type::R_Promotion));
					moves.push_back(Move(pos.x, pos.y, pos.x, pos.y - 1, Type::B_Promotion));
					moves.push_back(Move(pos.x, pos.y, pos.x, pos.y - 1, Type::N_Promotion));
				}
				if (checkMove(board, x, y, 1, -1)) {
					moves.push_back(Move(pos.x, pos.y, pos.x + 1, pos.y - 1, Type::Q_Promotion));
					moves.push_back(Move(pos.x, pos.y, pos.x + 1, pos.y - 1, Type::R_Promotion));
					moves.push_back(Move(pos.x, pos.y, pos.x + 1, pos.y - 1, Type::B_Promotion));
					moves.push_back(Move(pos.x, pos.y, pos.x + 1, pos.y - 1, Type::N_Promotion));
				}
				if (checkMove(board, x, y, -1, -1)) {
					moves.push_back(Move(pos.x, pos.y, pos.x - 1, pos.y - 1, Type::Q_Promotion));
					moves.push_back(Move(pos.x, pos.y, pos.x - 1, pos.y - 1, Type::R_Promotion));
					moves.push_back(Move(pos.x, pos.y, pos.x - 1, pos.y - 1, Type::B_Promotion));
					moves.push_back(Move(pos.x, pos.y, pos.x - 1, pos.y - 1, Type::N_Promotion));
				}
			}
			else {
				if (checkMove(board, x, y, 0, -1)) {
					moves.push_back(Move(pos.x, pos.y, pos.x, pos.y - 1));
					if (timesMoved == 0 && checkMove(board, x, y, 0, -2)) {
						moves.push_back(Move(pos.x, pos.y, pos.x, pos.y - 2));
					}
				}
				if (checkMove(board, x, y, 1, -1)) moves.push_back(Move(pos.x, pos.y, pos.x + 1, pos.y - 1));
				if (checkMove(board, x, y, -1, -1)) moves.push_back(Move(pos.x, pos.y, pos.x - 1, pos.y - 1));
			}
		}

		return moves;
	}
	bool checkThreat(Piece*** board, int x, int y, int dx, int dy) {
		if (clamp(pos.x + dx, 0, x - 1) != pos.x + dx) return false;
		if (clamp(pos.y + dy, 0, y - 1) != pos.y + dy) return false;
		return true;
	}
	vector<Move> getThreats(Piece*** board, int x, int y) override {
		vector<Move> threats;
		if (color == Color::White) {
			if (checkThreat(board, x, y, 1, 1)) threats.push_back(Move(pos.x, pos.y, pos.x + 1, pos.y + 1));
			if (checkThreat(board, x, y, -1, 1)) threats.push_back(Move(pos.x, pos.y, pos.x - 1, pos.y + 1));
		}
		else {
			if (checkThreat(board, x, y, 1, -1)) threats.push_back(Move(pos.x, pos.y, pos.x + 1, pos.y - 1));
			if (checkThreat(board, x, y, -1, -1)) threats.push_back(Move(pos.x, pos.y, pos.x - 1, pos.y - 1));
		}
		return threats;
	}

	Pawn* clone() override {
		Pawn* c = new Pawn(pos, color);
		c->timesMoved = timesMoved;
		return c;
	}
};

