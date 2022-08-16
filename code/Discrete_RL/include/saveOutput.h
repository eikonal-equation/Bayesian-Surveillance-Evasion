// Author & copyright to: Dongping Qi 

#pragma once

#include <iostream>
#include <fstream>
#include <vector>

#include "Grid.h"

using namespace std;


// Overload the operator "<<"
template<typename T>
ostream &operator << (ostream& out, const vector<T> & A) {
   for (int i = 0; i < A.size(); ++i) {
        out << A[i] << '\t';
   }
   return out;
}

template<typename T>
ostream &operator << (ostream& out, const vector<vector<T>> & A) {
   for (int i = 0; i < A.size(); ++i) {
        for (int j = 0; j < A[i].size(); ++j) {
            out << A[i][j] << '\t';
        }
        out << '\n';
   }
   return out;
}

void saveValueFunction(shared_ptr<Grid> fmm, string filename) {
    double n = fmm->n1d;
    vector<vector<double>> value_func(n, vector<double>(n, 0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            value_func[i][j] = fmm->grid[i * n + j]->uValue;
        }
    }
    ofstream myFile("../output/" + filename + "_value_function.txt");
    myFile << value_func;
}

template<typename T>
void saveMatrix(vector<vector<T>> mat, string filename) {
    ofstream myFile("../output/" + filename + ".txt");
    myFile << mat;
}

template<typename T>
void saveVector(vector<T> vec, string filename) {
    ofstream myFile("../output/" + filename + ".txt");
    myFile << vec;
}