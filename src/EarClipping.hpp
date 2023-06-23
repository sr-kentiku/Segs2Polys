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
		Vec2Util::directionRotatePoly(poly, 1);
		ret = ClipTriangle(poly, safety);
		return poly.size() - 2 == ret.size();
	}

	static bool EarClipHoles(std::vector<Vec2> hull, std::vector<std::vector<Vec2>>& holes, std::vector<std::vector<Vec2>>& ret, size_t safety = SIZE_MAX)
	{
		Vec2Util::directionRotatePoly(hull, 1);
		for (size_t i = 0; i < holes.size(); i++)
			Vec2Util::directionRotatePoly(holes[i], 0);
		if (!addConnectLine(hull, holes))
			return false;
		ret = ClipTriangle(hull, safety);
		return hull.size() - 2 == ret.size();
	}

	// normalize the direction of rotation before using.
	static std::vector<std::vector<Vec2>> ClipTriangle(std::vector<Vec2>& poly, size_t safety = SIZE_MAX)
	{
		std::vector<std::vector<Vec2>> o;

		std::vector<long> nL;

		size_t nI;
		size_t n;
		size_t nN;

		bool f;

		size_t i;
		size_t j;

		size_t s;

		s = poly.size();
		nL = std::vector<long>(s);
		for (i = 0; i < s; i++)
			nL[i] = (long)i;

	GO_FIRST:
		if (safety-- <= 0)
			return std::vector<std::vector<Vec2>>();

		nL.erase(
			std::remove_if(
				std::begin(nL),
				std::end(nL),
				[](long  v)
				{
					return v < 0;
				}
			),
			std::end(nL)
		);
		
		if (nL.size() >= 3)
		{
			s = nL.size();
			for (i = 0; i < nL.size(); i++)
			{
				nI = nL[Mathf::repeat(i - 1, s)];
				n = nL[Mathf::repeat(i, s)];
				nN = nL[Mathf::repeat(i + 1, s)];
				
				if (Vec2::cross3(poly[n], poly[nI], poly[nN]) >= 0)
					continue;
				
				f = true;
				for (j = 0; j < poly.size(); j++)
				{
					if (nI == j || n == j || nN == j)
						continue;
					if (!isTrianglePointInside(poly[nI], poly[n], poly[nN], poly[j], false))
						continue;
					f = false;
					break;
				}

				if (f)
				{
					o.emplace_back(std::vector<Vec2>({
						poly[nI],
						poly[n],
						poly[nN]
					}));
					nL[i] = -1;
					goto GO_FIRST;
				}
			}
		}
		return o;
	}

private:
	static bool isTrianglePointInside(Vec2& p1, Vec2& p2, Vec2& p3, Vec2& p, bool includeBorder)
	{
		double c1 = Vec2::cross(p1 - p2, p2 - p);
		double c2 = Vec2::cross(p2 - p3, p3 - p);
		double c3 = Vec2::cross(p3 - p1, p1 - p);
		if (includeBorder)
			return (c1 >= 0 && c2 >= 0 && c3 >= 0) || (c1 <= 0 && c2 <= 0 && c3 <= 0);
		else
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
