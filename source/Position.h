#pragma once

#include <array>
#include <iostream>
#include <map>
#include <set>

#include "Piece.h"
#include "Pawn.h"
#include "Knight.h"
#include "Bishop.h"
#include "Rook.h"
#include "Queen.h"
#include "King.h"

using namespace std;

class Position {
private:
	int x = 8;
	int y = 8;

	Piece*** board = nullptr;
	pair<set<Piece*>, set<Piece*>>** threats = nullptr;
	Piece*** blocks = nullptr;

	Color turn = Color::White;
public:
	Position(){}
	Position(int x, int y) {
		this->x = x;
		this->y = y;

		board = new Piece**[x];
		for (int i = 0; i < x; i++) {
			board[i] = new Piece * [y] {nullptr};
		}

		threats = new pair<set<Piece*>, set<Piece*>>*[x];
		for (int i = 0; i < x; i++) {
			threats[i] = new pair<set<Piece*>, set<Piece*>>[y];
			for (int j = 0; j < y; j++) {
				threats[i][j] = pair<set<Piece*>, set<Piece*>>({}, {});
			}
		}

		resetThreats();
	}
	~Position() {

		for (int i = 0; i < x; i++) {
			for (int j = 0; j < y; j++) {
				if (board[i][j] != nullptr) {
					delete board[i][j];
				}
			}
			delete[] board[i];
		}
		delete[] board;

		for (int i = 0; i < x; i++) {
			delete[] threats[i];
		}
		delete[] threats;

	}

	int getX() { return x; }
	int getY() { return y; }
	Piece*** getBoard() { return board; }
	Color getTurn() { return turn; }

	void setTurn(Color turn) { this->turn = turn; }

	Piece*& at(Point p) { return board[p.x][p.y]; }
	void setSpot(int x, int y, Piece* pc) { board[x][y] = pc; }

	bool checkLegal(Move m) const {
		Position* nextPos = makeMove(m);
		for (int i = 0; i < x; i++) {
			for (int j = 0; j < y; j++) {
				// check threats array for threats on key piece from enemy color
				if (nextPos->board[i][j] != nullptr) {
					if (nextPos->board[i][j]->isKey() && nextPos->board[i][j]->getColor() == turn) {
						if (nextPos->board[i][j]->getColor() == Color::White) {
							if (!nextPos->threats[i][j].second.empty()) {
								delete nextPos;
								return false;
							}
						}
						else {
							if (!nextPos->threats[i][j].first.empty()) {
								delete nextPos;
								return false;
							}
						}
					}
				}
			}
		}
		delete nextPos;
		return true;
	}

	Position* makeMove(Move m) const {
		Position* np = new Position(x, y);
		for (int i = 0; i < x; i++) {
			for (int j = 0; j < y; j++) {
				if (board[i][j] == nullptr) np->board[i][j] = nullptr;
				else {
					np->board[i][j] = board[i][j]->clone();
				}
			}
		}
		for (int i = 0; i < x; i++) {
			for (int j = 0; j < y; j++) {
				np->threats[i][j] = threats[i][j];
			}
		}

		np->turn = (turn == Color::Black) ? Color::White : Color::Black;
		if (m.type == Type::Replace || m.type == Type::Q_Promotion || m.type == Type::R_Promotion || m.type == Type::B_Promotion || m.type == Type::N_Promotion) {
			if (np->at(m.second) != nullptr) delete np->at(m.second);

			np->at(m.second) = np->at(m.first);
			np->at(m.second)->setPos(m.second);
			np->at(m.second)->setTimesMoved(np->at(m.second)->getTimesMoved() + 1);
			np->at(m.first) = nullptr;

			// handles pawn promotion
			if (m.type == Type::Q_Promotion) {
				delete np->at(m.second);
				np->at(m.second) = new Queen(m.second, turn);
			}
			else if (m.type == Type::R_Promotion) {
				delete np->at(m.second);
				np->at(m.second) = new Rook(m.second, turn);
			}
			else if (m.type == Type::B_Promotion) {
				delete np->at(m.second);
				np->at(m.second) = new Bishop(m.second, turn);
			}
			else if (m.type == Type::N_Promotion) {
				delete np->at(m.second);
				np->at(m.second) = new Knight(m.second, turn);
			}
		}
		else if (m.type == Type::Castle) {
			swap(np->at(m.first), np->at(m.second)); // swaps king and empty square
			np->at(m.second)->setPos(m.second); // updates king position
			np->at(m.second)->setTimesMoved(np->at(m.second)->getTimesMoved() + 1); // iterates king times moved

			// swaps rooks, updates times moved, updates positions
			if (m.second == Point(2, 0)) {
				np->at(Point(0,0))->setTimesMoved(np->at(Point(0,0))->getTimesMoved() + 1);
				swap(np->at(Point(0, 0)), np->at(Point(3, 0)));
				np->at(Point(3, 0))->setPos(Point(3, 0));
			}
			else if (m.second == Point(6, 0)) {
				np->at(Point(7, 0))->setTimesMoved(np->at(Point(7, 0))->getTimesMoved() + 1);
				swap(np->at(Point(7, 0)), np->at(Point(5, 0)));
				np->at(Point(5, 0))->setPos(Point(5, 0));
			}
			else if (m.second == Point(2, 7)) {
				np->at(Point(0, 7))->setTimesMoved(np->at(Point(0, 7))->getTimesMoved() + 1);
				swap(np->at(Point(0, 7)), np->at(Point(3, 7)));
				np->at(Point(3, 7))->setPos(Point(3, 7));
			}
			else if (m.second == Point(6, 7)) {
				np->at(Point(7, 7))->setTimesMoved(np->at(Point(7, 7))->getTimesMoved() + 1);
				swap(np->at(Point(7, 7)), np->at(Point(5, 7)));
				np->at(Point(5, 7))->setPos(Point(5, 7));
			}
		}
		
		// resets threats, could use optimization
		np->resetThreats();
		return np;
	}

	vector<Move> getMoves() const  {
		vector<Move> moves;
		for (int i = 0; i < x; i++) {
			for (int j = 0; j < y; j++) {
				if (board[i][j] != nullptr) {
					if (board[i][j]->getColor() == turn) {
						vector<Move> m = board[i][j]->getMoves(board, x, y);
						moves.insert(moves.end(), m.begin(), m.end());
					}
				}
			}
		}
		return moves;
	}
	vector<Move> getLegalMoves() const {
		vector<Move> legalMoves = getMoves();

		// prunes moves that are illegal
		auto it = legalMoves.begin();
		while (it != legalMoves.end()) {
			if (!checkLegal(*it)) {
				it = legalMoves.erase(it);
			}
			else {
				++it;
			}
		}
		
		// adds castling as a move option
		if (turn == Color::White) {
			if (board[4][0] != nullptr) {
				if (!threats[4][0].second.size()) {
					if (board[0][0] != nullptr) {
						bool castle = true;
						if (board[4][0]->getTimesMoved() == 0 && board[0][0]->getTimesMoved() == 0) {
							if (threats[3][0].second.size() || board[3][0] != nullptr) castle = false;
							if (threats[2][0].second.size() || board[2][0] != nullptr) castle = false;
						}
						else castle = false;
						if (castle) legalMoves.push_back(Move(4, 0, 2, 0, Type::Castle));
					}
					if (board[7][0] != nullptr) {
						bool castle = true;
						if (board[4][0]->getTimesMoved() == 0 && board[7][0]->getTimesMoved() == 0) {
							if (threats[5][0].second.size() || board[5][0] != nullptr) castle = false;
							if (threats[6][0].second.size() || board[6][0] != nullptr) castle = false;
						}
						else castle = false;
						if (castle) legalMoves.push_back(Move(4, 0, 6, 0, Type::Castle));
					}
				}
			}
		}
		else {
			// checks the king square is not empty
			if (board[4][7] != nullptr) {
				// checks the king is not in check
				if (!threats[4][7].first.size()) {
					// queenside castle
					if (board[0][7] != nullptr) {
						bool castle = true;
						if (board[4][7]->getTimesMoved() == 0 && board[0][7]->getTimesMoved() == 0) {
							if (threats[3][7].first.size() || board[3][7] != nullptr) castle = false;
							if (threats[2][7].first.size() || board[2][7] != nullptr) castle = false;
						}
						else castle = false;
						if (castle) legalMoves.push_back(Move(4, 7, 2, 7, Type::Castle));
					}
					// kingside castle
					if (board[7][7] != nullptr) {
						bool castle = true;
						if (board[4][7]->getTimesMoved() == 0 && board[7][7]->getTimesMoved() == 0) {
							if (threats[5][7].first.size() || board[5][7] != nullptr) castle = false;
							if (threats[6][7].first.size() || board[6][7] != nullptr) castle = false;
						}
						else castle = false;
						if (castle) legalMoves.push_back(Move(4, 7, 6, 7, Type::Castle));
					}
				}
			}
		}

		// adds en passant as a move option (will add later)

		return legalMoves;
	}

	void resetThreats() {
		int t = 0;
		for (int i = 0; i < x; i++) {
			for (int j = 0; j < y; j++) {

				// clears threat sets
				threats[i][j].first.clear();
				threats[i][j].second.clear();

			}
		}
		for (int i = 0; i < x; i++) {
			for (int j = 0; j < y; j++) {

				// if the piece is not empty, then process threats
				if (board[i][j] != nullptr) {
					vector<Move> t = board[i][j]->getThreats(board, x, y);
					if (board[i][j]->getColor() == Color::White) {
						for (auto& move : t) {
							// threatened by a white piece, only if the move is a replace move
							if (move.type == Type::Replace) threats[move.second.x][move.second.y].first.insert(board[i][j]);
						}
					}
					else {
						for (auto& move : t) {
							// threatened by a black piece, only if the move is a replace move
							if (move.type == Type::Replace) threats[move.second.x][move.second.y].second.insert(board[i][j]);
						}
					}
				}

			}
		}
	}

};

static Position* standardSetup() {
	Position* pos = new Position(8, 8);

	Piece* W_R1 = new Rook(Point(0,0), Color::White);
	pos->setSpot(0, 0, W_R1);
	
	Piece* W_N1 = new Knight(Point(1, 0), Color::White);
	pos->setSpot(1, 0, W_N1);

	Piece* W_B1 = new Bishop(Point(2, 0), Color::White);
	pos->setSpot(2, 0, W_B1);

	Piece* W_Q1 = new Queen(Point(3, 0), Color::White);
	pos->setSpot(3, 0, W_Q1);

	Piece* W_K1 = new King(Point(4, 0), Color::White);
	pos->setSpot(4, 0, W_K1);

	Piece* W_B2 = new Bishop(Point(5, 0), Color::White);
	pos->setSpot(5, 0, W_B2);

	Piece* W_N2 = new Knight(Point(6, 0), Color::White);
	pos->setSpot(6, 0, W_N2);

	Piece* W_R2 = new Rook(Point(7, 0), Color::White);
	pos->setSpot(7, 0, W_R2);

	for (int i = 0; i < 8; i++) {
		Piece* W_P = new Pawn(Point(i, 1), Color::White);
		pos->setSpot(i, 1, W_P);
	}

	Piece* B_R1 = new Rook(Point(0, 7), Color::Black);
	pos->setSpot(0, 7, B_R1);

	Piece* B_N1 = new Knight(Point(1, 7), Color::Black);
	pos->setSpot(1, 7, B_N1);

	Piece* B_B1 = new Bishop(Point(2, 7), Color::Black);
	pos->setSpot(2, 7, B_B1);

	Piece* B_Q1 = new Queen(Point(3, 7), Color::Black);
	pos->setSpot(3, 7, B_Q1);

	Piece* B_K1 = new King(Point(4, 7), Color::Black);
	pos->setSpot(4, 7, B_K1);

	Piece* B_B2 = new Bishop(Point(5, 7), Color::Black);
	pos->setSpot(5, 7, B_B2);

	Piece* B_N2 = new Knight(Point(6, 7), Color::Black);
	pos->setSpot(6, 7, B_N2);

	Piece* B_R2 = new Rook(Point(7, 7), Color::Black);
	pos->setSpot(7, 7, B_R2);

	for (int i = 0; i < 8; i++) {
		Piece* B_P = new Pawn(Point(i, 6), Color::Black);
		pos->setSpot(i, 6, B_P);
	}
	
	return pos;
}


