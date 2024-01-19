//
//  main.c
//  Snake
//
//  Created by Julien on 19/01/2024.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <termios.h>

#define LARGEUR 40
#define HAUTEUR 20

typedef enum { HAUT, BAS, GAUCHE, DROITE } Direction;

typedef struct {
    int x, y;
} Segment;

typedef struct {
    int x, y;
} Fruit;

// Le serpent est représenté par une liste de segments
Segment serpent[LARGEUR * HAUTEUR];
int tailleSerpent = 0;
Direction direction = DROITE;

Fruit fruit;

void placerFruit() {
    bool estSurSerpent;
    do {
        estSurSerpent = false;
        fruit.x = rand() % LARGEUR;
        fruit.y = rand() % HAUTEUR;

        for (int i = 0; i < tailleSerpent; i++) {
            if (serpent[i].x == fruit.x && serpent[i].y == fruit.y) {
                estSurSerpent = true;
                break; // Sortir de la boucle si le fruit est sur le serpent
            }
        }
    } while (estSurSerpent); // Continue jusqu'à ce que le fruit ne soit pas sur le serpent
}

// Structure pour les paramètres du terminal
struct termios orig_termios;

// Fonctions pour gérer le mode terminal
void desactiverModeRaw() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void activerModeRaw() {
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(desactiverModeRaw);

    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON);
    raw.c_cc[VMIN] = 0;  // Minimun de caractères à lire
    raw.c_cc[VTIME] = 1; // Temps d'attente en dixièmes de seconde

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

// Fonctions principales du jeu
void initialiserJeu();
void afficherJeu();
void traiterEntree();
void mettreAJourJeu();

int main() {
    // Initialisation du jeu
    initialiserJeu();

    // Boucle principale du jeu
    while (true) {
        afficherJeu();
        traiterEntree();
        mettreAJourJeu();

        // Temporisation (à améliorer plus tard pour un meilleur contrôle du temps)
        sleep(1);
    }

    return 0;
}

void initialiserJeu() {
    activerModeRaw();
    serpent[0].x = LARGEUR / 2;
    serpent[0].y = HAUTEUR / 2;
    tailleSerpent = 1;
    placerFruit(); // Initialise le fruit
}

void afficherJeu() {
    system("clear"); // Efface l'écran

    for (int y = 0; y < HAUTEUR; y++) {
        for (int x = 0; x < LARGEUR; x++) {
            if (x == 0 || y == 0 || x == LARGEUR - 1 || y == HAUTEUR - 1) {
                // Afficher les murs sur les bords
                printf("#");
            } else if (serpent[0].x == x && serpent[0].y == y) {
                // Tête du serpent
                printf("S");
            } else if (fruit.x == x && fruit.y == y) {
                // Fruit
                printf("F");
            } else {
                // Espace vide
                bool estSerpent = false;
                for (int i = 1; i < tailleSerpent; i++) {
                    if (serpent[i].x == x && serpent[i].y == y) {
                        estSerpent = true;
                        printf("s");
                        break;
                    }
                }
                if (!estSerpent) {
                    printf(" ");
                }
            }
        }
        printf("\n");
    }
}

void traiterEntree() {
    char c;
    if (read(STDIN_FILENO, &c, 1) == 1) {
        switch (c) {
            case 'q':
                desactiverModeRaw();
                exit(0);
                break;
            case 'e':
                if (direction != BAS) direction = HAUT;
                break;
            case 'd':
                if (direction != HAUT) direction = BAS;
                break;
            case 's':
                if (direction != DROITE) direction = GAUCHE;
                break;
            case 'f':
                if (direction != GAUCHE) direction = DROITE;
                break;
        }
    }
}

void mettreAJourJeu() {
    // Déplacement de la queue vers la tête (sauf la tête)
    for (int i = tailleSerpent - 1; i > 0; i--) {
        serpent[i] = serpent[i - 1];
    }

    // Mise à jour de la position de la tête en fonction de la direction
    switch (direction) {
        case HAUT:
            serpent[0].y--;
            break;
        case BAS:
            serpent[0].y++;
            break;
        case GAUCHE:
            serpent[0].x--;
            break;
        case DROITE:
            serpent[0].x++;
            break;
    }
    
    // Vérifie si le serpent a mangé le fruit
    if (serpent[0].x == fruit.x && serpent[0].y == fruit.y) {
        tailleSerpent++;
        placerFruit(); // Place un nouveau fruit
    }

    // Vérification des collisions avec les murs
    if (serpent[0].x < 0 || serpent[0].x >= LARGEUR ||
        serpent[0].y < 0 || serpent[0].y >= HAUTEUR) {
        desactiverModeRaw();
        printf("Game Over: Collision avec le mur!\n");
        exit(0);
    }

    // Vérification des collisions avec le corps du serpent
    for (int i = 1; i < tailleSerpent; i++) {
        if (serpent[0].x == serpent[i].x && serpent[0].y == serpent[i].y) {
            desactiverModeRaw();
            printf("Game Over: Collision avec le corps!\n");
            exit(0);
        }
    }
}
