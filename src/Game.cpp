#include "Game.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <chrono>

namespace {
std::string trim(const std::string& s) {
    size_t start = 0;
    while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start]))) start++;
    size_t end = s.size();
    while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1]))) end--;
    return s.substr(start, end - start);
}
}

Game::Game() : rng(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count())) {
    buildActCatalog();
}

void Game::buildActCatalog() {
    actCatalog["JOKE"] = {"JOKE", "Vous racontez une blague absurde. Le monstre hésite entre rire et soupirer.", 22};
    actCatalog["COMPLIMENT"] = {"COMPLIMENT", "Vous faites un compliment très sérieux sur sa posture de combat.", 20};
    actCatalog["DISCUSS"] = {"DISCUSS", "Vous lancez une discussion étrange sur le sable et le destin.", 18};
    actCatalog["OBSERVE"] = {"OBSERVE", "Vous observez le monstre avec respect. Il semble un peu moins agressif.", 15};
    actCatalog["PET"] = {"PET", "Vous tentez une approche amicale. Le monstre est touché par ce geste.", 28};
    actCatalog["OFFER_SNACK"] = {"OFFER_SNACK", "Vous proposez un snack. Le monstre oublie brièvement le combat.", 25};
    actCatalog["REASON"] = {"REASON", "Vous essayez de raisonner votre adversaire. Le ton devient plus calme.", 24};
    actCatalog["DANCE"] = {"DANCE", "Vous improvisez une danse douteuse mais étonnamment sincère.", 17};
    actCatalog["INSULT"] = {"INSULT", "Vous lancez une remarque vexante. Le monstre se met en colère.", -20};
    actCatalog["TAUNT"] = {"TAUNT", "Vous provoquez l'ennemi avec un petit sourire arrogant.", -15};
}

void Game::printBanner() const {
    std::cout << "=====================================\n";
    std::cout << "         ALTERDUNE - CONSOLE RPG     \n";
    std::cout << "=====================================\n";
}

void Game::printInitialSummary() const {
    std::cout << "\n--- Resume initial ---\n";
    std::cout << "Joueur : " << player.getName() << "\n";
    std::cout << "HP : " << player.getHp() << "/" << player.getMaxHp() << "\n";
    std::cout << "Items disponibles :\n";
    for (const Item& item : player.getInventory()) {
        std::cout << " - " << item.getName() << " x" << item.getQuantity()
                  << " (" << item.getDescription() << ")\n";
    }
}

bool Game::loadItems(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cout << "Erreur : impossible d'ouvrir " << path << "\n";
        return false;
    }

    std::string line;
    std::getline(file, line); // header

    while (std::getline(file, line)) {
        if (trim(line).empty()) continue;
        std::stringstream ss(line);
        std::string nom, type, valeurStr, qteStr;

        if (!std::getline(ss, nom, ';') ||
            !std::getline(ss, type, ';') ||
            !std::getline(ss, valeurStr, ';') ||
            !std::getline(ss, qteStr, ';')) {
            std::cout << "Ligne mal formee ignoree dans items.csv : " << line << "\n";
            continue;
        }

        try {
            int valeur = std::stoi(trim(valeurStr));
            int qte = std::stoi(trim(qteStr));
            player.addItem(Item(trim(nom), trim(type), valeur, qte));
        } catch (...) {
            std::cout << "Ligne mal formee ignoree dans items.csv : " << line << "\n";
        }
    }
    return true;
}

bool Game::loadMonsters(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cout << "Erreur : impossible d'ouvrir " << path << "\n";
        return false;
    }

    std::string line;
    std::getline(file, line); // header

    while (std::getline(file, line)) {
        if (trim(line).empty()) continue;
        std::stringstream ss(line);
        std::vector<std::string> parts;
        std::string cell;
        while (std::getline(ss, cell, ';')) {
            parts.push_back(trim(cell));
        }

        if (parts.size() < 10) {
            std::cout << "Ligne mal formee ignoree dans monsters.csv : " << line << "\n";
            continue;
        }

        try {
            std::string category = parts[0];
            std::string name = parts[1];
            int hp = std::stoi(parts[2]);
            int atk = std::stoi(parts[3]);
            int def = std::stoi(parts[4]);
            int mercyGoal = std::stoi(parts[5]);
            std::vector<std::string> actIds;
            for (size_t i = 6; i < parts.size(); ++i) {
                if (parts[i] != "-" && !parts[i].empty()) actIds.push_back(parts[i]);
            }

            for (const std::string& id : actIds) {
                if (actCatalog.find(id) == actCatalog.end()) {
                    std::cout << "Action inconnue ignoree pour " << name << " : " << id << "\n";
                }
            }

            if (category == "NORMAL") {
                monsterTemplates.push_back(std::make_unique<NormalMonster>(name, hp, atk, def, mercyGoal, actIds));
            } else if (category == "MINIBOSS") {
                monsterTemplates.push_back(std::make_unique<MiniBoss>(name, hp, atk, def, mercyGoal, actIds));
            } else if (category == "BOSS") {
                monsterTemplates.push_back(std::make_unique<Boss>(name, hp, atk, def, mercyGoal, actIds));
            } else {
                std::cout << "Categorie inconnue ignoree : " << category << "\n";
            }
        } catch (...) {
            std::cout << "Ligne mal formee ignoree dans monsters.csv : " << line << "\n";
        }
    }

    return !monsterTemplates.empty();
}

int Game::promptInt(int minValue, int maxValue) {
    int value;
    while (true) {
        std::cout << "> ";
        if (std::cin >> value && value >= minValue && value <= maxValue) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        std::cout << "Choix invalide. Reessayez.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

int Game::rollDamage(int defenderMaxHp, int attackerAtk, int defenderDef) {
    // Sujet : degats = rand(0, HP max du defenseur).
    // Les parametres ATK/DEF restent dans la signature pour limiter les modifications du code.
    (void)attackerAtk;
    (void)defenderDef;
    std::uniform_int_distribution<int> dist(0, defenderMaxHp);
    return dist(rng);
}

std::unique_ptr<Monster> Game::pickRandomMonster() {
    // Apparition equilibree : le debut favorise les NORMAL,
    // puis les MINIBOSS, puis les BOSS vers la fin de la partie.
    std::vector<const Monster*> possible;
    int v = player.getVictories();

    std::uniform_int_distribution<int> percent(1, 100);
    int roll = percent(rng);

    std::vector<std::string> allowedCategories;
    if (v < 3) {
        allowedCategories = {"NORMAL"};
    } else if (v < 6) {
        allowedCategories = (roll <= 65) ? std::vector<std::string>{"NORMAL"}
                                         : std::vector<std::string>{"MINIBOSS"};
    } else if (v < 9) {
        if (roll <= 35) allowedCategories = {"NORMAL"};
        else if (roll <= 80) allowedCategories = {"MINIBOSS"};
        else allowedCategories = {"BOSS"};
    } else {
        allowedCategories = {"BOSS"};
    }

    for (const auto& m : monsterTemplates) {
        for (const std::string& cat : allowedCategories) {
            if (m->getCategory() == cat) {
                possible.push_back(m.get());
                break;
            }
        }
    }

    if (possible.empty()) {
        std::uniform_int_distribution<size_t> dist(0, monsterTemplates.size() - 1);
        return monsterTemplates[dist(rng)]->clone();
    }

    std::uniform_int_distribution<size_t> dist(0, possible.size() - 1);
    return possible[dist(rng)]->clone();
}

void Game::pressEnter() const {
    std::cout << "\nAppuyez sur Entree pour continuer...";
    std::cin.get();
}

std::vector<std::string> Game::visibleActIds(const Monster& monster) const {
    std::vector<std::string> acts = monster.getActIds();
    int limit = monster.getAllowedActCount();
    if (static_cast<int>(acts.size()) > limit) acts.resize(limit);
    return acts;
}

void Game::showBestiary() const {
    std::cout << "\n========== BESTIAIRE ==========\n";
    if (bestiary.empty()) {
        std::cout << "Aucun monstre vaincu pour le moment.\n";
        return;
    }

    for (size_t i = 0; i < bestiary.size(); ++i) {
        const BestiaryEntry& e = bestiary[i];
        std::cout << i + 1 << ". " << e.name << " | " << e.category
                  << " | HP " << e.maxHp
                  << " | ATK " << e.atk
                  << " | DEF " << e.def
                  << " | Resultat : " << e.result << "\n";
    }
}

void Game::showStats() const {
    std::cout << "\n========== STATISTIQUES ==========\n";
    std::cout << "Nom : " << player.getName() << "\n";
    std::cout << "HP : " << player.getHp() << "/" << player.getMaxHp() << "\n";
    std::cout << "Victoires : " << player.getVictories() << "/10\n";
    std::cout << "Monstres tues : " << player.getKilledCount() << "\n";
    std::cout << "Monstres epargnes : " << player.getSparedCount() << "\n";
}

void Game::showItemsMenu() {
    std::cout << "\n========== ITEMS ==========\n";
    auto& inventory = player.getInventory();

    for (size_t i = 0; i < inventory.size(); ++i) {
        std::cout << i + 1 << ". " << inventory[i].getName()
                  << " x" << inventory[i].getQuantity()
                  << " - " << inventory[i].getDescription() << "\n";
    }

    std::cout << "0. Retour\n";
    std::cout << "Choisissez un item a utiliser hors combat.\n";

    int choice = promptInt(0, static_cast<int>(inventory.size()));
    if (choice == 0) return;

    Item& item = inventory[choice - 1];

    if (item.getType() != "HEAL") {
        std::cout << "Cet item est strategique et s'utilise uniquement en combat.\n";
        return;
    }

    if (item.use(player)) {
        std::cout << "Item utilise. HP actuels : " << player.getHp() << "/" << player.getMaxHp() << "\n";
    } else {
        std::cout << "Impossible d'utiliser cet item.\n";
    }
}

void Game::startCombat() {
    std::unique_ptr<Monster> monster = pickRandomMonster();

    std::cout << "\n========== COMBAT ==========\n";
    std::cout << "Un ennemi apparait : " << monster->getName()
              << " [" << monster->getCategory() << "]\n";

    while (player.isAlive() && monster->isAlive()) {
        std::cout << "\n-------------------------------------\n";
        std::cout << player.getName() << " HP : " << player.getHp() << "/" << player.getMaxHp() << "\n";
        std::cout << monster->getName() << " HP : " << monster->getHp() << "/" << monster->getMaxHp()
                  << " | Mercy : " << monster->getMercy() << "/" << monster->getMercyGoal() << "\n";
        std::cout << "1. FIGHT\n";
        std::cout << "2. ACT\n";
        std::cout << "3. ITEM\n";
        std::cout << "4. MERCY\n";

        int action = promptInt(1, 4);
        bool playerTurnConsumed = false;
        bool combatEnded = false;

        if (action == 1) {
            int damage = rollDamage(monster->getMaxHp(), player.getAtk(), monster->getDef());

            if (nextPlayerBoost) {
                damage *= 2;
                nextPlayerBoost = false;
                std::cout << "PowerPotion activee : vos degats sont doubles !\n";
            }

            std::cout << player.getName() << " attaque " << monster->getName() << " et inflige "
                      << damage << " degats.\n";
            monster->takeDamage(damage);
            playerTurnConsumed = true;

            if (!monster->isAlive()) {
                std::cout << monster->getName() << " est vaincu.\n";
                player.addVictory();
                player.addKilled();
                bestiary.push_back({monster->getName(), monster->getCategory(), monster->getMaxHp(),
                                    monster->getAtk(), monster->getDef(), "Tue"});
                combatEnded = true;
            }
        } else if (action == 2) {
            std::vector<std::string> acts = visibleActIds(*monster);
            std::cout << "Actions ACT disponibles :\n";
            for (size_t i = 0; i < acts.size(); ++i) {
                std::cout << i + 1 << ". " << acts[i] << "\n";
            }
            int choice = promptInt(1, static_cast<int>(acts.size()));
            const std::string& selectedId = acts[choice - 1];
            auto it = actCatalog.find(selectedId);
            if (it != actCatalog.end()) {
                std::cout << it->second.text << "\n";
                monster->changeMercy(it->second.mercyImpact);
                std::cout << "Mercy modifiee de " << it->second.mercyImpact
                          << ". Nouvelle valeur : " << monster->getMercy() << "/" << monster->getMercyGoal() << "\n";
            } else {
                std::cout << "Action introuvable dans le catalogue.\n";
            }
            playerTurnConsumed = true;
        } else if (action == 3) {
            auto& inventory = player.getInventory();

            if (inventory.empty()) {
                std::cout << "Inventaire vide.\n";
            } else {
                for (size_t i = 0; i < inventory.size(); ++i) {
                    std::cout << i + 1 << ". " << inventory[i].getName()
                              << " x" << inventory[i].getQuantity()
                              << " - " << inventory[i].getDescription() << "\n";
                }

                int choice = promptInt(1, static_cast<int>(inventory.size()));
                Item& item = inventory[choice - 1];

                if (!item.hasStock()) {
                    std::cout << "Impossible d'utiliser cet item : stock vide.\n";
                } else if (item.getType() == "HEAL") {
                    item.consumeOne();
                    player.heal(item.getValue());
                    std::cout << item.getName() << " utilise. HP : "
                              << player.getHp() << "/" << player.getMaxHp() << "\n";
                    playerTurnConsumed = true;
                } else if (item.getType() == "BOOST") {
                    item.consumeOne();
                    nextPlayerBoost = true;
                    std::cout << item.getName() << " utilise : vos degats du prochain FIGHT seront doubles.\n";
                    playerTurnConsumed = true;
                } else if (item.getType() == "WEAKEN") {
                    item.consumeOne();
                    nextMonsterWeakened = true;
                    std::cout << item.getName() << " utilise : la prochaine attaque du monstre sera reduite.\n";
                    playerTurnConsumed = true;
                } else if (item.getType() == "MERCY") {
                    item.consumeOne();
                    monster->changeMercy(monster->getMercyGoal() / 2);
                    std::cout << item.getName() << " utilise : Mercy passe a "
                              << monster->getMercy() << "/" << monster->getMercyGoal() << ".\n";
                    playerTurnConsumed = true;
                } else if (item.getType() == "MERCY_FULL") {
                    item.consumeOne();
                    monster->changeMercy(monster->getMercyGoal());
                    std::cout << item.getName() << " utilise : Mercy est maintenant a "
                              << monster->getMercy() << "/" << monster->getMercyGoal() << ".\n";
                    playerTurnConsumed = true;
                } else if (item.getType() == "KILL") {
                    item.consumeOne();
                    monster->takeDamage(monster->getHp());
                    std::cout << item.getName() << " utilise : " << monster->getName()
                              << " est instantanement vaincu.\n";
                    player.addVictory();
                    player.addKilled();
                    bestiary.push_back({monster->getName(), monster->getCategory(), monster->getMaxHp(),
                                        monster->getAtk(), monster->getDef(), "Tue"});
                    combatEnded = true;
                } else {
                    std::cout << "Type d'item inconnu.\n";
                }
            }
        } else if (action == 4) {
            if (monster->canBeSpared()) {
                std::cout << "Vous epargnez " << monster->getName() << ". Le combat est gagne sans le tuer.\n";
                player.addVictory();
                player.addSpared();
                bestiary.push_back({monster->getName(), monster->getCategory(), monster->getMaxHp(),
                                    monster->getAtk(), monster->getDef(), "Epargne"});
                combatEnded = true;
            } else {
                std::cout << "Mercy insuffisante. Il faut atteindre " << monster->getMercyGoal() << ".\n";
                playerTurnConsumed = true;
            }
        }

        if (combatEnded) break;

        if (playerTurnConsumed && monster->isAlive()) {
            int damage = rollDamage(player.getMaxHp(), monster->getAtk(), player.getDef());

            if (nextMonsterWeakened) {
                damage /= 2;
                nextMonsterWeakened = false;
                std::cout << "SmokePotion activee : l'attaque du monstre est reduite !\n";
            }

            std::cout << monster->getName() << " contre-attaque et inflige "
                      << damage << " degats.\n";
            player.takeDamage(damage);
            if (!player.isAlive()) {
                std::cout << "Vous avez perdu... Fin de partie immediate.\n";
                return;
            }
        }
    }
}

void Game::printEnding() const {
    std::cout << "\n=====================================\n";
    std::cout << "              FIN DE PARTIE          \n";
    std::cout << "=====================================\n";
    std::cout << "Victoires obtenues : " << player.getVictories() << "/10\n";
    std::cout << "Tues : " << player.getKilledCount() << "\n";
    std::cout << "Epargnes : " << player.getSparedCount() << "\n\n";

    if (player.getKilledCount() > 0 && player.getSparedCount() == 0) {
        std::cout << "Fin Genocidaire : vous avez elimine tous vos adversaires.\n";
    } else if (player.getSparedCount() > 0 && player.getKilledCount() == 0) {
        std::cout << "Fin Pacifiste : aucun monstre n'a ete tue.\n";
    } else {
        std::cout << "Fin Neutre : vos choix ont alterne entre violence et compassion.\n";
    }
}

void Game::showMainMenu() {
    while (player.isAlive() && player.getVictories() < 10) {
        std::cout << "\n========== MENU PRINCIPAL ==========\n";
        std::cout << "1. Bestiaire\n";
        std::cout << "2. Demarrer un combat\n";
        std::cout << "3. Statistiques du personnage\n";
        std::cout << "4. Items\n";
        std::cout << "5. Quitter\n";

        int choice = promptInt(1, 5);
        switch (choice) {
            case 1: showBestiary(); break;
            case 2: startCombat(); break;
            case 3: showStats(); break;
            case 4: showItemsMenu(); break;
            case 5:
                std::cout << "Au revoir.\n";
                return;
        }
    }

    if (player.getVictories() >= 10) {
        printEnding();
    }
}

void Game::run() {
    printBanner();

    std::cout << "Entrez le nom du personnage : ";
    std::string playerName;
    std::getline(std::cin, playerName);
    if (trim(playerName).empty()) playerName = "Voyageur";
    player = Player(trim(playerName));

    if (!loadItems("data/items.csv") || !loadMonsters("data/monsters.csv")) {
        std::cout << "Arret du programme.\n";
        return;
    }

    printInitialSummary();
    pressEnter();
    showMainMenu();
}
