#pragma once

#include "Piece.h"

class Bishop : public Piece {
private:

public:
	Bishop() {
		if (color == Color::White) {
			id = Id::W_Bishop;
		}
		else {
			id = Id::B_Bishop;
		}
	}
	Bishop(Point pos, Color const& color) {
		this->color = color;
		this->pos = pos;

		if (color == Color::White) {
			id = Id::W_Bishop;
		}
		else {
			id = Id::B_Bishop;
		}
	}
	~Bishop(){}

	bool checkMove(Piece*** board, int x, int y, int dx, int dy) {
		if (clamp(pos.x + dx, 0, x - 1) != pos.x + dx) return false;
		if (clamp(pos.y + dy, 0, y - 1) != pos.y + dy) return false;

		if (board[pos.x + dx][pos.y + dy] == nullptr) return true;
		else if (board[pos.x + dx][pos.y + dy]->getColor() != color) return true;

		return false;
	}
	vector<Move> getMoves(Piece*** board, int x, int y) override {
		vector<Move> moves;
		for (int i = 1; i < max(x, y); i++) {
			if (checkMove(board, x, y, i, i)) {
				moves.push_back(Move(pos.x, pos.y, i + pos.x, i + pos.y));
				if (board[i + pos.x][i + pos.y] != nullptr) {
					if (board[i + pos.x][i + pos.y]->getColor() != color) break;
				}
			}
			else break;
		}
		for (int i = 1; i < max(x, y); i++) {
			if (checkMove(board, x, y, i, -i)) {
				moves.push_back(Move(pos.x, pos.y, i + pos.x, -i + pos.y));
				if (board[i + pos.x][-i + pos.y] != nullptr) {
					if (board[i + pos.x][-i + pos.y]->getColor() != color) break;
				}
			}
			else break;
		}
		for (int i = 1; i < max(x, y); i++) {
			if (checkMove(board, x, y, -i, i)) {
				moves.push_back(Move(pos.x, pos.y, -i + pos.x, i + pos.y));
				if (board[-i + pos.x][i + pos.y] != nullptr) {
					if (board[-i + pos.x][i + pos.y]->getColor() != color) break;
				}
			}
			else break;
		}
		for (int i = 1; i < max(x, y); i++) {
			if (checkMove(board, x, y, -i, -i)) {
				moves.push_back(Move(pos.x, pos.y, -i + pos.x, -i + pos.y));
				if (board[-i + pos.x][-i + pos.y] != nullptr) {
					if (board[-i + pos.x][-i + pos.y]->getColor() != color) break;
				}
			}
			else break;
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

		for (int i = 1; i < max(x, y); i++) {
			if (checkThreat(board, x, y, i, i)) {
				threats.push_back(Move(pos.x, pos.y, i + pos.x, i + pos.y));
				if (board[i + pos.x][i + pos.y] != nullptr) break;
			}
			else break;
		}
		for (int i = 1; i < max(x, y); i++) {
			if (checkThreat(board, x, y, i, -i)) {
				threats.push_back(Move(pos.x, pos.y, i + pos.x, -i + pos.y));
				if (board[i + pos.x][-i + pos.y] != nullptr) break;
			}
			else break;
		}
		for (int i = 1; i < max(x, y); i++) {
			if (checkThreat(board, x, y, -i, i)) {
				threats.push_back(Move(pos.x, pos.y, -i + pos.x, i + pos.y));
				if (board[-i + pos.x][i + pos.y] != nullptr) break;
			}
			else break;
		}
		for (int i = 1; i < max(x, y); i++) {
			if (checkThreat(board, x, y, -i, -i)) {
				threats.push_back(Move(pos.x, pos.y, -i + pos.x, -i + pos.y));
				if (board[-i + pos.x][-i + pos.y] != nullptr) break;
			}
			else break;
		}

		return threats;
	}

	Bishop* clone() override {
		Bishop* c = new Bishop(pos, color);
		c->timesMoved = timesMoved;
		return c;
	}
};

