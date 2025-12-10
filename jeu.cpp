#include "jeu.hpp"
#include "utils.hpp"

// verifier la logique des coord x, y !
// on considere les coords a partir du coin haut gauche.
// (x,y) correspond à m[y][x]

void solveur(mat & m, mat & mat_deduc, queue<int> & q, int & x, int & y){
    if (q.empty()){
        vector<int> l = deductions(m, mat_deduc);
        for (int i=0; i<(int) l.size();i++){
            q.push(l[i]); 
        }
        if (q.empty()){
            x = rand()%m.size();
            y = rand()%m.size();
            for(;!((mat_deduc[x][y])&bombe) && !((mat_deduc[x][y])&activation);){
                x = rand()%m.size();
                y = rand()%m.size();
            }
        }
    }
    else{
        x = q.front(); q.pop();
        y = q.front(); q.pop();
    }

}

int partie(RenderWindow & window){
    mat m = initialisation_grille();
    affiche_tout_matbrut(m);
    int n = m.size();
    int partie_en_cours = 1;
    int joueur_humain = 0;
    queue<int> q;
    cout << "Qui joue ?" << "\n" << "1 -> humain" << "\n" << "0 -> robot" << "\n";
    cin >> joueur_humain;
    mat mat_deduc = initialisation_grille_deduc(m, !joueur_humain);
    for(;partie_en_cours;){
        
        if(!window.isOpen()) partie_en_cours = 0;
        int x; int y;int drap=0;
        if (joueur_humain){
            gererEvenements(m, window, x, y, drap);
            if (x != -1 && y != -1 && coord(x, y, n)) {
                //verifier que les coords sont bien entrees
               partie_en_cours = cliquer_case(x, y, drap, m);
            }
        } else {
            solveur(m, mat_deduc, q, x, y);
            partie_en_cours = cliquer_case(x, y, drap, m);
            cout<<"je clique sur la case "<<x<<" "<< y<<"\n";
        } 
        
        
        fenetre(m, drapeaux_restants, window);
        if (!joueur_humain){
            Time t = seconds(1);
            sleep(t);
        }
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