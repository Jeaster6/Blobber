#pragma once

#include "Direction.hpp"

class Player {

	private:
		int positionX;
		int positionY;
		Direction direction;

	public:
		Player();
		~Player();

		int getX();
		int getY();
		Direction getDirection();
		void turnRight();
		void turnLeft();
		void move(Direction);
};