#ifndef PSTEST_HPP
#define PSTEST_HPP

#define _USE_MATH_DEFINES

#include<Eigen/Sparse>
#include<Eigen/IterativeLinearSolvers>
#include <cmath>
#include <fstream>

#include "../../include/Grid.hpp"
#include "../../include/espic1d1v.hpp"

// Driver code for testing Poisson solve
int main(int argc, char *argv[]){
    size_t Nx = std::stoi(argv[1]), routineFlag;
    double x_min = -M_PI, x_max = M_PI;
    Grid1d1v testGrid(Nx, x_min, x_max);
    double dx = testGrid.getDX();

    // Initialize solver
    Eigen::SparseMatrix<double> A(Nx-1,Nx-1); // Periodic Boundary Conditions so don't need last line 
    A.reserve(Eigen::VectorXi::Constant(Nx-1,3)); // Poisson's equation so triangular
    routineFlag = BuildSparseLapl(A,dx);
    Eigen::VectorXd rhoEig(A.rows()), phiEig(A.rows());

    // Case 1: rho(x) = sin(x)
    std::ofstream sinLog("./data/sin.csv");
    sinLog << "x_j," << "rho_x," << "phi_x" << std::endl;
    for (size_t j = 0; j < Nx; j++){
        testGrid.RhoX(j) = sin(testGrid.Xgrid(j));
    }
    routineFlag = FieldSolveMatrix(A, testGrid, rhoEig, phiEig, dx, Nx);
    /* Stream output into csv */
    for (size_t j = 0; j < Nx; j++){
        sinLog << testGrid.Xgrid(j) << "," << testGrid.RhoX(j) << "," << testGrid.PhiX(j) << std::endl;
    }

    // Case 2: rho(x) = cos(x)
    std::ofstream cosLog("./data/cos.csv");
    cosLog << "x_j," << "rho_x," << "phi_x" << std::endl;
    for (size_t j = 0; j < Nx; j++){
        testGrid.RhoX(j) = cos(testGrid.Xgrid(j));
    }
    routineFlag = FieldSolveMatrix(A, testGrid, rhoEig, phiEig, dx, Nx);
    /* Stream output into csv */
    for (size_t j = 0; j < Nx; j++){
        cosLog << testGrid.Xgrid(j) << "," << testGrid.RhoX(j) << "," << testGrid.PhiX(j) << std::endl;
    }

    // Case 3: rho(x) = x^2
    std::ofstream paraLog("./data/parabola.csv");
    paraLog << "x_j," << "rho_x," << "phi_x" << std::endl;
    for (size_t j = 0; j < Nx; j++){
        testGrid.RhoX(j) = pow(testGrid.Xgrid(j),2);
    }
    routineFlag = FieldSolveMatrix(A, testGrid, rhoEig, phiEig, dx, Nx);
    /* Stream output into csv */
    for (size_t j = 0; j < Nx; j++){
        paraLog << testGrid.Xgrid(j) << "," << testGrid.RhoX(j) << "," << testGrid.PhiX(j) << std::endl;
    }

    // Case 4: rho(x) = e^x
    std::ofstream expLog("./data/exponential.csv");
    expLog << "x_j," << "rho_x," << "phi_x" << std::endl;
    for (size_t j = 0; j < Nx; j++){
        testGrid.RhoX(j) = exp(testGrid.Xgrid(j));
    }   
    routineFlag = FieldSolveMatrix(A, testGrid, rhoEig, phiEig, dx, Nx);
    /* Stream output into csv */
    for (size_t j = 0; j < Nx; j++){
        expLog << testGrid.Xgrid(j) << "," << testGrid.RhoX(j) << "," << testGrid.PhiX(j) << std::endl;
    }

    return 0;
}
#endif