#include "grille.hpp"
#include <queue>
#include <cstdlib>
#include <ctime>

mat creation(int n){
    vector<int> ligne(n,0);
    mat m(n,ligne);
    return m;
}

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
Les deux premiers bits représentent l'état drapeau/pas drapeau, l'état activé/caché.
Le 3e bit représente si la case est une bome.
Les quatres derniers bits correspondent au nombre de bombes adjacentes. */

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

// affichage dans terminal
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

// pb quand on ajoute un drapeau, le nombre de bombe restante diminue (ok) mais quand on devoile une case, le nombre de bombe restante augmente

//fonctions de jeu
mat revel_cases(int i, int j, mat & m, int & cases_decouvertes){
    int n = m.size();
    queue<int> q;
    q.push(i);
    q.push(j);
    for(;!q.empty();){
        int a=q.front(); q.pop();
        int b=q.front(); q.pop();
        int& emplac = m[a][b];
        cases_decouvertes++;
        if(!(emplac&bombe) && !(emplac&drapeau)){
            emplac = emplac|activation;
        }
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

mat revel_bombes(mat & m){
    int n = m.size();
    for(int i = 0; i<n; i++){
        for(int j = 0; j<n; j++){
            if(bombe&m[i][j]) m[i][j]= m[i][j]|activation;
        }
    }
    return m;
}

// Y a un pb avec le comptage de bombes restantes, ca fait moins dans tous les cas
int cliquer_case(int i, int j, bool drap, mat & m, int & bombes_restantes, int & cases_decouvertes){
    if (!(activation&m[j][i])){
        if(drap) {
            if (m[i][j]&drapeau){ // operateur ternaire !!!
                bombes_restantes++;
            }
            else {bombes_restantes--;}
            m[j][i]=m[j][i]^drapeau;
        }

        else if(!(m[j][i]&drapeau)){
            m[j][i]=m[j][i]|activation; revel_cases(j, i, m,cases_decouvertes);
            return !(bombe&m[j][i]);
        }
    }
    return 1;
}

mat initialisation_grille(){
    srand(time(0));
    mat m = creation();
    placer(m);
    remplir(m);
    return m;
}
//fin fonctions de jeu

//robot et deduction grille

mat reveler_une_case(mat & m){
   return m; 
}

//fin robot
