#pragma once
#include "types.hpp"
#include <SFML/Graphics.hpp>
using namespace sf;

Color gcn(int nombre);
void carre(int i, int j, int couleur, RenderWindow & window);
void bombes(int i, int j, RenderWindow & window);
void drapeaux(int i, int j, RenderWindow & window);
void afficheNombre(int number, int i, int j, RenderWindow & window);
void cases(int n, int i, int j, int adja, RenderWindow & window);
void affichecases(mat & m);
int coord(int & x, int & y, int n);
void gererEvenements(mat & m, RenderWindow & window, int & nb1, int & nb2, int & nb3);
int vict(RenderWindow & window);
int defaite(RenderWindow & window);
void fenetre(mat & m, int drapeaux_restants, RenderWindow & window);
