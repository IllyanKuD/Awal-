#include <stdio.h>
#include <stdbool.h>

/////////////////////
// INITIALISATION  //
/////////////////////

/*
 * Fonction : remplirPlateau
 * --------------------------
 * Initialise le plateau de jeu avec 4 graines dans chaque case.
 *
 * taille   : nombre de trous par joueur
 * plateau  : tableau 2D représentant les deux rangées du plateau
 */
void remplirPlateau(int taille, int plateau[2][taille]){
    for (int i = 0; i < 2; i++){           // Parcours des deux joueurs
        for (int j = 0; j < taille; j++){  // Parcours des trous
            plateau[i][j] = 4;             // 4 graines par case
        }
    }
}

/////////////////////
// AFFICHAGE       //
/////////////////////

/*
 * Fonction : afficherPlateau
 * --------------------------
 * Affiche l'état actuel du plateau pour les deux joueurs.
 *
 * taille  : nombre de trous par joueur
 * plateau : tableau 2D représentant le plateau
 */
void afficherPlateau(int taille, int plateau[2][taille]){
    for (int i = 0; i < 2; i++){
        printf("Joueur %d : ", i + 1);
        for (int j = 0; j < taille; j++){
            printf("[%d] ", plateau[i][j]);
        }
        printf("\n");
    }
}

/*
 * Fonction : deplacement
 * ----------------------
 * Effectue le déplacement des graines depuis la case choisie.
 * Met à jour la position finale (dernierJoueur, derniereCase) pour la capture.
 *
 * taille         : nombre de trous par joueur
 * place          : indice de la case choisie par le joueur
 * Joueur         : joueur courant (0 ou 1)
 * plateau        : plateau de jeu
 * dernierJoueur  : pointeur pour stocker la rangée de la dernière graine
 * derniereCase   : pointeur pour stocker l'indice de la dernière case
 */
void deplacement(int taille, int place, int Joueur,
                 int plateau[2][taille],
                 int *dernierJoueur, int *derniereCase){

    int valeur = plateau[Joueur][place];   // nombre de graines à semer
    plateau[Joueur][place] = 0;            // case vidée

    // Répartition des graines une par une
    for (int i = 0; i < valeur; i++){
        if (Joueur == 0){                  // côté joueur 1
            place++;
            if (place >= taille){          // passage à l'autre rangée
                Joueur = 1;
                place = taille - 1;
            }
        } else {                           // côté joueur 2
            place--;
            if (place < 0){
                Joueur = 0;
                place = 0;
            }
        }
        plateau[Joueur][place]++;          // ajoute une graine
    }

    *dernierJoueur = Joueur;               // sauvegarde dernière position
    *derniereCase = place;
}

/*
 * Fonction : capture
 * ------------------
 * Gère la capture de graines après un déplacement selon les règles de l'Awalé.
 * Respecte la règle anti-famine : on ne peut pas capturer si l’adversaire se retrouve vide.
 *
 * taille         : nombre de trous par joueur
 * plateau        : plateau de jeu
 * dernierJoueur  : rangée de la dernière graine semée
 * derniereCase   : indice de la dernière case semée
 * Joueur         : joueur courant
 * p_Joueur1      : score joueur 1 (pointeur)
 * p_Joueur2      : score joueur 2 (pointeur)
 */
void capture(int taille, int plateau[2][taille],
             int dernierJoueur, int derniereCase,
             int Joueur,
             int *p_Joueur1, int *p_Joueur2){

    if (dernierJoueur == Joueur) return;   // pas de capture sur ses propres cases

    int j = dernierJoueur;
    int c = derniereCase;

    int graines_capturees = 0;

    // 1 Calcul de ce qui serait capturé
    int temp_j = j;
    int temp_c = c;

    while (temp_c >= 0 && temp_c < taille &&
           (plateau[temp_j][temp_c] == 2 || plateau[temp_j][temp_c] == 3)){

        graines_capturees += plateau[temp_j][temp_c];

        // reculer dans le sens inverse du semis
        if (temp_j == 1){
            temp_c++;
            if (temp_c >= taille){
                temp_j = 0;
                temp_c = taille - 1;
            }
        } else {
            temp_c--;
            if (temp_c < 0){
                temp_j = 1;
                temp_c = 0;
            }
        }
    }

    // 2️Vérification anti-famine : adversaire doit rester au moins avec 1 graine
    int total_adversaire = 0;
    for (int i = 0; i < taille; i++){
        total_adversaire += plateau[dernierJoueur][i];
    }
    if (total_adversaire - graines_capturees == 0) return;

    // 3️ Capture effective
    while (c >= 0 && c < taille &&
           (plateau[j][c] == 2 || plateau[j][c] == 3)){

        if (Joueur == 0)
            *p_Joueur1 += plateau[j][c];
        else
            *p_Joueur2 += plateau[j][c];

        plateau[j][c] = 0;

        if (j == 1){
            c++;
            if (c >= taille){
                j = 0;
                c = taille - 1;
            }
        } else {
            c--;
            if (c < 0){
                j = 1;
                c = 0;
            }
        }
    }
}

/*
 * Fonction : changer_joueur
 * --------------------------
 * Change le joueur courant.
 *
 * Joueur : joueur actuel
 * retourne : le joueur suivant (0 ou 1)
 */
int changer_joueur(int Joueur){
    return (Joueur == 0) ? 1 : 0;
}

/*
 * Fonction : condition
 * -------------------
 * Vérifie si l'une des rangées est vide.
 *
 * taille  : nombre de trous par joueur
 * plateau : plateau de jeu
 * retourne : 1 si joueur 1 gagne, 2 si joueur 2 gagne, -1 sinon
 */
int condition(int taille, int plateau[2][taille]){
    int stock1 = 0, stock2 = 0;

    for (int i = 0; i < taille; i++){
        stock1 += plateau[0][i];
        stock2 += plateau[1][i];
    }

    if (stock1 == 0) return 2; // joueur 2 gagne
    if (stock2 == 0) return 1; // joueur 1 gagne

    return -1; // continuer le jeu
}

/*
 * Fonction : gagner
 * -----------------
 * Détermine si un joueur a atteint la condition de victoire.
 *
 * taille  : nombre de trous par joueur
 * plateau : plateau de jeu
 * p_Joueur1 : score joueur 1
 * p_Joueur2 : score joueur 2
 * retourne : 0 = continuer, 1 = joueur 1 gagne, 2 = joueur 2 gagne
 */
int gagner(int taille,int plateau[2][taille], int p_Joueur1, int p_Joueur2){
    if (p_Joueur1 >= 4*taille){
        printf("Le jeu est gagné par le Joueur 1 !\n");
        return 1;
    }
    if (p_Joueur2 >= 4*taille){
        printf("Le jeu est gagné par le Joueur 2 !\n");
        return 2;
    }

    int fin = condition(taille, plateau);
    if (fin != -1) return fin;

    return 0;
}

/*
 * Fonction : afficherScore
 * ------------------------
 * Affiche le score actuel des deux joueurs.
 */
void afficherScore(int p_Joueur1, int p_Joueur2){
    printf("Score Joueur 1 : %d\n", p_Joueur1);
    printf("Score Joueur 2 : %d\n", p_Joueur2);
}

/*
 * Fonction : selection_trous
 * --------------------------
 * Demande au joueur de sélectionner un trou valide pour jouer.
 * Vérifie :
 *  - sortie du jeu (-1)
 *  - limites valides
 *  - case non vide
 *
 * retourne : l'indice du trou choisi ou -1 pour quitter
 */
int selection_trous(int taille, int Joueur, int plateau[2][taille]){
    int selection = -2;

    while (1){
        printf("Choisir la case (-1 = quitter le jeu) : ");
        scanf("%d", &selection);

        if (selection == -1) return -1;

        if (selection < 0 || selection >= taille){
            printf("Choix invalide.\n");
            continue;
        }

        if (plateau[Joueur][selection] == 0){
            printf("Case vide, choisissez une autre case.\n");
            continue;
        }

        return selection;
    }
}
void copierPlateau(int taille, int src[2][taille], int dest[2][taille]) {
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < taille; j++)
            dest[i][j] = src[i][j];
}
// IA simple : choisit le coup qui rapporte le plus de graines
int ia(int taille, int plateau[2][taille], int p_Joueur2) {
    int meilleurCoup = -1;
    int maxGain = -1;

    for (int i = 0; i < taille; i++) {
        if (plateau[1][i] == 0) continue; // case vide

        // Copier le plateau pour simuler le coup
        int plateauSim[2][taille];
        copierPlateau(taille, plateau, plateauSim);
        int dernierJoueur, derniereCase;
        int scoreSim = p_Joueur2;

        // Simuler le déplacement
        deplacement(taille, i, 1, plateauSim, &dernierJoueur, &derniereCase);

        // Simuler la capture
        capture(taille, plateauSim, dernierJoueur, derniereCase, 1, &scoreSim, &(int){0});

        // Choisir le coup avec le gain maximal
        if (scoreSim > maxGain) {
            maxGain = scoreSim;
            meilleurCoup = i;
        }
    }

    // Si tous les coups rapportent 0, choisir le premier valide
    if (meilleurCoup == -1)
        for (int i = 0; i < taille; i++)
            if (plateau[1][i] > 0) return i;

    return meilleurCoup;
}

/*
 * Fonction : jeu
 * ---------------
 * Gère la boucle principale du jeu.
 * Initialise le plateau, les scores et la taille.
 * Alterne les tours et appelle toutes les fonctions nécessaires.
 */
void jeu(){
    int taille = 0;
    int dernierJoueur, derniereCase;
    int p_Joueur1 = 0;
    int p_Joueur2 = 0;    
    int gagnee = 0;
    int trous = 0;
    int Joueur = 0; // 0 = joueur 1, 1 = joueur 2
    int mode = 0;   // 1 = jouer contre IA, 2 = deux joueurs humains

    // Choix du mode de jeu
    while (mode != 1 && mode != 2){
        printf("Voulez-vous jouer à 1 joueur (contre l'IA) ou 2 joueurs ? (1/2) : ");
        scanf("%d", &mode);
    }

    // Demande de la taille du plateau
    while (taille <=4 || taille >=10){
        printf("Choisissez le nombre de trous (5 à 9) : ");
        scanf("%d", &taille);
    }

    int plateau[2][taille];
    remplirPlateau(taille, plateau);

    // Boucle principale
    while (gagnee == 0){
        printf("\n--- Tour du Joueur %d ---\n", Joueur + 1);
        afficherPlateau(taille, plateau);
        afficherScore(p_Joueur1, p_Joueur2);

        // Choisir le coup selon le mode
        if (Joueur == 1 && mode == 1) { 
            // IA joue pour le joueur 2
            trous = ia(taille, plateau, p_Joueur2);
            printf("IA joue : %d\n", trous);
        } else {
            // Humain joue
            trous = selection_trous(taille, Joueur, plateau);
        }

        if (trous == -1){
            printf("Fin du jeu.\n");
            break;
        }

        deplacement(taille, trous, Joueur, plateau,
                    &dernierJoueur, &derniereCase);

        capture(taille, plateau,
                dernierJoueur, derniereCase,
                Joueur,
                &p_Joueur1, &p_Joueur2);

        gagnee = gagner(taille, plateau, p_Joueur1, p_Joueur2);

        Joueur = changer_joueur(Joueur);
    }
}


/*
 * Fonction main
 * --------------
 * Point d'entrée du programme
 */
int main(){
    jeu();
    return 0;
}
