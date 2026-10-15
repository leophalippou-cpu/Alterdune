#ifndef GAME_H
#define GAME_H

#include "ActAction.h"
#include "BestiaryEntry.h"
#include "Monster.h"
#include "Player.h"
#include <map>
#include <memory>
#include <random>
#include <string>
#include <vector>

class Game {
private:
    Player player;
    std::vector<std::unique_ptr<Monster>> monsterTemplates;
    std::vector<BestiaryEntry> bestiary;
    std::map<std::string, ActAction> actCatalog;
    std::mt19937 rng;
    bool nextPlayerBoost = false;
    bool nextMonsterWeakened = false;

    void buildActCatalog();
    void printBanner() const;
    void printInitialSummary() const;
    bool loadItems(const std::string& path);
    bool loadMonsters(const std::string& path);

    void showMainMenu();
    void showBestiary() const;
    void showStats() const;
    void showItemsMenu();
    void startCombat();
    void printEnding() const;

    int promptInt(int minValue, int maxValue);
    int rollDamage(int defenderMaxHp, int attackerAtk, int defenderDef);
    std::unique_ptr<Monster> pickRandomMonster();
    void pressEnter() const;
    std::vector<std::string> visibleActIds(const Monster& monster) const;

public:
    Game();
    void run();
};

#endif
