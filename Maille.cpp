#include "Maille.h"

Maille::Maille(double cr) : _sc(1.), _cr(cr), _broken(false)
{
}

std::vector<int> Maille::voisins()
{
    // FONCTION A CODER
    std::vector<int> V;
    return V;
}

std::vector<int> Maille::rompre()
{
    // FONCTION A CODER
    this->_broken = true;
    std::vector<int> V;
    return V;
}

double Maille::compute_dc(double contrainte) const
{
    return this->_cr - (this->_sc - 1.) * contrainte;
}
