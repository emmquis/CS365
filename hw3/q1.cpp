//Black Scholes Merton Formula


#include <iostream>
#include <cmath>
using namespace std;

double cum_norm(double x)
{
    const double root = sqrt(0.5);
    return 0.5*(1.0 + erf(x*root));
}

int BS_Put_Call(const double &S, const double &K, const double &r, const double &q,
                const double &sigma, const double &T, const double &t, 
				double &call, double &delta_c, double &put, double &delta_p)
{
    call = 0;
    put = 0;
    delta_c = 0;
    delta_p = 0;
    
    double time = T-t;
    if (time < 0) 
		return 1;
		
    if (sigma < 0) 
		return 1;
		
	if (S <= 0) 
	{
		put = K*exp(-r*time) - S*exp(-q*time);
		delta_p = -exp(-q*time);
		return 0;
	}
			
    if (K <= 0) 
	{
        call = S*exp(-q*time) - K*exp(-r*time);
        delta_c = exp(-q*time);
        return 0;
    }
		
	if (time == 0) 
	{
        call = (S > K) ? (S-K) : 0.0;
        put = (S < K) ? (K-S) : 0.0;
        delta_c = (S > K) ? 1.0 : 0.0;
        delta_p = (S < K) ? 1.0 : 0.0;
        return 0;
    }
    
    double d1 = (log(S/K) +(r-q)*time)/(sigma*sqrt(time)) +0.5*(sigma*sqrt(time));
    double d2 = d1 - sigma*sqrt(time);
    double negd1 = -1*((log(S/K) +(r-q)*time)/(sigma*sqrt(time)) +0.5*(sigma*sqrt(time)));
    double negd2 = -1*(d1 - sigma*sqrt(time));
    
    double Nd1 = cum_norm(d1);
    double Nd2 = cum_norm(d2);
    double NNd1 = cum_norm(negd1);
    double NNd2 = cum_norm(negd2);
    
    call = S*exp(-q*time)*Nd1 -K*exp(-r*time)*Nd2;
    put = K*exp(-r*time)*NNd2 - S*exp(-q*time)*NNd1;
    delta_c = exp(-q*time)*Nd1;
    delta_p = -1*exp(-q*time)*NNd1;
    
    return 0;
}


int main() 
{

    double S = 100;
    double K = 100;
    double r = 0.1;
    double q = 0.0;
    double sigma = 0.5;
    double T = 0.3;
    double t = 0.0;
    double call, put, delta_c, delta_p;
    
    cout << "Example from Lecture 17a" << endl;
    BS_Put_Call(S, K, r, q, sigma, T, t, call, delta_c, put, delta_p);
    cout << "S = " << S << ", K = " << K << ", r = " << r << ", q = " << q
    	 << ", sigma = " << sigma << ", T = " << T << ", t0 = " << t << endl;
    cout << "Cbsm = " << call << endl;
    cout << "Pbsm = " << put << endl;
    cout << "Delta_c = " << delta_c << endl;
    cout << "Delta_p = " << delta_p << endl;
    
    cout << endl;
    
    double S2 = 100;
    double K2 = 100;
    double r2 = 0.1;
    double q2 = 0.1;
    double sigma2 = 0.5;
    double T2 = 0.4;
    double t2 = 0.0;
    double call2, put2, delta_c2, delta_p2;
    
    cout << "Example from Lecture 19a" << endl;
    BS_Put_Call(S2, K2, r2, q2, sigma2, T2, t2, call2, delta_c2, put2, delta_p2);
    cout << "S = " << S2 << ", K = " << K2 << ", r = " << r2 << ", q = " << q2
    	 << ", sigma = " << sigma2 << ", T = " << T2 << ", t0 = " << t2 << endl;
    cout << "Cbsm = " << call2 << endl;
    cout << "Pbsm = " << put2 << endl;
    cout << "Delta_c = " << delta_c2 << endl;
    cout << "Delta_p = " << delta_p2 << endl;

    return 0;
}
