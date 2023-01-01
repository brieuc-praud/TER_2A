#pragma once

#include <vector>

class Maille
{
private:
    double _cc;                     // coefficient de contrainte
    double const _sr;               // contrainte à la rupture
    bool _broken;                   // état de la maille
    std::vector<Maille *> _voisins; // attention à la différence entre "const vector<Maille *>" et "vector<Maille *> const"

public:
    Maille(double cr);

    void rompre(); // gère la rupture de la maille et ses conséquences

    double compute_breaking_stress() const;         // contrainte réduite minimale à appliquer pour rompre la maille
    double compute_stress(double contrainte) const; // contrainte réduite minimale à appliquer pour rompre la maille

    void add_cc(double dcc);
    void add_voisins(Maille *V);

    double get_cc() const { return this->_cc; };
    void set_cc(double cc) { this->_cc = cc; };
    double get_sr() const { return this->_sr; };
    std::vector<Maille *> get_voisins() const { return this->_voisins; };
    bool isBroken() const { return this->_broken; };
};