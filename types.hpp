#pragma once
#include <vector>
#include <iostream>
using namespace std;

const int nb_bombes = 40;
const int bombe = 1<<4;
const int drapeau = 1<<6;
const int activation = 1<<5;
const int bombadja = 15;
const int directions[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};

const int taille_case = 40;

//coord haut_gauche (origine grille)
const int chg[2] = {0,3*taille_case};

typedef vector<vector<int>> mat;
