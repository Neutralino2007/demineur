#include "jeu.hpp"
#include <SFML/Graphics.hpp>
using namespace sf;

int main(){
    cout << "Mode Graphique" << '\n';
    int jouer = 1;
    for(;jouer;){
        RenderWindow window(VideoMode(16*taille_case, 3*taille_case+16*taille_case), "Demineur");

        partie(window);
        cout << '\n' << "voulez-vous rejouer" << '\n';
        cin >> jouer;
    }
    return 0;
}