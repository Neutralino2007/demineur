#pragma once
#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
using namespace sf;
using namespace std;

// taille cote carre de la grille
const int nb_cases = 16 ;
// nb total de bombes a place
const int nb_bombes = 40;
// cases a decouvrir pour detecter victoire
const int cases_a_decouvrir = nb_cases*nb_cases - nb_bombes;

// constantes permettant d'expliciter les operations bit a bit avec les masques binaires
const int bombe = 1<<4;
const int drapeau = 1<<6;
const int activation = 1<<5;
const int bombadja = 15;

// facilite le parcours des voisins dans une grilles avec toutes les directions possibles
const int directions[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};

// taille de la case pour calculs
const int taille_case = 40;

//coord haut_gauche (origine grille)
const int chg[2] = {0,3*taille_case};

// Bit de verification de traitement solveur
const int queued = 1<<7;

// type matrice
typedef vector<vector<int>> mat;
