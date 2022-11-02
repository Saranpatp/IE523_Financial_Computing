#include <iostream>
#include <cmath>
using namespace std;

class Simulator{
private:
    double nCr(int n, int r){
        int value = tgamma(n+1)/(tgamma(r+1)*tgamma(n-r+1));
        return tgamma(n+1)/(tgamma(r+1)*tgamma(n-r+1));
    }
public:
    double getAliceWonProb(int n, double q, double p){
        double prob = 0;
        for(int r = 0; r <= n; r++){
            for (int s = r + 1; s <= n; s++){
                prob += nCr(n,r)*pow(p,r)*pow(1-p,n-r)* nCr(n,s)*pow(q,s)*pow(1-q,n-s);
            }
        }
        return prob;
    }

};

int main(int argc, char* argv[]) {
    double p,q;
    int res = -1, counter = 0;
    sscanf(argv[1],"%lf",&q);
    sscanf(argv[2],"%lf",&p);
//    p = 0.2;
//    q = 0.18;
    if (q > p) {
        cout << "ERROR p > q";
        return 1;
    }
    Simulator S;
    double prev_prob = -1;
    while (counter < pow(10,6)){
        double prob = S.getAliceWonProb(counter+2,q,p); // since we have to have more than tow coin toss
        prev_prob = max(prev_prob,prob);
        counter++;

        if (prev_prob > prob) break;
    }


    cout << "Probability of head up for Alice = " << q << endl;
    cout << "Probability of head up for Bob = " << p << endl;

    cout << "The optimal number of coin tosses is " << counter;
    return 0;
}
