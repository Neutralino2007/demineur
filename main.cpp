#include "jeu.hpp"
#include <SFML/Graphics.hpp>
using namespace sf;

int main(){
    cout << "Mode Graphique" << '\n';
    int jouer = 1;
    int bombes_restantes = nb_bombes;
    int cases_decouvertes = 0;
    for(;jouer;){
        RenderWindow window(VideoMode(16*taille_case, 3*taille_case+16*taille_case), "Demineur");
        jouer = partie(window, bombes_restantes, cases_decouvertes);
    }
    return 0;
}