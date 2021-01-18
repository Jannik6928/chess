#pragma once

#include "Point.h"

enum class Type { Replace, Castle, Q_Promotion, R_Promotion, B_Promotion, N_Promotion };
struct Move {
	Point first;
	Point second;

	Type type = Type::Replace;

	Move(){}
	Move(Point f, Point s) {
		first = f;
		second = s;
	}
	Move(int fx, int fy, int sx, int sy) {
		first.x = fx;
		first.y = fy;
		second.x = sx;
		second.y = sy;
	}
	Move(Point f, Point s, Type const& t) {
		first = f;
		second = s;
		type = t;
	}
	Move(int fx, int fy, int sx, int sy, Type const& t) {
		first.x = fx;
		first.y = fy;
		second.x = sx;
		second.y = sy;
		type = t;
	}
};

