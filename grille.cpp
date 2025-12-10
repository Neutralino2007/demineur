#include "grille.hpp"
#include <queue>
#include <cstdlib>
#include <ctime>

// gestion de l'affichage du nb de bombes restantes suppose (en fonction des drapeaux places)
int drapeaux_restants = nb_bombes;
// peremet de detecter la victoire
int cases_decouvertes = 0;

// initialisation d'une matrice vide
mat creation(int n){
    vector<int> ligne(n,0);
    mat m(n,ligne);
    return m;
}

// placer le nombre de bombe souhaite aleatoirement dans une grille vide
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

/* On utilise le format 00 0 0000
Les deux premiers bits de plus fort poids représentent l'état drapeau/pas drapeau, l'état activé/caché.
Le 3e bit représente si la case est une bome.
Les quatres derniers bits correspondent au nombre de bombes adjacentes. */

// determine les contraintes en calculant pour chaque case le nb de bombes adjacentes.
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

// initialise une grille pour une partie
mat initialisation_grille(){
    srand(time(0));
    mat m = creation();
    placer(m);
    remplir(m);
    return m;
}

// affichage dans terminal du jeu
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

// permet d'avoir acces a l'affcihage de toute la matrice pour debbogage
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

//fin fonctions affichage terminal

//fonctions de jeu

// cette fonction revele les cases qui doivent etre revelees apres un clique
mat revel_cases(int i, int j, mat & m){
    int n = m.size();
    // on gere les case a traiter par une file pour eviter d'eventuels problemes de recursion
    queue<int> q;
    q.push(i);
    q.push(j);
    for(;!q.empty();){
        int a=q.front(); q.pop();
        int b=q.front(); q.pop();
        int& emplac = m[a][b];
        // condition pour determiner si la case courante doit etre revelee
        if(!(emplac&bombe) && !(emplac&drapeau) && !(emplac&activation)){
            emplac = emplac|activation; cases_decouvertes++;
        }
        // si la case est vide on ajoute ses voisines pour une potentielle revelation
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

// en cas de defaite, cette fonction permet d'activer toutes les bombes en meme temps
mat revel_bombes(mat & m){
    int n = m.size();
    for(int i = 0; i<n; i++){
        for(int j = 0; j<n; j++){
            if(bombe&m[i][j]) m[i][j]= m[i][j]|activation;
        }
    }
    return m;
}

// cette fonction gere le fait d'avoir cliquer sur une case
int cliquer_case(int i, int j, bool drap, mat & m){
    // si la case n'est pas deja active
    if (!(activation&m[i][j])){
        // si mode drapeau : changement du bit de drapeau et gestion du nombre de drapeaux restants a afficher
        if(drap) {m[i][j]=m[i][j]^drapeau; (m[i][j]&drapeau) ? drapeaux_restants-- : drapeaux_restants++;}

        // sinon et si la case n'a pas un drapeau de place on revele la cases et potentiellement ses voisines
        else if(!(m[i][j]&drapeau)){
            revel_cases(i, j, m);
            // si la case cliquee etait une bombe on renverra 0 pour signifier la defaite
            return !(bombe&m[i][j]);
        }
    }
    return 1;
}

//robot et deduction grille
// initialisation d'une matrice parallele pour faciliter les deductions et simuler ce a quoi le joueur a rellement acces
mat initialisation_grille_deduc(mat & m, int f){
    mat mat_deduc = creation();
    for(int i = 0; i<(int) m.size(); i++){
        for(int j = 0; j<(int) m.size(); j++){
            mat_deduc[i][j]=m[i][j]&bombadja; // on ne place que les valeurs des contraintes
        }
    }
    // activation d'une case vide aleatoire pour que le robot commence ses deductions
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

//le bool en entree indique s'il y a eu une modification ou non
//retourne la liste des coordonnees des cases deduites comme safes
vector<int> deductions(mat & m, mat & mat_deduc){
    int n = m.size();
    vector<int> l;
    for(int i = 0; i<n; i++){
        for(int j = 0; j<n; j++){
            // mise a jour des connaissances
            mat_deduc[i][j] |= (m[i][j] & activation);

            //cas 1 : une case a autant de voisins non reveles que son nombre de bombes
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
                // coord a les coordonnees des voisins non reveles
                if (coord.size() == 2 * (mat_deduc[i][j] & bombadja)){
                    // si la condition est satisfaite, les voisins sauvegardes doivent etre des bombes
                    for (int e = 0; e < (int) coord.size(); e += 2){
                        int bx = coord[e], by = coord[e+1];
                        // la bombe est marquee dans mat_deduc et un drapeau est affiche sur la matrice reelle
                        mat_deduc[bx][by] |= bombe;
                        // les cases traites sont marques comme pushed dans la file pour eviter les duplicatats
                        mat_deduc[bx][by] |= queued;
                        cliquer_case(bx, by, 1, m);

                        // les valeurs des cases adjacentes doivent être mise à jour.
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
