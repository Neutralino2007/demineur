#include "grille.hpp"
#include <queue>
#include <cstdlib>
#include <ctime>


int drapeaux_restants = nb_bombes;
int cases_decouvertes = 0;


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

//fonctions de jeu
mat revel_cases(int i, int j, mat & m){
    int n = m.size();
    queue<int> q;
    q.push(i);
    q.push(j);
    for(;!q.empty();){
        int a=q.front(); q.pop();
        int b=q.front(); q.pop();
        int& emplac = m[a][b];
        if(!(emplac&bombe) && !(emplac&drapeau) && !(emplac&activation)){
            emplac = emplac|activation; cases_decouvertes++;
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

int cliquer_case(int i, int j, bool drap, mat & m){
    if (!(activation&m[j][i])){
        if(drap) {m[i][j]=m[i][j]^drapeau; (m[i][j]&drapeau) ? drapeaux_restants-- : drapeaux_restants++;}

        else if(!(m[i][j]&drapeau)){
            revel_cases(i, j, m);
            return !(bombe&m[i][j]);
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
mat initialisation_grille_deduc(mat & m, int f){
    srand(time(0));
    mat mat_deduc = creation();
    for(int i = 0; i<(int) m.size(); i++){
        for(int j = 0; j<(int) m.size(); j++){
            mat_deduc[i][j]=m[i][j]&bombadja;
        }
    }
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
//retourne la liste des coordonnees ou il y eu modif

vector<int> deductions(mat & m, mat & mat_deduc){
    int n = m.size();
    vector<int> l;
    for(int i = 0; i<n; i++){
        for(int j = 0; j<n; j++){
            // mise a jour des connaissances
            mat_deduc[i][j]|=m[i][j]&activation;

            //cas 1 : une case a autant de voisins non reveles que son nombre de bombes
            if((activation&mat_deduc[i][j]) && mat_deduc[i][j]&bombadja && !(bombe&mat_deduc[i][j])){
                int itemp, jtemp;
                vector<int> coord;

                for(auto& dir : directions){
                    itemp = i+dir[0];
                    jtemp = j+dir[1];
                    if (0<=itemp && 0<=jtemp && n>itemp && n>jtemp && !(activation&mat_deduc[itemp][jtemp]) && !(bombe&mat_deduc[itemp][jtemp])){
                        coord.push_back(itemp);
                        coord.push_back(jtemp);
                    }
                    cout<<coord.size()<<"\n";
                }
                if (coord.size()==2*(bombadja&mat_deduc[i][j])){
                    
                    for (int e=0; e<(int) coord.size(); e+=2){
                        cout<<"je trouve des bombes \n";
                        mat_deduc[coord[e]][coord[e+1]]|=bombe;
                        cliquer_case(coord[e], coord[e+1], 1, m);
                        cout<<coord[e]<< coord[e+1];
                        //cas 2 : une case est entouree par autant de bombes que son numero
                        for(auto & dir : directions){
                            int itemp = coord[e] + dir[0];
                            int jtemp = coord[e+1] + dir[1];
                            if(0<=itemp && 0<=jtemp && n>itemp && n>jtemp) {
                                mat_deduc[itemp][jtemp]--;
                                if (!(mat_deduc[itemp][jtemp]&bombadja)){
                                    for(auto & dire : directions){
                                        int ibis = itemp + dire[0];
                                        int jbis = jtemp + dire[1];
                                        if(0<=ibis && 0<=jbis && n>ibis && n>jbis && !(mat_deduc[ibis][jbis]&bombe) && !(mat_deduc[ibis][jbis]&activation)){
                                           if(mat_deduc[ibis][jbis]&drapeau){
                                                l.push_back(ibis);
                                                l.push_back(jbis);
                                                mat_deduc[ibis][jbis]|=drapeau;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }                    
	              }  
              }

            
//recup deduction dans solveur dans jeu


            


            //cas3 : voir si dispositions possibles puis conflits : a faire plus tard eventuellement avec backtrcking
        }
    }
    return l; 
}

//fin robot
