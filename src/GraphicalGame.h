#ifndef GRAPHICALGAME_H
#define GRAPHICALGAME_H

#include <SFML/Graphics.hpp>
#include <optional>
#include <string>
#include <vector>
#include <map>
#include <random>

class GraphicalGame {
private:
    enum class Screen {
        NameInput,
        MainMenu,
        Battle,
        BattleItems,
        BattleAct,
        Bestiary,
        Stats,
        Items,
        Ending,
        GameOver,
        LoadError
    };

    struct Button {
        sf::FloatRect rect;
        std::string label;
    };

    struct ActAction {
        std::string id;
        std::string text;
        int mercyImpact;
    };

    struct GraphicItem {
        std::string name;
        std::string type;
        int healValue;
        int quantity;
    };

    struct GraphicMonster {
        std::string category;
        std::string name;
        int hp;
        int atk;
        int def;
        int mercyGoal;
        std::vector<std::string> actIds;
        int spriteType;
    };

    struct BestiaryGraphicEntry {
        std::string name;
        std::string category;
        int hp;
        int atk;
        int def;
        std::string result;
        int spriteType;
    };

    sf::RenderWindow window;
    sf::Font font;

    sf::Texture playerTexture;
    sf::Texture cursorTexture;

    sf::Texture froggitTexture;
    sf::Texture dustmoleTexture;
    sf::Texture sandbugTexture;
    sf::Texture pebblinTexture;
    sf::Texture glitchratTexture;
    sf::Texture cactinyTexture;
    sf::Texture scrapwormTexture;
    sf::Texture shadebatTexture;
    sf::Texture dryslimeTexture;
    sf::Texture tinytuskTexture;

    sf::Texture mimicboxTexture;
    sf::Texture rusthowlerTexture;
    sf::Texture bonecrawlerTexture;
    sf::Texture ironclawTexture;
    sf::Texture mirageknightTexture;
    sf::Texture venomrootTexture;
    sf::Texture hollowguardTexture;
    sf::Texture ashdrakeTexture;

    sf::Texture queenbyteTexture;
    sf::Texture archonSandTexture;
    sf::Texture titanrootTexture;
    sf::Texture nullemperorTexture;
    sf::Texture crimsonhydraTexture;
    sf::Texture dunereaperTexture;

    Screen currentScreen;

    std::string typedName;
    std::string playerName;
    std::string message;
    std::string loadErrorMessage;

    int selectedMenuIndex;

    int playerHp;
    int playerMaxHp;

    int monsterHp;
    int monsterMaxHp;
    int mercy;
    int mercyGoal;

    int victories;
    int killed;
    int spared;

    bool nextPlayerBoost = false;
    bool nextMonsterWeakened = false;

    int itemScrollOffset = 0;
    int battleItemScrollOffset = 0;

    GraphicMonster currentMonster;

    std::vector<GraphicMonster> monsters;
    std::vector<GraphicItem> items;
    std::vector<BestiaryGraphicEntry> bestiaryEntries;

    std::map<std::string, ActAction> actCatalog;

    std::vector<Button> mainButtons;
    std::vector<Button> battleButtons;
    std::vector<Button> battleItemButtons;
    std::vector<Button> battleActButtons;

    std::mt19937 rng;

    void loadAssets();
    void buildActCatalog();

    bool loadItems(const std::string& path);
    bool loadMonsters(const std::string& path);

    std::string trim(const std::string& s) const;
    int spriteTypeForMonster(const std::string& name, const std::string& category) const;
    int allowedActCount(const std::string& category) const;

    void handleEvents();
    void handleClick(float x, float y);
    void handleTextEntered(char c);

    void render();

    void drawNameInput();
    void drawMainMenu();
    void drawBattle();
    void drawBattleItems();
    void drawBattleAct();
    void drawBestiary();
    void drawStats();
    void drawItems();
    void drawEnding();
    void drawGameOver();
    void drawLoadError();

    void drawText(const std::string& text, float x, float y, unsigned int size);
    void drawBox(float x, float y, float w, float h);
    void drawDoubleBox(float x, float y, float w, float h);
    void drawButton(const Button& button);
    void drawHpBar(float x, float y, int hp, int maxHp);
    void drawMercyBar(float x, float y);
    void drawSpriteByType(int spriteType, float x, float y, float scale);
    void drawSprite(const sf::Texture& texture, float x, float y, float scale);

    int rollDamage(int defenderMaxHp);

    void startBattle();
    void attack();
    void openBattleItems();
    void useBattleItem(int index);
    void openBattleAct();
    void useAct(int index);
    void mercyAction();
    void monsterTurn();

    void addBestiaryEntry(const std::string& result);
    void checkVictoryEnding();

public:
    GraphicalGame();
    void run();
};

#endif