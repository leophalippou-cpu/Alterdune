#include "Player.h"

Player::Player() : Entity("Player", 100, 12, 4), victories(0), killedCount(0), sparedCount(0) {}
Player::Player(const std::string& name) : Entity(name, 100, 12, 4), victories(0), killedCount(0), sparedCount(0) {}

void Player::setName(const std::string& newName) { name = newName; }

void Player::addItem(const Item& item) { inventory.push_back(item); }

std::vector<Item>& Player::getInventory() { return inventory; }
const std::vector<Item>& Player::getInventory() const { return inventory; }

bool Player::useItemByIndex(int index) {
    if (index < 0 || index >= static_cast<int>(inventory.size())) return false;
    return inventory[index].use(*this);
}

void Player::addVictory() { victories++; }
void Player::addKilled() { killedCount++; }
void Player::addSpared() { sparedCount++; }

int Player::getVictories() const { return victories; }
int Player::getKilledCount() const { return killedCount; }
int Player::getSparedCount() const { return sparedCount; }
