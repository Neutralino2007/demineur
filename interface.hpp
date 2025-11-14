#pragma once
#include "grille.hpp"
#include <SFML/Graphics.hpp>
using namespace sf;

Color gcn(int nombre);
void carre(int i, int j, int couleur, RenderWindow & window);
void bombes(int i, int j, RenderWindow & window);
void drapeaux(int i, int j, RenderWindow & window);
void cases(int n, int i, int j, RenderWindow & window);
void affichecases(mat & m);
int coord(int & x, int & y, int n);
int gererEvenements(mat & m, RenderWindow & window);
int fenetre(mat & m, RenderWindow & window);
