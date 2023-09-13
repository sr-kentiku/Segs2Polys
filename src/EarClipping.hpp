#pragma once

#include <stdint.h>
#include <algorithm>
#include <vector>
#include <cmath>
#include <limits>

// my
#include "Vec2.hpp"
#include "Vec2Util.hpp"
#include "Line2.hpp"
#include "Mathf.hpp"

#if DEBUG_LOG
#include <iostream>
#endif

class EarClipping
{
public:
	EarClipping() {}

	static bool EarClip(std::vector<Vec2>& poly, std::vector<std::vector<Vec2>>& ret, size_t safety = SIZE_MAX)
	{
		Vec2Util::directionRotatePoly(poly, 0);
		ret = ClipTriangle(poly, safety);
		// return poly.size() - 2 == ret.size();
		return ret.size() > 0;
	}

	static bool EarClipHoles(std::vector<Vec2> hull, std::vector<std::vector<Vec2>> holes, std::vector<std::vector<Vec2>>& ret, size_t safety = SIZE_MAX)
	{
		if (holes.size() <= 0)
			return EarClip(hull, ret, safety);
		for (size_t i = 0; i < holes.size(); i++)
			if (!Vec2Util::isPolyInside(hull, holes[i]))
				holes.erase(holes.begin() + i);
		Vec2Util::directionRotatePoly(hull, 0);
		for (size_t i = 0; i < holes.size(); i++)
			Vec2Util::directionRotatePoly(holes[i], 1);
		if (!addConnectLine(hull, holes))
			return false;
// for (size_t i = 0; i < hull.size(); i++)
// {
// std::cout << "(";
// hull[i].debug(0, false);
// std::cout << ", ";
// hull[fmod(i + 1, hull.size())].debug(0, false);
// std::cout << ")," << std::endl;
// }
		ret = ClipTriangle(hull, safety);
		// return hull.size() - 2 == ret.size();
		return ret.size() > 0;
	}

	// normalize the direction of rotation before using.
	static std::vector<std::vector<Vec2>> ClipTriangle(std::vector<Vec2> poly, size_t safety = SIZE_MAX)
	{
		std::vector<std::vector<Vec2>> o = std::vector<std::vector<Vec2>>();
		bool f;
		Vec2* a;
		Vec2* b;
		Vec2* c;
		size_t ai;
		size_t bi;
		size_t ci;

		if (poly.size() < 2)
			return o;

		while (poly.size() > 3)
		{
			if (safety-- <= 0)
				break;

// std::cout << "safety:" << safety << std::endl;

			for (size_t i = 0; i < poly.size(); i++)
			{
				ai = i;
				bi = fmod(i + 1, poly.size());
				ci = fmod(i + 2, poly.size());
				a = &poly[ai];
				b = &poly[bi];
				c = &poly[ci];

				if (Vec2::cross((*a) - (*b), (*c) - (*b)) < 0)
					continue;
				
				f = true;
				for (size_t j = 0; j < poly.size(); j++)
				{
					if (j == ai || j == bi || j == ci)
						continue;
					if (!isPointInTriangle(*a, *b, *c, poly[j]))
						continue;
					f = false;
					break;
				}

				if (!f)
					continue;

				o.emplace_back(std::vector<Vec2>({
					*a, *b, *c
				}));
				poly.erase(poly.begin() + bi);
				break;
			}
		}

		o.emplace_back(std::vector<Vec2>({
			poly[0],
			poly[1],
			poly[2]
		}));

		return o;
	}

private:
	static bool isPointInTriangle(Vec2& a, Vec2& b, Vec2& c, Vec2& p, const bool isInclude = false )
	{
		double c1;
		double c2;
		double c3;
		c1 = Vec2::cross(a - b, b - p);
		c2 = Vec2::cross(b - c, c - p);
		c3 = Vec2::cross(c - a, a - p);
		return (c1 > 0 && c2 > 0 && c3 > 0) || (c1 < 0 && c2 < 0 && c3 < 0);
	}

	// normalize the direction of rotation before using.
	static bool addConnectLine(std::vector<Vec2>& hull, std::vector<std::vector<Vec2>>& holes)
	{
		long hullIdx;
		long holeIdx;
		double dist;
		double d;

		std::vector<Vec2> vs;

		bool f;

		Line2 l1;
		Line2 l2;

		size_t i;
		size_t j;
		size_t k;
		size_t m;
		size_t mm;
		size_t s;

		std::vector<Vec2>::iterator it;

		double dblinf = std::numeric_limits<double>::infinity();

		for (size_t i = 0; i < holes.size(); i++)
		{
			hullIdx = -1;
			holeIdx = -1;
			dist = dblinf;
			for (size_t j = 0; j < holes[i].size(); j++)
			{
				for (size_t k = 0; k < hull.size(); k++)
				{
					f = true;
					s = hull.size();
					for (size_t m = 0; m < s; m++)
					{
						mm = Mathf::repeat(m + 1, s);
						if (k == m || k == mm)
							continue;
						l1 = Line2(hull[k], holes[i][j]);
						l2 = Line2(hull[m], hull[mm]);
						if (!Line2::isCross(l1, l2, true))
							continue;
						f = false;
						break;
					}

					if (!f)
						continue;

					s = holes[i].size();
					for (size_t m = 0; m < s; m++)
					{
						mm = Mathf::repeat(m + 1, s);
						if (j == m || j == mm)
							continue;
						l1 = Line2(hull[k], holes[i][j]);
						l2 = Line2(holes[i][m], holes[i][mm]);
						if (!Line2::isCross(l1, l2, true))
							continue;
						f = false;
						break;
					}

					if (!f)
						continue;

					d = Vec2::distance(hull[k], holes[i][j]);
					if (d < dist || dist == dblinf)
					{
						hullIdx = k;
						holeIdx = j;
						dist = d;
					}
				}
			}

			if (dist == dblinf)
				return false;

			vs.clear();
			s = holes[i].size();
			vs = std::vector<Vec2>(s + 2);
			for (size_t j = 0; j < s; j++)
				vs[j] = holes[i][Mathf::repeat(j + holeIdx, s)];
			vs[s] = holes[i][holeIdx];
			vs[s + 1] = hull[hullIdx];
			it = hull.begin() + hullIdx + 1;
			hull.insert(it, vs.begin(), vs.end());
		}
		return true;
	}
};
