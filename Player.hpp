#pragma once

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include "Direction.hpp"

class Player {

	private:
        friend class boost::serialization::access;
        template<class Archive> void serialize(Archive& ar, const unsigned int version) {
            ar& positionX;
            ar& positionY;
            ar& direction;
            ar& currentMap;
        }

		int positionX;
		int positionY;
		Direction direction;
        std::string currentMap;

		void moveInDirection(Direction);

	public:
		Player();
		Player(int, int, Direction, const std::string&);
		~Player();

		int getX() const;
		int getY() const;
		Direction getDirection() const;
        const std::string& getCurrentMapFileName() const;

		void turnRight();
		void turnLeft();
		void moveForward();
		void moveBackward();
		void moveRight();
		void moveLeft();
		void teleportToCoordinates(int, int, const std::string&);
};
