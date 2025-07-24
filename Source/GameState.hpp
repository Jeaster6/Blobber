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
#include "Tile.hpp"
#include "WorldChange.hpp"
#include "Graphics.hpp"

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
        bool messageDisplayed;

        void loadCurrentMap();
        void applyChangesToWorld();
        void initMap();
        void markTileAsExplored();
        void checkAndHandleTrigger();

    public:
        GameState();
        GameState(const std::string&);
        ~GameState();

        Player getPlayer() const;
        GameMap getMap() const;

        void movePlayerForward();
        void movePlayerBackward();
        void movePlayerLeft();
        void movePlayerRight();
        void turnPlayerLeft();
        void turnPlayerRight();
        void renderPlayerView();
        void addToListOfChanges(const std::string&, int, int, ChangeType, const std::string&);
        void loadGame(const std::string&);
        void saveGame(const std::string&) const;
        void closeMessage();
        bool isMessageDisplayed() const;
        void addItemToPartyInventory(const std::string&);
        void removeItemFromPartyInventory(int);
        void addItemToObject(int, int, const std::string&);
        void removeItemFromObject(int, int, int);
        bool isTileInFrontOfPlayerFree() const;
        Tile getTileInFrontOfPlayer() const;
        void getCoordinatesOfTileInFrontOfPlayer(int&, int&);
};
