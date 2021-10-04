# License

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version. This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.

--------------------------------------------
# Manuscript

The primary purpose in distributing this source code is to enable readers to reproduce the numerical results reported in the manuscript "Surveillance Evasion Through Bayesian Reinforcement Learning" by Dongping Qi, David Bindel, and Alexander Vladimirsky. A pre-print of this article can be found on Arxiv [here](https://arxiv.org/abs/2109.14811). 

--------------------------------------------
# Website

In addition to the manuscript, more information about the project can also be found at:
* [https://eikonal-equation.github.io/Bayesian-Surveillance-Evasion/](https://eikonal-equation.github.io/Bayesian-Surveillance-Evasion/).

--------------------------------------------
# Contributions / Acknowledgements

* Problem statement by Alex Vladimirsky (inspired by an earlier project on modelling environmental crimes joint with Elliot Cartee [further reading](https://epubs.siam.org/doi/abs/10.1137/19M1270483)).
* Alg-PC Algorithm: by Alex Vladimirsky & Dongping Qi.
* Alg-GP Algorithm: key idea suggested by David Bindel, details filled in by Dongping Qi and Alex Vladimirsky. 
* Dongping Qi has been responsible for the implementation and testing of both algorithms, with guidance from Alex Vladimirsky and David Bindel. 
* Dongping Qi and Alex Vladimirsky were mainly responsible for writing the manuscript, with editing help from David Bindel.  
* The authors are also grateful to M. Wegkamp and M. Nussbaum for providing advice on handling the right-censored data.

--------------------------------------------
# Instructions

## Requirements
The C++ Code requires no external libraries.  
Currently the C++ code is complied and linked to MATLAB through a `mex` function, `fastMarching.cpp`, which calls a corresponding C++ function.

The `mex` function helps compile the C++ code, so that MATLAB can directly call a C++ function, just like a MATLAB function.

To make sure `mex` is properly setup on your computer, run `mex -setup C++` in MATLAB. 
To change the default compiler, see [here](https://www.mathworks.com/help/matlab/matlab_external/changing-default-compiler.html). 
For further information about `mex` functions, see [here](https://www.mathworks.com/help/matlab/ref/mex.html).

The algorithm using Gaussian process regression requires `fmincon` package to tune the hyperparameters.


## Running the code

Assuming the mex function works and MATLAB is able to call the C++ function `fastMarching()`.
The examples in the manuscript/website can be reproduced by just running:
* ` learning_K_pwc.m `, for Algorithm 1 with a piecewise constant model;

* ` learning_K_gp.m `,  for Algorithm 2 with a Gaussian process regression model;

You can change the following parameters:
* `episodes`;
* `kn`:             the number of cells in each direction;
* `gamma`:          the confidence level;
* `nobs`:           the number of observers, corresponding to difference examples;


## Visualizing output

The plotting scripts are in MATLAB files of running the examples, therefore no additional post-processing is needed.

Each algorithm contains 8 output sub-figures, which are
* True `K(x)` & predicted `K(x)`;
* True `u(x)` & predicted `u(x)`;
* Estimated/Posterior variance;
* Instant regret in each episode;
* Averaged regret & observed capture rate;
