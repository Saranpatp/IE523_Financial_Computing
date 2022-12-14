#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main(int argc,char* argv[]) {
    int size_of_array, n;
    int *array;

    sscanf(argv[1],"%d",&size_of_array);
    sscanf(argv[2],"%d",&n);

//    size_of_array = 5;
//    n = 3;

    array = new int[size_of_array];
    for (int i = 0; i<size_of_array; i++){
        array[i]=i+1;
    }

    vector<bool> did_i_pick_this(size_of_array,true);
    fill(did_i_pick_this.begin() ,did_i_pick_this.begin()+n, false);

    do{
        cout<<"{ ";
        for(int i = 0;i<size_of_array; i++){
            if(!did_i_pick_this[i]) cout<<array[i]<< " ";

        }
        cout<<"}"<<endl;
    }while(next_permutation(did_i_pick_this.begin(),did_i_pick_this.end()));
    return 0;
}
