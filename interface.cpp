#include <iostream>
#include "grille.hpp"
#include "interface.hpp"

using namespace sf;


//Centralisation des couleurs
Color gcn(int nombre) {
    switch(nombre) {
        case 1: return Color(152, 235, 152);// vert clair
        case 2: return Color(61, 209, 62); //vert moyen
        case 3: return Color(39, 97, 39); // vert fonce
        case 4: return Color(0, 0, 128); // Bleu foncé
        case 5: return Color(140, 58, 7); // Marron
        case 6: return Color(245, 190, 39); // jaune citron
        case 7: return Color::Red; 
        case 8: return Color::Magenta;
        default: return Color::Black;
    }
}

//grille
void carre(int i, int j, int couleur, RenderWindow & window){
    RectangleShape carre(Vector2f(taille_case,taille_case));
    carre.setPosition (j*taille_case + chg[0],i*taille_case + chg[1]);
    carre.setFillColor(gcn(couleur));
    carre.setOutlineThickness(2.f);
    carre.setOutlineColor(gcn(3));
    window.draw(carre);
}

void bombes(int i, int j, RenderWindow & window){
    int r = taille_case/4 -1;
    CircleShape pique(r, 8);
    pique.setPosition ((j+0.25)*taille_case+ chg[0],(i+0.25)*taille_case + chg[1]);
    pique.setFillColor(gcn(0));
    pique.setOutlineThickness(2.f);
    pique.setOutlineColor(gcn(6));
    window.draw(pique);  
}

void drapeaux(int i, int j, RenderWindow & window){
    sf::ConvexShape drapeau;
    drapeau.setPointCount(3);
    drapeau.setPoint(0, Vector2f(5, 5));
    drapeau.setPoint(1, Vector2f(5, 20));
    drapeau.setPoint(2, Vector2f(15, 12));
    drapeau.setFillColor(sf::Color::Red);
    drapeau.setPosition(chg[0]+j * taille_case, +chg[1]+i * taille_case);
    window.draw(drapeau);
                    
    // Bâton du drapeau
    RectangleShape baton(Vector2f(2, 35));
    baton.setFillColor(Color::Black);
    baton.setPosition(chg[0]+j * taille_case + 4, chg[1]+i * taille_case + 5);
    window.draw(baton);
}

// verifier switch case
void cases(int n, int i, int j, RenderWindow & window){
    switch(n){
        // case non revelee, pas de souris dessus
        case 1 : carre(i, j, 2, window); break;
        // case decouverte
        case 2 : carre(i, j, 5, window); break;
        //case bombe
        case 3 : carre(i, j, 5, window); bombes(i, j, window); break;
        //case drapeautée
        case 4 : carre(i, j, 2, window); drapeaux(i, j, window); break;
	default : carre(i, j, 8, window);
                       
    }
    
}

//création des cases
void affichecases(mat & m, RenderWindow & window){
    int n = m.size();
    int k = 1;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int & ca = m[i][j];

            if (!ca&&activation){(ca&&drapeau) ? k=4 : k=1;}
            else{(ca&&bombe) ? k=3 : k=2;} 
 
            cases(k, i, j, window);

        }
    }
}

//affichage graphique

// attention logique des coordonnées!
int coord(int & x, int & y, int n){
    int temp = x;
    x-=chg[0]; y/=taille_case;
    y-=chg[0]; temp/=taille_case;
    if(x>=n || y >=n) return 0;
    return 1;
}

// fin affichage graphique

int gererEvenements(mat & m, RenderWindow & window) {
    int n = m.size();
    Event event;
    bool drap = 0;
    int x, y;
    while (window.pollEvent(event)) {    
        if (event.type == Event::MouseButtonPressed) {
            if (event.mouseButton.button == Mouse::Left) {
                x = event.mouseButton.x;
                y = event.mouseButton.y;
                drap = 1;
            }  
            else if (event.mouseButton.button == Mouse::Right) {
                x = event.mouseButton.x;
                y = event.mouseButton.y;  
            }
        }
    }
    if (coord(x, y, n)) return cliquer_case(x, y, drap, m);
    return 1;
}

//affichage de la fenetre
// ctr
int fenetre(mat & m, RenderWindow & window) {
    RectangleShape contourext (Vector2f(16*taille_case,16*taille_case));
    contourext.setPosition (0,3*taille_case);
    
    int continuer = gererEvenements(m, window);

    window.clear();
    window.draw (contourext);
    affichecases(m, window);
    window.display();
    
    return continuer;
}

// Drapeau
/*                   
                    */