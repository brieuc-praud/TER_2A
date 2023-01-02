#pragma once

#include <string>
#include "Cell.h"

void SaveSolution(std::string file_name, int Nx, int Ny, int Nz, std::vector<Cell *> const &M, double global_stress);

void ReadParameters(int &Nx, int &Ny, int &Nz, double &V, double &s0, int &m, std::string &file_name_prefix);