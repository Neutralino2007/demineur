#include "jeu.hpp"
#include "utils.hpp"

// gère la partie de jeu en elle même.
// on considere les coords a partir du coin haut gauche.

// fonction de jeu du robot. Joue à partir d'une pile de coordonnées qui ont été determinées comme safe, et lorsqu'elle est vide, choisit au hasard.
void solveur(mat & m, mat & mat_deduc, queue<int> & q, int & x, int & y){
    if (q.empty()){
        vector<int> l = deductions(m, mat_deduc);
        for (int i=0; i<(int) l.size();i++){
            q.push(l[i]); 
        }
        if (q.empty()){
            x = rand()%m.size();
            y = rand()%m.size();
            for(;(mat_deduc[x][y])&bombe || (mat_deduc[x][y])&activation;){
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
//gère l'entièreté de la partie
int partie(RenderWindow & window){
    //début de partie : choix du joueur et initialisation de la partie.
    mat m = initialisation_grille();
    //affiche_tout_matbrut(m);
    int n = m.size();
    int partie_en_cours = 1;
    int joueur_humain = 0;
    queue<int> q;
    cout << "Qui joue ?" << "\n" << "1 -> humain" << "\n" << "0 -> robot" << "\n";
    cin >> joueur_humain;
    mat mat_deduc = initialisation_grille_deduc(m, !joueur_humain);
    //gère les actions du joueur et la fin/continuation de la partie.
    for(;partie_en_cours;){
        if(!window.isOpen()) partie_en_cours = 0;
        int x = -1; int y = -1; int drap=0;
        //Joueur humain : on utilise la position de la souris pour cliquer sur le jeu et on vérifie que les coordonnéess sont bien entrées.
        if (joueur_humain){
            gererEvenements(m, window, x, y, drap);
            if (x != -1 && y != -1 && coord(x, y, n)) {
                
               partie_en_cours = cliquer_case(x, y, drap, m);
            }
        } 
        //Joueur robot : utilise ses déductions pour cliquer sur les cases. 
        else {
            solveur(m, mat_deduc, q, x, y);
            partie_en_cours = cliquer_case(x, y, 0, m);
        } 
        fenetre(m, drapeaux_restants, window);
        //permet de voir l'avancement du raisonnement du robot
        /*if (!joueur_humain){
            Time t = seconds(1);
            sleep(t);
        }*/
        //détermination de la fin du jeu : Si une bombe est révélée, on perd.
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

// Si l'on a decouvert toutes les cases à decouvrir, la partie s'arrête
bool victoire(mat & m) {
    return cases_a_decouvrir==cases_decouvertes;

}


