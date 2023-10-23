#pragma once

#include <stdint.h>
#include <cmath>
#include <limits>
#include <string>
#include <functional>

// my
#include "Mathf.hpp"
#include "Vec2.hpp"

#if DEBUG_LOG
#include <iostream>
// #include <bitset>
#endif

class Vec3
{
public:
	static constexpr double kEpsilon = 0.00001;
	static constexpr double kEpsilond = kEpsilon * kEpsilon;

	static const Vec3 zero;
	static const Vec3 five;
	static const Vec3 one;
	static const Vec3 mone;
	static const Vec3 up;
	static const Vec3 down;
	static const Vec3 left;
	static const Vec3 right;
	static const Vec3 forward;
	static const Vec3 back;
	static const Vec3 positiveInfinityVector;
	static const Vec3 negativeInfinityVector;

	long    id 	= -1;
	long    w	= -1;     //work

	double  x  	= 0;
	double  y	= 0;
	double  z	= 0;

	Vec3() {}
	Vec3(double xyz) : x(xyz), y(xyz), z(xyz) {}
	Vec3(double x, double y) : x(x), y(y) {}
	Vec3(double x, double y, long id) : id(id), x(x), y(y) {}
	Vec3(double x, double y, long id, long w) : id(id), w(w), x(x), y(y) {}
	Vec3(double x, double y, double z) : x(x), y(y), z(z) {}
	Vec3(double x, double y, double z, long id) : id(id), x(x), y(y), z(z) {}
	Vec3(double x, double y, double z, long id, long w) : id(id), w(w), x(x), y(y), z(z) {}

	Vec3 Set(const Vec2& v) { x = v.x; y = v.y; return *this; }
	Vec3 Set(const Vec3& v) { x = v.x; y = v.y; z = v.z; return *this; }

	Vec3 normalized(const int isqr = 0) const
	{
		Vec3 v(x, y);
		v.normalize(isqr);
		return v;
	}
	Vec3 normalize(const int isqr = 0)
	{
		double d = magnitude(isqr);
		if (d > kEpsilon)
			*this /= d;
		else
			*this *= zero;
		return *this;
	}

	static double distance(const Vec3& a, const Vec3& b, const int isqr = 0) { return magnitude(CalcSub(b, a), isqr); }
	static std::function<double(Vec3, Vec3)> distance() { return [](Vec3 a, Vec3 b) { return distance(b, a); }; }

	double magnitude(const int isqr = 0) const { double d = sqrmagnitude(); return Mathf::sqrtd(d, isqr); }
	static double magnitude(const Vec3& v, const int isqr = 0) { double d = sqrmagnitude(v); return Mathf::sqrtd(d, isqr); }

	double sqrmagnitude() const { return x * x + y * y + z * z; }
	static double sqrmagnitude(const Vec3& v) { return v.x * v.x + v.y * v.y + v.z * v.z; }

	double angle(const Vec3& to, const int isqr = 0) { return angle(*this, to, isqr); }
	static double angle(const Vec3& from, const Vec3& to, const int isqr = 0)
	{
		double ss = from.sqrmagnitude() * to.sqrmagnitude();
		double den = Mathf::sqrtd(ss, isqr);
		if (den < kEpsilond)
			return 0;
		double d = Mathf::clamp(dot(from, to) / den, -1, 1);
		return std::acos(d) * Mathf::RED2DEG;
	}

	double signedAngle(const Vec3& to, const int isqr = 0) { return signedAngle(*this, to, isqr); }
	static double signedAngle(const Vec3& from, const Vec3& to, const int isqr = 0)
	{
		double ua = angle(from, to);
		double s = Mathf::sign(from.x * to.y - from.y * to.x);
		return ua * s;
	}

	static double dot(const Vec3& lhs, const Vec3& rhs) { return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z; }
	double dot(const Vec3& rhs) const { return x * rhs.x + y * rhs.y + z * rhs.z; }

	static Vec3 cross(const Vec3& lhs, const Vec3& rhs)
	{
		return Vec3(
			lhs.y * rhs.z - lhs.z * rhs.y,
			lhs.z * rhs.x - lhs.x * rhs.z,
			lhs.x * rhs.y - lhs.y * rhs.x
		);
	}
	Vec3 cross(const Vec3& rhs) const
	{
		return Vec3(
			y * rhs.z - z * rhs.y,
			z * rhs.x - x * rhs.z,
			x * rhs.y - y * rhs.x
		);	
	}

	static Vec3 cross3(const Vec3& bhs, const Vec3& lhs, const Vec3& rhs) { return cross(CalcSub(lhs, bhs), CalcSub(rhs, bhs)); }

	static Vec3 Lerp(Vec3& a, Vec3& b, double t) { return a + (b - a) * t; }
	static Vec3 InverseLerp(Vec3& a, Vec3& b, Vec3& v)
	{
		if (a != b)
			return (v - a) / (b - a);
		else
			return zero;
	}

	static Vec3 perpendicularxy(const Vec3& a) { return Vec3(-a.y, a.x, a.z); }
	Vec3 perpendicularxy() { return Set(Vec3(-y, x, z)); }
	static Vec3 perpendicularxz(const Vec3& a) { return Vec3(-a.z, a.y, a.x); }
	Vec3 perpendicularxz() { return Set(Vec3(-z, y, x)); }
	static Vec3 perpendicularyz(const Vec3& a) { return Vec3(a.x, -a.z, a.y); }
	Vec3 perpendicularyz() { return Set(Vec3(x, -z, y)); }

	double operator[](int i) const
	{
		switch (i)
		{
		case 0: return x;
		case 1: return y;
		case 2: return z;
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
		case 2: return z;
		default:
			return x;
		}
	}

	Vec2 xy() { return Vec2(x, y); }
	Vec2 yx() { return Vec2(y, x); }
	Vec2 xz() { return Vec2(x, z); }
	Vec2 zx() { return Vec2(z, x); }
	Vec2 yz() { return Vec2(y, z); }
	Vec2 zy() { return Vec2(z, y); }

	Vec3 xyz() { return Vec3(x, y, z); }
	Vec3 xzy() { return Vec3(x, z, y); }
	Vec3 yxz() { return Vec3(y, x, z); }
	Vec3 yzx() { return Vec3(y, z, x); }
	Vec3 zxy() { return Vec3(z, x, y); }
	Vec3 zyx() { return Vec3(z, y, x); }
	
	static Vec3 min(Vec3& a, Vec3& b) { return Vec3(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z)); }
	static Vec3 max(Vec3& a, Vec3& b) { return Vec3(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z)); }

	Vec3 operator+(const Vec3& v) { return Vec3(x + v.x, y + v.y, z + v.z); }
	Vec3 operator+(const double& d) { return Vec3(x + d, y + d, z + d); }
	Vec3 operator+=(const Vec3& v) { x += v.x; y += v.y; z += v.z; return *this; }
	Vec3 operator+=(const double& d) { x += d; y += d; z += d; return *this; }
	static Vec3 CalcAdd(const Vec3& a, const Vec3& b) { return Vec3(a.x + b.x, a.y + b.y, a.z + b.z); }
	static Vec3 CalcAdd(const Vec3& a, const double& d) { return Vec3(a.x + d, a.y + d, a.z + d); }

	Vec3 operator-(const Vec3& v) { return Vec3(x - v.x, y - v.y, z - v.z); }
	Vec3 operator-(const double& d) { return Vec3(x - d, y - d, z - d); }
	Vec3 operator-() { return Vec3(-x, -y); }
	Vec3 operator-=(const Vec3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
	Vec3 operator-=(const double& d) { x -= d; y -= d; z -= d; return *this; }
	static Vec3 CalcSub(const Vec3& a, const Vec3& b) { return Vec3(a.x - b.x, a.y - b.y, a.z - b.z); }
	static Vec3 CalcSub(const Vec3& a, const double& d) { return Vec3(a.x - d, a.y - d, a.z - d); }

	Vec3 operator*(const Vec3& v) { return Vec3(x * v.x, y * v.y, z * v.z); }
	Vec3 operator*(const double& d) { return Vec3(x * d, y * d, z * d); }
	Vec3 operator*=(const Vec3& v) { x *= v.x; y *= v.y; z *= v.z; return *this; }
	Vec3 operator*=(const double& d) { x *= d; y *= d; z *= d; return *this; }
	static Vec3 CalcMul(const Vec3& a, const Vec3& b) { return Vec3(a.x * b.x, a.y * b.y, a.z * b.z); }
	static Vec3 CalcMul(const Vec3& a, const double& d) { return Vec3(a.x * d, a.y * d, a.z * d); }

	Vec3 operator/(const Vec3& v) { return Vec3(x / v.x, y / v.y, z / v.z); }
	Vec3 operator/(const double& d) { return Vec3(x / d, y / d, z / d); }
	Vec3 operator/=(const Vec3& v) { x /= v.x; y /= v.y; z /= v.z; return *this; }
	Vec3 operator/=(const double& d) { x /= d; y /= d; z /= d; return *this; }
	static Vec3 CalcDiv(const Vec3& a, const Vec3& b) { return Vec3(a.x / b.x, a.y / b.y, a.z / b.z); }
	static Vec3 CalcDiv(const Vec3& a, const double& d) { return Vec3(a.x / d, a.y / d, a.z / d); }

	Vec3 operator%(const Vec3& v) { return Vec3(fmod(x, v.x), fmod(y, v.y), fmod(z, v.z)); }
	Vec3 operator%(const double& d) { return Vec3(fmod(x, d), fmod(y, d), fmod(z, d)); }
	Vec3 operator%=(const Vec3& v) { x = fmod(x, v.x); y = fmod(y, v.y); z = fmod(z, v.z); return *this; }
	Vec3 operator%=(const double& d) { x = fmod(x, d); y = fmod(y, d); z = fmod(z, d); return *this; }
	static Vec3 CalcMod(const Vec3& a, const Vec3& b) { return Vec3(fmod(a.x, b.x), fmod(a.y, b.y), fmod(a.z, b.z)); }
	static Vec3 CalcMod(const Vec3& a, const double& d) { return Vec3(fmod(a.x, d), fmod(a.y, d), fmod(a.z, d)); }
	
	bool operator<(const Vec3& v) { return x < v.x && y < v.y && z < v.z; }
	bool operator<(const double& d) { return x < d && y < d && z < d; }
	bool operator<=(const Vec3& v) { return x <= v.x && y <= v.y && z <= v.z; }
	bool operator<=(const double& d) { return x <= d && y <= d && z <= d; }
	static bool less(const Vec3& a, const Vec3& b) { return lessx(a, b) && lessy(a, b) && lessz(a, b); }
	static bool lessx(const Vec3& a, const Vec3& b) { return a.x < b.x; }
	static bool lessy(const Vec3& a, const Vec3& b) { return a.y < b.y; }
	static bool lessz(const Vec3& a, const Vec3& b) { return a.z < b.z; }

	bool operator>(const Vec3& v) { return x > v.x && y > v.y && z > v.z; }
	bool operator>(const double& d) { return x > d && y > d && z > d; }
	bool operator>=(const Vec3& v) { return x >= v.x && y >= v.y && z >= v.z; }
	bool operator>=(const double& d) { return x >= d && y >= d && z >= d; }
	static bool greater(const Vec3& a, const Vec3& b) { return greaterx(a, b) && greatery(a, b) && greaterz(a, b); }
	static bool greaterx(const Vec3& a, const Vec3& b) { return a.x > b.x; }
	static bool greatery(const Vec3& a, const Vec3& b) { return a.y > b.y; }
	static bool greaterz(const Vec3& a, const Vec3& b) { return a.z > b.z; }

	bool operator|=(const Vec3& v) { return sqrmagnitude() - v.sqrmagnitude() < kEpsilond; }
	bool operator==(const Vec3& v) { return std::abs(x - v.x) < kEpsilond && std::abs(y - v.y) < kEpsilond && std::abs(z - v.z) < kEpsilond; }
	bool operator==(const Vec3& v) const { return std::abs(x - v.x) < kEpsilond && std::abs(y - v.y) < kEpsilond && std::abs(z - v.z) < kEpsilond; }
	bool operator!=(const Vec3& v) { return !(*this == v); }
	static bool equal(const Vec3& a, const Vec3& b) { return a == b; }
	static bool equalx(const Vec3& a, const Vec3& b) { return std::abs(a.x - b.x) < kEpsilond; }
	static bool equaly(const Vec3& a, const Vec3& b) { return std::abs(a.y - b.y) < kEpsilond; }
	static bool equalz(const Vec3& a, const Vec3& b) { return std::abs(a.z - b.z) < kEpsilond; }

	size_t HashCode() const
	{
		size_t hash = 0;
		std::hash<double> hasher;
		hash = hasher(x) ^ (hasher(y) << 1) ^ (hasher(z) << 2);
		return hash;
	}

#if DEBUG_LOG
	std::string ToString() const
	{
		std::string o = "";
		// o += std::to_string(id);
		// o += ",";
		// o += std::to_string(x);
		// o += ",";
		// o += std::to_string(y);
		o += "(";
		o += std::to_string(x);
		o += ", ";
		o += std::to_string(y);
		o += ", ";
		o += std::to_string(z);
		o += ")";
		return o;
	}

	// 0=none
	// 1=id
	// 2=w
	// 3=idw
    void debug(uint_fast8_t t = 0, bool r = true) const
    {
        std::cout << "(" << x << ", " << y << ", " << z << ")";
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

namespace std
{
    template <>
    struct hash<Vec3>
	{
        size_t operator()(const Vec3& v) const noexcept
		{
			return v.HashCode();
		}
	};
}

const Vec3 Vec3::zero = Vec3(0.0, 0.0, 0.0);
const Vec3 Vec3::five = Vec3(0.5, 0.5, 0.5);
const Vec3 Vec3::one = Vec3(1.0, 1.0, 1.0);
const Vec3 Vec3::mone = Vec3(-1.0, -1.0, -1.0);
const Vec3 Vec3::up = Vec3(0.0, 1.0, 0.0);
const Vec3 Vec3::down = Vec3(0.0, -1.0, 0.0);
const Vec3 Vec3::left = Vec3(-1.0, 0.0, 0.0);
const Vec3 Vec3::right = Vec3(1.0, 0.0, 0.0);
const Vec3 Vec3::forward = Vec3(0.0, 0.0, 1.0);
const Vec3 Vec3::back = Vec3(0.0, 0.0, -1.0);
const Vec3 Vec3::positiveInfinityVector = Vec3(std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity());
const Vec3 Vec3::negativeInfinityVector = Vec3(-std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity());
