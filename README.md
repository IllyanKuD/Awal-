# Awalé en C

Une implémentation complète du jeu traditionnel africain **Awalé (Mancala)** en **C**, avec la possibilité de jouer **contre un autre joueur humain** ou **contre une IA simple**.

---

## Sommaire

1. [Présentation du jeu](#présentation-du-jeu)  
2. [Fonctionnalités du programme](#fonctionnalités-du-programme)  
3. [Installation et exécution](#installation-et-exécution)  
4. [Règles implémentées](#règles-implémentées)  
5. [Explication du code](#explication-du-code)  
6. [IA](#ia)  
7. [Exemple de partie](#exemple-de-partie)  
8. [Améliorations possibles](#améliorations-possibles)  

---

## Présentation du jeu

L’Awalé est un jeu de stratégie traditionnel africain. Chaque joueur possède une rangée de **cases (trous)** contenant des graines.  
Le but du jeu est de **capturer plus de graines que son adversaire** en semant les graines et en respectant certaines règles, notamment la **règle anti-famine**.

---

## Fonctionnalités du programme

- Plateau paramétrable : de **5 à 9 trous par joueur**.  
- **Deux modes de jeu** :
  - 1 joueur : jouer contre une IA (greedy)  
  - 2 joueurs : humain vs humain  
- Déplacement et capture selon les règles de l’Awalé.  
- Respect de la **règle anti-famine** : un joueur ne peut pas vider complètement l’adversaire.  
- Affichage clair du plateau et des scores.  
- Détermination automatique du gagnant.  

---

## Installation et exécution

1. Copier le fichier source `awale.c`.  
2. Compiler le programme avec GCC ou un compilateur C compatible :

```bash
gcc -o awale awale.c
