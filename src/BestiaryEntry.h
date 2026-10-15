#ifndef BESTIARY_ENTRY_H
#define BESTIARY_ENTRY_H

#include <string>

struct BestiaryEntry {
    std::string name;
    std::string category;
    int maxHp;
    int atk;
    int def;
    std::string result;
};

#endif
