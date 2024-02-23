#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include "CurrentDirectory.hpp"
#include "Direction.hpp"
#include "Player.hpp"
#include "GameMap.hpp"
#include "WorldChange.hpp"

class GameState {

    private:
        friend class boost::serialization::access;
        template<class Archive> void serialize(Archive& ar, const unsigned int version) {
            ar& player;
            ar& listOfChanges;
        }

        Player player;
        GameMap gameMap;
        std::vector<WorldChange> listOfChanges;

        void loadCurrentMap();
        void applyChangesToWorld();
        void initMap();

    public:
        GameState();
        ~GameState();

        Player getPlayer() const;
        GameMap getMap() const;

        bool movePlayerForward();
        bool movePlayerBackward();
        bool movePlayerLeft();
        bool movePlayerRight();
        void turnPlayerLeft();
        void turnPlayerRight();
        void addToListOfChanges(const std::string&, int, int, ChangeType, const std::string&);
        void markTileAsExplored();
        void loadGame(const std::string&);
        void saveGame(const std::string&) const;
};
