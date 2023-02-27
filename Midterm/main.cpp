#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <map>
#include<vector>
#include <algorithm>
#include <math.h>
#include<random>
#include<chrono>
using namespace std;
double r;// risk_free_rate
double K;// strike_price
double S_0;// initial_stock_price
double sigma;//volatility
int m;//no_of_divisions
int T;//Expiration_time
double B;// barrier_price
int n;//no_of_trials
double Theoretical_Call_Price, Theoretical_Put_Price;
double call, Adjust_Call, put, Adjust_Put;
default_random_engine generator;

double get_uniform() {
	uniform_real_distribution<double> distribution(0.0, 1.0);
	double number = distribution(generator);
	return (number);
}

double max(double a, double b) {
	return (b < a) ? a : b;
}

double N(const double& z) {
	if (z > 6.0) { return 1.0; };
	if (z < -6.0) { return 0.0; };
	double b1 = 0.31938153;
	double b2 = -0.356563782;
	double b3 = 1.781477937;
	double b4 = -1.821255978;
	double b5 = 1.330274429;
	double p = 0.2316419;
	double c2 = 0.3989423;
	double a = fabs(z);
	double t = 1.0 / (1.0 + a * p);
	double b = c2 * exp((-z) * (z / 2.0));
	double n = ((((b5 * t + b4) * t + b3) * t + b2) * t + b1) * t;
	n = 1.0 - b * n;
	if (z < 0.0) n = 1.0 - n;
	return n;
}

double option_price_put_black_scholes(const double& S, const double& K, const double& r, const double& sigma, const double& time) {
	double time_sqrt = sqrt(time);
	double d1 = (log(S / K) + r * time) / (sigma * time_sqrt) + 0.5 * sigma * time_sqrt;
	double d2 = d1 - (sigma * time_sqrt);
	return K * exp(-r * time) * N(-d2) - S * N(-d1);
}

double option_price_call_black_scholes(const double& S, const double& K, const double& r, const double& sigma, const double& time) {  // time to maturity
	double time_sqrt = sqrt(time);
	double d1 = (log(S / K) + r * time) / (sigma * time_sqrt) + 0.5 * sigma * time_sqrt;
	double d2 = d1 - (sigma * time_sqrt);
	return S * N(d1) - K * exp(-r * time) * N(d2);
}

double closed_form_down_and_out_european_call_option(double S_0, double K, double r, double sigma, int T)
{
	// I took this formula from Wilmott, Howison and Dewynne, "The Mathematics of Financial Derivatives"
	double K1 = (2 * r) / (sigma * sigma);
	double A = option_price_call_black_scholes(S_0, K,
		r, sigma, T);
	double B1 = (B * B) / S_0;
	double C = pow(S_0 / B, -(K1 - 1));
	double D = option_price_call_black_scholes(B1, K, r, sigma, T);
	return (A - D * C);
}

double closed_form_down_and_in_european_put_option(double S_0, double K, double r, double sigma, int T)
{
	// just making it easier by renaming the global variables locally
	double S = S_0;
	double rr = r;
	double TT = T;
	double sigmaa = sigma;
	double H = B;
	double X = K;

	// Took these formula from some online reference
	double lambda = (rr + ((sigmaa * sigmaa) / 2)) / (sigmaa * sigmaa);
	double temp = 2 * lambda - 2.0;
	double x1 = (log(S / H) / (sigmaa * sqrt(TT))) + (lambda * sigmaa * sqrt(TT));
	double y = (log(H * H / (S * X)) / (sigmaa * sqrt(TT))) + (lambda * sigmaa * sqrt(TT));
	double y1 = (log(H / S) / (sigmaa * sqrt(TT))) + (lambda * sigmaa * sqrt(TT));
	return (-S * N(-x1) + X * exp(-rr * TT) * N(-x1 + sigmaa * sqrt(TT)) +
		S * pow(H / S, 2 * lambda) * (N(y) - N(y1)) -
		X * exp(-rr * TT) * pow(H / S, temp) * (N(y - sigmaa * sqrt(TT)) - N(y1 - sigmaa * sqrt(TT))));
}

double closed_form_down_and_out_european_put_option(double S_0, double K, double r, double sigma, int T)
{
	double vanilla_put = option_price_put_black_scholes(S_0, K,
		r, sigma, T);
	double put_down_in = closed_form_down_and_in_european_put_option(S_0, K,
		r, sigma, T);
	return (vanilla_put - put_down_in);
}
void euro(int T, double r, double sigma, double S_0, double K, int n, int m, double B) {
	double deltaT, deltaR, deltaSD;
	double p;
	int count = 0;
	vector<double> S(5, 0);
	deltaT = T / (double)m;
	deltaR = (r - 0.5 * pow(sigma, 2)) * deltaT;
	deltaSD = sigma * sqrt(deltaT);
	double sum_payoff_call = 0.0;
	double sum_payoff_put = 0.0;
	double adj_sum_call = 0.0;
	double adj_sum_put = 0.0;

	for (int i = 0; i < n; ++i)
	{
		S[1] = S[2] = S[3] = S[4] = S_0;
		double callpf = 0.0;
		double putpf = 0.0;
		for (int j = 0; j < m; ++j) {
			double x = get_uniform();
			double y = get_uniform();
			double a = sqrt(-2.0 * log(x)) * cos(6.283185307999998 * y);
			double b = sqrt(-2.0 * log(x)) * sin(6.283185307999998 * y);
			S[1] *= exp(deltaR + deltaSD * a);
			S[2] *= exp(deltaR - deltaSD * a);
			S[3] *= exp(deltaR + deltaSD * b);
			S[4] *= exp(deltaR - deltaSD * b);
			for (int k = 1; k < 5; ++k) {
				if (S[k] <= B) {
					S[k] = 0;
				}
			}
		}
		for (int f = 1; f < 5; ++f) {
			if (S[f] > 0) {
				putpf += max(0, K - S[f]);
				callpf += max(0, S[f] - K);
				p = exp((-2) * log(S_0 / B) * log(S[f] / B) / (pow(sigma, 2) * T));
				adj_sum_put += max(0, K - S[f]) * (1 - p);
				adj_sum_call += max(0, S[f] - K) * (1 - p);
				count++;
			}
		}
		callpf /= 4;
		sum_payoff_call += callpf;
		putpf /= 4;
		sum_payoff_put += putpf;
	}

	call = sum_payoff_call / (n*exp(r * T));
	Adjust_Call = adj_sum_call / (count * exp(r * T));
	put = sum_payoff_put /(n* exp(r * T));
	Adjust_Put = adj_sum_put / (count * exp(r * T));
}

int main() {
/*
   cout << "expiration_time =";
   cin >> T;
   cout << "risk_free_rate =";
   cin >> r;
   cout << "volatility =";
   cin >> sigma;
   cout << "initial_stock_price =";
   cin >> S_0;
   cout << "strike_price =";
   cin >> K;
   cout << "barrier_price =";
   cin >> B;
   cout << "Number_of_trials =";
   cin >> n;
   cout << "Number_of_divisions =";
   cin >> m;
  */
    T = 1;
    r = 0.05;
    sigma = 0.25;
    S_0 = 50;
    K = 40;
    B = 20;
    n = 1000000;
    m = 1000;
    //n = 1000;
    //m = 100;

    cout << "European Down-and-Out Continuous Barrier Options Pricing via Monte Carlo Simulation" << endl;
    cout << "Expiration Time (Years) = " << T << endl;
    cout << "Risk Free Interest Rate = " << r << endl;
    cout << "Volatility (%age of stock value) = " << sigma * 100 << endl;
    cout << "Initial Stock Price = " << S_0 << endl;
    cout << "Strike Price = " << K << endl;
    cout << "Barrier Price = " << B << endl;
    cout << "Number of Trials = " << n << endl;
    cout << "Number of Divisions = " << m << endl;
    euro(T, r, sigma, S_0, K, n, m, B);
    Theoretical_Call_Price = closed_form_down_and_out_european_call_option(S_0, K, r, sigma, T);
    Theoretical_Put_Price = closed_form_down_and_out_european_put_option(S_0, K, r, sigma, T);
    cout << "---------------------------------" << endl;
    cout << "The average Call Price by explicit simulation  = " << call << endl;
    cout << "The call price using the (1-p)-adjustment term = " << Adjust_Call << endl;
    cout << "Theoretical Call Price                         = " << Theoretical_Call_Price << endl;
    cout << "---------------------------------" << endl;
    cout << "The average Put Price by explicit simulation   = " << put << endl;
    cout << "The put price using the (1-p)-adjustment term  = " << Adjust_Put << endl;
    cout << "Theoretical Put Price                          = " << Theoretical_Put_Price << endl;
    cout << "---------------------------------" << endl;
}