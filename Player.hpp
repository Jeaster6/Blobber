#pragma once

#include <string>
#include <vector>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include "Direction.hpp"
#include "Item.hpp"

class Player {

	private:
        friend class boost::serialization::access;
        template<class Archive> void serialize(Archive& ar, const unsigned int version) {
            ar& positionX;
            ar& positionY;
            ar& direction;
            ar& currentMap;
            ar& partyInventory;
        }

		int positionX;
		int positionY;
		Direction direction;
        std::string currentMap;
        std::vector<Item> partyInventory;

		void moveInDirection(Direction);

	public:
		Player();
		Player(int, int, Direction, const std::string&, const std::vector<Item>&);
		~Player();

		int getX() const;
		int getY() const;
		Direction getDirection() const;
        std::string getCurrentMapFileName() const;
        std::vector<Item> getPartyInventory() const;

		void turnRight();
		void turnLeft();
		void moveForward();
		void moveBackward();
		void moveRight();
		void moveLeft();
		void teleportToCoordinates(int, int, const std::string&);
        void addToPartyInventory(const std::string&);
        void removeFromPartyInventory(int);
};
