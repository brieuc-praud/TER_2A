#include "Maille.h"
#include "FileTools.h"
#include <time.h>
#include <cmath>
#include <random>
#include <algorithm>
#include <iostream>
#include <string>

int main(int argc, char **argv)
{
  // pour diminuer le nombre de fichiers en sortie
  int nf = 0;
  int mod_nf = 10; // on sort Nx*Ny/mod_nf fichiers

  // === Paramètres que l'on lira dans un fichier ===
  int const Nx = 10;                                        // dimension X
  int const Ny = 10;                                        // dimension Y
  int const Nz = 10;                                        // dimension Z
  std::string const file_name_prefix = "results/traction_"; // chemin des fichiers de sortie
  // --- paramètres de la loi de Weibull ---
  double const V = 5.;
  double const s0 = 2.;
  int const m = 1;
  // ======

  int const N = Nx * Ny * Nz; // nombre total de mailles

  // Initialisation
  std::default_random_engine generator(time(0));
  std::uniform_real_distribution<double> distribution(0.0, 1.0);
  std::vector<Maille *> M; // M stocke toutes les mailles
  std::vector<Maille *> E; // E stocke les mailles non rompues (ainsi, le nombre de mailles à parcourir décroît)
  M.resize(N);
  double coeff = s0 / pow(V, 1. / m);
  for (int i = 0; i < N; i++)
  {
    // Loi de Weibull
    double p = distribution(generator);
    double sr = coeff * pow(-log(p), 1. / m);
    // Création de la maille
    M[i] = new Maille(sr);
    E.push_back(M[i]);
  }

  // creer les relations de voisinage
  for (int i = 0; i < Nx; i++)
  {
    for (int j = 0; j < Ny; j++)
    {
      for (int k = 0; k < Nz; k++)
      {
        if (i > 0)
          M[i + (j + k * Ny) * Nx]->add_voisins(M[i - 1 + (j + k * Ny) * Nx]); // à gauche
        if (i < Nx - 1)
          M[i + (j + k * Ny) * Nx]->add_voisins(M[i + 1 + (j + k * Ny) * Nx]); // à droite
        if (j > 0)
          M[i + (j + k * Ny) * Nx]->add_voisins(M[i + (j - 1 + k * Ny) * Nx]); // derrière
        if (j < Ny - 1)
          M[i + (j + k * Ny) * Nx]->add_voisins(M[i + (j + 1 + k * Ny) * Nx]); // devant
        if (k > 0)
          M[i + (j + k * Ny) * Nx]->add_voisins(M[i + (j + (k - 1) * Ny) * Nx]); // en-dessous
        if (k < Nz - 1)
          M[i + (j + k * Ny) * Nx]->add_voisins(M[i + (j + (k + 1) * Ny) * Nx]); // au-dessus
      }
    }
  }

  // boucle principale
  double contrainte_red = 0.; // contrainte reduite = [contrainte appliquée à l'échantillon] / N
  while (E.size() > 0)        // Tant qu'il y a des mailles non rompues
  {
    if (nf % mod_nf == 0)
    {
      std::string nf_str = std::to_string(nf);
      std::string file_name = file_name_prefix + nf_str + ".vtk";
      SaveSolution(file_name, Nx, Ny, Nz, M, contrainte_red * N);
    }
    nf++;

    Maille *min = *min_element(E.begin(), E.end(), [](Maille *a, Maille *b)
                               { return a->compute_breaking_stress() < b->compute_breaking_stress(); });
    contrainte_red = min->compute_breaking_stress();

    bool breaking_flag = true;
    while (breaking_flag)
    {
      breaking_flag = false;
      std::vector<int> a_supprimer(0); // mailles à supprimer du vecteur E après rupture
      for (long unsigned int i = 0; i < E.size(); i++)
      {
        if (E[i]->compute_breaking_stress() <= contrainte_red)
        {
          E[i]->rompre();
          a_supprimer.push_back(i);
          breaking_flag = true;
        }
      }

      if (breaking_flag) // equivalent à "if (a_supprimer.size() > 0)"
      {
        for (long int i = a_supprimer.size() - 1; i >= 0; i--) // boucle à l'envers pour ne pas que les indices changent après une suppression
        {
          // suppression rapide (cf gestion memoire d'un vecteur en C++)
          E[a_supprimer[i]] = E.back();
          E.pop_back();
        }
      }
    }
  }

  // sauvegarde de l'état final
  if (nf % mod_nf == 0)
  {
    std::string nf_str = std::to_string(N);
    std::string file_name = file_name_prefix + nf_str + ".vtk";
    SaveSolution(file_name, Nx, Ny, Nz, M, contrainte_red * N);
  }

  // Désalloue la mémoire
  for (int i = 0; i < N; i++)
  {
    delete M[i];
    M.clear();
  }

  return 0;
}
