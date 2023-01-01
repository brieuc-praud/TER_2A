#pragma once

#include <string>
#include "Maille.h"

void SaveSolution(std::string file_name, int Nx, int Ny, std::vector<Maille *> const &M, double contrainte);