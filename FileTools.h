#pragma once

#include <string>
#include "Cell.h"

void SaveSolution(std::string file_name, int Nx, int Ny, int Nz, std::vector<Cell *> const &M, double global_stress);