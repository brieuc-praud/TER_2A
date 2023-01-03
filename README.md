# Unidirectional composite material failure

## General description
The program generates a set of fibers which are meshed into cells. A tensile test is then simulated on this mesh in order to observe the propagation of the breakings throughout the fibers. 

## Breaking stress threshold and Weibull's law
Each cell has its own breaking stress threshold which is determined randomly following the Weibull's law.
The parameters of the Weibull's law (V, s0, m) can be set from the 'parameters.toml' parameter file.

## Dimensions of the tensile specimen
The dimensions of the tensile specimen can be specified both in X and Y directions via the parameters 'Nx' and 'Ny'. The parameter 'Nz' defines how much cells each fiber is constituted of.

## Output
The program generates Visualization Toolkit files ('.vtk') each containing 4 scalar fields which are:
- breaking_stress: the breaking stress threshold of the cell (this field stays the same over the iterations)
- breakings : basically a boolean field which describe the state of the cell (1 if broken, 0 otherwise)
- stress : the stress applied on the cell
- stress_coefficient : the stress coefficient of the cell (it only differs from the stress field by a multiplication factor)
The number of output files is at most the number of fibers (because at each iteration there is one breaking or more). This number can be reduced by using the 'mod_nf' option: the number of output files is divided by the number mod_nf.