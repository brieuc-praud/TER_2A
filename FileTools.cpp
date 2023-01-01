#include <iostream>
#include <fstream>
#include "FileTools.h"

void SaveSolution(std::string file_name, int Nx, int Ny, int Nz, std::vector<Maille *> const &M, double contrainte)
{
    std::ofstream solution;
    solution.open(file_name, std::ios::out);

    solution << "# vtk DataFile Version 3.0" << std::endl;
    solution << "Essai de traction d'un materiau composite unidirectionnel" << std::endl;
    solution << "ASCII" << std::endl;
    solution << "DATASET STRUCTURED_POINTS" << std::endl;
    solution << "DIMENSIONS " << Nx << " " << Ny << " " << Nz << std::endl;
    solution << "ORIGIN " << 0. << " " << 0. << " " << 0. << std::endl;
    solution << "SPACING " << 1. << " " << 1. << " " << 1. << std::endl;

    solution << "POINT_DATA " << Nx * Ny * Nz << std::endl;
    solution << "SCALARS rupture float" << std::endl;
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

    solution << "SCALARS cc float" << std::endl;
    solution << "LOOKUP_TABLE default" << std::endl;
    for (int k = 0; k < Nz; k++)
    {
        for (int j = 0; j < Ny; j++)
        {
            for (int i = 0; i < Nx; i++)
            {
                solution << M[i + (j + k * Ny) * Nx]->get_cc() << " ";
            }
            solution << std::endl;
        }
        solution << std::endl;
        solution << std::endl;
    }

    solution << "SCALARS contrainte_de_rupture float" << std::endl;
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

    solution << "SCALARS contrainte float" << std::endl;
    solution << "LOOKUP_TABLE default" << std::endl;
    for (int k = 0; k < Nz; k++)
    {
        for (int j = 0; j < Ny; j++)
        {
            for (int i = 0; i < Nx; i++)
            {
                solution << M[i + (j + k * Ny) * Nx]->compute_stress(contrainte) << " ";
            }
            solution << std::endl;
        }
        solution << std::endl
                 << std::endl;
    }

    solution.close();
}
