#include "Item.h"
#include "Player.h"
#include <sstream>

Item::Item() : name(""), type("HEAL"), value(0), quantity(0) {}
Item::Item(const std::string& name, const std::string& type, int value, int quantity)
    : name(name), type(type), value(value), quantity(quantity) {}

const std::string& Item::getName() const { return name; }
const std::string& Item::getType() const { return type; }
int Item::getValue() const { return value; }
int Item::getQuantity() const { return quantity; }

std::string Item::getDescription() const {
    std::ostringstream oss;

    if (type == "HEAL") {
        oss << "Soigne " << value << " HP";
    } else if (type == "WEAKEN") {
        oss << "Reduit les degats du monstre au prochain tour";
    } else if (type == "BOOST") {
        oss << "Double les degats de votre prochain FIGHT";
    } else if (type == "MERCY") {
        oss << "Remplit la jauge Mercy a moitie";
    } else if (type == "MERCY_FULL") {
        oss << "Remplit completement la jauge Mercy";
    } else if (type == "KILL") {
        oss << "Bannit instantanement le monstre (test des fins)";
    } else {
        oss << "Effet inconnu";
    }

    return oss.str();
}

bool Item::hasStock() const { return quantity > 0; }

bool Item::consumeOne() {
    if (quantity <= 0) return false;
    quantity--;
    return true;
}

bool Item::use(Player& player) {
    if (quantity <= 0) return false;

    if (type == "HEAL") {
        player.heal(value);
        quantity--;
        return true;
    }

    return false;
}
