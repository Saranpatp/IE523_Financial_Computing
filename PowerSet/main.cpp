#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main(int argc,char* argv[]) {
    int size_of_set;
    int *array;

    sscanf(argv[1],"%d",&size_of_set);

    array = new int[size_of_set];
    for (int i = 0; i<size_of_set; i++){
        array[i]=i+1;
    }

    if (size_of_set >= 0) cout<<"{}"<<endl;

    vector<bool> did_i_pick_this(size_of_set);
    for(int j = 0; j<size_of_set; j++){
        did_i_pick_this[j]=true;
        do{
            cout<<"{ ";
            for(int i = 0;i<size_of_set; i++){
                if(did_i_pick_this[i]) cout<<array[i]<< " ";
            }
            cout<<"}"<<endl;
        }while(prev_permutation(did_i_pick_this.begin(),did_i_pick_this.end()));
    }

    return 0;
}
