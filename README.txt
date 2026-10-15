
Contenu :
- data/items.csv : inventaire enrichi et equilibre.
- data/monsters.csv : plus de monstres, categories NORMAL / MINIBOSS / BOSS.
- src/Game.cpp : version console avec apparition equilibree et degats conformes au sujet.
- src/GraphicalGame.cpp / .h : version graphique SFML qui lit les memes CSV.
- assets/ : sprites PNG pour la version graphique.

Equilibrage des apparitions :
- Victoires 0 a 2 : NORMAL uniquement.
- Victoires 3 a 5 : NORMAL majoritaire, MINIBOSS possible.
- Victoires 6 a 8 : NORMAL / MINIBOSS / BOSS avec boss encore rare.
- Victoire 9 : BOSS pour la fin de partie.

Compilation :
g++ -std=c++17 src/*.cpp -o alterdune.exe -I./include -L./lib -lsfml-graphics -lsfml-window -lsfml-system

Lancement :
.\alterdune.exe

