#include "Player.hpp"

Player::Player() {
	positionX = 0;
	positionY = 0;
	direction = Direction::S;
}

Player::Player(int positionX, int positionY, Direction direction) {
	this->positionX = positionX;
	this->positionY = positionY;
	this->direction = direction;
}

Player::~Player() {
}

void Player::moveInDirection(Direction direction) {
	switch (direction) {

		case Direction::N:
			positionY--;
			break;

		case Direction::E:
			positionX++;
			break;

		case Direction::S:
			positionY++;
			break;

		case Direction::W:
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

void Player::teleportToCoordinates(int positionX, int positionY) {
	this->positionX = positionX;
	this->positionY = positionY;
}