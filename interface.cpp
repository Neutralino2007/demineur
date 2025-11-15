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

void afficheNombre(int number, int i, int j, RenderWindow & window) {
    static Font font;
    static bool fontLoaded = false;
    
    if (!fontLoaded) {
        font.loadFromFile("arial.ttf");
        fontLoaded = true;
    }
    
    Text text;
    text.setFont(font);
    text.setString(to_string(number));
    text.setCharacterSize(20);
    text.setFillColor(gcn(8));
    text.setPosition(j * taille_case + chg[0] + 15, i * taille_case + chg[1] + 10); //modifier 15 et 10 en des calculs dépendants de taille_case pour plus de flexibilite
    window.draw(text);
}

// verifier switch case
void cases(int n, int i, int j, int adja, RenderWindow & window){
    switch(n){
        // case non revelee, pas de souris dessus
        case 1 : carre(i, j, 2, window); break;
        // case decouverte
        case 2 : carre(i, j, 5, window); if (adja) afficheNombre(adja, i, j, window); break;
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
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int & cell = m[i][j];
            int k = 1; // case cachee par defaut
            
            if (cell & activation) {
                // case revelee
                if (cell & bombe) {
                    k = 3; // Bombe
                } else {
                    k = 2; // case revelee sans nombre ou avec nombre
                }
            } else if (cell & drapeau) {
                k = 4; // case drapeautee
            } else {
                k = 1; // case cachee
            }
            
            cases(k, i, j, cell&bombadja, window);
        }
    }
}

// attention logique des coordonnées!
int coord(int & x, int & y, int n){

    int gridX = (x - chg[0]) / taille_case;
    int gridY = (y - chg[1]) / taille_case;
    
    if(gridX >= 0 && gridX < n && gridY >= 0 && gridY < n) {
        x = gridX;
        y = gridY;
        return 1;
    }
    return 0;
}

int gererEvenements(mat & m, RenderWindow & window) {
    int n = m.size();
    Event event;
    bool drap = false;  // false = clique gauche, true = clique droit
    int x = -1, y = -1;
    
    while (window.pollEvent(event)) {    
        if (event.type == Event::Closed) {
            window.close();
            return 0;
        }
        else if (event.type == Event::MouseButtonPressed) {
            if (event.mouseButton.button == Mouse::Left) {
                x = event.mouseButton.x;
                y = event.mouseButton.y;
                drap = false;
            }  
            else if (event.mouseButton.button == Mouse::Right) {
                x = event.mouseButton.x;
                y = event.mouseButton.y;  
                drap = true;
            }
        }
        if (x != -1 && y != -1 && coord(x, y, n)) {
            return cliquer_case(x, y, drap, m);
        }
    }
    
    return 1;
}

//affichage de la fenetre
// ctr
int fenetre(mat & m, RenderWindow & window) {
    RectangleShape contourext (Vector2f(16*taille_case,16*taille_case));
    contourext.setPosition (0,3*taille_case);
    
    // Affichage du nombre de bombes restantes
    RectangleShape bombe_rest (Vector2f(6*taille_case,taille_case));
    bombe_rest.setPosition(taille_case,taille_case);
    bombe_rest.setFillColor(gcn(8)); // changer la couleur
    
    static Font font;
    font.loadFromFile("arial.ttf");
    Text text;
    text.setFont(font);
    text.setString("bombes restantes"); 
    text.setCharacterSize(20);
    text.setFillColor(gcn(6)); // changer la couleur, ca fait mal aux yeux
    text.setPosition(1.25*taille_case,1.12*taille_case);
    

    
    int continuer = gererEvenements(m, window);
    if(!continuer) revel_bombes(m);

    window.clear();
    window.draw (contourext);
    window.draw (bombe_rest);
    window.draw(text);
    affichecases(m, window);
    window.display();
    
    return continuer;

}

