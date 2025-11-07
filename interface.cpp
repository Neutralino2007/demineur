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
        case 6: return Color::Cyan;
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
    CircleShape square(r, 4);
    square.setPosition ((j+1/2)*taille_case+ chg[0],(i+1/2)*taille_case + chg[1]);
    square.setFillColor(gcn(0));
    square.setOutlineThickness(2.f);
    square.setOutlineColor(gcn(0));
    window.draw(square);  
}

// verifier switch case
void cases(int n, int i, int j, RenderWindow & window){
    switch(n){
        // case non revelee, pas de souris dessus
        case 1 : carre(i, j, 5, window);
        // case decouverte
        case 2 : carre(i, j, 2, window);
        //case bombe
        case 3 : carre(i, j, 2, window); bombes(i, j, window);
        
                       
    }
    
}

//création des cases
void affichecases(mat & m, RenderWindow & window){
    int n = m.size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cases(3, i, j, window);
        }
    }
}

//affichage de la fenetre
// ctr
void fenetre(mat & m) {
    RenderWindow window(VideoMode(16*taille_case, 3*taille_case+16*taille_case), "Demineur");
    RectangleShape contourext (Vector2f(16*taille_case,16*taille_case));
    contourext.setPosition (0,3*taille_case);
    
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        window.clear();
        window.draw (contourext);
        affichecases(m, window);
        window.display();
    }

}

// Drapeau
/*                   sf::ConvexShape drapeau;
                    drapeau.setPointCount(3);
                    drapeau.setPoint(0, sf::Vector2f(5, 5));
                    drapeau.setPoint(1, sf::Vector2f(5, 20));
                    drapeau.setPoint(2, sf::Vector2f(15, 12));
                    drapeau.setFillColor(sf::Color::Red);
                    drapeau.setPosition(j * taille_case_px, i * taille_case_px);
                    fenetre.draw(drapeau);
                    
                    // Bâton du drapeau
                    sf::RectangleShape baton(sf::Vector2f(2, 35));
                    baton.setFillColor(sf::Color::Black);
                    baton.setPosition(j * taille_case_px + 4, i * taille_case_px + 5);
                    fenetre.draw(baton);
                    */