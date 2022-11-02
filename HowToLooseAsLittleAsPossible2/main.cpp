#include <iostream>
#include <cmath>
#include <fstream>
#include <random>
using namespace std;

class Simulator{
    default_random_engine generator;
private:
    double nCr(int n, int r){
        int value = tgamma(n+1)/(tgamma(r+1)*tgamma(n-r+1));
        return tgamma(n+1)/(tgamma(r+1)*tgamma(n-r+1));
    }
    double get_uniform(){
        uniform_real_distribution<double> distribution(0.0,1.0);
        return distribution(generator);
    }
public:
    double getAliceWonTheProb(int n, double q, double p){ //Theoretical prob
        double prob = 0;
        for(int r = 0; r <= n; r++){
            for (int s = r + 1; s <= n; s++){
                prob += nCr(n,r)*pow(p,r)*pow(1-p,n-r)* nCr(n,s)*pow(q,s)*pow(1-q,n-s);
            }
        }
        return prob;
    }
    double getAliceWonSimProb(int n, int nTrails, double q, double p){
        int nWins = 0;
        for(int i = 0; i < nTrails; i++){
            int nHeadAlice = 0;
            int nHeadBob = 0;
            for(int j = 0; j < n; j++){
                if (get_uniform() < q) nHeadAlice++;
                if (get_uniform() < p) nHeadBob++;
            }
            if (nHeadAlice > nHeadBob) nWins++;
        }
        return ((double) nWins/(double) nTrails);
    }

    void getBothTheAndSimProb(int n, int nTrails, double q, double p){ // also save a file
        ofstream file;
        file.open("result.csv");
        for (int i = 1; i <= n; i++){
            cout<< i <<", " << getAliceWonSimProb(i,nTrails,q,p) << ", "<< getAliceWonTheProb(i,q,p)<<endl;
            file << i <<", " << getAliceWonSimProb(i,nTrails,q,p) << ", "<< getAliceWonTheProb(i,q,p)<<endl;
        }
        file.close();
    }

};

int main(int argc, char* argv[]) {
    double p,q;
    int n = 30,nTrails;
    sscanf(argv[1],"%d",&nTrails);
    sscanf(argv[2],"%lf",&q);
    sscanf(argv[3],"%lf",&p);

//    p = 0.2;
//    q = 0.18;

    if (q > p) {
        cout << "ERROR p > q";
        return 1;
    }
    Simulator S;

    cout << "Probability of head up for Alice = " << q << endl;
    cout << "Probability of head up for Bob = " << p << endl;
    cout << "Number of Trails = " << nTrails << endl;
    S.getBothTheAndSimProb(n,nTrails,q,p);

    return 0;
}
