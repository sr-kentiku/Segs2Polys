#pragma once

#include <stdint.h>
#include <vector>

// my
#include "Vec2.hpp"
#include "LIne2.hpp"
#include "CellInternal.hpp"

#if DEBUG_LOG
#include <iostream>
#endif

class Cell
{
public:
	Vec2 swh;
	Vec2 ewh;
	CellInternal shape;

	Cell() :swh(Vec2()), ewh(Vec2()), shape(CellInternal()) {}
	Cell(Vec2 b, Vec2 wh, std::vector<Line2> lines) : swh(b), ewh(wh + b), shape(
		CellInternal(
			std::vector<Vec2>({
				swh,
				Vec2(ewh.x, swh.y),
				ewh,
				Vec2(swh.x, ewh.y)
				}),
			lines,
			swh,
			ewh
		)) {}

	std::vector<size_t> getRayNoHitPair(Cell& cell)
	{
		std::vector<size_t> o;

		for (size_t i = 0; i < shape.rays.size(); i++)
		{
			for (size_t j = 0; j < cell.shape.rays.size(); j++)
			{
				Line2 ray = Line2(shape.rays[i], cell.shape.rays[j]);
				if (shape.isHitRay(ray))
					continue;
				if (cell.shape.isHitRay(ray))
					continue;
				o.emplace_back(i);
				o.emplace_back(j);
				break;
			}
		}
		return o;
	}

	bool isRayHit(Cell& cell)
	{
		for (size_t i = 0; i < shape.rays.size(); i++)
			for (size_t j = 0; j < cell.shape.rays.size(); j++)
			{
				Line2 ray = Line2(shape.rays[i], cell.shape.rays[j]);
				if (shape.isHitRay(ray))
					return true;
				if (cell.shape.isHitRay(ray))
					return true;
			}
		return false;
	}

	std::vector<std::vector<size_t>> getRayHit(Cell& cell)
	{
		std::vector<std::vector<size_t>> o;
		for (size_t i = 0; i < shape.rays.size(); i++)
			for (size_t j = 0; j < cell.shape.rays.size(); j++)
			{
				Line2 ray = Line2(shape.rays[i], cell.shape.rays[j]);
				bool f = true;
				if (shape.isHitRay(ray))
					f = false;
				else if (cell.shape.isHitRay(ray))
					f = false;
				if (f)
					o.emplace_back(std::vector<size_t>({ i, j }));
			}
		return o;
	}

#if DEBUG_LOG
	void debug(uint_fast8_t t1 = 0, uint_fast8_t t2 = 0, uint_fast8_t t3 = 0, bool r = true) const
	{
		std::cout << "(" << std::endl;
		std::cout << "swh:";
		swh.debug(t1, true);
		std::cout << "ewh:";
		ewh.debug(t1, true);
		std::cout << "shape:" << std::endl;
		shape.debug(t2, t3, true);
		std::cout << ")";
		if (r)
			std::cout << std::endl;
	}
#endif
};
