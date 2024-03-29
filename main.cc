#include "Cell.h"
#include "FileTools.h"
#include <time.h>
#include <cmath>
#include <random>
#include <algorithm>
#include <iostream>
#include <string>

int main(int argc, char **argv)
{

  /* Parameters to read in a file */
  int Nx;                       // X dimension
  int Ny;                       // Y dimension
  int Nz;                       // Z dimension
  int R;                        // L = (R*sigma)/tau
  int sigma; 
  int tau;                       // reload length, only a number of maillon for the moment
  std::string file_name_prefix; // path and filename prefix for the results files
  /* Weibull's law parameters */
  double V;
  double s0;
  double m;
  double Odg;
  /* For reducing the number of output files */
  int nf = 0;
  int mod_nf; // Number of output files : Nx*Ny*Nz/mod_nf
  ReadParameters(Nx, Ny, Nz, R, sigma, tau, Odg, V, s0, m, file_name_prefix, mod_nf);

  int const N = Nx * Ny * Nz; // total number of cells
  int const L = (R*sigma)/tau; // longueur de rechargement

  // Initialization
  std::default_random_engine generator(time(0));
  std::uniform_real_distribution<double> distribution(0.0, 1.0);
  std::vector<Cell *> CELL;   // CELL is storing all the Cells
  std::vector<Cell *> UBCELL; // UBCELL stores only unbroken cells (as a result, the number of Cells to check decline at each iteration)
  std::vector<Cell *>UPCELL;
  std::vector<Cell *>DOWNCELL;


  CELL.resize(N);
  double const coeff = s0 / pow(V, 1. / m);
  for (int i = 0; i < N; i++)
  {
    /* Weibull's law */
    double const p = distribution(generator);
    double const bs = coeff * pow(-log(p), 1. / m);
    /* Create the cell */
    CELL[i] = new Cell(bs);
    UBCELL.push_back(CELL[i]);
  }

  /* Link the Cells */
  for (int i = 0; i < Nx; i++)
  {
    for (int j = 0; j < Ny; j++)
    {
      for (int k = 0; k < Nz; k++)
      {
        if (i > 0)
          CELL[i + (j + k * Ny) * Nx]->add_NB(CELL[i - 1 + (j + k * Ny) * Nx]); // left
        if (i < Nx - 1)
          CELL[i + (j + k * Ny) * Nx]->add_NB(CELL[i + 1 + (j + k * Ny) * Nx]); // right
        if (j > 0)
          CELL[i + (j + k * Ny) * Nx]->add_NB(CELL[i + (j - 1 + k * Ny) * Nx]); // front
        if (j < Ny - 1)
          CELL[i + (j + k * Ny) * Nx]->add_NB(CELL[i + (j + 1 + k * Ny) * Nx]); // back
        if (k > 0 ){
          int l = 0;
          while (l < 3 && k -l > 0){
            CELL[i + (j + k * Ny) * Nx]->add_dnb(CELL[i + (j + (k - 1 - l) * Ny) * Nx]); // below
            l+= 1;
          }
        }
        if (k < Nz - 1 ){
          int l = 0;
          while (l < L && k+l < Nz - 1){
            CELL[i + (j + k * Ny) * Nx]->add_unb(CELL[i + (j + (k + 1 + l ) * Ny) * Nx]); // above
            l+= 1;
          }
      }
      }
    }
  }

  /* Main loop */
  double red_stress = 0.;   // reduced stress = [global stress applied to the tensile specimen] / N
  while (UBCELL.size() > 0) // While there are still unbroken Cells
  {
    if (nf % mod_nf == 0)
    {
      std::string nf_str = std::to_string(nf);
      std::string file_name = file_name_prefix + nf_str + ".vtk";
      SaveSolution(file_name, Nx, Ny, Nz, CELL, red_stress * N);
    }
    nf++;

    Cell *min = *min_element(UBCELL.begin(), UBCELL.end(), [](Cell *a, Cell *b)
                             { return a->compute_breaking_stress() < b->compute_breaking_stress(); });
    red_stress = min->compute_breaking_stress();

    bool breaking_flag = true;
    while (breaking_flag)
    {
      breaking_flag = false;
      std::vector<int> to_delete(0); // Cells to delete from the UBCELL vector after its breaking
      for (long unsigned int i = 0; i < UBCELL.size(); i++)
      {
        if (UBCELL[i]->compute_breaking_stress() <= red_stress)
        {
          UBCELL[i]->shatter(L,red_stress);
          to_delete.push_back(i);
          breaking_flag = true;
        //   for (int l = 0; l < Nx; i++)
        //   {
        //     for (int j = 0; j < Ny; j++)
        //     {
        //       for (int k = 0; k < Nz; k++)
        //       {
        // if (k > 0 && k < 3)
        //   CELL[i + (j + (k - 1) * Ny) * Nx]->add_sc(7.); // below
        // if (k < Nz - 1 && k > 3)
        //   CELL[i + (j + (k + 1) * Ny) * Nx]->add_sc(7.); // above
        //       }
        //     }
        //   }
        }
      }

      if (breaking_flag) // equivalent to "if (to_delete.size() > 0)"
      {
        for (long int i = to_delete.size() - 1; i >= 0; i--) // reversed loop for not messing up with the indexes after a removal
        {
          /* quick delete (see the memory management of a vector in C++) */
          UBCELL[to_delete[i]] = UBCELL.back();
          UBCELL.pop_back();
        }
      }
    }
    std::cout << "contrainte" << red_stress << std::endl ;

  }

  /* Saving the final state */
  if (nf % mod_nf == 0)
  {
    std::string nf_str = std::to_string(N);
    std::string file_name = file_name_prefix + nf_str + ".vtk";
    SaveSolution(file_name, Nx, Ny, Nz, CELL, red_stress * N);
  }

  /* Deallocate memory */
  for (int i = 0; i < N; i++)
  {
    delete CELL[i];
    CELL.clear();
    UBCELL.clear();
  }

  return 0;
}
