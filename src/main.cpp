#include <iostream>
#include <limits>

#include "Game.h"
#include "GraphicalGame.h"

int main() {
    std::cout << "=====================================\n";
    std::cout << "        ALTERDUNE - LANCEMENT        \n";
    std::cout << "=====================================\n";
    std::cout << "1. Version console\n";
    std::cout << "2. Version graphique SFML\n";
    std::cout << "Votre choix : ";

    int choix;
    std::cin >> choix;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (choix == 1) {
        Game game;
        game.run();
    } else if (choix == 2) {
        GraphicalGame game;
        game.run();
    } else {
        std::cout << "Choix invalide. Version console lancee.\n";
        Game game;
        game.run();
    }

    return 0;
}
