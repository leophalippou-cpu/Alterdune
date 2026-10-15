#include "Entity.h"
#include <algorithm>

Entity::Entity(const std::string& name, int hp, int atk, int def)
    : name(name), hp(hp), maxHp(hp), atk(atk), def(def) {}

const std::string& Entity::getName() const { return name; }
int Entity::getHp() const { return hp; }
int Entity::getMaxHp() const { return maxHp; }
int Entity::getAtk() const { return atk; }
int Entity::getDef() const { return def; }

bool Entity::isAlive() const { return hp > 0; }

void Entity::takeDamage(int amount) {
    if (amount < 0) amount = 0;
    hp = std::max(0, hp - amount);
}

void Entity::heal(int amount) {
    if (amount < 0) return;
    hp = std::min(maxHp, hp + amount);
}

void Entity::fullHeal() {
    hp = maxHp;
}
