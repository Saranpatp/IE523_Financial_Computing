#include "gurobi_c++.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>
#include <cmath>
using namespace std;

vector<double> presentValues;
vector<int> maturities;
vector<vector<double>> cashFlows;
vector<double> convexities;
vector<double> durations;
vector<double> ytms;
double debt;
int debtDuration;

int nCfs; //number of Cash flows

void printResult() {
    cout << "We owed " << debt << " in " << debtDuration << " years" << endl;
    cout << "Numnber of Cash flow: " << nCfs << endl;
    for (int i = 0; i < nCfs; i++) {
        cout << "----------------------------------------------------" << endl;
        cout << "Cash Flow #" << i << endl;
        cout << "Price = " << presentValues[i] << endl;
        cout << "Maturity = " << maturities[i] << endl;
        //cout << "Yield to Matruity = " << ytms[i] << endl;
        //cout << "Duration = " << durations[i] << endl;
        //cout << "Convexity = " << convexities[i] << endl;
    }
    // add print gurobi part here
}

void readData(int argc, char* const argv[]) {
    int value, maturity;
    double curPV, cf;
    //ifstream inputFile(argv[1]);
    // cout << "Input File : " << argv[1] <<endl;
    ifstream inputFile("input1.txt");
    if (inputFile.is_open()) {
        inputFile >> nCfs; //number of CFs
        //init values
        for (int i = 0; i < nCfs; i++) {
            inputFile >> curPV;
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
        inputFile >> debt;
        inputFile >> debtDuration;
    }
    printResult();
}

double f(double r, int maturity, vector<double> cf, double pv) { //Newton Raphson function
    double discountedCf;
    for (int t = 0; t < maturity; t++) {
        double power = (double) maturity - (double) t;
        discountedCf += cf[t] * pow(1+r,power);
    }
    double res = pow(pv*(1+r),(maturity-1));
}


//double calYtm() { //calculate yield to maturity
//    for (int i; i < )
//}






int main(int argc, char* argv[])
{    
    readData(argc, argv);
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