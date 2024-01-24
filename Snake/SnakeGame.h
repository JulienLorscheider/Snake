//
//  main.h
//  Snake
//
//  Created by Julien on 20/01/2024.
//

#include <iostream>
#include <vector>
#include <cstdlib>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>

const int LARGEUR = 45;
const int HAUTEUR = 20;

enum Direction { HAUT, BAS, GAUCHE, DROITE };

struct Segment {
    int x, y;
};

struct Fruit {
    int x, y;
};

class SnakeGame {
public:
    SnakeGame() : direction(DROITE), tailleSerpent(1), enPause(false) {
        serpent.push_back({LARGEUR / 2, HAUTEUR / 2});
        placerFruit();
    }

    void initialiserJeu();
    void afficherJeu();
    void traiterEntree();
    void mettreAJourJeu();
    static void desactiverModeRaw();

    void bouclePrincipale() {
        while (true) {
            afficherJeu();
            traiterEntree();
            
            if (!enPause) {
                mettreAJourJeu();
            }

            usleep(1000);  // pause en microsecondes
        }
    }

private:
    std::vector<Segment> serpent;
    Direction direction;
    int tailleSerpent;
    Fruit fruit;
    bool enPause;
    char fruitsIcones[3] = {'@', '%', '*'};
    char fruitActuel;
    static struct termios orig_termios;

    void placerFruit();
    void effacerEcran();
    void activerModeRaw();
};
