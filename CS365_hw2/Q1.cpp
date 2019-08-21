/*
HW #2
Calculating Fair Value	
*/

#include <iostream>
#include <vector>
#include <math.h>

class Bond
{
private:
	// data
	double Face;
	double T_maturity;
	int cpn_freq;
	int num_coupon_periods;
	std::vector<double> coupons;
public:
	Bond(double T, double F, std::vector<double>& c, int freq)
	{
		if(F >= 0) 
			Face = F;
		if(F < 0)
			Face = 0;		
		if(freq >= 1)
			cpn_freq = freq;
		if(freq < 1)
			cpn_freq = 1;

			
		const double tol = 1.0e-6;
		num_coupon_periods = int(cpn_freq*T + tol);
		
		if(num_coupon_periods < 0)
			num_coupon_periods = 0;
		
		if(num_coupon_periods >= 0)
			T_maturity = (double)num_coupon_periods / (double)cpn_freq;
		
		if(num_coupon_periods > 0)
		{
			coupons.resize(num_coupon_periods);
			set_coupons(c);
		}
	}

	// public methods
	void set_coupons(const std::vector<double> & c)
	{
		for(int i = 0; i < num_coupon_periods; ++i)
		{
			if(i < c.size())
				coupons[i] = std::max(c[i], 0.0);
			
			else 
				coupons[i] = std::max(c.back(), 0.0);
		}
	}

	int FV_duration(double t0, double y, double &B, double &Macaulay_duration, double &modified_duration) const
	{
		B = 0;
		Macaulay_duration = 0;
		modified_duration = 0;
		const double tol = 1.0e-6;
		
		if(num_coupon_periods <= 0 || t0 >= T_maturity)
			return 1;
		double y_decimal = 0.01 * y;
		
		for(int i = 1; i <= num_coupon_periods; ++i)
		{
			double t_i = double(i)/double(cpn_freq);
			double numerator_i = coupons[i-1]/cpn_freq;
			
			if(t_i  >= (t0+tol))
			{
				if(i < num_coupon_periods)
				{
					B += (numerator_i) / pow((1+y_decimal/cpn_freq), (cpn_freq*(t_i - t0)));
					
					Macaulay_duration += (((t_i - t0) * numerator_i) / pow((1+y_decimal/cpn_freq) , (cpn_freq*(t_i - t0))));
				}
				else
					B += Face + (numerator_i) / pow((1+y_decimal/cpn_freq), (cpn_freq*(t_i - t0)));				
			}

		}
		Macaulay_duration = Macaulay_duration*(1/B);
		modified_duration = Macaulay_duration/(1+y_decimal/cpn_freq);
		
		return 0;
	}

	double FairValue(double t0, double y) const
	{
		double B = 0;
		double Macaulay_duration = 0;
		double modified_duration = 0;
		FV_duration(t0, y, B, Macaulay_duration, modified_duration);
		return B;
	}

	double maturity() const { return T_maturity; }
	
	int num_periods() const { return num_coupon_periods; }
	
	int yield(const Bond& bond, double B_target, double t0, double & y, int & num_iter, double tol=1.0e-4, int max_iter=100)
	{
		y = 0;
		num_iter = 0;
		if(B_target <= 0.0 || bond.num_periods() <= 0 || bond.maturity())
			return 1;
		
		double y_low = 0.0;
		double y_high = 100.0;
		double B_y_low = FairValue(t0, y_low);
		double diff_B_y_low = B_y_low - B_target;
		
		if(std::abs(diff_B_y_low <= tol))
		{
			y = y_low;
			return 0;
		}
		double B_y_high = bond.FairValue(t0, y_high);
		double diff_B_y_high = B_y_high - B_target;
		if(std::abs(diff_B_y_high <= tol))
		{
			y = y_high;
			return 0;
		}
		if(diff_B_y_low * diff_B_y_high > 0)
		{
			y = 0;
			return 1;
		}
		double B = 0;
		double diff_B;
		for(num_iter = 1; num_iter < max_iter; ++num_iter)
		{
			y = (y_low+y_high)/2.0;
			B = bond.FairValue(t0, y);
			diff_B = B - B_target;
			if(std::abs(diff_B < tol))
			{
				return 0;
			}
			if(diff_B*diff_B_y_low > 0.0)
			{
				y_low = y;
			}
			else
			{
				y_high = y;			
			}
			if(y_high - y_low < tol)
				return 0;
		}
		y = 0;
		return 1;
		
		return 0;		
	}

};

int main()
{
	std::vector<double> c (1, 0);
	Bond *bond = new Bond(1,100,c,1);
	double fv = bond->FairValue(0,0);
	std::cout << "Fair Value (t0=0 & y=0) = " << fv << "\n";
		
	return 0;
}
