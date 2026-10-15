#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "Item.h"
#include <vector>
#include <string>

class Player : public Entity {
private:
    std::vector<Item> inventory;
    int victories;
    int killedCount;
    int sparedCount;

public:
    Player();
    explicit Player(const std::string& name);

    void setName(const std::string& name);
    void addItem(const Item& item);
    std::vector<Item>& getInventory();
    const std::vector<Item>& getInventory() const;

    bool useItemByIndex(int index);
    void addVictory();
    void addKilled();
    void addSpared();

    int getVictories() const;
    int getKilledCount() const;
    int getSparedCount() const;
};

#endif
