#include "grille.hpp"
#include <queue>
#include <cstdlib>
#include <ctime>
//Fabrication de la grille de départ et gestion de sa modification sur place.
//Gestion des déductions du joueur robot.

// Gestion de l'affichage du nombre de bombes restantes supposé (en fonction des drapeaux placés).
int drapeaux_restants = nb_bombes;
// Permet de détecter la victoire.
int cases_decouvertes = 0;

// Initialisation d'une matrice vide
mat creation(int n){
    vector<int> ligne(n,0);
    mat m(n,ligne);
    return m;
}

// Place le nombre de bombe souhaité aléatoirement dans une grille vide.
mat placer(mat & m, int bombes){
    int ligne, colonne;
    for(;bombes>0;){
        ligne=rand()%m.size();
        colonne=rand()%m.size();
        if (m[ligne][colonne]!=bombe) {
            m[ligne][colonne]=bombe;
            bombes--;
        }
    }
    return m;
}

/* On utilise le format 00 0 0000 afin de stocker les informations contenues dans une case.
Premier bit : 1: drapeau; 0: pas de drapeau.
Deuxième bit : 1: case activée; 0: case cachée.
Troisième bit : 1: bombe; 0: case sans bombe.
Bit 4-7 : nombre de bombes adjacentes.*/

//Permet de déterminer le nombre de bombes adjacentes pour chaque case, qui servira comme contrainte pour le joueur.
mat remplir(mat & m){
    int n = m.size();
    for(int i = 0; i<n; i++){
        for(int j = 0; j<n; j++){
            if(m[i][j]&bombe){
                for(auto& dir : directions){
                    int itemp = i+dir[0];
                    int jtemp = j+dir[1];
                    if(0<=itemp && 0<=jtemp && n>itemp && n>jtemp) m[itemp][jtemp]+=1;
                }
            }
        }
    }
    return m;
}

//Initialise de la grille pour une partie
mat initialisation_grille(){
    srand(time(0));
    mat m = creation();
    placer(m);
    remplir(m);
    return m;
}

// Afficher la matrice de jeu dans le terminal (permet de jouer sans affichage sfml)
void affiche_matbrut(mat & m) {
    int n = m.size();
    cout << "  ";
    for(int i = 0; i<n; i++) cout << i%10 << ' ';
    cout << '\n';
    for (int i = 0; i < n; i++) {
        cout << i%10 << ' ';
        for (int j = 0; j < n; j++) {
            if (m[i][j] & drapeau) {
                cout << "F ";
            } else if (m[i][j] & activation) {
                if (m[i][j] & bombe) {
                    cout << "# ";
                } else {
                    cout << to_string((m[i][j] & bombadja)) << ' ';
                }
            } else {
                cout << "_ ";
            }
        }
        cout << '\n';
    }
}

//Permet d'avoir accès a l'affichage de toute la matrice pour debbogage éventuel.
void affiche_tout_matbrut(mat & m) {
    int n = m.size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (m[i][j] & bombe) cout << "# ";
            else cout << to_string((m[i][j] & bombadja)) << ' ';
        }
        cout << '\n';
    }
}

//Fin des fonctions d'affichage dans le terminal

//Fonctions de jeu.

//Cette fonction révèle les cases qui doivent être révelées après un clic.
mat revel_cases(int i, int j, mat & m){
    int n = m.size();
    // on gère les cases à traiter par une file pour éviter d'éventuels problèmes de récursion.
    queue<int> q;
    q.push(i);
    q.push(j);
    for(;!q.empty();){
        int a=q.front(); q.pop();
        int b=q.front(); q.pop();
        int& emplac = m[a][b];
        //condition pour déterminer si la case courante doit être révelée ou non.
        if(!(emplac&bombe) && !(emplac&drapeau) && !(emplac&activation)){
            emplac = emplac|activation; cases_decouvertes++;
        }
        //Si la case est vide, on met de coté ses voisines pour potentiellement les réveler aussi.
        if (!(emplac&bombadja)) for(auto& dir : directions){
            int itemp = a+dir[0];
            int jtemp = b+dir[1];
            if(0<=itemp && 0<=jtemp && n>itemp && n>jtemp){
                int& emplact = m[itemp][jtemp];
                if(!(emplact&bombe) && !(emplact&drapeau) && !(emplact&activation)){q.push(itemp); q.push(jtemp);}
            }
        }
    }
    return m;
}

//En cas de défaite, cette fonction permet d'activer toutes les bombes en même temps.
mat revel_bombes(mat & m){
    int n = m.size();
    for(int i = 0; i<n; i++){
        for(int j = 0; j<n; j++){
            if(bombe&m[i][j]) m[i][j]= m[i][j]|activation;
        }
    }
    return m;
}

//Cette fonction gère le clic du joueur sur une case.
int cliquer_case(int i, int j, bool drap, mat & m){
    //Si la case n'est pas deja active.
    if (!(activation&m[i][j])){
        //Si on met un drapeau, la case devient drapeautée et on change le nombre de drapeau à placer dans le compteur.
        if(drap) {m[i][j]=m[i][j]^drapeau; (m[i][j]&drapeau) ? drapeaux_restants-- : drapeaux_restants++;}

        //Si on cherche à dévoiler la case et que celle-ci n'est pas drapeautée, on révèle la case et potentiellement ses voisines seront aussi révélées.
        else if(!(m[i][j]&drapeau)){
            revel_cases(i, j, m);
            //Si la case cliquée était une bombe, on renvoit 0 pour signifier la défaite du joueur.
            return !(bombe&m[i][j]);
        }
    }
    return 1;
}

//Joueur robot et déduction de la grille
// L'idée est de se servir d'une deuxième matrice afin de simuler ce à quoi le joueur a réellement accès. 
// Elle est construite à partir des déductions du robot et des informations qui lui sont révélées au fur et à mesur du jeu.

//Initialisation de cette matrice parallèle
mat initialisation_grille_deduc(mat & m, int f){
    mat mat_deduc = creation();
    for(int i = 0; i<(int) m.size(); i++){
        for(int j = 0; j<(int) m.size(); j++){
            mat_deduc[i][j]=m[i][j]&bombadja; // on ne place que les valeurs des contraintes
        }
    }
    //Activation d'une case vide de arbitraire pour que le robot puisse commencer ses déductions.
    int x=rand()%m.size();
    int y=rand()%m.size();
    for(;(m[x][y]&bombe) || (m[x][y]&bombadja);){
        x=rand()%m.size();
        y=rand()%m.size();   
    }
    mat_deduc[x][y]|=activation;
    if(f) cliquer_case(x, y, 0, m);
    return mat_deduc;
}

//Retourne la liste des coordonnées des cases déduites comme safe.
vector<int> deductions(mat & m, mat & mat_deduc){
    int n = m.size();
    vector<int> l;
    for(int i = 0; i<n; i++){
        for(int j = 0; j<n; j++){
            // mise a jour des connaissances
            mat_deduc[i][j] |= (m[i][j] & activation);

            //Cas 1 : Une case a autant de voisins non révélés que son nombre de bombes. IOn en déduit que tous ses voisins sont des bombes.
            if((activation & mat_deduc[i][j]) && (mat_deduc[i][j] & bombadja) && !(bombe & mat_deduc[i][j])){
                int itemp, jtemp;
                vector<int> coord;

                for(auto& dir : directions){
                    itemp = i+dir[0];
                    jtemp = j+dir[1];
                    if (0<=itemp && 0<=jtemp && n>itemp && n>jtemp && !(activation & mat_deduc[itemp][jtemp]) && !(bombe & mat_deduc[itemp][jtemp])){
                        coord.push_back(itemp);
                        coord.push_back(jtemp);
                    }
                }
                //coord est la liste des coordonnées des voisins non révélés. 
                if (coord.size() == 2 * (mat_deduc[i][j] & bombadja)){
                    //Si cette condition est satisfaite, les voisins sauvegardés doivent donc être des bombes.
                    for (int e = 0; e < (int) coord.size(); e += 2){
                        int bx = coord[e], by = coord[e+1];
                        //La bombe est marquée dans mat_deduc et un drapeau est affiché sur la matrice réelle.
                        mat_deduc[bx][by] |= bombe;
                        //Les cases traitées sont marquées comme pushed dans la file pour éviter les duplicatats (ie de regarder plusieurs fois la même cas)
                        mat_deduc[bx][by] |= queued;
                        cliquer_case(bx, by, 1, m);

                        //Les valeurs des cases adjacentes sont mises à jour afin de disposer de nouvelles informations pour la déduction.
                        for(auto & dir : directions){
                            int itemp = bx + dir[0];
                            int jtemp = by + dir[1];
                            if(0<=itemp && 0<=jtemp && n>itemp && n>jtemp) {
                                // decrementation securisee des 4 bits de poids faible
                                int nb = mat_deduc[itemp][jtemp] & bombadja;
                                if (nb > 0) nb--;
                                mat_deduc[itemp][jtemp] = (mat_deduc[itemp][jtemp] & ~bombadja) | (nb & bombadja);

                                // cases alentours safes
                                if ((mat_deduc[itemp][jtemp] & bombadja) == 0){
                                    for(auto & dire : directions){
                                        int ibis = itemp + dire[0];
                                        int jbis = jtemp + dire[1];
                                        if(0<=ibis && 0<=jbis && n>ibis && n>jbis && !(mat_deduc[ibis][jbis] & bombe) && !(mat_deduc[ibis][jbis] & activation)){
                                            // securite avec queued
                                            if(!(mat_deduc[ibis][jbis] & queued)){
                                                l.push_back(ibis);
                                                l.push_back(jbis);
                                                mat_deduc[ibis][jbis] |= queued;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            //cas3 : voir si dispositions possibles puis conflits : a faire plus tard eventuellement avec backtrcking
        }
    }
    return l; 
}


//fin robot

