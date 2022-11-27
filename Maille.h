#pragma once

#include <vector>

class Maille
{
private:
    double _sc;       // coefficient de surcontrainte
    double const _cr; // contrainte à la rupture
    bool _broken;     // état de la maille

public:
    Maille(double cr);

    std::vector<int> voisins(); // renvoie les indices des mailles voisines

    std::vector<int> rompre(); // gère la rupture de la maille et ses conséquences

    bool operator<(const Maille &m) const; // opérateur de comparaison pour la fonction sort()

    double compute_dc(double contrainte) const;

    double get_cr() const { return this->_cr; };
    double get_sc() const { return this->_sc; };
    bool isBroken() const { return this->_broken; };
};