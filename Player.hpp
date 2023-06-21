#pragma once

#include "Direction.hpp"

class Player {

	private:
		int positionX;
		int positionY;
		Direction direction;

		void moveInDirection(Direction);

	public:
		Player();
		Player(int, int, Direction);
		~Player();

		int getX();
		int getY();
		Direction getDirection();
		void turnRight();
		void turnLeft();
		void moveForward();
		void moveBackward();
		void moveRight();
		void moveLeft();
		void teleportToCoordinates(int, int);
};