#include "gurobi_c++.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>
#include <cmath>
using namespace std;

const double TOLERANCE = 1e-3;

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
        cout << "Cash Flow #" << i + 1 << endl;
        cout << "Price = " << presentValues[i] << endl;
        cout << "Maturity = " << maturities[i] << endl;
        cout << "Yield to Matruity = " << ytms[i] << endl;
        cout << "Duration = " << durations[i] << endl;
        cout << "Convexity = " << convexities[i] << endl;
    }
    // add print gurobi part here
}


double f(double r, double maturity, vector<double> cf, double pv) { //Newton Raphson function
    double discountedCf = 0;
    for (int t = 1; t <= (int) maturity; t++) {
        double power = maturity - (double) t;
        discountedCf += cf[t-1] * pow(1+r,power);// cf start at 0 so use t-1
    }
    double res = pv * pow((1+r), maturity) - discountedCf;
    return res;
}

double df(double r, double maturity, vector<double> cf, double pv) {
    double cumSum = 0;
    for (int t = 1; t <= (int) maturity - 1; t++) {
        cumSum += cf[t - 1] *  (maturity - (double) t) * pow(1+r, maturity - (double)t - 1); // cf start at 0 so use t-1
    }
    double res = (maturity * pv * pow(1 + r, maturity - 1)) - cumSum;
    return res;
}

double solveYtm(double r, double maturity, vector<double> cf, double pv) { // solve yield marturity using Newton-Raphson Method
    while (double error = abs(f(r, maturity, cf, pv)) > TOLERANCE) {
        r = r - f(r, maturity, cf, pv) / df(r, maturity, cf, pv); // Raphson function
    }
    return r;
}

double calDuration(double r, double maturity, vector<double> cf, double pv) {
    double res = 0;
    for (int t = 1; t <= maturity; t++) {
        res += (cf[t - 1] / pow(1 + r, t) * t) / pv;
    }
    return res;
}

double calConvexity(double r, double maturity, vector<double> cf, double pv) {
    double res = 0;
    for (int t = 1; t <= maturity; t++) {
        res += (t*(t + 1) * cf[t - 1])/pow(1 + r,t+2);
    }
    return res / pv;
}

void readData(int argc, char* const argv[]) {
    int maturity;
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
            double r = solveYtm(0, (double)maturity, tmpCf, curPV);
            ytms.push_back(r); // calculate ytm
            durations.push_back(calDuration(r, (double)maturity, tmpCf, curPV));
            convexities.push_back(calConvexity(r, (double)maturity, tmpCf, curPV));
        }
        inputFile >> debt;
        inputFile >> debtDuration;
    }
    printResult();
}

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