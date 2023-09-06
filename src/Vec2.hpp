#pragma once

#include <stdint.h>
#include <cmath>
#include <limits>
#include <string>

// my
#include "Mathf.hpp"

#if DEBUG_LOG
#include <iostream>
#endif

class Vec2
{
public:
	static constexpr double kEpsilon = 0.00001;
	static constexpr double kEpsilond = kEpsilon * kEpsilon;

	static const Vec2 zero;
	static const Vec2 five;
	static const Vec2 one;
	static const Vec2 mone;
	static const Vec2 up;
	static const Vec2 down;
	static const Vec2 left;
	static const Vec2 right;
	static const Vec2 positiveInfinityVector;
	static const Vec2 negativeInfinityVector;

	long    id;
	long    w;     //work

	double  x;
	double  y;

	Vec2() : id(-1), w(-1), x(0), y(0) {}
	Vec2(double xy) : id(-1), w(-1), x(xy), y(xy) {}
	Vec2(double x, double y) : id(-1), w(-1), x(x), y(y) {}
	Vec2(double x, double y, long id) : id(id), w(-1), x(x), y(y) {}
	Vec2(double x, double y, long id, long w) : id(id), w(w), x(x), y(y) {}

	Vec2 Set(const Vec2& v) { x = v.x; y = v.y; return *this; }

	Vec2 normalized(const int isqr = 0) const
	{
		Vec2 v(x, y);
		v.normalize(isqr);
		return v;
	}
	Vec2 normalize(const int isqr = 0)
	{
		double d = magnitude(isqr);
		if (d > kEpsilon)
			*this /= d;
		else
			*this *= zero;
		return *this;
	}

	static double distance(const Vec2& a, const Vec2& b, const int isqr = 0) { return magnitude(CalcSub(b, a), isqr); }

	double magnitude(const int isqr = 0) const { double d = sqrmagnitude(); return Mathf::sqrtd(d, isqr); }
	static double magnitude(const Vec2& v, const int isqr = 0) { double d = sqrmagnitude(v); return Mathf::sqrtd(d, isqr); }

	double sqrmagnitude() const { return x * x + y * y; }
	static double sqrmagnitude(const Vec2& v) { return v.x * v.x + v.y * v.y; }

	double angle(const Vec2& to, const int isqr = 0) { return angle(*this, to, isqr); }
	static double angle(const Vec2& from, const Vec2& to, const int isqr = 0)
	{
		double ss = from.sqrmagnitude() * to.sqrmagnitude();
		double den = Mathf::sqrtd(ss, isqr);
		if (den < kEpsilond)
			return 0;
		double d = Mathf::clamp(dot(from, to) / den, -1, 1);
		return std::acos(d) * Mathf::RED2DEG;
	}

	double signedAngle(const Vec2& to, const int isqr = 0) { return signedAngle(*this, to, isqr); }
	static double signedAngle(const Vec2& from, const Vec2& to, const int isqr = 0)
	{
		double ua = angle(from, to);
		double s = Mathf::sign(from.x * to.y - from.y * to.x);
		return ua * s;
	}

	static double dot(const Vec2& lhs, const Vec2& rhs) { return lhs.x * rhs.x + lhs.y * rhs.y; }
	double dot(const Vec2& rhs) const { return x * rhs.x + y * rhs.y; }

	static double cross(const Vec2& lhs, const Vec2& rhs) { return lhs.x * rhs.y - rhs.x * lhs.y; }
	double cross(const Vec2& rhs) const { return x * rhs.y - rhs.x * y; }

	static double cross3(const Vec2& bhs, const Vec2& lhs, const Vec2& rhs) { return cross(CalcSub(lhs, bhs), CalcSub(rhs, bhs)); }

	static Vec2 perpendicular(const Vec2& a) { return Vec2(-a.y, a.x); }
	Vec2 perpendicular() { return Set(Vec2(-y, x)); }

	double operator[](int i) const
	{
		switch (i)
		{
		case 0: return x;
		case 1: return y;
		default:
			return 0;
		}
	}
	double& operator[](int i)
	{
		switch (i)
		{
		case 0: return x;
		case 1: return y;
		default:
			return x;
		}
	}

	Vec2 operator+(const Vec2& v) { return Vec2(x + v.x, y + v.y); }
	Vec2 operator+(const double& d) { return Vec2(x + d, y + d); }
	Vec2 operator+=(const Vec2& v) { x += v.x; y += v.y; return *this; }
	Vec2 operator+=(const double& d) { x += d; y += d; return *this; }
	static Vec2 CalcAdd(const Vec2& a, const Vec2& b) { return Vec2(a.x + b.x, a.y + b.y); }
	static Vec2 CalcAdd(const Vec2& a, const double& d) { return Vec2(a.x + d, a.y + d); }

	Vec2 operator-(const Vec2& v) { return Vec2(x - v.x, y - v.y); }
	Vec2 operator-(const double& d) { return Vec2(x - d, y - d); }
	Vec2 operator-() { return Vec2(-x, -y); }
	Vec2 operator-=(const Vec2& v) { x -= v.x; y -= v.y; return *this; }
	Vec2 operator-=(const double& d) { x -= d; y -= d; return *this; }
	static Vec2 CalcSub(const Vec2& a, const Vec2& b) { return Vec2(a.x - b.x, a.y - b.y); }
	static Vec2 CalcSub(const Vec2& a, const double& d) { return Vec2(a.x - d, a.y - d); }

	Vec2 operator*(const Vec2& v) { return Vec2(x * v.x, y * v.y); }
	Vec2 operator*(const double& d) { return Vec2(x * d, y * d); }
	Vec2 operator*=(const Vec2& v) { x *= v.x; y *= v.y; return *this; }
	Vec2 operator*=(const double& d) { x *= d; y *= d; return *this; }
	static Vec2 CalcMul(const Vec2& a, const Vec2& b) { return Vec2(a.x * b.x, a.y * b.y); }
	static Vec2 CalcMul(const Vec2& a, const double& d) { return Vec2(a.x * d, a.y * d); }

	Vec2 operator/(const Vec2& v) { return Vec2(x / v.x, y / v.y); }
	Vec2 operator/(const double& d) { return Vec2(x / d, y / d); }
	Vec2 operator/=(const Vec2& v) { x /= v.x; y /= v.y; return *this; }
	Vec2 operator/=(const double& d) { x /= d; y /= d; return *this; }
	static Vec2 CalcDiv(const Vec2& a, const Vec2& b) { return Vec2(a.x / b.x, a.y / b.y); }
	static Vec2 CalcDiv(const Vec2& a, const double& d) { return Vec2(a.x / d, a.y / d); }

	Vec2 operator%(const Vec2& v) { return Vec2(fmod(x, v.x), fmod(y, v.y)); }
	Vec2 operator%(const double& d) { return Vec2(fmod(x, d), fmod(y, d)); }
	Vec2 operator%=(const Vec2& v) { x = fmod(x, v.x); y = fmod(y, v.y); return *this; }
	Vec2 operator%=(const double& d) { x = fmod(x, d); y = fmod(y, d); return *this; }
	static Vec2 CalcMod(const Vec2& a, const Vec2& b) { return Vec2(fmod(a.x, b.x), fmod(a.y, b.y)); }
	static Vec2 CalcMod(const Vec2& a, const double& d) { return Vec2(fmod(a.x, d), fmod(a.y, d)); }
	
	bool operator<(const Vec2& v) { return x < v.x && y < v.y; }
	bool operator<(const double& d) { return x < d && y < d; }
	bool operator<=(const Vec2& v) { return x <= v.x && y <= v.y; }
	bool operator<=(const double& d) { return x <= d && y <= d; }

	bool operator>(const Vec2& v) { return x > v.x && y > v.y; }
	bool operator>(const double& d) { return x > d && y > d; }
	bool operator>=(const Vec2& v) { return x >= v.x && y >= v.y; }
	bool operator>=(const double& d) { return x >= d && y >= d; }

	bool operator|=(const Vec2& v) { return sqrmagnitude() - v.sqrmagnitude() < kEpsilond; }
	bool operator==(const Vec2& v) { return std::abs(x - v.x) < kEpsilond && std::abs(y - v.y) < kEpsilond; }
	bool operator!=(const Vec2& v) { return !(*this == v); }

	std::size_t HashCode() const {
		std::size_t hash = 0;
		std::hash<double> hasher;

		hash ^= hasher(x) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
		hash ^= hasher(y) + 0x9e3779b9 + (hash << 6) + (hash >> 2);

		return hash;
	}

	std::string ToString() const
	{
		std::string o = "";
		o += std::to_string(id);
		o += ",";
		o += std::to_string(x);
		o += ",";
		o += std::to_string(y);
		return o;
	}

#if DEBUG_LOG
	// 0=none
	// 1=id
	// 2=w
	// 3=idw
    void debug(uint_fast8_t t = 0, bool r = true) const
    {
        std::cout << "(" << x << ", " << y << ")";
        switch (t)
        {
            case 1:
                std::cout << "\tid:" << id;
                break;
            case 2:
                std::cout << "\tw:" << w;
                break;
            case 3:
                std::cout << "\tid:" << id << "\tw:" << w;
                break;
            case 0: 
            default:
                break;
        }
        if (r)
            std::cout << std::endl;
    }
#endif
};

const Vec2 Vec2::zero = Vec2(0.0, 0.0);
const Vec2 Vec2::five = Vec2(0.5, 0.5);
const Vec2 Vec2::one = Vec2(1.0, 1.0);
const Vec2 Vec2::mone = Vec2(-1.0, -1.0);
const Vec2 Vec2::up = Vec2(0.0, 1.0);
const Vec2 Vec2::down = Vec2(0.0, -1.0);
const Vec2 Vec2::left = Vec2(-1.0, 0.0);
const Vec2 Vec2::right = Vec2(1.0, 0.0);
const Vec2 Vec2::positiveInfinityVector = Vec2(std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity());
const Vec2 Vec2::negativeInfinityVector = Vec2(-std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity());
