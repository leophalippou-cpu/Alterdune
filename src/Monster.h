#ifndef MONSTER_H
#define MONSTER_H

#include "Entity.h"
#include <string>
#include <vector>
#include <memory>

class Monster : public Entity {
protected:
    std::string category;
    int mercy;
    int mercyGoal;
    std::vector<std::string> actIds;

public:
    Monster(const std::string& name, int hp, int atk, int def,
            const std::string& category, int mercyGoal,
            const std::vector<std::string>& actIds);
    virtual ~Monster() = default;

    const std::string& getCategory() const;
    int getMercy() const;
    int getMercyGoal() const;
    const std::vector<std::string>& getActIds() const;

    void changeMercy(int delta);
    bool canBeSpared() const;

    virtual int getAllowedActCount() const = 0;
    virtual std::unique_ptr<Monster> clone() const = 0;
};

class NormalMonster : public Monster {
public:
    NormalMonster(const std::string& name, int hp, int atk, int def, int mercyGoal,
                  const std::vector<std::string>& actIds);
    int getAllowedActCount() const override;
    std::unique_ptr<Monster> clone() const override;
};

class MiniBoss : public Monster {
public:
    MiniBoss(const std::string& name, int hp, int atk, int def, int mercyGoal,
             const std::vector<std::string>& actIds);
    int getAllowedActCount() const override;
    std::unique_ptr<Monster> clone() const override;
};

class Boss : public Monster {
public:
    Boss(const std::string& name, int hp, int atk, int def, int mercyGoal,
         const std::vector<std::string>& actIds);
    int getAllowedActCount() const override;
    std::unique_ptr<Monster> clone() const override;
};

#endif
