#include "jeu.hpp"


int main(){
    cout << "Mode Graphique" << '\n';
    int jouer = 1;
    for(;jouer;){
        RenderWindow window(VideoMode(16*taille_case, 3*taille_case+16*taille_case), "Demineur");
        cases_decouvertes=0;
        jouer = partie(window);
    }
    return 0;
}