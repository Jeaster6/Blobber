#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/unique_ptr.hpp>
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
        std::unique_ptr<GameMap> gameMap;
        std::vector<WorldChange> listOfChanges;

        void loadCurrentMap();
        void applyChangesToWorld();

    public:
        GameState();
        ~GameState();

        void initMap();
        void movePlayerForward();
        void movePlayerBackward();
        void movePlayerLeft();
        void movePlayerRight();
        void turnPlayerLeft();
        void turnPlayerRight();
        void renderPlayerView();
        void addToListOfChanges(const std::string&, int, int, ChangeType, const std::string&);
        void markTileAsExplored();
        void loadGame(const std::string&);
        void saveGame(const std::string&) const;
};
