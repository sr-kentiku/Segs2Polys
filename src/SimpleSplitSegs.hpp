#pragma once

#include <stdint.h>
#include <vector>
#include <cmath>
#include <algorithm>

// my
#include "Vec2.hpp"
#include "Line2.hpp"

#if DEBUG_LOG
#include <iostream>
#endif

class SimpleSplitSegs
{
public:
	SimpleSplitSegs() {}
	
	static std::vector<Line2> splitSegs(std::vector<Line2>& segs, bool extF = false, double extDist = 0.0)
	{
		std::vector<Line2> o;
		std::vector<Vec2> intrs;
		Vec2 c;
		bool f;

		for (size_t i = 0; i < segs.size(); i++)
		{
			intrs.clear();
			for (size_t j = 0; j < segs.size(); j++)
			{
				if (i == j)
					continue;

				if (extF)
				{
					if (!segs[i].isCrossInf(segs[j]))
						continue;
					c = segs[i].getCross(segs[j]);
					if (segs[i].distancePointLineDist(c) > extDist)
						continue;
				}
				else
				{
					if (!segs[i].isCross(segs[j]))
						continue;
					c = segs[i].getCross(segs[j]);
				}

				c.id = segs[i].s.id;

				f = true;
				for (size_t k = 0; k < intrs.size(); k++)
					if (intrs[k] == c)
					{
						f = false;
						break;
					}
				if (f)
					intrs.emplace_back(c);
			}

			if (intrs.size() < 2)
				continue;

			// sort
			for (size_t j = 0; j < intrs.size(); j++)
				for (size_t k = 0; k < intrs.size() - j - 1; k++)
					if (intrs[k].x > intrs[k + 1].x)
						std::swap(intrs[k], intrs[k + 1]);
					else if (intrs[k].x == intrs[k + 1].x && intrs[k].y > intrs[k + 1].y)
						std::swap(intrs[k], intrs[k + 1]);

			for (size_t j = 0; j < intrs.size() - 1; j++)
				o.emplace_back(Line2(intrs[j], intrs[j + 1]));
		}
		return o;
	}
};