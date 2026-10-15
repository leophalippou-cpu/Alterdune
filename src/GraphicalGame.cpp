#include "GraphicalGame.h"

#include <algorithm>
#include <cctype>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>

GraphicalGame::GraphicalGame()
    : window(sf::VideoMode({1024u, 768u}), "ALTERDUNE - GameBoy Edition"),
      currentScreen(Screen::NameInput),
      typedName(""),
      playerName(""),
      message("Entrez votre pseudo."),
      loadErrorMessage(""),
      selectedMenuIndex(0),
      playerHp(100),
      playerMaxHp(100),
      monsterHp(0),
      monsterMaxHp(0),
      mercy(0),
      mercyGoal(100),
      victories(0),
      killed(0),
      spared(0),
      rng(static_cast<unsigned int>(
          std::chrono::system_clock::now().time_since_epoch().count()))
{
    loadAssets();
    buildActCatalog();

    if (!loadItems("data/items.csv") || !loadMonsters("data/monsters.csv")) {
        currentScreen = Screen::LoadError;
    }

    mainButtons = {
        { sf::FloatRect({130.f, 215.f}, {600.f, 48.f}), "1. BESTIAIRE" },
        { sf::FloatRect({130.f, 275.f}, {600.f, 48.f}), "2. DEMARRER UN COMBAT" },
        { sf::FloatRect({130.f, 335.f}, {600.f, 48.f}), "3. STATISTIQUES" },
        { sf::FloatRect({130.f, 395.f}, {600.f, 48.f}), "4. ITEMS" },
        { sf::FloatRect({130.f, 455.f}, {600.f, 48.f}), "5. QUITTER" }
    };

    battleButtons = {
        { sf::FloatRect({90.f, 680.f}, {190.f, 50.f}), "FIGHT" },
        { sf::FloatRect({310.f, 680.f}, {170.f, 50.f}), "ACT" },
        { sf::FloatRect({510.f, 680.f}, {170.f, 50.f}), "ITEM" },
        { sf::FloatRect({710.f, 680.f}, {190.f, 50.f}), "MERCY" }
    };
}

void GraphicalGame::loadAssets() {
    if (!font.openFromFile("assets/PressStart2P-Regular.ttf")) {
        std::cout << "Erreur : impossible de charger assets/PressStart2P-Regular.ttf\n";
    }

    if (!playerTexture.loadFromFile("assets/player_trainer.png")) std::cout << "Erreur player_trainer.png\n";
    if (!cursorTexture.loadFromFile("assets/cursor.png")) std::cout << "Erreur cursor.png\n";

    if (!froggitTexture.loadFromFile("assets/froggit.png")) std::cout << "Erreur froggit.png\n";
    if (!dustmoleTexture.loadFromFile("assets/dustmole.png")) std::cout << "Erreur dustmole.png\n";
    if (!sandbugTexture.loadFromFile("assets/sandbug.png")) std::cout << "Erreur sandbug.png\n";
    if (!pebblinTexture.loadFromFile("assets/pebblin.png")) std::cout << "Erreur pebblin.png\n";
    if (!glitchratTexture.loadFromFile("assets/glitchrat.png")) std::cout << "Erreur glitchrat.png\n";
    if (!cactinyTexture.loadFromFile("assets/cactiny.png")) std::cout << "Erreur cactiny.png\n";
    if (!scrapwormTexture.loadFromFile("assets/scrapworm.png")) std::cout << "Erreur scrapworm.png\n";
    if (!shadebatTexture.loadFromFile("assets/shadebat.png")) std::cout << "Erreur shadebat.png\n";
    if (!dryslimeTexture.loadFromFile("assets/dryslime.png")) std::cout << "Erreur dryslime.png\n";
    if (!tinytuskTexture.loadFromFile("assets/tinytusk.png")) std::cout << "Erreur tinytusk.png\n";

    if (!mimicboxTexture.loadFromFile("assets/mimicbox.png")) std::cout << "Erreur mimicbox.png\n";
    if (!rusthowlerTexture.loadFromFile("assets/rusthowler.png")) std::cout << "Erreur rusthowler.png\n";
    if (!bonecrawlerTexture.loadFromFile("assets/bonecrawler.png")) std::cout << "Erreur bonecrawler.png\n";
    if (!ironclawTexture.loadFromFile("assets/ironclaw.png")) std::cout << "Erreur ironclaw.png\n";
    if (!mirageknightTexture.loadFromFile("assets/mirageknight.png")) std::cout << "Erreur mirageknight.png\n";
    if (!venomrootTexture.loadFromFile("assets/venomroot.png")) std::cout << "Erreur venomroot.png\n";
    if (!hollowguardTexture.loadFromFile("assets/hollowguard.png")) std::cout << "Erreur hollowguard.png\n";
    if (!ashdrakeTexture.loadFromFile("assets/ashdrake.png")) std::cout << "Erreur ashdrake.png\n";

    if (!queenbyteTexture.loadFromFile("assets/queenbyte.png")) std::cout << "Erreur queenbyte.png\n";
    if (!archonSandTexture.loadFromFile("assets/archon_sand.png")) std::cout << "Erreur archon_sand.png\n";
    if (!titanrootTexture.loadFromFile("assets/titan_root.png")) std::cout << "Erreur titan_root.png\n";
    if (!nullemperorTexture.loadFromFile("assets/null_emperor.png")) std::cout << "Erreur null_emperor.png\n";
    if (!crimsonhydraTexture.loadFromFile("assets/crimson_hydra.png")) std::cout << "Erreur crimson_hydra.png\n";
    if (!dunereaperTexture.loadFromFile("assets/dune_reaper.png")) std::cout << "Erreur dune_reaper.png\n";

    playerTexture.setSmooth(false);
    cursorTexture.setSmooth(false);

    froggitTexture.setSmooth(false); dustmoleTexture.setSmooth(false); sandbugTexture.setSmooth(false);
    pebblinTexture.setSmooth(false); glitchratTexture.setSmooth(false); cactinyTexture.setSmooth(false);
    scrapwormTexture.setSmooth(false); shadebatTexture.setSmooth(false); dryslimeTexture.setSmooth(false);
    tinytuskTexture.setSmooth(false); mimicboxTexture.setSmooth(false); rusthowlerTexture.setSmooth(false);
    bonecrawlerTexture.setSmooth(false); ironclawTexture.setSmooth(false); mirageknightTexture.setSmooth(false);
    venomrootTexture.setSmooth(false); hollowguardTexture.setSmooth(false); ashdrakeTexture.setSmooth(false);
    queenbyteTexture.setSmooth(false); archonSandTexture.setSmooth(false); titanrootTexture.setSmooth(false);
    nullemperorTexture.setSmooth(false); crimsonhydraTexture.setSmooth(false); dunereaperTexture.setSmooth(false);
}

void GraphicalGame::buildActCatalog() {
    actCatalog["JOKE"] = {"JOKE", "Vous racontez une blague absurde.", 22};
    actCatalog["COMPLIMENT"] = {"COMPLIMENT", "Vous complimentez le monstre.", 20};
    actCatalog["DISCUSS"] = {"DISCUSS", "Vous discutez calmement avec lui.", 18};
    actCatalog["OBSERVE"] = {"OBSERVE", "Vous observez son comportement.", 15};
    actCatalog["PET"] = {"PET", "Vous tentez une approche amicale.", 28};
    actCatalog["OFFER_SNACK"] = {"OFFER_SNACK", "Vous proposez un snack.", 25};
    actCatalog["REASON"] = {"REASON", "Vous essayez de le raisonner.", 24};
    actCatalog["DANCE"] = {"DANCE", "Vous improvisez une danse.", 17};
    actCatalog["INSULT"] = {"INSULT", "Vous le vexez. Il s'enerve.", -20};
    actCatalog["TAUNT"] = {"TAUNT", "Vous provoquez l'ennemi.", -15};
}

std::string GraphicalGame::trim(const std::string& s) const {
    size_t start = 0;
    while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start]))) start++;
    size_t end = s.size();
    while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1]))) end--;
    return s.substr(start, end - start);
}

bool GraphicalGame::loadItems(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        loadErrorMessage = "Impossible d'ouvrir " + path;
        return false;
    }

    std::string line;
    std::getline(file, line);

    while (std::getline(file, line)) {
        if (trim(line).empty()) continue;

        std::stringstream ss(line);
        std::string name, type, valueStr, quantityStr;

        if (!std::getline(ss, name, ';') || !std::getline(ss, type, ';') ||
            !std::getline(ss, valueStr, ';') || !std::getline(ss, quantityStr, ';')) {
            continue;
        }

        try {
            GraphicItem item;
            item.name = trim(name);
            item.type = trim(type);
            item.healValue = std::stoi(trim(valueStr));
            item.quantity = std::stoi(trim(quantityStr));
            items.push_back(item);
        } catch (...) {
            continue;
        }
    }

    return !items.empty();
}

bool GraphicalGame::loadMonsters(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        loadErrorMessage = "Impossible d'ouvrir " + path;
        return false;
    }

    std::string line;
    std::getline(file, line);

    while (std::getline(file, line)) {
        if (trim(line).empty()) continue;

        std::stringstream ss(line);
        std::vector<std::string> parts;
        std::string cell;
        while (std::getline(ss, cell, ';')) parts.push_back(trim(cell));
        if (parts.size() < 10) continue;

        try {
            GraphicMonster monster;
            monster.category = parts[0];
            monster.name = parts[1];
            monster.hp = std::stoi(parts[2]);
            monster.atk = std::stoi(parts[3]);
            monster.def = std::stoi(parts[4]);
            monster.mercyGoal = std::stoi(parts[5]);

            for (size_t i = 6; i < parts.size(); ++i) {
                if (parts[i] != "-" && !parts[i].empty() && actCatalog.find(parts[i]) != actCatalog.end()) {
                    monster.actIds.push_back(parts[i]);
                }
            }

            monster.spriteType = spriteTypeForMonster(monster.name, monster.category);

            if (monster.category == "NORMAL" || monster.category == "MINIBOSS" || monster.category == "BOSS") {
                monsters.push_back(monster);
            }
        } catch (...) {
            continue;
        }
    }

    if (monsters.empty()) {
        loadErrorMessage = "Aucun monstre valide dans monsters.csv";
        return false;
    }
    return true;
}

int GraphicalGame::spriteTypeForMonster(const std::string& name, const std::string& category) const {
    std::string n = name;
    std::transform(n.begin(), n.end(), n.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });

    if (n.find("froggit") != std::string::npos) return 0;
    if (n.find("dustmole") != std::string::npos) return 1;
    if (n.find("sandbug") != std::string::npos) return 2;
    if (n.find("pebblin") != std::string::npos) return 3;
    if (n.find("glitchrat") != std::string::npos) return 4;
    if (n.find("cactiny") != std::string::npos) return 5;
    if (n.find("scrapworm") != std::string::npos) return 6;
    if (n.find("shadebat") != std::string::npos) return 7;
    if (n.find("dryslime") != std::string::npos) return 8;
    if (n.find("tinytusk") != std::string::npos) return 9;
    if (n.find("mimicbox") != std::string::npos) return 10;
    if (n.find("rusthowler") != std::string::npos) return 11;
    if (n.find("bonecrawler") != std::string::npos) return 12;
    if (n.find("ironclaw") != std::string::npos) return 13;
    if (n.find("mirageknight") != std::string::npos) return 14;
    if (n.find("venomroot") != std::string::npos) return 15;
    if (n.find("hollowguard") != std::string::npos) return 16;
    if (n.find("ashdrake") != std::string::npos) return 17;
    if (n.find("queenbyte") != std::string::npos) return 18;
    if (n.find("archon") != std::string::npos) return 19;
    if (n.find("titan") != std::string::npos) return 20;
    if (n.find("null") != std::string::npos) return 21;
    if (n.find("crimson") != std::string::npos) return 22;
    if (n.find("dune") != std::string::npos) return 23;

    if (category == "NORMAL") return 0;
    if (category == "MINIBOSS") return 10;
    return 18;
}

int GraphicalGame::allowedActCount(const std::string& category) const {
    if (category == "NORMAL") return 2;
    if (category == "MINIBOSS") return 3;
    return 4;
}

void GraphicalGame::run() {
    while (window.isOpen()) {
        handleEvents();
        render();
    }
}

void GraphicalGame::handleEvents() {
    while (std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) { window.close(); continue; }

        if (const auto* text = event->getIf<sf::Event::TextEntered>()) {
            if (currentScreen == Screen::NameInput) handleTextEntered(static_cast<char>(text->unicode));
        }

        if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
            if (currentScreen == Screen::Items) {
                if (key->scancode == sf::Keyboard::Scancode::Down && itemScrollOffset < static_cast<int>(items.size()) - 5) itemScrollOffset++;
                if (key->scancode == sf::Keyboard::Scancode::Up && itemScrollOffset > 0) itemScrollOffset--;
            }

            if (currentScreen == Screen::BattleItems) {
                if (key->scancode == sf::Keyboard::Scancode::Down && battleItemScrollOffset < static_cast<int>(items.size()) - 5) battleItemScrollOffset++;
                if (key->scancode == sf::Keyboard::Scancode::Up && battleItemScrollOffset > 0) battleItemScrollOffset--;
            }

            if (key->scancode == sf::Keyboard::Scancode::Escape) {
                if (currentScreen != Screen::NameInput && currentScreen != Screen::Ending && currentScreen != Screen::GameOver && currentScreen != Screen::LoadError) {
                    currentScreen = Screen::MainMenu;
                    message = "Choisissez une option.";
                }
                continue;
            }

            if (currentScreen == Screen::NameInput && key->scancode == sf::Keyboard::Scancode::Enter) {
                playerName = typedName.empty() ? "JOUEUR" : typedName;
                currentScreen = Screen::MainMenu;
                selectedMenuIndex = 0;
                message = "Choisissez une option.";
                continue;
            }

            if (currentScreen == Screen::MainMenu) {
                if (key->scancode == sf::Keyboard::Scancode::Down) { selectedMenuIndex++; if (selectedMenuIndex > 4) selectedMenuIndex = 0; }
                if (key->scancode == sf::Keyboard::Scancode::Up) { selectedMenuIndex--; if (selectedMenuIndex < 0) selectedMenuIndex = 4; }
                if (key->scancode == sf::Keyboard::Scancode::Enter) {
                    if (selectedMenuIndex == 0) currentScreen = Screen::Bestiary;
                    else if (selectedMenuIndex == 1) startBattle();
                    else if (selectedMenuIndex == 2) currentScreen = Screen::Stats;
                    else if (selectedMenuIndex == 3) currentScreen = Screen::Items;
                    else if (selectedMenuIndex == 4) window.close();
                }
            }
        }

        if (const auto* mouse = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mouse->button == sf::Mouse::Button::Left) handleClick(static_cast<float>(mouse->position.x), static_cast<float>(mouse->position.y));
        }
    }
}

void GraphicalGame::handleTextEntered(char c) {
    if (c == '\b' || c == 8) { if (!typedName.empty()) typedName.pop_back(); return; }
    if (typedName.size() >= 10) return;
    if (c >= 32 && c <= 126) typedName += static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
}

void GraphicalGame::handleClick(float x, float y) {
    const sf::Vector2f mousePosition{x, y};

    if (currentScreen == Screen::MainMenu) {
        for (size_t i = 0; i < mainButtons.size(); ++i) {
            if (mainButtons[i].rect.contains(mousePosition)) {
                selectedMenuIndex = static_cast<int>(i);
                if (i == 0) currentScreen = Screen::Bestiary;
                else if (i == 1) startBattle();
                else if (i == 2) currentScreen = Screen::Stats;
                else if (i == 3) currentScreen = Screen::Items;
                else if (i == 4) window.close();
            }
        }
    } else if (currentScreen == Screen::Battle) {
        for (size_t i = 0; i < battleButtons.size(); ++i) {
            if (battleButtons[i].rect.contains(mousePosition)) {
                if (i == 0) attack();
                else if (i == 1) openBattleAct();
                else if (i == 2) openBattleItems();
                else if (i == 3) mercyAction();
            }
        }
    } else if (currentScreen == Screen::BattleItems) {
        for (size_t i = 0; i < battleItemButtons.size(); ++i) {
            if (battleItemButtons[i].rect.contains(mousePosition)) {
                int realIndex = battleItemScrollOffset + static_cast<int>(i);
                if (realIndex < static_cast<int>(items.size())) useBattleItem(realIndex);
                else currentScreen = Screen::Battle;
            }
        }
    } else if (currentScreen == Screen::BattleAct) {
        for (size_t i = 0; i < battleActButtons.size(); ++i) {
            if (battleActButtons[i].rect.contains(mousePosition)) {
                if (i < currentMonster.actIds.size()) useAct(static_cast<int>(i));
                else currentScreen = Screen::Battle;
            }
        }
    }
}

int GraphicalGame::rollDamage(int defenderMaxHp) {
    std::uniform_int_distribution<int> dist(0, defenderMaxHp);
    return dist(rng);
}

void GraphicalGame::startBattle() {
    currentScreen = Screen::Battle;
    battleItemScrollOffset = 0;

    std::vector<GraphicMonster> possibleMonsters;
    for (const auto& monster : monsters) {
        if (victories < 3 && monster.category == "NORMAL") possibleMonsters.push_back(monster);
        else if (victories >= 3 && victories < 6 && (monster.category == "NORMAL" || monster.category == "MINIBOSS")) possibleMonsters.push_back(monster);
        else if (victories >= 6 && victories < 9) possibleMonsters.push_back(monster);
        else if (victories >= 9 && monster.category == "BOSS") possibleMonsters.push_back(monster);
    }
    if (possibleMonsters.empty()) possibleMonsters = monsters;

    std::uniform_int_distribution<size_t> dist(0, possibleMonsters.size() - 1);
    currentMonster = possibleMonsters[dist(rng)];
    monsterHp = currentMonster.hp;
    monsterMaxHp = currentMonster.hp;
    mercy = 0;
    mercyGoal = currentMonster.mercyGoal;
    message = currentMonster.name + " est apparu !";
}

void GraphicalGame::attack() {
    int damage = rollDamage(monsterMaxHp);
    if (nextPlayerBoost) { damage *= 2; nextPlayerBoost = false; }
    monsterHp -= damage;
    if (monsterHp < 0) monsterHp = 0;
    message = (damage == 0) ? "Votre attaque rate !" : "Vous infligez " + std::to_string(damage) + " degats !";
    if (monsterHp == 0) {
        victories++; killed++; addBestiaryEntry("TUE");
        message = currentMonster.name + " est vaincu !";
        checkVictoryEnding(); return;
    }
    monsterTurn();
}

void GraphicalGame::openBattleItems() {
    currentScreen = Screen::BattleItems;
    message = "Choisissez un item.";
}

void GraphicalGame::useBattleItem(int index) {
    if (index < 0 || index >= static_cast<int>(items.size())) return;
    if (items[index].quantity <= 0) {
        message = "Vous n'avez plus de " + items[index].name + " !";
        currentScreen = Screen::Battle;
        return;
    }
    items[index].quantity--;

    if (items[index].type == "HEAL") {
        playerHp += items[index].healValue;
        if (playerHp > playerMaxHp) playerHp = playerMaxHp;
        message = items[index].name + " utilise ! HP +" + std::to_string(items[index].healValue);
        currentScreen = Screen::Battle; monsterTurn(); return;
    }
    if (items[index].type == "BUFF") {
        nextPlayerBoost = true;
        message = items[index].name + " utilise ! Prochaine attaque boostee.";
        currentScreen = Screen::Battle; monsterTurn(); return;
    }
    if (items[index].type == "DEBUFF") {
        nextMonsterWeakened = true;
        message = items[index].name + " utilise ! Prochaine attaque ennemie reduite.";
        currentScreen = Screen::Battle; monsterTurn(); return;
    }
    if (items[index].type == "MERCY") {
        mercy += mercyGoal / 2;
        if (mercy > mercyGoal) mercy = mercyGoal;
        message = items[index].name + " utilise ! Mercy +50%.";
        currentScreen = Screen::Battle; monsterTurn(); return;
    }
    if (items[index].type == "MERCY_FULL") {
        mercy = mercyGoal;
        message = items[index].name + " utilise ! Mercy au maximum.";
        currentScreen = Screen::Battle; monsterTurn(); return;
    }
    if (items[index].type == "KILL") {
        monsterHp = 0; victories++; killed++; addBestiaryEntry("TUE");
        message = items[index].name + " utilise ! " + currentMonster.name + " est vaincu.";
        checkVictoryEnding(); return;
    }
    message = "Item sans effet.";
    currentScreen = Screen::Battle;
}

void GraphicalGame::openBattleAct() {
    battleActButtons.clear();
    int limit = allowedActCount(currentMonster.category);
    int count = std::min(limit, static_cast<int>(currentMonster.actIds.size()));
    float y = 230.f;
    for (int i = 0; i < count; ++i) {
        battleActButtons.push_back({sf::FloatRect({220.f, y}, {580.f, 50.f}), currentMonster.actIds[i]});
        y += 70.f;
    }
    battleActButtons.push_back({sf::FloatRect({220.f, y}, {580.f, 50.f}), "RETOUR"});
    currentScreen = Screen::BattleAct;
    message = "Choisissez une action ACT.";
}

void GraphicalGame::useAct(int index) {
    if (index < 0 || index >= static_cast<int>(currentMonster.actIds.size())) return;
    std::string id = currentMonster.actIds[index];
    if (actCatalog.find(id) != actCatalog.end()) {
        ActAction action = actCatalog[id];
        mercy += action.mercyImpact;
        if (mercy < 0) mercy = 0;
        if (mercy > mercyGoal) mercy = mercyGoal;
        message = action.text + " Mercy " + std::to_string(action.mercyImpact);
    } else message = "Action ACT inconnue.";
    currentScreen = Screen::Battle;
    monsterTurn();
}

void GraphicalGame::mercyAction() {
    if (mercy >= mercyGoal) {
        victories++; spared++; addBestiaryEntry("EPARGNE");
        message = "Vous epargnez " + currentMonster.name + " !";
        checkVictoryEnding();
    } else {
        message = "Mercy insuffisante !";
        monsterTurn();
    }
}

void GraphicalGame::monsterTurn() {
    if (currentScreen == Screen::Ending) return;
    int damage = rollDamage(playerMaxHp);
    if (nextMonsterWeakened) { damage /= 2; nextMonsterWeakened = false; }
    playerHp -= damage;
    if (playerHp < 0) playerHp = 0;
    if (damage == 0) message += " Le monstre rate son attaque.";
    else message += " " + currentMonster.name + " inflige " + std::to_string(damage) + " degats.";
    if (playerHp == 0) { currentScreen = Screen::GameOver; message = "Vous avez perdu."; }
}

void GraphicalGame::addBestiaryEntry(const std::string& result) {
    bestiaryEntries.push_back({currentMonster.name, currentMonster.category, currentMonster.hp, currentMonster.atk, currentMonster.def, result, currentMonster.spriteType});
}

void GraphicalGame::checkVictoryEnding() {
    currentScreen = (victories >= 10) ? Screen::Ending : Screen::MainMenu;
}

void GraphicalGame::render() {
    window.clear(sf::Color::White);
    if (currentScreen == Screen::NameInput) drawNameInput();
    else if (currentScreen == Screen::MainMenu) drawMainMenu();
    else if (currentScreen == Screen::Battle) drawBattle();
    else if (currentScreen == Screen::BattleItems) drawBattleItems();
    else if (currentScreen == Screen::BattleAct) drawBattleAct();
    else if (currentScreen == Screen::Bestiary) drawBestiary();
    else if (currentScreen == Screen::Stats) drawStats();
    else if (currentScreen == Screen::Items) drawItems();
    else if (currentScreen == Screen::Ending) drawEnding();
    else if (currentScreen == Screen::GameOver) drawGameOver();
    else if (currentScreen == Screen::LoadError) drawLoadError();
    window.display();
}

void GraphicalGame::drawNameInput() {
    drawDoubleBox(70.f, 70.f, 880.f, 600.f);
    drawText("ALTERDUNE", 245.f, 135.f, 48);
    drawDoubleBox(150.f, 270.f, 720.f, 130.f);
    drawText("Entrez le nom", 200.f, 305.f, 22);
    drawText("de votre personnage.", 200.f, 345.f, 22);
    drawSprite(cursorTexture, 175.f, 495.f, 0.45f);
    drawText("PSEUDO: " + typedName + "_", 230.f, 500.f, 22);
}

void GraphicalGame::drawMainMenu() {
    drawDoubleBox(70.f, 60.f, 880.f, 610.f);
    drawText(playerName, 115.f, 95.f, 22);
    drawText("HP: " + std::to_string(playerHp) + "/" + std::to_string(playerMaxHp), 680.f, 95.f, 22);
    drawText("Victoires: " + std::to_string(victories) + "/10", 115.f, 135.f, 20);
    for (size_t i = 0; i < mainButtons.size(); ++i) {
        if (static_cast<int>(i) == selectedMenuIndex) drawSprite(cursorTexture, mainButtons[i].rect.position.x - 45.f, mainButtons[i].rect.position.y + 9.f, 0.28f);
        drawText(mainButtons[i].label, mainButtons[i].rect.position.x, mainButtons[i].rect.position.y + 10.f, 20);
    }
    drawDoubleBox(70.f, 560.f, 880.f, 85.f);
    drawText(message, 115.f, 590.f, 12);
}

void GraphicalGame::drawBattle() {
    drawDoubleBox(50.f, 45.f, 925.f, 700.f);
    drawText(playerName, 80.f, 75.f, 18);
    drawText("HP:", 80.f, 112.f, 18);
    drawHpBar(135.f, 114.f, playerHp, playerMaxHp);
    drawText(currentMonster.name, 610.f, 75.f, 16);
    drawText("HP:", 610.f, 112.f, 18);
    drawHpBar(665.f, 114.f, monsterHp, monsterMaxHp);
    drawMercyBar(610.f, 160.f);
    drawSprite(playerTexture, 145.f, 295.f, 0.72f);
    drawSpriteByType(currentMonster.spriteType, 700.f, 280.f, 0.68f);
    drawDoubleBox(75.f, 515.f, 875.f, 90.f);
    drawText(message, 105.f, 545.f, 10);
    for (const Button& button : battleButtons) drawButton(button);
}

void GraphicalGame::drawBattleItems() {
    drawDoubleBox(150.f, 120.f, 740.f, 520.f);
    drawText("CHOISIR UN ITEM", 290.f, 160.f, 22);
    battleItemButtons.clear();
    int maxVisible = 5;
    float y = 230.f;
    for (int i = 0; i < maxVisible; ++i) {
        int index = battleItemScrollOffset + i;
        if (index >= static_cast<int>(items.size())) break;
        Button b;
        b.rect = sf::FloatRect({220.f, y}, {580.f, 50.f});
        b.label = items[index].name + " x" + std::to_string(items[index].quantity);
        battleItemButtons.push_back(b);
        drawButton(b);
        y += 70.f;
    }
    drawText("UP / DOWN pour defiler", 260.f, 600.f, 12);
}

void GraphicalGame::drawBattleAct() {
    drawDoubleBox(150.f, 120.f, 740.f, 520.f);
    drawText("CHOISIR ACT", 340.f, 160.f, 22);
    for (const Button& button : battleActButtons) drawButton(button);
}

void GraphicalGame::drawBestiary() {
    drawDoubleBox(90.f, 65.f, 840.f, 610.f);
    drawText("BESTIAIRE", 385.f, 100.f, 28);
    if (bestiaryEntries.empty()) {
        drawText("Aucun monstre vaincu.", 260.f, 300.f, 18);
        drawText("Combattez ou epargnez un monstre.", 190.f, 345.f, 14);
        drawText("Appuyez sur ESC pour revenir.", 210.f, 615.f, 14);
        return;
    }
    float y = 150.f;
    for (size_t i = 0; i < bestiaryEntries.size() && i < 4; ++i) {
        const auto& e = bestiaryEntries[i];
        drawDoubleBox(130.f, y, 760.f, 105.f);
        drawSpriteByType(e.spriteType, 150.f, y + 15.f, 0.28f);
        drawText(e.name, 300.f, y + 15.f, 14);
        drawText(e.category, 300.f, y + 40.f, 12);
        drawText("HP:" + std::to_string(e.hp) + " ATK:" + std::to_string(e.atk) + " DEF:" + std::to_string(e.def), 300.f, y + 65.f, 12);
        drawText(e.result, 700.f, y + 38.f, 12);
        y += 120.f;
    }
    drawText("Appuyez sur ESC pour revenir.", 210.f, 615.f, 14);
}

void GraphicalGame::drawStats() {
    drawDoubleBox(90.f, 80.f, 840.f, 580.f);
    drawText("STATISTIQUES", 320.f, 115.f, 24);
    drawSprite(playerTexture, 135.f, 255.f, 0.55f);
    drawText("Nom: " + playerName, 380.f, 250.f, 16);
    drawText("HP: " + std::to_string(playerHp) + "/" + std::to_string(playerMaxHp), 380.f, 295.f, 16);
    drawText("Victoires: " + std::to_string(victories) + "/10", 380.f, 340.f, 16);
    drawText("Monstres tues: " + std::to_string(killed), 380.f, 385.f, 16);
    drawText("Monstres epargnes: " + std::to_string(spared), 380.f, 430.f, 16);
    drawText("Appuyez sur ESC pour revenir.", 230.f, 600.f, 14);
}

void GraphicalGame::drawItems() {
    drawDoubleBox(160.f, 95.f, 700.f, 555.f);
    drawText("ITEMS", 415.f, 130.f, 30);
    int maxVisible = 5;
    float y = 210.f;
    for (int i = 0; i < maxVisible; ++i) {
        int index = itemScrollOffset + i;
        if (index >= static_cast<int>(items.size())) break;
        drawText(items[index].name + " x" + std::to_string(items[index].quantity), 220.f, y, 14);
        y += 65.f;
    }
    drawDoubleBox(160.f, 530.f, 700.f, 95.f);
    drawText("UP / DOWN pour defiler", 220.f, 555.f, 12);
    drawText("Items utilisables en combat.", 220.f, 585.f, 12);
}

void GraphicalGame::drawEnding() {
    drawDoubleBox(90.f, 90.f, 840.f, 560.f);
    drawText("FIN DE PARTIE", 300.f, 130.f, 28);
    drawText("Victoires: " + std::to_string(victories) + "/10", 250.f, 230.f, 18);
    drawText("Tues: " + std::to_string(killed), 250.f, 275.f, 18);
    drawText("Epargnes: " + std::to_string(spared), 250.f, 320.f, 18);
    if (killed > 0 && spared == 0) drawText("FIN GENOCIDAIRE", 250.f, 410.f, 22);
    else if (spared > 0 && killed == 0) drawText("FIN PACIFISTE", 250.f, 410.f, 22);
    else drawText("FIN NEUTRE", 250.f, 410.f, 22);
}

void GraphicalGame::drawGameOver() {
    drawDoubleBox(120.f, 120.f, 780.f, 500.f);
    drawText("GAME OVER", 330.f, 260.f, 34);
    drawText("La partie est perdue.", 270.f, 340.f, 18);
}

void GraphicalGame::drawLoadError() {
    drawDoubleBox(100.f, 150.f, 820.f, 420.f);
    drawText("ERREUR CHARGEMENT", 210.f, 230.f, 24);
    drawText(loadErrorMessage, 170.f, 320.f, 14);
}

void GraphicalGame::drawText(const std::string& text, float x, float y, unsigned int size) {
    sf::Text drawableText(font, text, size);
    drawableText.setFillColor(sf::Color::Black);
    drawableText.setPosition({x, y});
    window.draw(drawableText);
}

void GraphicalGame::drawBox(float x, float y, float w, float h) {
    sf::RectangleShape rectangle({w, h});
    rectangle.setPosition({x, y});
    rectangle.setFillColor(sf::Color::White);
    rectangle.setOutlineColor(sf::Color::Black);
    rectangle.setOutlineThickness(4.f);
    window.draw(rectangle);
}

void GraphicalGame::drawDoubleBox(float x, float y, float w, float h) {
    drawBox(x, y, w, h);
    drawBox(x + 8.f, y + 8.f, w - 16.f, h - 16.f);
}

void GraphicalGame::drawButton(const Button& button) {
    drawDoubleBox(button.rect.position.x, button.rect.position.y, button.rect.size.x, button.rect.size.y);
    drawText(button.label, button.rect.position.x + 20.f, button.rect.position.y + 15.f, 11);
}

void GraphicalGame::drawHpBar(float x, float y, int hp, int maxHp) {
    sf::RectangleShape border({170.f, 16.f});
    border.setPosition({x, y});
    border.setFillColor(sf::Color::White);
    border.setOutlineColor(sf::Color::Black);
    border.setOutlineThickness(3.f);
    window.draw(border);
    float ratio = 0.f;
    if (maxHp > 0) ratio = static_cast<float>(hp) / static_cast<float>(maxHp);
    ratio = std::clamp(ratio, 0.f, 1.f);
    sf::RectangleShape bar({170.f * ratio, 16.f});
    bar.setPosition({x, y});
    bar.setFillColor(sf::Color::Black);
    window.draw(bar);
    drawText(std::to_string(hp) + "/" + std::to_string(maxHp), x + 185.f, y - 2.f, 12);
}

void GraphicalGame::drawMercyBar(float x, float y) {
    drawText("MERCY:", x, y, 12);
    sf::RectangleShape border({170.f, 14.f});
    border.setPosition({x + 100.f, y + 2.f});
    border.setFillColor(sf::Color::White);
    border.setOutlineColor(sf::Color::Black);
    border.setOutlineThickness(3.f);
    window.draw(border);
    float ratio = 0.f;
    if (mercyGoal > 0) ratio = static_cast<float>(mercy) / static_cast<float>(mercyGoal);
    ratio = std::clamp(ratio, 0.f, 1.f);
    sf::RectangleShape bar({170.f * ratio, 14.f});
    bar.setPosition({x + 100.f, y + 2.f});
    bar.setFillColor(sf::Color::Black);
    window.draw(bar);
}

void GraphicalGame::drawSpriteByType(int spriteType, float x, float y, float scale) {
    if (spriteType == 0) drawSprite(froggitTexture, x, y, scale);
    else if (spriteType == 1) drawSprite(dustmoleTexture, x, y, scale);
    else if (spriteType == 2) drawSprite(sandbugTexture, x, y, scale);
    else if (spriteType == 3) drawSprite(pebblinTexture, x, y, scale);
    else if (spriteType == 4) drawSprite(glitchratTexture, x, y, scale);
    else if (spriteType == 5) drawSprite(cactinyTexture, x, y, scale);
    else if (spriteType == 6) drawSprite(scrapwormTexture, x, y, scale);
    else if (spriteType == 7) drawSprite(shadebatTexture, x, y, scale);
    else if (spriteType == 8) drawSprite(dryslimeTexture, x, y, scale);
    else if (spriteType == 9) drawSprite(tinytuskTexture, x, y, scale);
    else if (spriteType == 10) drawSprite(mimicboxTexture, x, y, scale);
    else if (spriteType == 11) drawSprite(rusthowlerTexture, x, y, scale);
    else if (spriteType == 12) drawSprite(bonecrawlerTexture, x, y, scale);
    else if (spriteType == 13) drawSprite(ironclawTexture, x, y, scale);
    else if (spriteType == 14) drawSprite(mirageknightTexture, x, y, scale);
    else if (spriteType == 15) drawSprite(venomrootTexture, x, y, scale);
    else if (spriteType == 16) drawSprite(hollowguardTexture, x, y, scale);
    else if (spriteType == 17) drawSprite(ashdrakeTexture, x, y, scale);
    else if (spriteType == 18) drawSprite(queenbyteTexture, x, y, scale);
    else if (spriteType == 19) drawSprite(archonSandTexture, x, y, scale);
    else if (spriteType == 20) drawSprite(titanrootTexture, x, y, scale);
    else if (spriteType == 21) drawSprite(nullemperorTexture, x, y, scale);
    else if (spriteType == 22) drawSprite(crimsonhydraTexture, x, y, scale);
    else if (spriteType == 23) drawSprite(dunereaperTexture, x, y, scale);
}

void GraphicalGame::drawSprite(const sf::Texture& texture, float x, float y, float scale) {
    sf::Sprite sprite(texture);
    sprite.setPosition({x, y});
    sprite.setScale({scale, scale});
    window.draw(sprite);
}
