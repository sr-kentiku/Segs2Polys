#pragma once

#include <stdint.h>
#include <vector>
#include <cmath>

// my
#include "Vec2.hpp"
#include "Line2.hpp"

class Vec2Util
{
public:
	Vec2Util() {}

	static double AreaTwoPoly(std::vector<Vec2>& poly)
	{
		double a = 0;
		for (size_t i = 0; i < poly.size(); i++)
			a += Vec2::cross(poly[i], poly[fmod(i + 1, poly.size())]);
		return a;
	}

	static double AreaMulTwoPoly(std::vector<std::vector<Vec2>>& polys)
	{
		double a = 0;
		for (size_t i = 0; i < polys.size(); i++)
			a += AreaTwoPoly(polys[i]);
		return a;
	}

	// right = 0
	// left  = 1
	static void directionRotatePoly(std::vector<Vec2>& poly, int r = 0)
	{
		double a = AreaTwoPoly(poly);
		if (r == 0 && a > 0)
			std::reverse(poly.begin(), poly.end());
		else if (r == 1 && a < 0)
			std::reverse(poly.begin(), poly.end());
	}

	static bool isPolyPointInside(std::vector<Vec2>& hull, Vec2& p)
	{
		size_t dc = 0;
		size_t rc = 0;
		size_t ii;
		for (size_t i = 0; i < hull.size(); i++)
		{
			ii = fmod(i + 1, hull.size());
			if (p.x >= hull[i].x || p.x >= hull[ii].x)
				if (std::min(hull[i].y, hull[ii].y) <= p.y && p.y <= std::max(hull[i].y, hull[ii].y))
					dc++;
			if (p.y >= hull[i].y || p.y >= hull[ii].y)
				if (std::min(hull[i].x, hull[ii].x) <= p.x && p.x <= std::max(hull[i].x, hull[ii].x))
					rc++;
		}
		return fmod(dc, 2) == 1 && fmod(rc, 2) == 1 && dc > 0 && rc > 0;
	}

	static bool isPolyCross(std::vector<Vec2>& hull, std::vector<Vec2>& hole)
	{
		Line2 l;
		for (size_t i = 0; i < hull.size(); i++)
		{
			l = Line2(hull[i], hull[fmod(i + 1, hull.size())], false);
			for (size_t j = 0; j < hole.size(); j++)
				if (l.isCross(Line2(hole[j], hole[fmod(j + 1, hole.size())], false)))
					return false;
		}
		return true;
	}

	static bool isPolyInside(std::vector<Vec2>& hull, std::vector<Vec2>& hole)
	{
		for (size_t i = 0; i < hole.size(); i++)
			if (!isPolyPointInside(hull, hole[i]))
				return false;
		return isPolyCross(hull, hole);
	}
};
