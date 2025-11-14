#pragma once
#include "types.hpp"
#include <vector>

mat creation(int n=16);
mat placer(mat & m, int bombes=nb_bombes);
mat remplir(mat & m);
void affiche_matbrut(mat & m);
void affiche_tout_matbrut(mat & m);
mat revel_cases(int i, int j, mat & m);
mat revel_bombes(mat & m);
int cliquer_case(int i, int j, bool drap, mat & m);
mat initialisation_grille();
