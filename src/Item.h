#ifndef ITEM_H
#define ITEM_H

#include <string>

class Player;

class Item {
private:
    std::string name;
    std::string type;
    int value;
    int quantity;

public:
    Item();
    Item(const std::string& name, const std::string& type, int value, int quantity);

    const std::string& getName() const;
    const std::string& getType() const;
    int getValue() const;
    int getQuantity() const;
    std::string getDescription() const;

    bool hasStock() const;
    bool consumeOne();
    bool use(Player& player);
};

#endif
