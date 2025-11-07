CXX = g++
CXXFLAGS = -std=c++23 -Wall
SFML_FLAGS = -lsfml-graphics -lsfml-window -lsfml-system

# Détection du système pour les chemins SFML
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    # Linux
    SFML_INCLUDE = 
    SFML_LIB = 
else ifeq ($(UNAME_S),Darwin)
    # macOS
    SFML_INCLUDE = 
    SFML_LIB = 
else
    # Windows (suppose que SFML est dans le PATH ou répertoire courant)
    SFML_INCLUDE = C:\msys64\mingw64\include\SFML
    SFML_LIB = C:\msys64\mingw64\lib
endif

all: main

main: main.o jeu.o grille.o utils.o interface.o 
	$(CXX) $(CXXFLAGS) -o main main.o jeu.o grille.o utils.o interface.o $(SFML_FLAGS)

main.o: main.cpp jeu.hpp interface.o
	$(CXX) $(CXXFLAGS) -c main.cpp

jeu.o: jeu.cpp jeu.hpp grille.hpp
	$(CXX) $(CXXFLAGS) -c jeu.cpp

grille.o: grille.cpp grille.hpp types.hpp
	$(CXX) $(CXXFLAGS) -c grille.cpp

utils.o: utils.cpp utils.hpp
	$(CXX) $(CXXFLAGS) -c utils.cpp

interface.o: interface.cpp interface.hpp grille.hpp
	$(CXX) $(CXXFLAGS) -c interface.cpp

clean:
	rm -f *.o main

# Cible pour exécuter le programme
run: main
	./main

.PHONY: all clean install-deps run