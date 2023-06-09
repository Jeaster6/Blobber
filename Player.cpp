#include "Player.hpp"

Player::Player() {
	positionX = 0;
	positionY = 0;
	direction = N;
}

Player::~Player() {

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

void Player::move(Direction) {

}