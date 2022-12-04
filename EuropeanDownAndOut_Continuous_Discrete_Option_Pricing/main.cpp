#include <iostream>
#include <chrono>
#include <random>

using namespace std;
// configure seed for get uniform
unsigned seed = (unsigned)std::chrono::system_clock::now().time_since_epoch().count();
default_random_engine generator(seed);

double duration, rf, volatility, initial_stock_price ,K, B; // B Barrier Price
int n, m; // number of trails , number of sample point;

double getUniform() {
    uniform_real_distribution <double> distribution(0, 1.0);
    return distribution(generator);
}

int main(int argc, char* argv[]) {
    sscanf(argv[1], "%lf", &duration);
    sscanf(argv[2], "%lf", &rf);
    sscanf(argv[3], "%lf", &volatility);
    sscanf(argv[4], "%lf", &initial_stock_price);
    sscanf(argv[5], "%lf", &K);
    sscanf(argv[6], "%d", &n);
    sscanf(argv[7], "%d", &m);
    sscanf(argv[8], "%lf", &B);


    return 0;
}
