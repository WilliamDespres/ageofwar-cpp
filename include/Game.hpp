#ifndef GAME_H
#define GAME_H

#define TURN_TIME_MS 2000

#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "players/Human.hpp"
#include "players/AI.hpp"

class Game {
  private:
    std::string m_name; // Game name
    Player* m_player1;
    Player* m_player2;
    int m_turnNumber, m_maxTurnNumber;
    bool m_playerTurn; // False for player1 and true for player2
    bool m_isSaved, m_isLoaded, m_isPlaying; // To know if current game progression is saved or not, isLoaded allows to keep track if a new game has been loaded, isPlaying is used to consider game inputs or not while being in pause menu
    char m_player1Key, m_player2Key;
    int m_menuItemSelected;

    void initAllStatics();
    void runGame();
    void gameTurn();
    Player* getUnitOwnerAtPosition(int position);
    Player* getPlayingPlayer() { if (m_playerTurn) return m_player2; return m_player1; };
    Player* getWaitingPlayer() { if (!m_playerTurn) return m_player2; return m_player1; };;
    void killAllDeadUnits();
    void spawn(UnitNames unit, Player &p);
    char getInput(float time);
    int  getMenuInput();
    void printGame();
    void save();
    void load();

  public:
    Game(int maxTurnNumber, int argc, char *argv[]);
    ~Game() { delete m_player1; delete m_player2; };

    void pause();
    void resume();
};

#endif