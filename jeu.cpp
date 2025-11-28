#include "jeu.hpp"
#include "utils.hpp"
#include <SFML/Graphics.hpp>
using namespace sf;

// verifier la logique des coord x, y !
// on considere les coords a partir du coin haut gauche.
// (x,y) correspond à m[y][x]

void solveur(mat & m){

}

int partie(RenderWindow & window){
    mat m = initialisation_grille();
    affiche_tout_matbrut(m);
    int n = m.size();
    int partie_en_cours = 1;
    int joueur_humain = 0;
    cin >> joueur_humain;
    for(;partie_en_cours;){
        
        if(!window.isOpen()) partie_en_cours = 0;
        
        if (joueur_humain){
            int x; int y; int drap;
            gererEvenements(m, window, x, y, drap);
            if (x != -1 && y != -1 && coord(x, y, n)) {
                //verifier que les coords sont bien entrees
                partie_en_cours = cliquer_case(x, y, drap, m);
            }
        } else { //a modifier
            solveur(m);
        }
        
        fenetre(m, drapeaux_restants, window);
        
        //tests
        if(!partie_en_cours) {
            revel_bombes(m);
           
            return defaite(window);
        }
        
        else if (victoire(m)) {
            return vict(window);
        }
    }
    return 0;
}

// pour que si on decouvre toutes les cases a decouvrir, la partie s'arrete
bool victoire(mat & m) {
    return cases_a_decouvrir==cases_decouvertes;
}

/*
    for(;partie_en_cours;){
        affiche_matbrut(m); cout << '\n';
        int drap;
        cout << "Indiquer si vous placer ou non un drapeau" << '\n';
        cin >> drap;
        int x=-1, y=-1;
        for(;x<0||y<0||x>n||y>n;){
            cout << "Veuillez indiquer les coordonnees de la case a cliquer" << '\n';
            cin >> x >> y;  
        }
        partie_en_cours = cliquer_case(x, y, drap, m);
    }
    revel_bombes(m);
    affiche_matbrut(m);
    */