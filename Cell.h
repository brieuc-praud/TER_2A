#pragma once

#include <vector>

class Cell
{
private:
    double _sc;              // stress coefficient
    double const _bs;        // breaking stress
    bool _broken;            // status of the Cell
    std::vector<Cell *> _NB; // neighbors (adjacent cells)

public:
    Cell(double cr);

    void shatter(); // break the Cell and manage all of its consequences

    double compute_breaking_stress() const;     // minimal stress to apply in order to break the Cell
    double compute_stress(double Stress) const; // local stress induced on the Cell by the Stress on the tensile specimen

    void add_sc(double dcc);
    void add_NB(Cell *neighbor);

    double get_sc() const { return this->_sc; };
    void set_sc(double sc) { this->_sc = sc; };
    double get_bs() const { return this->_bs; };
    std::vector<Cell *> get_NB() const { return this->_NB; };
    bool isBroken() const { return this->_broken; };
};