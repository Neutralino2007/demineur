#include <iostream>
#include "interface.hpp"


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
        case 9: return Color(166, 166, 166); // gris moyen
        default: return Color::Black;
    }
}

// pour affichage d'un petit carre de la grille
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
    text.setFillColor(gcn(0));
    text.setPosition(j * taille_case + chg[0] + 15, i * taille_case + chg[1] + 10);
    window.draw(text);
}

// gere l'affichage d'une cellule de la grille en fonction de son etat
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

// affiche toutes les cellules de la grille
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

// recupere les coordoonnees transmise paa la position de la souris
int coord(int & x, int & y, int n){

	// prise en compte du decalage de la grille avec l'origine et conversion en entier correspondant à la position dans la matrice
    int gridX = (x - chg[0]) / taille_case;
    int gridY = (y - chg[1]) / taille_case;

	// verification coords valides
    if(gridX >= 0 && gridX < n && gridY >= 0 && gridY < n) {
		// echange des coordonnees pour acceder a la bonne case de la matrice
        y = gridX;
        x = gridY;
        return 1;
    }
    return 0;
}

void gererEvenements(mat & m, RenderWindow & window, int & x, int & y, int & drap) {
    Event event;
    drap = false;  // false = clique gauche, true = clique droit
    x = -1; y = -1;

    while (window.pollEvent(event)) {    
        if (event.type == Event::Closed) {
            window.close();
        }
		// clique gauche, placement d'un drapeau
        else if (event.type == Event::MouseButtonPressed) {
            if (event.mouseButton.button == Mouse::Left) {
                x = event.mouseButton.x;
                y = event.mouseButton.y;
                drap = 0;
            }  
			// clique droit, decouverte d'une case
            else if (event.mouseButton.button == Mouse::Right) {
                x = event.mouseButton.x;
                y = event.mouseButton.y;  
                drap = 1;
            }
        }
    }
}

// afficher bandeau victoire
int vict(RenderWindow & window) {
    RectangleShape band_vict (Vector2f(16*taille_case,3*taille_case));
    band_vict.setPosition(0,6*taille_case);
    band_vict.setFillColor(gcn(6));

    static Font font;
    font.loadFromFile("arial.ttf");
    Text text;
    text.setFont(font);
    text.setString("VICTOIRE !!!"); 
    text.setCharacterSize(50);
    text.setFillColor(gcn(3)); 
    text.setPosition(4*taille_case,6.5*taille_case);

    Text instruc;
    instruc.setFont(font);
    instruc.setString("Appuie sur une touche pour continuer");
    instruc.setCharacterSize(20);
    instruc.setFillColor(gcn(3));
    instruc.setPosition(3.5*taille_case,8*taille_case);

    window.draw(band_vict); window.draw(text);window.draw(instruc);
    window.display();

    Event event;
	// attente pour eventuelle nouvelle partie
    while(window.isOpen()){
        while (window.pollEvent(event)) {    
            if (event.type == Event::Closed) {
                window.close(); return 0;
            }
            if(event.type == Event::KeyPressed) {
                return 1;
            }
        }
    }
    return 0;
}

// afficher bandeau defaite
int defaite(RenderWindow & window) {
    RectangleShape band_def (Vector2f(16*taille_case,3*taille_case));
    band_def.setPosition(0,6*taille_case);
    band_def.setFillColor(gcn(6));

    static Font font;
    font.loadFromFile("arial.ttf");
    Text text;
    text.setFont(font);
    text.setString("DEFAITE ..."); 
    text.setCharacterSize(50);
    text.setFillColor(gcn(7)); 
    text.setPosition(4*taille_case,6.5*taille_case);

    Text instruct;
    instruct.setFont(font);
    instruct.setString("Appuie sur une touche pour continuer");
    instruct.setCharacterSize(20);
    instruct.setFillColor(gcn(4));
    instruct.setPosition(3.5*taille_case,8*taille_case);

    window.draw(band_def); window.draw(text);window.draw(instruct);
    window.display();
    
    Event event;
	// attente pour eventuelle nouvelle partie
    while(window.isOpen()){
        while (window.pollEvent(event)) {    
            if (event.type == Event::Closed) {
                window.close(); return 0;
            }
            if(event.type == Event::KeyPressed) {
                return 1;
            }
        }
    }
    return 0;
}

//affichage de la fenetre
void fenetre(mat & m, int drapeaux_restants, RenderWindow & window) {
    RectangleShape contourext (Vector2f(16*taille_case,3*taille_case));
    contourext.setPosition (0,0);
    contourext.setFillColor(gcn(9));
    
    // Affichage du nombre de bombes restantes
    RectangleShape affiche_bombe_rest (Vector2f(6*taille_case,taille_case));
    affiche_bombe_rest.setPosition(taille_case,taille_case);
    affiche_bombe_rest.setFillColor(gcn(3)); 
    
    static Font font;
    font.loadFromFile("arial.ttf");
    Text text;
    text.setFont(font);
    text.setString("bombes restantes  " + to_string(drapeaux_restants));
    text.setCharacterSize(20);
    text.setFillColor(gcn(6)); 
    text.setPosition(1.25*taille_case,1.12*taille_case);

    window.clear();
    window.draw (contourext);
    window.draw (affiche_bombe_rest);
    window.draw(text);
	// affichage du reste de la grille
    affichecases(m, window);
    window.display();

}

