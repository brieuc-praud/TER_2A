#pragma once

#include <vector>

class Cell
{
private:
    double _sc;              // stress coefficient
    double const _bs;        // breaking stress
    bool _broken;            // status of the Cell
    std::vector<Cell *> _NB; // side neighbors (adjacent cells)
    std::vector<Cell *>_unb; // up neighbor
   std::vector<Cell *>_dnb ; // down neighbor

public:
    Cell(double cr);

    void shatter(int L, double red_stress); // break the Cell and manage all of its consequences

    double compute_breaking_stress() const;     // minimal stress to apply in order to break the Cell
    double compute_stress(double Stress) const; // local stress induced on the Cell by the Stress on the tensile specimen

    void add_sc(double dcc);
    void add_NB(Cell *neighbor);
    void add_unb(Cell *neighbor);
    void add_dnb(Cell *neighbor);

    double get_sc() const { return this->_sc; };
    void set_sc(double sc) { this->_sc = sc; };
    double get_bs() const { return this->_bs; };
    std::vector<Cell *> get_NB() const { return this->_NB; };
    std::vector<Cell *> get_unb() const { return this->_unb; };
    std::vector<Cell *> get_dnb() const { return this->_dnb; };
    // void set_unb(Cell *unb) { this->_unb = unb; };
    // void set_dnb(Cell *dnb) { this->_dnb = dnb; };
    // Cell *get_unb() { return this->_unb; };
    // Cell *get_dnb() { return this->_dnb; };
    bool isBroken() const { return this->_broken; };
};