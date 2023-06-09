#include "Player.hpp"

Player::Player() {
	positionX = 0;
	positionY = 0;
	direction = N;
}

Player::~Player() {
}

void Player::moveInDirection(Direction direction) {
	switch (direction) {

		case N:
			positionY--;
			break;

		case E:
			positionX++;
			break;

		case S:
			positionY++;
			break;

		case W:
			positionX--;
			break;
	}
}

int Player::getX() {
	return positionX;
}

int Player::getY() {
	return positionY;
}

Direction Player::getDirection() {
	return direction;
}

void Player::turnRight() {
	direction++;
}

void Player::turnLeft() {
	direction--;
}

void Player::moveForward() {
	moveInDirection(direction);
}

void Player::moveBackward() {
	Direction targetDirection = direction;
	targetDirection++;
	targetDirection++;
	moveInDirection(targetDirection);
}

void Player::moveRight() {
	Direction targetDirection = direction;
	targetDirection++;
	moveInDirection(targetDirection);
}

void Player::moveLeft() {
	Direction targetDirection = direction;
	targetDirection--;
	moveInDirection(targetDirection);
}