#include "Player.hpp"

Player::Player() {
	positionX = 0;
	positionY = 0;
	direction = Direction::S;
    currentMap = "Map_1.dat";
    partyInventory.clear();
    partyInventory.push_back(Item("item1"));
    partyInventory.push_back(Item("item2"));
    partyInventory.push_back(Item("item4"));
    partyInventory.push_back(Item("item4"));
    partyInventory.push_back(Item("item5"));
}

Player::Player(int positionX, int positionY, Direction direction, const std::string& currentMap, const std::vector<Item>& partyInventory) {
	this->positionX = positionX;
	this->positionY = positionY;
	this->direction = direction;
    this->currentMap = currentMap;
    this->partyInventory = partyInventory;
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

int Player::getX() const {
	return positionX;
}

int Player::getY() const {
	return positionY;
}

Direction Player::getDirection() const {
	return direction;
}

std::string Player::getCurrentMapFileName() const {
    return currentMap;
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

void Player::teleportToCoordinates(int positionX, int positionY, const std::string& targetMap) {
	this->positionX = positionX;
	this->positionY = positionY;
    this->currentMap = targetMap;
}

std::vector<Item> Player::getPartyInventory() const {
    return partyInventory;
}

void Player::addToPartyInventory(const std::string& itemID) {
    partyInventory.push_back(Item(itemID));
}

void Player::removeFromPartyInventory(int itemIndex) {
    partyInventory.erase(partyInventory.begin() + itemIndex);
}
