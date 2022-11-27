#include "Maille.h"
#include "FileTools.h"
#include <time.h>
#include <cmath>
#include <random>
#include <algorithm>

int main(int argc, char **argv)
{
  // pour diminuer le nombre de fichiers en sortie
  int nf = 0;
  int mod_nf = 100; // on sort Nx*Ny/mod_nf fichiers

  // Paramètres que l'on lira dans un fichier
  int const Nx = 100;
  int const Ny = 100;
  double const V = 5.;
  double const s0 = 2.;
  int const m = 1;
  std::string const file_name_prefix = "results/traction_";

  int const N = Nx * Ny;

  // Initialisation
  std::uniform_real_distribution<double> distribution(0.0, 1.0);
  std::vector<Maille *> M; // M stocke toutes les mailles
  std::vector<Maille *> E; // E stocke les mailles non rompues
  M.resize(N);
  double coeff = s0 / pow(V, 1. / m);
  for (int i = 0; i < N; i++)
  {
    // Loi de Weibull
    std::default_random_engine generator(time(0) + i);
    double p = distribution(generator);
    double cr = coeff * pow(-log(p), 1. / m);
    // Création de la maille
    M[i] = new Maille(cr);
    E.push_back(M[i]);
  }

  // boucle principale
  double contrainte = 0.;
  while (E.size() > 0)
  {
    Maille *min = *min_element(E.begin(), E.end(), [&contrainte](Maille *a, Maille *b)
                               { return a->compute_dc(contrainte) < b->compute_dc(contrainte); });
    contrainte = min->compute_dc(contrainte);

    std::vector<int> a_supprimer;
    for (long unsigned int i = 0; i < E.size(); i++)
    {
      if (E[i]->compute_dc(contrainte) <= contrainte)
      {
        E[i]->rompre();
        a_supprimer.push_back(i);
      }
    }

    for (long unsigned int i = 0; i < a_supprimer.size(); i++)
    {
      E.erase(E.begin() + a_supprimer[i]); // complexié O(n) dans le pire cas, une std::list serait sûrement plus optimale
    }

    if (nf % mod_nf == 0)
    {
      std::string file_name = file_name_prefix + std::to_string(contrainte) + ".vtk";
      SaveSolution(file_name, Nx, Ny, M);
    }
    nf++;
  }

  // Désalloue la mémoire
  for (int i = 0; i < N; i++)
  {
    delete M[i];
    M.clear();
  }

  return 0;
}
