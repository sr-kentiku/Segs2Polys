#pragma once

#include <stdint.h>
#include <cmath>
#include <algorithm>
#include <string>

// my
#include "Vec2.hpp"
#include "Mathf.hpp"

#if DEBUG_LOG
#include <iostream>
#endif

class Line2
{
public:
	long id;
	long w;
	long w1;

	Vec2 s;
	Vec2 e;

	Line2() : id(-1), w(-1), s(Vec2()), e(Vec2()) { sort(); }
	Line2(Vec2 s, Vec2 e) : id(-1), w(-1), w1(-1), s(s), e(e) { sort(); }
	Line2(Vec2 s, Vec2 e, bool isort) : id(-1), w(-1), w1(-1), s(s), e(e) { if (isort) sort(); }
	Line2(Vec2 s, Vec2 e, long id) : id(id), w(-1), w1(-1), s(s), e(e) { sort(); }
	Line2(Vec2 s, Vec2 e, long id, bool isort) : id(id), w(-1), w1(-1), s(s), e(e) { if (isort) sort(); }
	Line2(Vec2 s, Vec2 e, long id, long w) : id(id), w(w), w1(-1), s(s), e(e) { sort(); }
	Line2(Vec2 s, Vec2 e, long id, long w, bool isort) : id(id), w(w), w1(-1), s(s), e(e) { if (isort) sort(); }
	Line2(Vec2 s, Vec2 e, long id, long w, long w1) : id(id), w(w), w1(w1), s(s), e(e) { sort(); }
	Line2(Vec2 s, Vec2 e, long id, long w, long w1, bool isort) : id(id), w(w), w1(w1), s(s), e(e) { if (isort) sort(); }

	Line2 Set(const Line2& l) { s = l.s; e = l.e; return *this; }

	Line2 sort()
	{
		if (s.x > e.x)
			swap();
		else if (s.x == e.x && s.y > e.y)
			swap();
		return *this;
	}

	bool isSorted()
	{
		if (s.x > e.x)
			return false;
		else if (s.x == e.x && s.y > e.y)
			return false;
		return true;
	}

	Line2 swap() { Line2 l = Line2(e, s); return Set(l); }
	
	Vec2 vec() const { return Vec2::CalcSub(e, s); }

	bool isVH()
	{
		Vec2 v = vec();
		if (Vec2::cross(v, Vec2::up) == 0)
		// if (std::abs(Vec2::cross(v, Vec2::up)) <= Vec2::kEpsilond)
			return true;
		if (Vec2::cross(v, Vec2::right) == 0)
		// if (std::abs(Vec2::cross(v, Vec2::right)) <= Vec2::kEpsilond)
			return true;
		return false;
	}
	bool isVH(const Line2& b) { return Line2::isVH(*this, b); }
	static bool isVH(const Line2& a, const Line2& b)
	{
		Vec2 v1 = a.vec();
		Vec2 v2 = b.vec();
		if (Vec2::cross(v1, v2) == 0)
		// if (std::abs(Vec2::cross(v1, v2)) <= Vec2::kEpsilond)
			return true;
		if (Vec2::cross(v1, v2.perpendicular()) == 0)
		// if (std::abs(Vec2::cross(v1, v2.perpendicular())) <= Vec2::kEpsilond)
			return true;
		return false;
	}

	Vec2 midp() { return vec() * Vec2::five + s; }

	double distance(const int isqr = 0) const { return Vec2::distance(s, e, isqr); }

	double sqrmagnitude(const int isqr = 0) { return vec().sqrmagnitude(isqr); }

	Line2 perpendicular() { return Line2(s, Vec2::perpendicular(vec()) + s, false); }

	Line2 midPerpendicular()
	{
		Vec2 m = midp();
		return Line2(
			m,
			vec().perpendicular() + m,
			false
		);
	}

	Line2 resized(const double& len) { Set(resize(len)); return *this; }
	Line2 resize(const double& len)
	{
		double d = distance();
		if (d == len)
			return *this;
		Vec2 r = vec() * Vec2(len / d);
		return Line2(
			s,
			r + s,
			id,
			w,
			w1,
			false
		);
	}

	double distancePointLineDist(Vec2& p, const int isqr = 0)
	{
		double d = distance(isqr);
		if (d == 0)
			return Vec2::distance(s, p, isqr);
		double d1 = Vec2::distance(s, p, isqr);
		double d2 = Vec2::distance(e, p, isqr);
		return (std::min)({ d1, d2 });
	}

	double distancePointLineT(Vec2& p, const int isqr = 0)
	{
		double d = distance(isqr);
		if (d == 0)
			return Vec2::distance(s, p, isqr);
		Vec2 ab = e - s;
		Vec2 ap = p - s;
		double ds = Vec2::dot(ap, ab) / d;
		Vec2 np = ab / d;
		np *= ds;
		np += s;
		return Vec2::distance(np, p, isqr);
	}

	double distancePointLine(Vec2& p, const int isqr = 0)
	{
		double o = 0;
		Vec2 ab = e - s;
		Vec2 ap = p - s;
		Vec2 ba = s - e;
		Vec2 bp = p - e;
		if (Vec2::dot(ap, ab) < 0)
			o = Vec2::magnitude(ap, isqr);
		else if (Vec2::dot(bp, ba) < 0)
			o = Vec2::magnitude(bp, isqr);
		else
			o = distancePointLineT(p, isqr);
		return o;

		//Vec2 v = vec();
		//double d = Vec2::dot(v, p);
		//if (d == 0)
		//	return 0;
		//double u = ((p.x - s.x) * v.x + (p.y - s.y) * v.y) / d;
		//if (u > 1)
		//	u = 1;
		//else if (u < 0)
		//	u = 0;

		//Vec2 dv = Vec2(
		//	(s.x + u * v.x) - p.x,
		//	(s.y + u * v.y) - p.y
		//);

		//d = Vec2::dot(dv, dv);
		//return Mathf::sqrtd(d, isqr);
	}

	static bool isCross(const Line2& l1, const Line2& l2) { return l1.isCross(l2); }
	bool isCross(const Line2& l) const
	{
		Vec2 ab = vec();
		Vec2 cd = l.vec();
		if (std::abs(Vec2::cross(ab, cd)) == 0)
			return false;

		Vec2 ac = Vec2::CalcSub(l.s, s);
		Vec2 ad = Vec2::CalcSub(l.e, s);

		Vec2 ca = Vec2::CalcSub(s, l.s);
		Vec2 cb = Vec2::CalcSub(e, l.s);

		double d1 = Vec2::cross(ab, ac);
		double d2 = Vec2::cross(ab, ad);
		if (d1 * d2 > 0)
			return false;
		d1 = Vec2::cross(cd, ca);
		d2 = Vec2::cross(cd, cb);
		if (d1 * d2 > 0)
			return false;
		return true;
	}
	static bool isCross(const Line2& l1, const Line2& l2, bool includeEndPoint = false)
	{
		Vec2 v43 = Vec2::CalcSub(l2.e, l2.s);
		Vec2 v21 = Vec2::CalcSub(l1.e, l1.s);

		double den = Vec2::cross(v43, v21);

		if (std::abs(den) == 0)
			return false;

		Vec2 v13 = Vec2::CalcSub(l1.s, l2.s);
		double u1 = Vec2::cross(v43, v13) / den;
		double u2 = Vec2::cross(v21, v13) / den;

		double z;
		double o;

		if (includeEndPoint)
		{
			z = -Vec2::kEpsilond;
			o = 1 + Vec2::kEpsilond;
			return u1 >= z && u1 <= o && u2 >= z && u2 <= o;
		}
		else
		{
			z = Vec2::kEpsilond;
			o = 1 - Vec2::kEpsilond;
			return u1 > z && u1 < o && u2 > z && u2 < o;
		}
	}

	// this=segment
	// l=line
	double orientation(const Vec2& p, const Vec2& q, const  Vec2& r) const { return (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y); }
	bool onSegment(const Vec2& p, const  Vec2& q, const  Vec2& r) const { return (std::min)({ p.x, r.x }) <= q.x && q.x <= (std::max)({ p.x, r.x }) && (std::min)({ p.y, r.y }) <= q.y && q.y <= (std::max)({ p.y, r.y }); }
	static bool isCrossSegInf(const Line2& l1, const  Line2& l2) { return l1.isCrossSegInf(l2); }
	bool isCrossSegInf(const Line2& l) const
	{
		if (orientation(l.s, l.e, s) == 0 && onSegment(l.s, s, l.e))
			return true;
		if (orientation(l.s, l.e, e) == 0 && onSegment(l.s, e, l.e))
			return true;
		if (orientation(s, e, l.s) == 0 && onSegment(s, l.s, e))
			return true;
		if (orientation(s, e, l.e) == 0 && onSegment(s, l.e, e))
			return true;
		if (orientation(l.s, l.e, s) != orientation(l.s, l.e, e))
			if (orientation(s, e, l.s) != orientation(s, e, l.e))
				return true;
		return false;
	}

	// inf line to inf line intersection
	static bool isCrossInf(const Line2& l1, const Line2& l2) { return l1.isCrossInf(l2); }
	bool isCrossInf(const Line2& l) const { return std::abs(Vec2::cross(vec(), l.vec())) != 0; }

	// inf line crossing point
	static Vec2 getCrossInf(Line2& l1, Line2& l2) { return l1.getCrossInf(l2); }
	static Vec2 getCrossInf(const Line2& l1, Line2& l2) { return l2.getCrossInf(l1); }
	static Vec2 getCrossInf(Line2& l1, const Line2& l2) { return l1.getCrossInf(l2); }
	Vec2 getCrossInf(const Line2& l)
	{
		Vec2 ab = vec();
		Vec2 cd = l.vec();
		double bD = ab.cross(cd);
		if (bD == 0)
			return Vec2();
		
		Vec2 c = Vec2::CalcSub(l.s, s);
		double t = c.cross(cd) / bD;
		return Vec2(
			s.x + t * ab.x,
			s.y + t * ab.y
		);
	}

	// line crossing point
	static Vec2 getCross(Line2& l1, Line2& l2) { return l1.getCross(l2); }
	static Vec2 getCross(const Line2& l1, Line2& l2) { return l2.getCross(l1); }
	static Vec2 getCross(Line2& l1, const Line2& l2) { return l1.getCross(l2); }
	Vec2 getCross(const Line2& l)
	{
		double den = (e.x - s.x) * (l.s.y - l.e.y) - (l.e.x - l.s.x) * (s.y - e.y);
		if (std::abs(den) == 0)
			return Vec2();

		double c1 = Vec2::cross(e, s);
		double c2 = Vec2::cross(l.e, l.s);
		return Vec2(
			((l.s.x - l.e.x) * c1 - (s.x - e.x) * c2) / den,
			((l.s.y - l.e.y) * c1 - (s.y - e.y) * c2) / den
		);
	}

	static bool isOnLine(Line2& l, Vec2& p) { return l.isOnLine(p); }
	bool isOnLine(Vec2& p)
	{
		Vec2 ab = e - s;
		Vec2 ap = p - s;
		return std::abs(Vec2::cross(ab, ap)) < Vec2::kEpsilond;
	}

	static bool isSegmentOnLine(Line2& l1, Line2& l2) { return l1.isSegmentOnLine(l2); }
	bool isSegmentOnLine(Line2& l)
	{
		if (!isCrossSegInf(l))
			return false;
		return isOnLine(l.s) && isOnLine(l.e);
	}

	// -1   not crossing
	// 0    share point contains or equal
	// 1    contains
	// 2    partial intersection
	// 3    point intersection
	static int_fast32_t isLineOverlapSub(Line2& l1, Line2& l2) { return l1.isLineOverlapSub(l2); }
	int_fast32_t isLineOverlapSub(const Line2& l)
	{
		Vec2 s1 = (s - l.s) * (e - l.e);
		Vec2 s2 = (s - l.e) * (e - l.s);
		double d1 = s1.x + s1.y;
		double d2 = s2.x + s2.y;

		if (std::abs(d1) < Vec2::kEpsilond)
			d1 = 0;
		if (std::abs(d2) < Vec2::kEpsilond)
			d2 = 0;

		if (d1 > 0 && d2 > 0)
			return -1;
		else if (d1 == 0 && d2 < 0)
			return 0;
		else if (d1 < 0 && d2 < 0)
			return 1;
		else if (d1 > 0 && d2 < 0)
			return 2;
		else if (d1 > 0 && d2 == 0)
			return 3;
		return -1;
	}
	static int_fast32_t isLineOverlap(Line2& l1, Line2& l2) { return l1.isLineOverlap(l2); }
	int_fast32_t isLineOverlap(Line2& l)
	{
		if (std::abs(Vec2::cross(vec(), l.vec())) >= Vec2::kEpsilond)
			return -1;
		if (!isOnLine(l.s))
			return -1;
		return isLineOverlapSub(l);
	}

	Line2 operator+(const Line2& l) { return Line2(s + l.s, e + l.e); }
	Line2 operator+(const Vec2& v) { return Line2(s + v, e + v); }
	Line2 operator+(const double& d) { return Line2(s + d, e + d); }
	Line2 operator+=(const Line2& l) { s += l.s; e += l.e; return *this; }
	Line2 operator+=(const Vec2& v) { s += v; e += v; return *this; }
	Line2 operator+=(const double& d) { s += d; e += d; return *this; }

	bool operator==(const Line2& l) { return s == l.s && e == l.e; }
	bool operator!=(const Line2& l) { return !(*this == l); }

	std::size_t HashCode() const { return s.HashCode() ^ (e.HashCode() << 1); }

	std::string ToString() const
	{
		std::string o = "";
		o += s.ToString();
		o += ",";
		o += e.ToString();
		return o;
	}

#if DEBUG_LOG
	// 0=none
	// 1=id
	// 2=w
	// 3=idw
	void debug(uint_fast8_t t1 = 0, uint_fast8_t t2 = 0, bool r = true) const
	{
		std::cout << "(";
		s.debug(t1, false);
		std::cout << ", ";
		e.debug(t1, false);
		std::cout << ")";
		switch (t2)
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
