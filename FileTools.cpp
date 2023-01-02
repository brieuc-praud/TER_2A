#include <iostream>
#include <fstream>
#include "FileTools.h"

void SaveSolution(std::string file_name, int Nx, int Ny, int Nz, std::vector<Cell *> const &M, double global_stress)
{
    std::ofstream solution;
    solution.open(file_name, std::ios::out);

    solution << "# vtk DataFile Version 3.0" << std::endl;
    solution << "Tensile test of a unidirectional composite material" << std::endl;
    solution << "ASCII" << std::endl;
    solution << "DATASET STRUCTURED_POINTS" << std::endl;
    solution << "DIMENSIONS " << Nx << " " << Ny << " " << Nz << std::endl;
    solution << "ORIGIN " << 0. << " " << 0. << " " << 0. << std::endl;
    solution << "SPACING " << 1. << " " << 1. << " " << 1. << std::endl;

    solution << "POINT_DATA " << Nx * Ny * Nz << std::endl;
    solution << "SCALARS breakings float" << std::endl;
    solution << "LOOKUP_TABLE default" << std::endl;
    for (int k = 0; k < Nz; k++)
    {
        for (int j = 0; j < Ny; j++)
        {
            for (int i = 0; i < Nx; i++)
            {
                solution << M[i + (j + k * Ny) * Nx]->isBroken() << " ";
            }
            solution << std::endl;
        }
        solution << std::endl
                 << std::endl;
    }

    solution << "SCALARS stress_coefficient float" << std::endl;
    solution << "LOOKUP_TABLE default" << std::endl;
    for (int k = 0; k < Nz; k++)
    {
        for (int j = 0; j < Ny; j++)
        {
            for (int i = 0; i < Nx; i++)
            {
                solution << M[i + (j + k * Ny) * Nx]->get_sc() << " ";
            }
            solution << std::endl;
        }
        solution << std::endl;
        solution << std::endl;
    }

    solution << "SCALARS breaking_stress float" << std::endl;
    solution << "LOOKUP_TABLE default" << std::endl;
    for (int k = 0; k < Nz; k++)
    {
        for (int j = 0; j < Ny; j++)
        {
            for (int i = 0; i < Nx; i++)
            {
                solution << M[i + (j + k * Ny) * Nx]->compute_breaking_stress() << " ";
            }
            solution << std::endl;
        }
        solution << std::endl
                 << std::endl;
    }

    solution << "SCALARS stress float" << std::endl;
    solution << "LOOKUP_TABLE default" << std::endl;
    for (int k = 0; k < Nz; k++)
    {
        for (int j = 0; j < Ny; j++)
        {
            for (int i = 0; i < Nx; i++)
            {
                solution << M[i + (j + k * Ny) * Nx]->compute_stress(global_stress) << " ";
            }
            solution << std::endl;
        }
        solution << std::endl
                 << std::endl;
    }

    solution.close();
}
