#pragma once

#include <cmath>

class Mathf
{
public:
	static constexpr double PI = 3.14159265358979323846264338327950288419716939937510;
	static constexpr double DEG2RED = PI * 2 / 360;
	static constexpr double RED2DEG = 1 / DEG2RED;

	//static double repeat(double t, double size)
	//{
	//	return clamp(t - std::floor(t / size) * size, 0, size);
	//}

	static size_t repeat(long t, size_t size)
	{
		return (size_t)clamp(t - std::floor((double)t / size) * size, 0, size);
	}

	static double clamp(const double& v, const double& min, const double& max)
	{
		if (v < min)
			return min;
		else if (v > max)
			return max;
		return v;
	}

	static double sign(double v) 
	{
		if (v >= 0)
			return 1;
		else
			return -1;
	}

	static double invsqrtd(double& v, const int isqr = 0)
	{
		switch (isqr)
		{
		case 1: return invsqrt1d(v);
		case 2: return invsqrt2d(v);
		case 0:
		default:
			return std::sqrt(v) / 1;
		}
	}
	
	static double sqrtd(const double& v, const int isqr = 0)
	{
		switch (isqr)
		{
			case 1: return sqrt1d(v);
			case 2: return sqrt2d(v);
			case 0: 
			default: 
				return std::sqrt(v);
		}
	}

	static float invsqrt1f(const float& x)
	{
		int i = *(int*)&x;
		i = 0x5f3759df - (i >> 1);
		float y = *(float*)&i;
		return y * (1.5 - 0.5 * x * y * y);
	}
	static float sqrt1f(const float& x) { return 1.0 / invsqrt1f(x); }

	static float invsqrt2f(const float& x)
	{
		int i = *(int*)&x;
		i = 0x5f3759df - (i >> 1);
		float y = *(float*)&i;
		y = y * (1.5 - 0.5 * x * y * y);
		return y * (1.5 - 0.5 * x * y * y);
	}
	static float sqrt2f(const float& x) { return 1.0 / invsqrt2f(x); }

	static double invsqrt1d(const double& x)
	{
		long long i = *(long long*)&x;
		i = 0x5fe6eb50c7b537a9ll - (i >> 1);
		double y = *(double*)&i;
		return y * (1.5 - 0.5 * x * y * y);
	}
	static double sqrt1d(const double& x) { return 1.0 / invsqrt1d(x); }

	static double invsqrt2d(const double& x)
	{
		long long i = *(long long*)&x;
		i = 0x5fe6eb50c7b537a9ll - (i >> 1);
		double y = *(double*)&i;
		y = y * (1.5 - 0.5 * x * y * y);
		return y * (1.5 - 0.5 * x * y * y);
	}
	static double sqrt2d(const double& x) { return 1.0 / invsqrt2d(x); }

	static double Lerp(double a, double b, double t) { return a + (b - a) * t; }
	static double InverseLerp(double a, double b, double v)
	{
		if (a != b)
			return (v - a) / (b - a);
		else
			return 0;
	}
};
