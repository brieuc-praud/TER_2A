#include "Cell.h"

Cell::Cell(double bs) : _sc(1.), _bs(bs), _broken(false)
{
}

void Cell::shatter()
{
    // simple distribution law
    this->_broken = true;
    std::vector<Cell *> NB = this->get_NB();
    std::vector<Cell *> UBNB; // unbroken neighbors

    for (long unsigned int i = 0; i < NB.size(); i++)
        if (!NB[i]->isBroken())
            UBNB.push_back(NB[i]);

    int n = UBNB.size();
    double dsc = this->get_sc() / n;
    this->set_sc(0.);

    for (int i = 0; i < n; i++)
        UBNB[i]->add_sc(dsc);
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

double Cell::compute_stress(double reduced_stress) const
{
    return this->get_sc() * reduced_stress;
}