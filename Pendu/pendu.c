#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "pendu.h"


#define NB_MOTS_FACILES_ANIMAUX 3
#define NB_MOTS_FACILES_OBJETS 3
#define NB_MOTS_FACILES_METIERS 3

#define NB_MOTS_MOYENS_ANIMAUX 3
#define NB_MOTS_MOYENS_OBJETS 3
#define NB_MOTS_MOYENS_METIERS 3

#define NB_MOTS_DIFFICILES_ANIMAUX 3
#define NB_MOTS_DIFFICILES_OBJETS 3
#define NB_MOTS_DIFFICILES_METIERS 3


const char* motsFacilesAnimaux[] = {"chat", "chien", "rat"};
const char* motsFacilesObjets[] = {"clé", "livre", "lampe"};
const char* motsFacilesMetiers[] = {"pompier", "cuistot", "maçon"};


const char* motsMoyensAnimaux[] = {"dauphin", "cheval", "pigeon"};
const char* motsMoyensObjets[] = {"tablette", "ordinateur", "canapé"};
const char* motsMoyensMetiers[] = {"dentiste", "plombier", "électricien"};


const char* motsDifficilesAnimaux[] = {"hippopotame", "chimpanzé", "ornithorynque"};
const char* motsDifficilesObjets[] = {"radiateur", "réfrigérateur", "aspirateur"};
const char* motsDifficilesMetiers[] = {"astrophysicien", "bioingénieur", "ethnologue"};

#define NB_MOTS_FACILES 3
#define NB_MOTS_MOYENS 3
#define NB_MOTS_DIFFICILES 3
#define NB_MOTS_ANIMAUX 3
#define NB_MOTS_OBJETS 3
#define NB_MOTS_METIERS 3

void selectionnerDifficulteEtTheme() {
    int choixDifficulte, choixTheme;
    
    printf("Choisissez un niveau de difficulté:\n");
    printf("1. Facile\n2. Moyen\n3. Difficile\nVotre choix: ");
    scanf("%d", &choixDifficulte);

    NiveauDifficulte niveau;
    switch (choixDifficulte) {
        case 1: niveau = FACILE; break;
        case 2: niveau = MOYEN; break;
        case 3: niveau = DIFFICILE; break;
        default: niveau = FACILE;
    }

    printf("Choisissez un thème:\n");
    printf("1. Animaux\n2. Objets\n3. Métiers\nVotre choix: ");
    scanf("%d", &choixTheme);

    Theme theme;
    switch (choixTheme) {
        case 1: theme = ANIMAUX; break;
        case 2: theme = OBJETS; break;
        case 3: theme = METIERS; break;
        default: theme = ANIMAUX;
    }

    demarrerJeu(niveau, theme);
}

void demarrerJeu(NiveauDifficulte niveau, Theme theme) {
    printf("Bienvenue dans le jeu de Pendu !\n");

    char* motSecret = choisirMotSecret(niveau, theme);
    int lenMot = strlen(motSecret);
    int* lettresDevinees = (int*)malloc(lenMot * sizeof(int));
    if (lettresDevinees == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < lenMot; i++) {
        lettresDevinees[i] = 0;
    }

    int essaisRestants = MAX_ESSAIS;
    int finDuJeu = 0;
    char lettre;

    
    char nomJoueur[50];
    printf("Entrez votre nom : ");
    scanf("%s", nomJoueur);

    while (essaisRestants > 0 && !finDuJeu) {
        afficherMotCache(motSecret, lettresDevinees);
        printf("\nEssais restants : %d\n", essaisRestants);
        afficherPendu(essaisRestants);

        printf("Entrez une lettre : ");
        scanf(" %c", &lettre);

        int nombreOccurrences = verifierLettre(motSecret, lettre, lettresDevinees);
        if (nombreOccurrences == 0) {
            essaisRestants--;
        }

        
        finDuJeu = 1;
        for (int i = 0; i < lenMot; i++) {
            if (lettresDevinees[i] == 0) {
                finDuJeu = 0;
                break;
            }
        }
    }

    if (finDuJeu) {
        printf("Félicitations ! Vous avez trouvé le mot : %s\n", motSecret);
        sauvegarderScore(nomJoueur, essaisRestants);
    } else {
        printf("Dommage ! Vous avez perdu. Le mot était : %s\n", motSecret);
    }

    free(lettresDevinees);
}

char* choisirMotSecret(NiveauDifficulte niveau, Theme theme) {
    srand(time(NULL)); 
    int index;

    switch (niveau) {
        case FACILE:
            switch (theme) {
                case ANIMAUX:
                    return (char*)motsFacilesAnimaux[rand() % NB_MOTS_FACILES_ANIMAUX];
                case OBJETS:
                    return (char*)motsFacilesObjets[rand() % NB_MOTS_FACILES_OBJETS];
                case METIERS:
                    return (char*)motsFacilesMetiers[rand() % NB_MOTS_FACILES_METIERS];
                default:
                    return (char*)motsFacilesAnimaux[0]; 
            }

        case MOYEN:
            switch (theme) {
                case ANIMAUX:
                    return (char*)motsMoyensAnimaux[rand() % NB_MOTS_MOYENS_ANIMAUX];
                case OBJETS:
                    return (char*)motsMoyensObjets[rand() % NB_MOTS_MOYENS_OBJETS];
                case METIERS:
                    return (char*)motsMoyensMetiers[rand() % NB_MOTS_MOYENS_METIERS];
                default:
                    return (char*)motsMoyensAnimaux[0]; 
            }

        case DIFFICILE:
            switch (theme) {
                case ANIMAUX:
                    return (char*)motsDifficilesAnimaux[rand() % NB_MOTS_DIFFICILES_ANIMAUX];
                case OBJETS:
                    return (char*)motsDifficilesObjets[rand() % NB_MOTS_DIFFICILES_OBJETS];
                case METIERS:
                    return (char*)motsDifficilesMetiers[rand() % NB_MOTS_DIFFICILES_METIERS];
                default:
                    return (char*)motsDifficilesAnimaux[0]; 
            }

        default:
            return (char*)motsFacilesAnimaux[0]; 
    }
}

void afficherMotCache(char* mot, int* lettresDevinees) {
    for (int i = 0; i < strlen(mot); i++) {
        if (lettresDevinees[i]) {
            printf("%c ", mot[i]);
        } else {
            printf("_ ");
        }
    }
    printf("\n");
}

int verifierLettre(char* mot, char lettre, int* lettresDevinees) {
    int nombreOccurrences = 0;
    for (int i = 0; i < strlen(mot); i++) {
        if (mot[i] == lettre && !lettresDevinees[i]) {
            lettresDevinees[i] = 1;
            nombreOccurrences++;
        }
    }
    return nombreOccurrences;
}

void afficherPendu(int essaisRestants) {
    switch (essaisRestants) {
        case 6:
            printf("\n\n\n\n\n");
            break;
        case 5:
            printf("  O\n\n\n\n");
            break;
        case 4:
            printf("  O\n  |\n\n\n");
            break;
        case 3:
            printf("  O\n /|\n\n\n");
            break;
        case 2:
            printf("  O\n /|\\\n\n\n");
            break;
        case 1:
            printf("  O\n /|\\\n /\n");
            break;
        case 0:
            printf("  O\n /|\\\n / \\\n");
            break;
    }
}

void sauvegarderScore(const char* joueur, int score) {
    FILE* fichier = fopen("scores.txt", "a");
    if (fichier != NULL) {
        fprintf(fichier, "Joueur: %s, Score: %d\n", joueur, score);
        fclose(fichier);
    }
}
