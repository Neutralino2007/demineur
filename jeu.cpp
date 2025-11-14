#include "jeu.hpp"
#include "interface.hpp"
#include <SFML/Graphics.hpp>
using namespace sf;

// verifier la logique des coord x, y !
// on considere les coords a partir du coin haut gauche.
// (x,y) correspond à m[y][x]

int partie(RenderWindow & window){
    mat m = initialisation_grille();
    affiche_tout_matbrut(m);
    int n = m.size();
    int partie_en_cours = 1;
    for(;partie_en_cours;){
        partie_en_cours = fenetre(m, window);
    }
    return 0;
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