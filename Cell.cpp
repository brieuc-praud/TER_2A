#include "Cell.h"
#include <iostream>
Cell::Cell(double bs) : _sc(1.), _bs(bs), _broken(false), _NB(0), _unb(0), _dnb(0)
{
}

void Cell::shatter()
{
    if (this->isBroken())
        return;

    /* simple distribution law */
    this->_broken = true;
    std::vector<Cell *> NB = this->get_NB(); // side neighbors
    std::vector<Cell *> unb = this->get_unb(); // side neighbors
    std::vector<Cell *> dnb = this->get_dnb(); // side neighbors
    std::vector<Cell *> UBNB;                // unbroken side neighbors

    for (long unsigned int i = 0; i < NB.size(); i++)
        if (!NB[i]->isBroken())
            UBNB.push_back(NB[i]);

    int n = UBNB.size();
    double dsc = this->get_sc() / n;
    this->set_sc(0.);

    for (int i = 0; i < n; i++){
        UBNB[i]->add_sc(dsc);
    }

    int n1 = unb.size(); // size of up neighbors
    int n2 = dnb.size(); // size of down neighbors
    double attenuation_factor = 1.0 ;
    for (int i = 0; i < n1; i++){
        attenuation_factor = 1.0-static_cast<double>(i) / L; // modifier ici pour renseigner ce qu'est L
        unb[i]->add_sc(red_stress*attenuation_factor);
    }
    for (int i = 0; i < n2; i++){
        attenuation_factor = 1.0-static_cast<double>(i) / L; // modifier ici pour renseigner ce qu'est L
        dnb[i]->add_sc(red_stress*attenuation_factor);
    }

}

double Cell::compute_breaking_stress() const
{
    if (this->isBroken())
        return 0.;
    else
        return this->get_bs() / this->get_sc();
}

void Cell::add_sc(double dsc)
{
    this->set_sc(this->get_sc() + dsc);
}

void Cell::add_NB(Cell *neighbor)
{
    this->_NB.push_back(neighbor);
}
void Cell::add_unb(Cell *neighbor)
{
    this->_unb.push_back(neighbor);
}
void Cell::add_dnb(Cell *neighbor)
{
    this->_dnb.push_back(neighbor);
}

double Cell::compute_stress(double reduced_stress) const
{
    return this->get_sc() * reduced_stress;
}