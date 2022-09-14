#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main(int argc,char* argv[]) {
    int size_of_set;
    int *array;

    sscanf(argv[1],"%d",&size_of_set);
    array = new int[size_of_set];
    for (int i = size_of_set-1; i>=0; i--){
        array[i]=size_of_set - i;
    }
    vector<bool> did_i_pick_this(size_of_set);

    for(int j = 0; j<size_of_set; j++){
        do{
            cout<<"{ ";
            for(int i = did_i_pick_this.size()-1;i>=0; i--){
                if(did_i_pick_this[i]) cout<<array[i]<< " ";
            }
            cout<<"}"<<endl;
        }while(next_permutation(did_i_pick_this.begin(),did_i_pick_this.end()));
        did_i_pick_this[size_of_set-1-j]=true;
    }
    cout<<"{ ";
    for(int i = did_i_pick_this.size()-1;i>=0; i--){
        if(did_i_pick_this[i]) cout<<array[i]<< " ";
    }
    cout<<"}"<<endl;
    return 0;
}
