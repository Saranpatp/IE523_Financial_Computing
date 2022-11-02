#include <iostream>
#include <random>
#include <cmath>
#include <vector>
#include <algorithm>
#include <fstream>

using namespace std;

class Distribution{
    default_random_engine generator;
private:
    double getUniform(){
        uniform_real_distribution<double> distribution(0.0,1.0);
        return distribution(generator);
    }
public:
//  inverse cauchy =  tan[pi(U-0.5)] from https://www.randomservices.org/random/special/Cauchy.html
    double getCauchy(){
        return tan(M_PI*(getUniform()-0.5));
    }
};
int main(int argc, char* argv[]) {
    double mean = 0;
    int nTrails;

    vector<double> data;

    Distribution D;
//    sscanf(argv[1],"%d",&nTrails);
    nTrails = 90000;


    //Copy from Prof files
    float y;
    int count[100];

    sscanf (argv[1], "%d", &nTrails);
    ofstream pdf_comparison_file("pdf.csv");

    for (int i = 0; i < 100; i++) {
        count[i] = 0;
    }

    for (int i = 0; i < nTrails; i++) {
        y = D.getCauchy();
        data.push_back(y);
        mean += y;
        for (int j = 0; j < 100; j++)
            if ( (y >= ((float) (j-51)/10)) && (y < ((float) (j-50)/10))) count[j]++;
    }

    int sum = 0;
    for (int j = 0; j < 100; j++) {
        sum += count[j];
        pdf_comparison_file << ((float) (j-50)/10) << ", " << ((float) count[j]/nTrails) << endl;
    }

    mean = mean/data.size();
    sort(data.begin(), data.end());
    double medIndx = (double) nTrails/2;
    int test = floor(medIndx);
    double med = (double) (data[floor(medIndx)] + data[ceil(medIndx)])/2;
    cout << "mean = " << mean << endl;
    cout << "med = " << med << endl;



    return 0;
}
