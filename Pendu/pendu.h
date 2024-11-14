#ifndef PENDU_H
#define PENDU_H

#define MAX_ESSAIS 6  // Maximum number of attempts

// Enumerations for difficulty levels and themes
typedef enum { FACILE, MOYEN, DIFFICILE } NiveauDifficulte;
typedef enum { ANIMAUX, OBJETS, METIERS } Theme;

// Prototypes of the functions
void demarrerJeu(NiveauDifficulte niveau, Theme theme);
char* choisirMotSecret(NiveauDifficulte niveau, Theme theme);
void afficherMotCache(char* mot, int* lettresDevinees);
int verifierLettre(char* mot, char lettre, int* lettresDevinees);
void afficherPendu(int essaisRestants);
void selectionnerDifficulteEtTheme();
void sauvegarderScore(const char* joueur, int score);

#endif // PENDU_H
