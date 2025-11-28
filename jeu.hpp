#pragma once
#include "grille.hpp"
#include "interface.hpp"
#include <SFML/Graphics.hpp>
using namespace sf;

void solveur(mat & m);

int partie(RenderWindow & window);
bool victoire(mat & m);
