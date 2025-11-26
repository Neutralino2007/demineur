#pragma once
#include "grille.hpp"
#include <SFML/Graphics.hpp>
using namespace sf;

int partie(RenderWindow & window, int & bombes_restantes, int & cases_decouvertes);
bool victoire(int & cases_decouvertes);
