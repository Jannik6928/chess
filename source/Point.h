#pragma once

struct Point {
	int x = 0;
	int y = 0;

	Point(){}
	Point(int x, int y) {
		this->x = x;
		this->y = y;
	}

	bool operator==(Point const& p) {
		return (x == p.x && y == p.y);
	}
};

