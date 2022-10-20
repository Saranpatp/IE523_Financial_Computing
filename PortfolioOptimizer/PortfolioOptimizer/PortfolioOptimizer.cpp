#include "gurobi_c++.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>
#include <cmath>

using namespace std;

const double TOLERANCE = 1e-10;

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

/*
* Optimize 
*/

void optimizePortfolio() {
    // getting the cost function that needs to be minimized
    vector<double> cost;
    for (int i = 0; i < nCfs; i++) {
        cost.push_back(-1 * convexities[i]);
    }

    // creating the "2 x (#cash-flows)"" constraint matrix
    vector<vector<double>> A;
    for (int i = 0; i < nCfs; i++) {
        for (int j = 0; j < 2; j++) {

        }
    }

    // Gurobi part
    // Create an environment
    cout << "****************************************************" << endl;
    try {
        GRBEnv env = GRBEnv(true);
        env.set("LogFile", "PortfolioOptimization.log");
        env.start();

        // Create an empty model
        GRBModel model = GRBModel(env);
        model.optimize();
        int optimistatus = model.get(GRB_IntAttr_Status);
        if (optimistatus == GRB_INF_OR_UNBD) {
            double objval = model.get(GRB_DoubleAttr_ObjVal);
            cout << "Optimal objective " << objval << endl;
            cout << "Optimal Values : " << endl;
            GRBVar* vars = model.getVars();
            int i = 0;
            for (GRBVar* p = vars; i < model.get(GRB_IntAttr_NumVars); i++, p++)
                printf("%s = %f \n", p->get(GRB_StringAttr_VarName).c_str(), p->get(GRB_DoubleAttr_X));
        }
        else if (optimistatus == GRB_INFEASIBLE) {
            cout << "Model is infeasible" << endl;
            model.computeIIS();
            model.write("model.ilp");
        }
        else {
            cout << "Optimization was stopped with status = " << optimistatus << endl;
        }
    }
    catch (GRBException e) {
        cout << "Error code = " << e.getErrorCode() << endl;
        cout << e.getMessage() << endl;
    }
    catch (...) {
        cout << "Error during optimization" << endl;
    }
    
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
    optimizePortfolio();
}

int main(int argc, char* argv[])
{    
    readData(argc, argv);
    return 0;
}