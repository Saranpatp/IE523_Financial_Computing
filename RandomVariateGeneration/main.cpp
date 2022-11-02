#include <iostream>
#include <random>
#include <cmath>
#include <vector>
#include <algorithm>

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
    int bin = 100; //Graph width
    double mean = 0;
    int nTrails;
    float y;

    vector<double> data;

    Distribution D;
//    sscanf(argv[1],"%d",&nTrails);
    nTrails = 1000;
    for (int i = 0; i < nTrails; i++) {
        double x = D.getCauchy();
        data.push_back(x);
        mean += x;
    }
    mean = mean/nTrails;
    sort(data.begin(), data.end());
    int med = nTrails/2;
    cout << "mean = " << mean << endl;
    cout << "med = " << data[med] << endl;

    return 0;
}
