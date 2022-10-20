#include "gurobi_c++.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <direct.h>
using namespace std;

vector<double> presentValues;
vector<int> maturities;
vector<vector<double>> cashFlows;


void read_data(int argc, char* const argv[]) {
    int value, nCfs, maturity;
    double curPV, cf;
    //ifstream inputFile(argv[1]);
    ifstream inputFile("input1.txt");
    if (inputFile.is_open()) {
        inputFile >> nCfs; //number of CFs
        //init values
        for (int i = 0; i < nCfs; i++) {
            inputFile >> curPV;
            cout << curPV << endl;
            presentValues.push_back(curPV);
            inputFile >> maturity;
            maturities.push_back(maturity);
            vector<double> tmpCf;
            for (int j = 0; j < maturity; j++) {
                inputFile >> cf;
                tmpCf.push_back(cf);
            }
            cashFlows.push_back(tmpCf);
        }
    }
}


int main(int argc, char* argv[])
{    
    read_data(argc, argv);
    return 0;
}
/*
void testGurobi() {
    try {

        // Create an environment
        GRBEnv env = GRBEnv(true);
        env.set("LogFile", "mip1.log");
        env.start();

        // Create an empty model
        GRBModel model = GRBModel(env);

        // Create variables
        GRBVar x = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "x");
        GRBVar y = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "y");
        GRBVar z = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "z");

        // Set objective: maximize x + y + 2 z
        model.setObjective(x + y + 2 * z, GRB_MAXIMIZE);

        // Add constraint: x + 2 y + 3 z <= 4
        model.addConstr(x + 2 * y + 3 * z <= 4, "c0");

        // Add constraint: x + y >= 1
        model.addConstr(x + y >= 1, "c1");

        // Optimize model
        model.optimize();

        cout << x.get(GRB_StringAttr_VarName) << " "
            << x.get(GRB_DoubleAttr_X) << endl;
        cout << y.get(GRB_StringAttr_VarName) << " "
            << y.get(GRB_DoubleAttr_X) << endl;
        cout << z.get(GRB_StringAttr_VarName) << " "
            << z.get(GRB_DoubleAttr_X) << endl;

        cout << "Obj: " << model.get(GRB_DoubleAttr_ObjVal) << endl;

    }
    catch (GRBException e) {
        cout << "Error code = " << e.getErrorCode() << endl;
        cout << e.getMessage() << endl;
    }
    catch (...) {
        cout << "Exception during optimization" << endl;
    }
}
*/