#include <iostream>
#include <vector>
#include <fstream>
#include <map>
using namespace std;

class stable_marriage_instance
{
    // Private
    int no_of_couples;
    vector <vector <int> > Preference_of_men;
    vector <vector <int> > Preference_of_women;
    vector <int> match_for_men;
    vector <int> match_for_women;

    // private member function: checks if anybody is free in boolean "my_array"
    // returns the index of the first-person who is free in "my_array"
    // if no one is free it returns a -1.
    int anybody_free(vector <bool> my_array)
    {
        for (int i = 0; i<my_array.size(); i++) return i;
        return -1;
    }

    // private member function: if index1 is ranked higher than index2
    // in a preference array called "my_array" it returns "true"; otherwise
    // it returns "false"
    bool rank_check (vector <int> my_array, int index1, int index2)
    {
        return my_array[index1] < my_array[index2]; // rank higher 0 is the higest rank
    }

    // private member function: implements the Gale-Shapley algorithm
    void Gale_Shapley()
    {
        vector <bool> is_man_free;
        vector <bool> is_woman_free;
        vector <vector <bool> > has_this_man_proposed_to_this_woman;

        int man_index, woman_index;

        // initializing everything
        for (int i= 0; i < no_of_couples; i++){
            is_man_free[i] = 1;
            is_woman_free[i] = 1;
            vector<bool> currMan_proposed_to_this_woman;
            for (int j = 0; j < no_of_couples ; j++) {
                currMan_proposed_to_this_woman.push_back(-1);
            }
            has_this_man_proposed_to_this_woman.push_back(currMan_proposed_to_this_woman);
        }

        // Gale-Shapley Algorithm
        while ( (man_index = anybody_free(is_man_free)) >= 0)
        {
            for (woman_index = 0; woman_index < Preference_of_men[man_index].size();woman_index++){
            }
        }
    }

    // private member function: reads data
    void read_data(int argc, const char * argv[])
    {
        // fill the necessary code here.  The first entry in the input
        // file is the #couples, followed by the preferences of the men
        // and preferences of the women.  Keep in mind all indices start
        // from 0.

//        ifstream input_file(argv[1]);
        // testing files
        ifstream input_file("input.txt");
        //input men
        int input_value;
        if (input_file.is_open()){
            input_file >> input_value;
            no_of_couples = input_value;
            //input men
            for (int i = 0; i<no_of_couples; i++){
                //init the match vector for men
                match_for_men.push_back(-1);
                vector<int> currMenPref;
                for (int j = 0; j < no_of_couples; j++){
                    input_file >> input_value;
                    currMenPref.push_back(input_value);
                }
                Preference_of_men.push_back(currMenPref);
            }
            //input women
            for (int i = 0; i<no_of_couples; i++){
                //init the match vector for women
                match_for_women.push_back(-1);
                vector<int> currWomenPref;
                for (int j = 0; j < no_of_couples; j++){
                    input_file >> input_value;
                    currWomenPref.push_back(input_value);
                }
                Preference_of_women.push_back(currWomenPref);
            }
        }
        print_soln();

    }

    // private member function: print solution
    void print_soln()
    {
        cout << "Preferences of Men" << endl;
        cout << "------------------" << endl;
        // iterated men preference
        for (int i = 0; i < Preference_of_men.size(); i++){
            cout << "(" << i << "): ";
            for (int j = 0; j<Preference_of_men[i].size(); j++){
                cout << Preference_of_men[i][j] <<" ";
            }
            cout << endl;
        }
        // iterated women preference
        cout << "Preferences of Women"<< endl;
        cout << "--------------------" << endl;
        for (int i = 0; i<Preference_of_women.size(); i++){
            cout << "(" << i << "): ";
            for (int j = 0; j<Preference_of_women[i].size(); j++){
                cout << Preference_of_women[i][j] <<" ";
            }
            cout << endl;
        }
        cout << "Matching for Men" <<endl;
        for (int i = 0; i<match_for_men.size(); i++) cout << "Man: "<<i<< " -> Woman: "<< match_for_men[i] << endl;
        cout << "Matching for Women" << endl;
        for (int i = 0; i<match_for_women.size(); i++) cout << "Woman: "<<i<< " -> Man: "<< match_for_women[i] << endl;

    }

public:
    void solve_it(int argc, const char * argv[])
    {
        read_data(argc, argv);

        Gale_Shapley();

        print_soln();
    }
};

int main(int argc, const char * argv[])
{
    stable_marriage_instance x;
    x.solve_it(argc, argv);
}