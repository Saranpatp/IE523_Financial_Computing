#include <iostream>
#include <map>

using namespace std;
//memotization
map<pair<int,int>,double> cardValues;

double value(double nred, double nblack){
    //Search in carValues first
    auto it = cardValues.find({nred,nblack});
    if (it != cardValues.end()) return cardValues[{nred,nblack}];

    double redProb = nred/(nred+nblack);
    double blackProb = nblack/(nred+nblack);

    if (nblack == 0 ) return 0; // you gain here
    if (nred == 0) return nblack; // the cost that you have to pay

    double curVal= redProb*value(nred-1,nblack)+blackProb* value(nred,nblack-1); // possible value that you could get
    cardValues[{(int) nred,(int) nblack}] = max(curVal,nblack - nred); // vs current value

    return cardValues[{nred,nblack}];
}
int main(int argc, char * const argv[]) {
    int totalCard;
//    totalCard = 20;
    sscanf(argv[1],"%d",&totalCard);
    cout<< "Total Number of Cards = "<< totalCard << endl;
    cout << "Value of the game is " << value(totalCard/2,totalCard/2) << endl;


}
