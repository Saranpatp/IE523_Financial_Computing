#include "gurobi_c++.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>
#include <cmath>
#include <sstream>

using namespace std;

const double TOLERANCE = 1e-10;

vector<int> maturities;
vector<double> presentValues,convexities, durations, ytms;

double debt, debtDuration, debtpv;
int nCfs;

void printResult() {
    cout << "We owed " << debt << " in " << debtDuration << " years" << endl;
    cout << "Number of Cash flow: " << nCfs << endl;
    for (int i = 0; i < nCfs; i++) {
        cout << "----------------------------------------------------" << endl;
        cout << "Cash Flow #" << i + 1 << endl;
        cout << "Price = " << presentValues[i] << endl;
        cout << "Maturity = " << maturities[i] << endl;
        cout << "Yield to Maturity = " << ytms[i] << endl;
        cout << "Duration = " << durations[i] << endl;
        cout << "Convexity = " << convexities[i] << endl;
    }
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

void optimizePortfolio() {
    // Gurobi part
    // Create an environment
    vector<double> immuneDollar = {500, debtpv, debt};
    cout << "****************************************************" << endl;
    try {
        GRBEnv env = GRBEnv(true);
        env.set("LogFile", "PortfolioOptimization.log");
        //env.set("LogToConsole", 0);
        env.start();

        // Create an empty model
        GRBModel model = GRBModel(env);
        
        // init variable
        GRBVar* lambda = new GRBVar[nCfs]; // matrix of lambda that we want to find.
        
        for (int i = 0; i < nCfs; i++) {
            lambda[i] = model.addVar(0.0, 1, 0.0, GRB_CONTINUOUS , "lambda_" + to_string(i)); // all lambdai > 0 and lambda < 1
        }

        //add linear constraint
        GRBLinExpr le1 = 0; 
        GRBLinExpr le2 = 0;
        GRBLinExpr bConvex = 0;
        for (int i = 0; i < nCfs; i++) {
            le1 += lambda[i];
            le2 += lambda[i] * durations[i];
            bConvex += lambda[i] * convexities[i];
        }
        // set objective as optimize convexity
        model.setObjective(bConvex, GRB_MAXIMIZE); 

        model.addConstr(le1 == 1); // Sum of lambda ==1
        model.addConstr(le2 == debtDuration); // Sum of lambda*D == N

        model.optimize();

        int optimistatus = model.get(GRB_IntAttr_Status); // Check if it able to find solution or not  

        cout << "****************************************************" << endl;

        if (optimistatus == GRB_OPTIMAL) { // found the solution
            //getting solution
            double objval = model.get(GRB_DoubleAttr_ObjVal);
            printf("Largest Convexity we can get is %.3f \n",objval);
            cout << "****************************************************" << endl;
            cout << "To immunize against small changes in 'r' for each $1 of PV, you should buy" << endl;
            GRBVar* vars = model.getVars();
            int i = 0;
            vector<pair<double, int>> solutions;
            for (GRBVar* p = vars; i < model.get(GRB_IntAttr_NumVars); i++, p++)
                if (p->get(GRB_DoubleAttr_X) > 0) {

                    printf("$%f of Cash-Flow %d \n", p->get(GRB_DoubleAttr_X), i + 1);
                    solutions.push_back({ p->get(GRB_DoubleAttr_X),i + 1 });
                }
            for (int i = 0; i < immuneDollar.size(); i++) {
                cout << "****************************************************" << endl;
                printf("To immunize against small changes in 'r' for each $%.2f of PV, you should buy\n",immuneDollar[i]);
                for (int j = 0; j< solutions.size(); j++)
                    printf("$%.2f x %f = %.3f of Cash-Flow %d \n",immuneDollar[i], solutions[j].first, immuneDollar[i]*solutions[j].first, solutions[j].second);
            }
            
            
        }
        else if (optimistatus == GRB_INFEASIBLE) {
            printf("There is no portfolio that meets the duration constraint of %.1f years", debtDuration);
            model.computeIIS();
            model.write("model.ilp");
        }
        else {
            cout << "Optimization was stopped with status = " << optimistatus << endl;
        }
        cout << "****************************************************" << endl;
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
    ifstream inputFile(argv[1]);
    cout << "Input File : " << argv[1] <<endl;
    //ifstream inputFile("input1.txt");
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
            double r = solveYtm(0, (double)maturity, tmpCf, curPV);
            ytms.push_back(r); // calculate ytm
            durations.push_back(calDuration(r, (double)maturity, tmpCf, curPV));
            convexities.push_back(calConvexity(r, (double)maturity, tmpCf, curPV));
        }
        inputFile >> debt;
        inputFile >> debtDuration;
    }

    printResult();
    // calculate pv of the debt using r as avg rate
    double avRate = 0;
    for (int i = 0; i < nCfs; i++) {
        avRate += ytms[i];
    }
    avRate = avRate / nCfs;
    
    debtpv = debt/pow(1+avRate,debtDuration);

    // run gurobi optimization
    optimizePortfolio();
}

int main(int argc, char* argv[])
{
    readData(argc, argv);
    return 0;
}