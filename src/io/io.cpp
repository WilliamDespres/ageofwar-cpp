#include "Game.hpp"
#include "io/conio.hpp"

#define ARROW_KEY 27
#define ARROW_KEY_2 91
#define KEY_UP 65
#define KEY_DOWN 66
#define KEY_LEFT 68
#define KEY_RIGHT 67

char Game::getInput(float time = -1) { // default value to -1
  char keyPressed = '\0'; // Null character
  auto begin = std::chrono::system_clock::now(), 
       end = begin;

  // Wait for key inputs for time ms
  do {
    if (kbhit()) keyPressed = getch();
    end = std::chrono::system_clock::now();
  } while (time != -1 ? std::chrono::duration_cast<std::chrono::milliseconds> (end - begin).count() < time : keyPressed == '\0'); // Game is printed 4 times in a turn

  keyPressed = tolower(keyPressed);
  switch(keyPressed) {
    case 'q': case 's': case 'd':
      if (m_isPlaying) m_player1Key = keyPressed; 
      break;
    case 'k': case 'l': case 'm':
      if (m_isPlaying) m_player2Key = keyPressed; 
      break;
    case 'p':
      pause(); break;
  }
  return keyPressed;
}

int Game::getMenuInput() {
  char keyPressed;
  while ((keyPressed = getch()) != '\r' && keyPressed != 'p') //enter
  {
    if (keyPressed == ARROW_KEY) { // An arrow key has been pressed
      if ((keyPressed = getch()) == ARROW_KEY_2) {
        keyPressed = getch();
        return keyPressed;
      }
    }
  }
  return keyPressed;
}

void Game::pause() {
  m_isPlaying = false;
  m_menuItemSelected = 0;
  std::string reset_print_color = "\033[0m", 
              highlight_in_blue = "\033[44m",
              print_in_black = "\033[30m";
  
  while (!m_isPlaying) {
    printGame();
    std::cout << "\n\tPaused" << (m_isLoaded ? " - Game " + m_name + " loaded" : (m_isSaved ? " - Game " + m_name + " saved" : "")) << "\n\n"
              << (m_menuItemSelected == 0 ? highlight_in_blue + print_in_black : "") << "SAVE GAME" << reset_print_color << "    "
              << (m_menuItemSelected == 1 ? highlight_in_blue + print_in_black : "") << "LOAD GAME" << reset_print_color << "    "
              << (m_menuItemSelected == 2 ? highlight_in_blue + print_in_black : "") << "RESUME" << reset_print_color << "    "
              << (m_menuItemSelected == 3 ? highlight_in_blue + print_in_black : "") << "QUIT" << reset_print_color << "\n";
  
    switch (getMenuInput()) {
      // Switch between menu items
      case KEY_LEFT:
        if (m_menuItemSelected > 0) m_menuItemSelected--;
        break;
      case KEY_RIGHT:
        if (m_menuItemSelected < 3) m_menuItemSelected++;
        break;
      // Validation
      case '\r':
        switch(m_menuItemSelected) {
          case 0:
            if (!std::filesystem::exists("saves")) {
              std::cout << "No saving folder found, creating one...\n";
              std::filesystem::create_directory("saves");
            }
            save();
            break;
          case 1:
            if (!std::filesystem::exists("saves")) {
              std::cout << "No saves found. Press any key...\n";
              getInput();
              break; // Abort
            }
            load();
            break;
          case 2:
            resume();
            break;
          case 3:
            if (m_isSaved) { std::cout << "Goodbye!\n"; exit(0); }
            else {
              std::cout << "Game progression isn't saved, quit? (y/n)\n";
              if (getInput() == 'y') { std::cout << "Goodbye!\n"; exit(0); }
              break;
            }
        }
        break;
      // Resume game
      case 'p':
        resume(); break;
      default: break;
    }
  }
}

void Game::resume() {
  m_isSaved = false;
  m_isPlaying = true;
}

void Game::save() {
  std::string name = "saves/" + m_name + ".txt"; // A save already exists, override automatically
  if (!std::filesystem::exists(name)) {
    std::cout << "Enter name for your game (q to quit this menu):\n";
    std::cin >> name;
    if (name == "q") { std::cout << "Aborting...\n"; return; }
    m_name = name;
    name = "saves/" + name + ".txt";
  }

  std::cout << "Hang on, game is saving...\n";
  std::ofstream saveFile(name); // Creating file and openning it

  // For reading, we add flags, they are Game, Player1, Player2

  saveFile << "Game " << m_playerTurn << " " << m_turnNumber << " " << m_maxTurnNumber << "\n";  // Game infos

  // Start with player one
  saveFile << "Player1 " << m_player1->getName() << " " << m_player1->getAge() << " " << m_player1->getHp() << " " << m_player1->getMoney() << " " << m_player1->getXp() << "\n"; // General infos
  for (Unit* u : m_player1->getUnits()) // Save each unit infos
    saveFile << u->name() << " " << u->getHp() << " " << u->getPosition() << "\n";

  // Then with player two
  saveFile << "Player2 " << m_player2->getName() << " " << m_player2->getAge() << " " << m_player2->getHp() << " " << m_player2->getMoney() << " " << m_player2->getXp()  << "\n"; // General infos
  for (Unit* u : m_player2->getUnits()) // Save each unit infos
    saveFile << u->name() << " " << u->getHp() << " " << u->getPosition() << "\n";

  saveFile.close();
  std::cout << "Game nicely saved! Press any key to resume...\n";
  m_isSaved = true;
}

void Game::load() {
  if (!m_isSaved) {
    std::cout << "Current game is not saved, any unsaved progression will be lost, continue? (y/n)\n";
    if (getInput() != 'y') { std::cout << "Aborting...\n"; return; }
  }

  std::string name = "", file = "saves/.txt";
  while (!std::filesystem::exists(file)) {
    std::cout << "Saved games:\n";
    for (const auto &entry : std::filesystem::directory_iterator("saves"))
      std::cout << entry.path().stem().string() << std::endl;
    
    std::cout << "Enter game name to load (q to quit this menu):\n";
    std::cin >> name;
    if (name == "q") { std::cout << "Aborting...\n"; return; }
    file = "saves/" + name + ".txt";
  }

  std::ifstream savedFile(file);

  delete m_player1; delete m_player2; // Delete current players
  
  std::string line, flag;
  bool setPlayer1 = true; // We start by setting player1

  while (getline(savedFile, line)) {
    std::istringstream l(line);
    l >> flag;
    if (flag == "Game") {
      l >> m_playerTurn; l >> m_turnNumber; l >> m_maxTurnNumber;
    } else if (flag == "Player1" || flag == "Player2") {
      std::string Pname, Page; int Php, Pmoney, Pxp;
      l >> Pname; l >> Page; l >> Php; l >> Pmoney; l >> Pxp;
      if (flag == "Player1") { // Player1
        if (Pname[0] == 'A') m_player1 = new AI(Pname, Page, Php, Pmoney, Pxp, false); // If it's an AI
        else m_player1 = new Human(Pname, Page, Php, Pmoney, Pxp, false);
      } else { // Player2
        setPlayer1 = false;
        if (Pname[0] == 'A') m_player2 = new AI(Pname, Page, Php, Pmoney, Pxp, true); // If it's an AI
        else m_player2 = new Human(Pname, Page, Php, Pmoney, Pxp, true);
      }
    } else { // It's an unit
      int Uhp, Upos;
      l >> Uhp; l >> Upos;
      if (flag == "s") {
        if (setPlayer1) m_player1->spawn(UnitNames::Soldier, Uhp, Upos);
        else m_player2->spawn(UnitNames::Soldier, Uhp, Upos);
      } else if (flag == "S") {
        if (setPlayer1) m_player1->spawn(UnitNames::SuperSoldier, Uhp, Upos);
        else m_player2->spawn(UnitNames::SuperSoldier, Uhp, Upos);
      } else if (flag == "B") {
        if (setPlayer1) m_player1->spawn(UnitNames::Bowman, Uhp, Upos);
        else m_player2->spawn(UnitNames::Bowman, Uhp, Upos);
      } else if (flag == "C") {
        if (setPlayer1) m_player1->spawn(UnitNames::Catapult, Uhp, Upos);
        else m_player2->spawn(UnitNames::Catapult, Uhp, Upos);
      } else if (flag == "T") {
        if (setPlayer1) m_player1->spawn(UnitNames::Trooper, Uhp, Upos);
        else m_player2->spawn(UnitNames::Trooper, Uhp, Upos);
      } else if (flag == "R") {
        if (setPlayer1) m_player1->spawn(UnitNames::RocketMan, Uhp, Upos);
        else m_player2->spawn(UnitNames::RocketMan, Uhp, Upos);
      } else if (flag == "TA") {
        if (setPlayer1) m_player1->spawn(UnitNames::Tank, Uhp, Upos);
        else m_player2->spawn(UnitNames::Tank, Uhp, Upos);
      } else if (flag == "R2") {
        if (setPlayer1) m_player1->spawn(UnitNames::R2D2, Uhp, Upos);
        else m_player2->spawn(UnitNames::R2D2, Uhp, Upos);
      } else if (flag == "Y") {
        if (setPlayer1) m_player1->spawn(UnitNames::Yoda, Uhp, Upos);
        else m_player2->spawn(UnitNames::Yoda, Uhp, Upos);
      } else if (flag == "ST") {
        if (setPlayer1) m_player1->spawn(UnitNames::Starfighter, Uhp, Upos);
        else m_player2->spawn(UnitNames::Starfighter, Uhp, Upos);
      }
    }
  }

  m_name = name;
  m_isLoaded = true;
}



void Game::printGame() {
  std::cout << "\033[2J\033[1;1H"; // Clear screen, see here for explanations https://stackoverflow.com/questions/4062045/clearing-terminal-in-linux-with-c-code

  int slot_size = 5;

  std::string reset_print_color = "\033[0m",
              print_in_red = "\033[34m",
              print_in_blue = "\033[31m",
              print_in_magenta = "\033[35m",
              print_in_green = "\033[32m",
              print_in_yellow = "\033[33m", 
              highlight_in_blue = "\033[44m",
              highlight_in_red = "\033[41m";

  std::cout << R"(
                                    ___   ____________   ____  ______   _       _____    ____
                                   /   | / ____/ ____/  / __ \/ ____/  | |     / /   |  / __ \
                                  / /| |/ / __/ __/    / / / / /_      | | /| / / /| | / /_/ /
                                 / ___ / /_/ / /___   / /_/ / __/      | |/ |/ / ___ |/ _  _/
                                /_/  |_\____/_____/   \____/_/         |__/|__/_/  |_/_/ |_|
)" << "\n";

  // Turn number display
  std::cout << std::string((MAP_SIZE * (slot_size + 3) - 4) / 2 + 8, ' ') 
            << "Turn n°" << m_turnNumber << "/" << m_maxTurnNumber << std::endl;

  // Player names display
  std::cout << "         ";
  if (!m_playerTurn) {
    std::cout << highlight_in_blue;
  }
  std::cout << m_player1->toString() << "1  ";
  std::cout << reset_print_color;
  std::cout << std::string((MAP_SIZE) * (slot_size + 3) - 4, ' ');
  if (m_playerTurn) {
    std::cout << highlight_in_red;
  }
  std::cout << m_player2->toString() << "2  ";
  std::cout << reset_print_color << std::endl << std::endl;

  // Player coins display
  std::cout << print_in_yellow << "         G" << std::setw(6) << m_player1->getMoney();
  std::cout << std::string((MAP_SIZE) * (slot_size + 3) - 4, ' ');
  std::cout << "G" << std::setw(6) << m_player2->getMoney();
  std::cout << reset_print_color << std::endl;

  // Player xp display
  std::cout << print_in_magenta << "         XP" << std::setw(4) << m_player1->getXp() << "%";
  std::cout << std::string((MAP_SIZE) * (slot_size + 3) - 4, ' ');
  std::cout << "XP" << std::setw(4) << m_player2->getXp() << "%";
  std::cout << reset_print_color << std::endl;

  // Player hp display
  std::cout << print_in_green << "         HP" << std::setw(5) << std::max(m_player1->getHp(), 0);
  std::cout << std::string((MAP_SIZE) * (slot_size + 3) - 4, ' ');
  std::cout << "HP" << std::setw(5) << std::max(m_player2->getHp(), 0);
  std::cout << reset_print_color << std::endl;

  // Towers display
  for (int h = 0; h < 8; h++) {
    std::cout << " " << print_in_red << m_player1->skin(h) << std::string(15 * slot_size + 1, ' ') << print_in_blue << m_player2->skin(h) << std::endl << reset_print_color;
  } 

  // Slots and Units display
  for (int h = 0; h < 4; h++) {
    std::cout << "        "; // Aligning with castle's bottom
    for (int i = 0; i < MAP_SIZE; i++) {
      std::string slot;
      Player* p = getUnitOwnerAtPosition(i);
      if (p) { 
        slot = p->getUnitAtPosition(i)->skin(h, p == m_player2);
        std::cout << (p == m_player1 ? print_in_red : print_in_blue);
      }
      else if (h == 3)
        slot = "    _    ";
      else
        slot = "         ";
      std::cout << std::setw(slot_size * 2 - 1) << slot;
      std::cout << reset_print_color;
    }
    std::cout << std::endl;
  }

  // HPs display
  std::cout << "         "; // Aligning with castle's bottom
  for (int i = 0; i < MAP_SIZE; i++) {
    int hp = 0;
    Player* p = getUnitOwnerAtPosition(i);
    if (p) {
      hp = p->getUnitAtPosition(i)->getHp();
      std::cout << print_in_green;
    }
    std::cout << std::setw(slot_size * 2 - 1) << (hp > 0 ? std::to_string(hp) + "     " : "         ");
    std::cout << reset_print_color;
  }

  std::cout << std::endl << std::endl << std::endl;

  // Commands and Prices
  std::string units_CaveAge = "Soldier (" + print_in_yellow + std::to_string(Unit::stats["CaveAge"]["Soldier"].price) + reset_print_color +
                          ")    Bowman (" + print_in_yellow + std::to_string(Unit::stats["CaveAge"]["Bowman"].price) + reset_print_color +
                          ")   Catapult (" + print_in_yellow + std::to_string(Unit::stats["CaveAge"]["Catapult"].price) + reset_print_color + ")";
  std::string units_ModernAge = "Trooper (" + print_in_yellow + std::to_string(Unit::stats["ModernAge"]["Trooper"].price) + reset_print_color +
                          ")    RocketMan (" + print_in_yellow + std::to_string(Unit::stats["ModernAge"]["RocketMan"].price) + reset_print_color +
                          ")   Tank (" + print_in_yellow + std::to_string(Unit::stats["ModernAge"]["Tank"].price) + reset_print_color + ")";
  std::string units_StarAge = "R2D2 (" + print_in_yellow + std::to_string(Unit::stats["StarAge"]["R2D2"].price) + reset_print_color +
                          ")    Yoda (" + print_in_yellow + std::to_string(Unit::stats["StarAge"]["Yoda"].price) + reset_print_color +
                          ")   Starfighter (" + print_in_yellow + std::to_string(Unit::stats["StarAge"]["Starfighter"].price) + reset_print_color + ")";
  

  std::cout << "Commands and Prices\n" << print_in_blue
            << "P1 Commands: Spawn... " << reset_print_color << (m_player1->getAge() == "CaveAge" ? units_CaveAge : (m_player1->getAge() == "ModernAge" ? units_ModernAge : units_StarAge))
            << print_in_blue << "\n                        Q/q              S/s              D/d  " << std::endl << print_in_red
            << "P2 Commands: Spawn... " << reset_print_color << (m_player2->getAge() == "CaveAge" ? units_CaveAge : (m_player2->getAge() == "ModernAge" ? units_ModernAge : units_StarAge))
            << print_in_red << "\n                        K/k              L/l              M/m  " << std::endl << reset_print_color
            << "Pause: P/p" << std::endl;
}