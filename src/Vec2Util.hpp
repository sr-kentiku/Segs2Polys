#pragma once

#include <stdint.h>
#include <vector>
#include <cmath>

// my
#include "Vec2.hpp"

class Vec2Util
{
public:
	Vec2Util() {}

	// right = 0
	// left  = 1
	static void directionRotatePoly(std::vector<Vec2>& polys, int r = 0)
	{
		double a = 0;
		size_t ps = polys.size();
		size_t ii;

		for (size_t i = 0; i < ps; i++)
		{
			ii = (i + 1) % ps;
			a += Vec2::cross(polys[i], polys[ii]);
		}
		a *= 0.5;
		if (r == 0 && a > 0)
			std::reverse(polys.begin(), polys.end());
		else if (r == 1 && a < 0)
			std::reverse(polys.begin(), polys.end());
	}
};
