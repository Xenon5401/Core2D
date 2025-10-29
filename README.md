## Core2D

Un moteur/boîte à outils 2D minimaliste en C++ (CMake): entités, systèmes d'objets, espace de partitionnement, physique simple et stratégie de rendu.

### Fonctionnalités
- **Entités et composants**: structures d'entités simples pour stocker état et comportements.
- **ObjectSystem**: gestion du cycle de vie et mise à jour des objets.
- **PhysicsSystem**: intégration et mises à jour physiques basiques.
- **SpaceTable**: structure de données spatiale pour requêtes efficaces.
- **RenderStrategy**: abstraction de rendu 2D (séparable et remplaçable).
- **Fenêtre**: gestion de la fenêtre et de la boucle principale d'exemple.

## Arborescence du projet
```text
/home/etann/Documents/GitHub/Core2D/
  CMakeLists.txt
  include/
    entity.hpp
    objectsystem.hpp
    physicsystem.hpp
    renderstrategy.hpp
    spacetable.hpp
    window.hpp
  src/
    main.cpp
    objectsystem.cpp
    physicsystem.cpp
    renderstrategy.cpp
    spacetable.cpp
    window.cpp
  build/               # répertoire de build généré par CMake
```

## Prérequis
- Un compilateur C++17 ou plus récent (GCC, Clang ou MSVC)
- CMake ≥ 3.20
- Make/Ninja (ou générateur CMake de votre choix)
- Une plateforme Linux (projet développé/testé sur Linux)

## Configuration et compilation
Recommandé: générer un répertoire de build séparé.

```bash
# Depuis la racine du projet
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

Cela produit l’exécutable d’exemple dans `build/Core2D`.

## Exécution de l’exemple
```bash
./build/Core2D
```

Des fichiers d’objets et binaires intermédiaires se trouvent sous `build/`.

## Détails techniques
- Le point d’entrée se trouve dans `src/main.cpp` et crée la fenêtre, instancie les systèmes, puis lance une boucle de mise à jour/rendu.
- `ObjectSystem`, `PhysicsSystem`, `SpaceTable` et `RenderStrategy` sont découplés pour faciliter les appelle des class et les tests.
- Les class sont exposées via `include/` afin de distinguer et simpliciter leur implémentation.


### Construire en mode Debug
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --config Debug
```

### Nettoyer le build
```bash
cmake --build build --target clean
# ou supprimer le dossier build
rm -rf build
```


## Roadmap
- Intégrer un backend de rendu (OpenGL/SDL2).
- Ajouter une suite de tests unitaires et d’intégration.
- Améliorer la `SpaceTable` (collisions, requêtes de voisinage, AABB).
- Exemples supplémentaires: ECS plus complet, scènes, sprites, tuiles.

