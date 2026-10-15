#ifndef ENTITY_H
#define ENTITY_H

#include <string>

class Entity {
protected:
    std::string name;
    int hp;
    int maxHp;
    int atk;
    int def;

public:
    Entity(const std::string& name, int hp, int atk, int def);
    virtual ~Entity() = default;

    const std::string& getName() const;
    int getHp() const;
    int getMaxHp() const;
    int getAtk() const;
    int getDef() const;

    bool isAlive() const;
    void takeDamage(int amount);
    void heal(int amount);
    void fullHeal();
};

#endif
