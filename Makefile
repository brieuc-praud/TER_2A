# Compilateur utilisé
CC=g++

# Options en mode optimisé - La variable NDEBUG est définie
OPTIM_FLAG = -O3 -DNDEBUG -I ~/TPC++/EigenLibrary/Eigen -std=c++11 -Wall
# Options en mode debug - La variable NDEBUG n’est pas définie
DEBUG_FLAG = -g -I ~/TPC++/EigenLibrary/Eigen -std=c++11 -Wall
# On choisit comment on compile
CXX_FLAGS = $(DEBUG_FLAG)

# Le nom de l’exécutable
PROG = rmcu # Rupture d'un matériau composite unidirectionnel

# Les fichiers source à compiler
SRC = main.cc Maille.cpp FileTools.cpp

# La commande complète : compile seulement si un fichier a été modifié
$(PROG) : $(SRC)
	$(CC) $(SRC) $(CXX_FLAGS) -o $(PROG)

# Évite de devoir connaitre le nom de l’exécutable
all : $(PROG)

exe : $(PROG)
	./$(PROG)

# Supprime l’exécutable, les fichiers binaires (.o) et ceux temporaires (∼)
clean :
	rm *.o *∼ $(PROG) ./results/*