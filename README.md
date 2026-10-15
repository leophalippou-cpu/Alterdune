# Alterdune

Jeu au tour par tour en C++ avec SFML 3. Deux versions sont disponibles : une version console et une version graphique. Les deux lisent les mêmes fichiers de données (`data/items.csv` et `data/monsters.csv`).

## Lancer le jeu (Windows 64 bits)

1. Sur la page GitHub du dépôt, cliquer sur **Code**, puis **Download ZIP**.
2. Extraire entièrement le ZIP dans un dossier. Ne pas lancer l'exécutable depuis l'aperçu de l'archive.
3. Double-cliquer sur `alterdune.exe`, ou lancer `.\alterdune.exe` depuis ce dossier.
4. Saisir `1` (version console) ou `2` (version graphique), puis valider avec Entrée.

Points à respecter :

- `alterdune.exe` cherche les dossiers `assets/` et `data/` à côté de lui. Il ne faut pas le déplacer seul.
- Les fichiers `.dll` doivent rester dans le même dossier que l'exécutable.
- Si Windows affiche un avertissement SmartScreen, cliquer sur **Informations complémentaires** puis **Exécuter quand même**. L'exécutable n'est pas signé.

## Contenu du dépôt

- `src/` : code source C++ (`Game.cpp` pour la version console, `GraphicalGame.cpp` et `GraphicalGame.h` pour la version graphique SFML).
- `data/items.csv` : inventaire équilibré.
- `data/monsters.csv` : monstres répartis en catégories NORMAL, MINIBOSS et BOSS.
- `assets/` : sprites PNG et police pour la version graphique.
- `alterdune.exe` et les fichiers `.dll` : version compilée pour Windows 64 bits.

Les fichiers `.dll` proviennent de MSYS2 (environnement mingw64). Ils comprennent SFML 3, FreeType et les bibliothèques d'exécution de MinGW. Ils restent sous leurs licences d'origine.

## Équilibrage des apparitions

- Victoires 0 à 2 : monstres NORMAL uniquement.
- Victoires 3 à 5 : NORMAL majoritaire, MINIBOSS possible.
- Victoires 6 à 8 : NORMAL, MINIBOSS et BOSS, avec des boss encore rares.
- Victoire 9 : BOSS pour la fin de partie.

## Compiler soi-même

Il faut un compilateur C++17 (g++) et SFML 3.

Windows (MinGW-w64), SFML placé dans `include/` et `lib/` à la racine du projet :

```
g++ -std=c++17 src/*.cpp -o alterdune.exe -I./include -L./lib -lsfml-graphics -lsfml-window -lsfml-system
```

Linux ou macOS, SFML 3 installé :

```
g++ -std=c++17 src/*.cpp -o alterdune -lsfml-graphics -lsfml-window -lsfml-system
```

Si SFML n'est pas dans un dossier standard, ajouter les options `-I` et `-L` vers ses dossiers `include` et `lib`.

Lancer ensuite le jeu depuis la racine du projet (`.\alterdune.exe` ou `./alterdune`), pour que `assets/` et `data/` soient trouvés.
