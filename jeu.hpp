#pragma once
#include "grille.hpp"
#include "interface.hpp"
#include <queue>
#include <cstdlib>
#include <ctime>

void solveur(mat & m, mat & mat_deduc, queue<int> & q, int & x, int & y);

int partie(RenderWindow & window);
bool victoire(mat & m);
