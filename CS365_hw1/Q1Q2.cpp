//Emmanuel Silan CS365 HW #1
#include <iostream>
#include <cmath>
using namespace std;

//problem 1.1
double future_value(double F0, double t0, double t1, double r)
{
	double r_decimal = 0.01*r;
	double F1 = F0*exp(r_decimal*(t1-t0));
	return F1;
}

//problem 1.2
int df_and_r(double F0, double F1, double t0, double t1, double &df, double &r)
{
	if(t1-t0 == 0.0)
	{
		df = 0;
		r = 0;
		return -1;
	}
	else if((F0 < 0.0) || (F1 < 0.0))
	{
		df = 0;
		r = 0;
		return -2;
	}
	else
	{
		df = F0/F1;					// todays cf/future cf, from 1.3.2.2
		r = -1*(log(F0/F1))/(t1-t0) * 100;	// (ln(df))/(t1-t0) * 100
	}
	return 0;
}

//problem 1.3
int bp_from_y(double F, double c, double y, double &B, int n)
{
	B = 0;
	double y2 = 0.01*y;					// change y to decimal
	for(int i = 1; i < n; i++)
	{
		B += (c/2)/(pow(1+(y2/2), i));	// sum of b from 1 to n-1
	}
	B += (F+(c/2))/(pow(1+(y2/2), n));	// sum of b from 1 to n-1 and n, from 1.6.1.1
}

//problem 1.3.1
int yield(double F, double c, double B_market, double tol, int iter, double &y, int n)
{
	double tol_decimal = 0.01*tol;
	double y_low = 0, y_high = 100, B_low = 0, B_high = 0;
	bp_from_y(F, c, y_low, B_low, n);
	if(abs(B_low-B_market) <= 100)
	{
		y = y_low;
		return 0;
	}
	
	bp_from_y(F, c, y_high, B_high, n);
	if(abs(B_high-B_market) <= tol_decimal)
	{
		y = y_high;
		return 0;
	}
	
	if(B_low < B_market || B_high > B_market)
	{
		y = 0;
		return 1;
	}
	
	double B_mid = 0;
	for(int i = 0; i < iter; i++)
	{
		double y_mid = (y_low + y_high)/2;
		bp_from_y(F, c, y_mid, B_mid, n);
		if(abs(B_mid - B_market) <= tol_decimal)	// from 2.3 #9
		{
			y = y_mid;
			return 0;
		}
		else if(B_mid > B_market)			// from 2.3 #10
			y_low = y_mid;
		else								//(B_mid < B_target)
			y_high = y_mid;					// from 2.3 #11
		if(abs(y_high - y_low) <= tol_decimal)		// from 2.3 #14
		{
			y = y_mid;
			return 0;
		}
	}
	return -1;
}

//problem 1.4
void yield_curve(double y_05, double y_10, double y_15)
{
	double d_05 = 0, d_10 = 0, d_15 = 0;
	double r_05 = 0, r_10 = 0, r_15 = 0;
	
	d_05 = 1/(1+(0.01*y_05)/2);
	r_05 = -1*(log(d_05))/0.5 * 100;
	
	cout << "At 6 months the DF is " << d_05 << " and the Spot Rate is " << r_05 << "%" << endl;
	
	d_10 = (1 - ((0.01*y_10*d_05)/2))/(1 + (0.01*y_10)/2);
	r_10 = -1*(log(d_10))/1.0 * 100;
	
	cout << "At 12 months the DF is " << d_10 << " and the Spot Rate is " << r_10 << "%" << endl;
	
	d_15 = (1 - ((0.01*y_15*d_05)/2) - ((0.01*y_15*d_10)/2))/(1+ (0.01*y_15)/2);
	r_15 = -1*(log(d_15))/1.5 * 100;
	
	cout << "At 18 months the DF is " << d_15 << " and the Spot Rate is " << r_15 << "%" << endl;
}

int main()
{
	
	double F0_1, t0_1, t1_1, r_1;
	cout << "Calculating Future Cashflow" << endl;
	cout << "Enter today's Cashflow: ";
	cin >>  F0_1;
	cout << "Enter Today's Year: ";
	cin >> t0_1;
	cout << "Enter Future Year: ";
	cin >> t1_1;
	cout << "Enter Interest Rate (%): ";
	cin >> r_1;
	cout << "The Future value is: " << future_value(F0_1, t0_1, t1_1, r_1) << endl << endl;
	
	double F0_2, F1_2, t0_2, t1_2, df_2, r_2;
	cout << "Calculating Discount Factor and Interest Rate" << endl;
	cout << "Enter Today's Cashflow: ";
	cin >> F0_2;
	cout << "Enter Future's Cashflow: ";
	cin >> F1_2;
	cout << "Enter Today's Year: ";
	cin >> t0_2;
	cout << "Enter Future Year: ";
	cin >> t1_2;
	df_and_r(F0_2, F1_2, t0_2, t1_2, df_2, r_2);
	cout << "The discount factor is " << df_2 << " and the rate is " << r_2 << "%"<< endl << endl;
	
	double F_3, c_3, y_3, B_3, n_3;
	cout << "Calculating the Bond Price" << endl;
	cout << "Enter the Cash Value: ";
	cin >> F_3;
	cout << "Enter the Coupon Value: ";
	cin >> c_3;
	cout << "Enter the Yield Rate (%): ";
	cin >> y_3;
	cout << "Enter Number of Cashflows: ";
	cin >> n_3;
	bp_from_y(F_3, c_3, y_3, B_3, n_3);
	cout << "The Bond Price is: " << B_3 << endl << endl;
	
	
	double F_4, c_4, Bm_4, tol_4, iter_4, y_4, n_4;
	cout << "Calculating the Yield" << endl;
	cout << "Enter the Cash Value: ";
	cin >> F_4;
	cout << "Enter the Coupon Value: ";
	cin >> c_4;
	cout << "Enter the Bond Market Price: ";
	cin >> Bm_4;
	cout << "Enter the Tolerance: "; 
	cin >> tol_4;
	cout << "Enter the Number of iterations: ";
	cin >> iter_4;
	cout << "Enter the Number of Cashflows: ";
	cin >> n_4;
	int yield_result = yield(F_4, c_4, Bm_4, tol_4, iter_4, y_4, n_4);
	if(yield_result == 0)
		cout << "The Yield is: " << y_4 << endl << endl;
	else
		cout << "The Yield is out of bounds" << endl << endl;
	
	double y05_5, y10_5, y15_5;
	cout << "Calculating the Yield Curve for an 18 month period" << endl;
	cout << "Enter the Yield for 6 months (%): ";
	cin >> y05_5; 
	cout << "Enter the Yield for 12 months (%): ";
	cin >> y10_5;
	cout << "Enter the Yield for 18 months (%): ";
	cin >> y15_5;
	yield_curve(y05_5, y10_5, y15_5);
	
 	return 0;
}
