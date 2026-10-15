#include "Monster.h"
#include <algorithm>

Monster::Monster(const std::string& name, int hp, int atk, int def,
                 const std::string& category, int mercyGoal,
                 const std::vector<std::string>& actIds)
    : Entity(name, hp, atk, def),
      category(category),
      mercy(0),
      mercyGoal(mercyGoal),
      actIds(actIds) {}

const std::string& Monster::getCategory() const { return category; }
int Monster::getMercy() const { return mercy; }
int Monster::getMercyGoal() const { return mercyGoal; }
const std::vector<std::string>& Monster::getActIds() const { return actIds; }

void Monster::changeMercy(int delta) {
    mercy += delta;
    if (mercy < 0) mercy = 0;
    if (mercy > mercyGoal) mercy = mercyGoal;
}

bool Monster::canBeSpared() const {
    return mercy >= mercyGoal;
}

NormalMonster::NormalMonster(const std::string& name, int hp, int atk, int def, int mercyGoal,
                             const std::vector<std::string>& actIds)
    : Monster(name, hp, atk, def, "NORMAL", mercyGoal, actIds) {}

int NormalMonster::getAllowedActCount() const { return 2; }
std::unique_ptr<Monster> NormalMonster::clone() const { return std::make_unique<NormalMonster>(*this); }

MiniBoss::MiniBoss(const std::string& name, int hp, int atk, int def, int mercyGoal,
                   const std::vector<std::string>& actIds)
    : Monster(name, hp, atk, def, "MINIBOSS", mercyGoal, actIds) {}

int MiniBoss::getAllowedActCount() const { return 3; }
std::unique_ptr<Monster> MiniBoss::clone() const { return std::make_unique<MiniBoss>(*this); }

Boss::Boss(const std::string& name, int hp, int atk, int def, int mercyGoal,
           const std::vector<std::string>& actIds)
    : Monster(name, hp, atk, def, "BOSS", mercyGoal, actIds) {}

int Boss::getAllowedActCount() const { return 4; }
std::unique_ptr<Monster> Boss::clone() const { return std::make_unique<Boss>(*this); }
