#include <iostream>
#include <map>

using namespace std;
//memotization
map<pair<int,int>,double> cardValues;

double value(int nred, int nblack){
    auto it = cardValues.find({nred,nblack});
    if (it != cardValues.end()) return it->second;

    double redProb = nred/(nred+nblack);
    double blackProb = nblack/(nred+nblack);

    if (nblack == 0 && nred ==1) return 1;
    if (nred == 0 && nblack ==1) return -1;

    double curVal= redProb*value(nred-1,nblack)+blackProb* value(nred,nblack-1);
    cardValues[{nred,nblack}] = max(curVal,(double) (nblack - nred));

    return curVal;
}
int main(int argc, char * const argv[]) {
    int totalCard;
    totalCard = 10;
//    sscanf(argv[1],"d",&totalCard);
    cout<< "Total Number of Cards = "<< totalCard << endl;
    cout << "Value of the game is " << value(totalCard/2,totalCard/2) << endl;


}
