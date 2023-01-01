#include "Maille.h"

Maille::Maille(double sr) : _cc(1.), _sr(sr), _broken(false)
{
}

void Maille::rompre()
{
    // une loi de répartition très simple
    this->_broken = true;
    std::vector<Maille *> V = this->get_voisins();
    std::vector<Maille *> not_broken_V;

    for (long unsigned int i = 0; i < V.size(); i++)
        if (!V[i]->isBroken())
            not_broken_V.push_back(V[i]);

    int n = not_broken_V.size();
    double dcc = this->get_cc() / n;
    this->set_cc(0.);

    for (int i = 0; i < n; i++)
        not_broken_V[i]->add_cc(dcc);
}

double Maille::compute_breaking_stress() const
{
    if (this->isBroken())
        return 0.;
    else
        return this->get_sr() / this->get_cc();
}

void Maille::add_cc(double dcc)
{
    this->set_cc(this->get_cc() + dcc);
}

void Maille::add_voisins(Maille *V)
{
    this->_voisins.push_back(V);
}

double Maille::compute_stress(double contrainte_red) const
{
    return this->get_cc() * contrainte_red;
}