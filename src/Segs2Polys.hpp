#pragma once

#ifdef _DEBUG
#define DEBUG_LOG			true
#define DEBUG_TIMER			true
#define DEBUG_TIMER_TOTAL	true
#else
#define DEBUG_LOG			false
#define DEBUG_TIMER			false
#define DEBUG_TIMER_TOTAL	false
#endif

#ifdef _DEBUG
#define EXPORT
#define WINAPI
// #define INT		int
// #define LONG    long
// #define DOUBLE	double
#else
#define EXPORT extern "C" __declspec( dllexport )
#include <windows.h>
#endif

#include <stdint.h>
#include <vector>
#include <cmath>
#include <algorithm>
#include <string>

// my
#include "Mathf.hpp"
#include "Vec2.hpp"
#include "Vec2Util.hpp"
#include "Line2.hpp"
#include "Cell.hpp"
#include "CellInternal.hpp"
// #include "SweepLineAlgorithm.hpp"
#include "SimpleSplitSegs.hpp"
// #include "EarClipping.hpp"
// #include "Csv2Line2.hpp"
#include "TriangleUtil.hpp"
#include "SplitTrisSegs.hpp"

#if DEBUG_LOG || DEBUG_TIMER || DEBUG_TIMER_TOTAL
#include <string>
#endif
#if DEBUG_TIMER || DEBUG_TIMER_TOTAL
#include <time.h>
#endif

struct stCellLink
{
	size_t cell11;
	size_t cell12;
	size_t cell21;
	size_t cell22;
	size_t cellLink1;
	size_t cellLink2;

#if DEBUG_LOG
	void debug()
	{
		std::cout << "cell1 : (" << cell12 << ", " << cell11 << ") " << cellLink1;
		std::cout << "\t";
		std::cout << "cell2 : (" << cell22 << ", " << cell21 << ") " << cellLink2;
		std::cout << std::endl;
	}
#endif
};

struct stLineLink
{
	Line2 me;
	std::vector<Line2> lines;
};

class Segs2Polys
{
public:
	Segs2Polys() {}

	static std::vector<std::vector<Cell>> createGrid(std::vector<Line2>& segs)
	{
		std::vector<std::vector<Cell>> cells;
		std::vector<double> xs;
		std::vector<double> ys;

		std::vector<Vec2> se;
		bool f;

		for (size_t i = 0; i < segs.size(); i++)
		{
			se = std::vector<Vec2>({segs[i].s, segs[i].e});
			for (size_t j = 0; j < se.size(); j++)
			{
				f = true;
				for (size_t k = 0; k < xs.size(); k++)
					if (xs[k] == se[j].x)
					{
						f = false;
						break;
					}
				if (f)
					xs.emplace_back(se[j].x);

				f = true;
				for (size_t k = 0; k < ys.size(); k++)
					if (ys[k] == se[j].y)
					{
						f = false;
						break;
					}
				if (f)
					ys.emplace_back(se[j].y);
			}
		}

		// sort
		for (size_t i = 0; i < xs.size(); i++)
			for (size_t j = 0; j < xs.size() - i - 1; j++)
				if (xs[j] > xs[j + 1])
					std::swap(xs[j], xs[j + 1]);
		for (size_t i = 0; i < ys.size(); i++)
			for (size_t j = 0; j < ys.size() - i - 1; j++)
				if (ys[j] > ys[j + 1])
					std::swap(ys[j], ys[j + 1]);

#if DEBUG_LOG
		std::cout << "[";
		for (const auto& vd : xs)
			std::cout << vd << ",";
		std::cout << "]" << std::endl;
		std::cout << "[";
		for (const auto& vd : ys)
			std::cout << vd << ",";
		std::cout << "]" << std::endl;
#endif

		// make grid
		Vec2 b;
		Vec2 wh;
		Cell c;
		std::vector<Line2> rl;

		cells = std::vector<std::vector<Cell>>(ys.size() - 1, std::vector<Cell>(xs.size() - 1));

		//std::vector<std::vector<Vec2>> pb = std::vector<std::vector<Vec2>>(ys.size() - 1, std::vector<Vec2>(xs.size() - 1));
		//std::vector<std::vector<Vec2>> pwh = std::vector<std::vector<Vec2>>(ys.size() - 1, std::vector<Vec2>(xs.size() - 1));

		for (size_t i = 0; i < ys.size() - 1; i++)
		{
			for (size_t j = 0; j < xs.size() - 1; j++)
			{
				b = Vec2(xs[j], ys[i]);
				wh = Vec2(xs[j + 1], ys[i + 1]) - b;
				rl = getRangeLines(segs, b, wh);
				cells[i][j] = Cell(b, wh, rl);

				//pb[i][j] = Vec2(xs[j], ys[i]);
				//pwh[i][j] = Vec2(xs[j + 1], ys[i + 1]) - b;
			}
		}

		return cells;
	}

	static std::vector<stCellLink> getLinkRaycast(std::vector<std::vector<Cell>>& cells)
	{
		std::vector<stCellLink> links;
		std::vector<size_t> r;

		size_t h = cells.size();
		size_t w = cells[0].size();

		for (size_t i = 0; i < h; i++)
			for (size_t j = 0; j < w; j++)
			{
				// right
				if (j < w - 1)
				{
					r = cells[i][j].getRayNoHitPair(cells[i][j + 1]);
					if (r.size() > 0)
					{
						for (size_t k = 0; k < r.size(); k += 2)
						{
							links.emplace_back((stCellLink){
								i,    j,
								i,    j + 1,
								r[k], r[k + 1]
							});
						}
					}
				}

				// up
				if (i < h - 1)
				{
					r = cells[i][j].getRayNoHitPair(cells[i + 1][j]);
					if (r.size() > 0)
					{
						for (size_t k = 0; k < r.size(); k += 2)
						{
							links.emplace_back((stCellLink){
								i,     j,
								i + 1, j,
								r[k],  r[k + 1]
							});
						}
					}
				}
			}
		return links;
	}

	static std::vector<std::vector<Vec2>> trailCells(std::vector<std::vector<Cell>>& cells, std::vector<stCellLink>& links)
	{
		std::vector<std::vector<Vec2>> polys;
		std::vector<Vec2> poly;
		
		std::vector<std::vector<stCellLink*>> linkstack;
		
		// stack links
		{
			std::vector<stCellLink*> ls;
			std::vector<stCellLink*> stack;
			std::vector<stCellLink*> vis;
			stCellLink* cur;
			stCellLink* next;
			bool f;

// #if DEBUG_LOG
// 			for (size_t i = 0; i < links.size(); i++)
// 				std::cout << "(("     << links[i].cell11
// 						<< ", "     << links[i].cell12
// 						<< ", "     << links[i].cellLink1
// 						<< "), ("   << links[i].cell21
// 						<< ", "     << links[i].cell22
// 						<< ", "     << links[i].cellLink2
// 						<< ")), " << std::endl;
// #endif

			for (size_t i = 0; i < links.size(); i++)
			{
				f = true;
				for (size_t j = 0; j < vis.size(); j++)
					if (vis[j] == &links[i])
					{
						f = false;
						break;
					}
				if (!f)
					continue;
				
				stack.emplace_back(&links[i]);
				// vis.clear();
				ls.clear();
				while (stack.size() > 0)
				{
					cur = stack.back();
					stack.pop_back();
					vis.emplace_back(cur);
					ls.emplace_back(cur);

					for (size_t j = 0; j < links.size(); j++)
					{
						if (     (*cur).cell11    == links[j].cell11 &&
						    	 (*cur).cell12    == links[j].cell12 &&
							     (*cur).cellLink1 == links[j].cellLink1)
							next = &links[j];
						else if ((*cur).cell11    == links[j].cell21 &&
						    	 (*cur).cell12    == links[j].cell22 &&
							     (*cur).cellLink1 == links[j].cellLink2)
							next = &links[j];
						else if ((*cur).cell21    == links[j].cell11 &&
						    	 (*cur).cell22    == links[j].cell12 &&
							     (*cur).cellLink2 == links[j].cellLink1)
							next = &links[j];
						else if ((*cur).cell21    == links[j].cell21 &&
						    	 (*cur).cell22    == links[j].cell22 &&
							     (*cur).cellLink2 == links[j].cellLink2)
							next = &links[j];
						else
							continue;

						// if (     (*cur).cell21    == links[j].cell11 &&
						//     	 (*cur).cell22    == links[j].cell12 &&
						// 	     (*cur).cellLink2 == links[j].cellLink1)
						// 	next = &links[j];
						// else if ((*cur).cell21    == links[j].cell21 &&
						//     	 (*cur).cell22    == links[j].cell22 &&
						// 	     (*cur).cellLink2 == links[j].cellLink2)
						// 	next = &links[j];
						// else
						// 	continue;

						f = true;
						for (size_t k = 0; k < vis.size(); k++)
							if (vis[k] == next)
							{
								f = false;
								break;
							}
						if (!f)
							continue;

						stack.emplace_back(next);
					}
				}
				if (ls.size() <= 0)
					continue;
				linkstack.emplace_back(ls);
// #if DEBUG_LOG
// 				std::cout << "[";
// 				for (size_t j = 0; j < ls.size(); j++)
// 					std::cout << "(("     << ls[j]->cell11
// 							<< ", "     << ls[j]->cell12
// 							<< ", "     << ls[j]->cellLink1
// 							<< "), ("   << ls[j]->cell21
// 							<< ", "     << ls[j]->cell22
// 							<< ", "     << ls[j]->cellLink2
// 							<< ")), ";
// 				std::cout << "]" << std::endl;
// #endif
			}
		}

// #if DEBUG_LOG
// 		std::cout << std::endl << "linkstack.size():" << linkstack.size() << std::endl;
// #endif

		// trail lines
		{
			std::vector<Line2> lines;
			CellInternal* shape;
			std::vector<size_t*> cell2 = std::vector<size_t*>(6);
			Line2* l2;
			bool f;
			for (size_t i = 0; i < linkstack.size(); i++)
			{
// #if DEBUG_LOG
// 				std::cout << "[" << std::endl;
// #endif
				lines.clear();
				for (size_t j = 0; j < linkstack[i].size(); j++)
				{
					cell2[0] = &(*linkstack[i][j]).cell11;
					cell2[1] = &(*linkstack[i][j]).cell12;
					cell2[2] = &(*linkstack[i][j]).cellLink1;
					cell2[3] = &(*linkstack[i][j]).cell21;
					cell2[4] = &(*linkstack[i][j]).cell22;
					cell2[5] = &(*linkstack[i][j]).cellLink2;
					for (size_t k = 0; k < cell2.size(); k += 3)
					{
						shape = &cells[(*cell2[k])][(*cell2[k + 1])].shape;
						// (*shape).rays[(*cell2[k + 2])].w = 0;
						for (size_t l = 0; l < (*shape).sp[(*cell2[k + 2])].size(); l++)
						{
							l2 = &(*shape).lines[(*shape).sp[(*cell2[k + 2])][l]];
							f = true;
							for (size_t m = 0; m < lines.size(); m++)
								if (lines[m] == *l2)
								{
									f = false;
									break;
								}
							if (!f)
								continue;
							lines.emplace_back(*l2);
// #if DEBUG_LOG
// 							std::cout << "    ";
// 							(*l2).debug(0, 0, false);
// 							std::cout << "," << std::endl;
// #endif
						}
					}
				}

// #if DEBUG_LOG
// 				std::cout << "]" << std::endl;
// #endif
				if (lines.size() < 3)
					continue;
				
				// trail
				poly = trailLines(lines);
				if (poly.size() < 3)
					continue;
				polys.emplace_back(poly);

				for (size_t j = 0; j < linkstack[i].size(); j++)
				{
					cells[(*linkstack[i][j]).cell11][(*linkstack[i][j]).cell12].shape.rays[(*linkstack[i][j]).cellLink1].w = -2;
					cells[(*linkstack[i][j]).cell21][(*linkstack[i][j]).cell22].shape.rays[(*linkstack[i][j]).cellLink2].w = -2;
				}
			}
		}

		// single cell
		{
			size_t h = cells.size();
			size_t w = cells[0].size();

// std::cout << "h\t" << h << "\tw\t" << w << std::endl;

			std::vector<Line2> lines;

			for (size_t i = 0; i < h; i++)
				for (size_t j = 0; j < w; j++)
					for (size_t k = 0; k < cells[i][j].shape.rays.size(); k++)
					{
						if (cells[i][j].shape.rays[k].w == -2)
							continue;

						if (cells[i][j].shape.sp[k].size() < 3)
							continue;

// std::cout << "i\t" << i << "\tj\t" << j << std::endl;
// std::cout << "cells[i][j].shape.sp[k].size()\t" << cells[i][j].shape.sp[k].size() << std::endl;

// // std::cout << "cells[i][j].shape.sp[k].size()\t" << cells[i][j].shape.sp[k].size() << std::endl;
// std::cout << "[";
// for (size_t l = 0; l < cells[i][j].shape.sp[k].size(); l++)
// 	cells[i][j].shape.lines[cells[i][j].shape.sp[k][l]].debug(0, 0, false);
// std::cout << "]" << std::endl;

						lines.clear();
						for (size_t l = 0; l < cells[i][j].shape.sp[k].size(); l++)
							lines.emplace_back(cells[i][j].shape.lines[cells[i][j].shape.sp[k][l]]);
						
						if (lines.size() < 3)
							continue;
						
						// trail
						poly = trailLines(lines);
// std::cout << "poly\t" << poly.size() << std::endl;
						if (poly.size() < 3)
							continue;
						polys.emplace_back(poly);
					}
		}
		return polys;
	}

	// unuseful
	static std::vector<std::vector<Vec2>> trailCells1(std::vector<std::vector<Cell>>& cells, std::vector<stCellLink>& links)
	{
		std::vector<std::vector<Vec2>> polys;
		std::vector<Vec2> poly;

		std::vector<std::vector<std::vector<size_t>>> linkstack;

		// stack links
		{
			std::vector<std::vector<size_t>> ls;
			bool f;
			for (size_t i = 0; i < links.size(); i++)
			{
				f = true;
				for (size_t j = 0; j < linkstack.size(); j++)
				{
					for (size_t k = 0; k < linkstack[j].size(); k++)
					{
						if (linkstack[j][k][0] == links[i].cell11 &&
							linkstack[j][k][1] == links[i].cell12 &&
							linkstack[j][k][2] == links[i].cellLink1)
						{
							linkstack[j].emplace_back(std::vector<size_t>({
								links[i].cell21,
								links[i].cell22,
								links[i].cellLink2
							}));
							f = false;
							break;
						}
					}
					if (!f)
						break;
				}

				if (f)
				{
					ls.clear();
					ls.emplace_back(std::vector<size_t>({
						links[i].cell11,
						links[i].cell12,
						links[i].cellLink1,
					}));
					ls.emplace_back(std::vector<size_t>({
						links[i].cell21,
						links[i].cell22,
						links[i].cellLink2
					}));
					linkstack.emplace_back(ls);
				}
			}
		}

#if DEBUG_LOG
std::cout << "[" << std::endl;
for (size_t i = 0; i < linkstack.size(); i++)
{
	std::cout << "    [" << std::endl;
	for (size_t j = 0; j < linkstack[i].size() - 1; j++)
	{
	std::cout << "        (";
	cells[linkstack[i][j][0]][linkstack[i][j][1]].shape.rays[linkstack[i][j][2]].debug(0, false);
	std::cout << ", ";
	cells[linkstack[i][j][0]][linkstack[i][j + 1][1]].shape.rays[linkstack[i][j + 1][2]].debug(0, false);
	std::cout << "),";
	std::cout << std::endl;
	}
	std::cout << "    ]," << std::endl;
}
std::cout << "]" << std::endl;
#endif

		// trail lines
		{
			std::vector<Line2> lines;
			Line2* line;
			CellInternal* ci;
			bool f;
			for (size_t i = 0; i < linkstack.size(); i++)
			{
				lines.clear();
				for (size_t j = 0; j < linkstack[i].size(); j++)
				{
					ci = &cells[linkstack[i][j][0]][linkstack[i][j][1]].shape;
					(*ci).rays[linkstack[i][j][2]].w = -2;
					for (size_t k = 0; k < (*ci).sp[linkstack[i][j][2]].size(); k++)
					{
						line = &(*ci).lines[(*ci).sp[linkstack[i][j][2]][k]];
						(*line).w = 1;
						f = true;
						for (size_t l = 0; l < lines.size(); l++)
							if ((*line).s == lines[l].s && (*line).e == lines[l].e)
							{
								f = false;
								break;
							}
						if (f)
							lines.emplace_back(*line);
					}
				}

				if (lines.size() < 3)
					continue;

				// trail
				poly = trailLines(lines);
				if (poly.size() >= 3)
					polys.emplace_back(poly);
			}
		}

		// single cell
		{
			size_t h = cells.size();
			size_t w = cells[0].size();

			std::vector<Line2> lines;
			CellInternal* ci;
			for (size_t i = 0; i < h; i++)
				for (size_t j = 0; j < w; j++)
					for (size_t k = 0; k < cells[i][j].shape.rays.size(); k++)
					{
						if (cells[i][j].shape.rays[k].w == -2)
							continue;
						if (cells[i][j].shape.sp[k].size() < 3)
							continue;

						lines.clear();
						ci = &cells[i][j].shape;
						for (size_t l = 0; l < (*ci).sp[k].size(); l++)
							lines.emplace_back((*ci).lines[(*ci).sp[k][l]]);

						poly = trailLines(lines);
						if (poly.size() >= 3)
							polys.emplace_back(poly);
					}
		}

		return polys;
	}

	static std::vector<std::vector<Vec2>> mergePolysLines(std::vector<std::vector<Vec2>>& polys)
	{
		std::vector<std::vector<Vec2>> opolys;
		std::vector<Vec2> poly;

		size_t s;
		bool c0f;
		Line2 c0l;
		size_t j1;
		size_t j2;
		size_t j3;
		Line2 l1;
		Line2 l2;

		for (size_t i = 0; i < polys.size(); i++)
		{
			s = 0;
			for (size_t j = 0; j < polys[i].size(); j++)
			{
				j1 = fmod(j + 1, polys[i].size());
				j2 = fmod(j + 2, polys[i].size());
				l1 = Line2(polys[i][j], polys[i][j1]);
				l2 = Line2(polys[i][j1], polys[i][j2]);
				// if (Vec2::cross(l1.vec(), l2.vec()) >= Vec2::kEpsilond)
				if (std::abs(Vec2::cross(l1.vec(), l2.vec())) >= Vec2::kEpsilond)
				{
					s = j1;
					break;
				}
			}

			poly.clear();
			c0f = false;
			for (size_t j = 0; j < polys[i].size(); j++)
			{
				j1 = fmod(j + s, polys[i].size());
				j2 = fmod(j + s + 1, polys[i].size());
				j3 = fmod(j + s + 2, polys[i].size());
				if (!c0f)
					l1 = Line2(polys[i][j1], polys[i][j2]);
				l2 = Line2(polys[i][j2], polys[i][j3]);

				if (c0f)
				{
					if (std::abs(Vec2::cross(c0l.vec(), l2.vec())) < Vec2::kEpsilond)
						continue;
					poly.emplace_back(polys[i][j2]);
					c0f = false;
					j++;
					continue;

				}
				else if (std::abs(Vec2::cross(l1.vec(), l2.vec())) >= Vec2::kEpsilond)
				{
					poly.emplace_back(polys[i][j1]);
					continue;
				}
				else
				{
					if (!c0f)
						c0l = l1;
					c0f = true;
					poly.emplace_back(polys[i][j1]);
				}
			}
			opolys.emplace_back(poly);
		}
		return opolys;
	}

	// right = 0
	// left  = 1
	static void directionRotatePolys(std::vector<std::vector<Vec2>>& polys, int r = 0)
	{
		for (size_t i = 0; i < polys.size(); i++)
			Vec2Util::directionRotatePoly(polys[i], r);
	}

	static std::vector<Line2> extendMarginIntersection(std::vector<Line2>& segs, double dist)
	{
		std::vector<Line2> w;
		w = SimpleSplitSegs::extendSegs(segs, dist);
		return SimpleSplitSegs::splitSegs(w);

		// std::vector<Line2> o;
		// std::vector<size_t> interL;
		// std::vector<size_t> interI;
		// std::vector<Vec2> inter;
		// std::vector<double> dis;
		// Vec2 c;
		// double maxdis;
		// Vec2 max;
		// double mindis;
		// Vec2 min;
		// for (size_t i = 0; i < segs.size(); i++)
		// {
		// 	interL.clear();
		// 	// interI.clear();
		// 	inter.clear();
		// 	dis.clear();
		// 	for (size_t j = 0; j < segs.size(); j++)
		// 	{
		// 		if (i == j)
		// 			continue;
		// 		if (segs[i].isCrossInf(segs[j]))
		// 			interL.emplace_back(j);
		// 		// else if (segs[i].distancePointLine(segs[j].s) <= dist)
		// 		// 	interI.emplace_back(j);
		// 		// else if (segs[i].distancePointLine(segs[j].e) <= dist)
		// 		// 	interI.emplace_back(j);
		// 	}
		// 	for (size_t j = 0; j < interL.size(); j++)
		// 	{
		// 		c = segs[i].getCrossInf(segs[interL[j]]);
		// 		if (segs[i].distancePointLine(c) <= dist)
		// 			inter.emplace_back(c);
		// 	}
		// 	// for (size_t j = 0; j < interI.size(); j++)
		// 	// {
		// 	// 	c = segs[i].getCross(segs[interI[j]]);
		// 	// 	inter.emplace_back(c);
		// 	// 	// if (segs[i].distancePointLine(segs[interI[j]].s) <= dist)
		// 	// 	// 	inter.emplace_back(segs[interI[j]].s);
		// 	// 	// else
		// 	// 	// 	inter.emplace_back(segs[interI[j]].e);
		// 	// }

		// 	// inter.emplace_back(segs[i].s);
		// 	// inter.emplace_back(segs[i].e);

		// 	// debug
		// 	segs[i].debug();
		// 	for (size_t j = 1; j < inter.size(); j++)
		// 		std::cout << j << "\t" << "(" << inter[j].x << "," << inter[j].y << ")," << std::endl; 

		// 	c = inter[0];
		// 	for (size_t j = 1; j < inter.size(); j++)
		// 		if (c.x < inter[j].x)
		// 			c = inter[j];
		// 		else if (c.x == inter[j].x && c.y == inter[j].y)
		// 			c = inter[j];

		// 	dis = std::vector<double>(inter.size());
		// 	for (size_t j = 0; j < inter.size(); j++)
		// 		dis[j] = Vec2::distance(c, inter[j]);

		// 	maxdis = dis[0];
		// 	max = inter[0];
		// 	mindis = dis[0];
		// 	min = inter[0];
		// 	for (size_t j = 1; j < dis.size(); j++)
		// 	{
		// 		if (maxdis > dis[j])
		// 		{
		// 			maxdis = dis[j];
		// 			max = inter[j];
		// 		}
		// 		if (mindis < dis[j])
		// 		{
		// 			mindis = dis[j];
		// 			min = inter[j];
		// 		}
		// 	}

		// 	o.emplace_back(Line2(min, max, segs[i].id, false));

		// 	//// sort
		// 	//for (size_t j = 0; j < dis.size(); j++)
		// 	//	for (size_t k = 0; k < dis.size() - j - 1; k++)
		// 	//		if (dis[k] > dis[k + 1])
		// 	//		{
		// 	//			std::swap(dis[k], dis[k + 1]);
		// 	//			std::swap(inter[k], inter[k + 1]);
		// 	//		}

		// 	//for (size_t j = 0; j < inter.size() - 1; j++)
		// 	//	o.emplace_back(Line2(inter[j], inter[j + 1], segs[i].id));
		// }
		// return o;
	}

	static std::vector<Line2> arc2segs(Vec2 s, Vec2 e, Vec2 c, long id, double r)
	{
		std::vector<Line2> o;
		if (r == 0)
			return o;

		double dis = Vec2::distance(s, c);
		if (dis == 0)
			return o;

		Line2 se = Line2(s, e, false);
		if (Line2::isOnLine(se, c))
			return o;

		s.id = id;
		e.id = id;
		c.id = id;

		Vec2 sc = s - c;
		Vec2 ec = e - c;

		double sign = Mathf::sign(r);

		if (sign < 0)
		{
			std::swap(sc, ec);
			std::swap(s, e);
		}

		// deg
		double bases = Vec2::signedAngle(Vec2::right, sc);
		double arcs = std::atan2(sc.x, sc.y) * Mathf::RED2DEG;
		double arce = std::atan2(ec.x, ec.y) * Mathf::RED2DEG;
		double arcse = arce - arcs;
		if (arcse < 0)
			arcse += 360;
		int_fast32_t rcnt = arcse / r;
		rcnt = rcnt;

		// red
		r *= Mathf::DEG2RED;
		bases *= Mathf::DEG2RED;

		std::vector<Vec2> vecs;

		vecs.emplace_back(s);
		Vec2 v;
		for (int_fast32_t i = sign; i != rcnt; i += sign)
		{
			v = Vec2(std::cos(r * -i + bases) * dis,
				     std::sin(r * -i + bases) * dis) + c;
			v.id = id;
			vecs.emplace_back(v);
		}
		if (vecs.back() != e)
			vecs.emplace_back(e);
		if (sign < 0)
			std::reverse(vecs.begin(), vecs.end());

		for (size_t i = 0; i < vecs.size() - 1; i++)
			o.emplace_back(Line2(vecs[i], vecs[i + 1], false));
		return o;
	}

	static std::vector<Vec2> arc2vecs(Vec2 s, Vec2 e, Vec2 c, long id, double r)
	{
		std::vector<Vec2> o;
		if (r == 0)
			return o;

		double dis = Vec2::distance(s, c);
		if (dis == 0)
			return o;

		Line2 se = Line2(s, e, false);
		if (Line2::isOnLine(se, c))
			return o;

		s.id = id;
		e.id = id;
		c.id = id;

		Vec2 sc = s - c;
		Vec2 ec = e - c;

		double sign = Mathf::sign(r);

		if (sign < 0)
		{
			std::swap(sc, ec);
			std::swap(s, e);
		}

		// deg
		double bases = Vec2::signedAngle(Vec2::right, sc);
		double arcs = std::atan2(sc.x, sc.y) * Mathf::RED2DEG;
		double arce = std::atan2(ec.x, ec.y) * Mathf::RED2DEG;
		double arcse = arce - arcs;
		if (arcse < 0)
			arcse += 360;
		int_fast32_t rcnt = arcse / r;
		rcnt = rcnt;

		// red
		r *= Mathf::DEG2RED;
		bases *= Mathf::DEG2RED;

		o.emplace_back(s);
		Vec2 v;
		for (int_fast32_t i = sign; i != rcnt; i += sign)
		{
			v = Vec2(std::cos(r * -i + bases) * dis,
				     std::sin(r * -i + bases) * dis) + c;
			v.id = id;
			o.emplace_back(v);
		}
		if (o.back() != e)
			o.emplace_back(e);
		if (sign < 0)
			std::reverse(o.begin(), o.end());
		return o;
	}

	static bool segs2arc(std::vector<Line2> segs, size_t sn, double r, double rmargin, size_t minchain, size_t* en, Vec2* s, Vec2* e, Vec2* c, long* id)
	{
		if (minchain < 2)
			minchain = 2;

		if (sn + 2 >= segs.size() || sn + minchain >= segs.size())
			return false;

		rmargin = std::abs(rmargin);
		r *= Mathf::DEG2RED;
		rmargin *= Mathf::DEG2RED;

		Vec2 v;
		v = segs[sn].vec();
		double arc1 = std::atan2(v.x, v.y);
		v = segs[sn + 1].vec();
		double arc2 = std::atan2(v.x, v.y);
		double arcdiff = arc2 - arc1;

		if (std::abs(arcdiff - r) > rmargin)
			return false;

		size_t i = sn + 2;
		while (i < segs.size())
		{
			arc1 = arc2;
			v = segs[i].vec();
			arc2 = std::atan2(v.x, v.y);
			arcdiff = arc2 - arc1;
			if (std::abs(arcdiff - r) > rmargin)
				break;
			i++;
		}

		if (i - sn < minchain)
			return false;

		//size_t mn = (i - sn) * 0.5;
		size_t mn = sn + 1;
		Line2 t1 = segs[sn].perpendicular();
		Line2 t2 = segs[mn].perpendicular();

		if (!Line2::isCrossInf(t1, t2))
			return false;

		*en = i;
		*s = segs[sn].s;
		*e = segs[*en - 1].e;
		*c = Line2::getCrossInf(t1, t2);
		*id = segs[mn].s.id;

		return true;
	}

	static bool poly2arc(std::vector<Vec2> poly, size_t sn, double r, double rmargin, size_t minchain, size_t* en, Vec2* s, Vec2* e, Vec2* c, long* id)
	{
		if (minchain < 2)
			minchain = 2;

		if (sn + 2 >= poly.size() || sn + minchain >= poly.size())
			return false;

		rmargin = std::abs(rmargin);
		r *= Mathf::DEG2RED;
		rmargin *= Mathf::DEG2RED;

		Vec2 v;
		v = poly[sn + 1] - poly[sn];
		double arc1 = std::atan2(v.x, v.y);
		v = poly[sn + 2] - poly[sn + 1];
		double arc2 = std::atan2(v.x, v.y);
		double arcdiff = arc2 - arc1;

		if (std::abs(arcdiff - r) > rmargin)
			return false;

		size_t i = sn + 2;
		while (i < poly.size() - 1)
		{
			arc1 = arc2;
			v = poly[i + 1] - poly[i];
			arc2 = std::atan2(v.x, v.y);
			arcdiff = arc2 - arc1;
			if (std::abs(arcdiff - r) > rmargin)
				break;
			i++;
		}

		if (i - sn < minchain)
			return false;

		//size_t mn = (i - sn) * 0.5;
		size_t mn = sn + 1;
		Line2 t1 = Line2(poly[sn], poly[sn + 1], false).perpendicular();
		Line2 t2 = Line2(poly[mn], poly[mn + 1], false).perpendicular();

		if (!Line2::isCrossInf(t1, t2))
			return false;

		*en = i;
		*s = poly[sn];
		*e = poly[*en];
		*c = Line2::getCrossInf(t1, t2);
		*id = poly[mn].id;

		return true;
	}

private:
	static std::vector<Vec2> trailLines(std::vector<Line2>& lines)
	{
		std::vector<Vec2> poly;

		std::vector<stLineLink> link;
		std::vector<std::vector<Vec2>> linkedLine;

// #if DEBUG_LOG
// 		for (size_t i = 0; i < lines.size(); i++)
// 		{
// 			lines[i].debug(0, 0, false);
// 			std::cout << "," << std::endl;
// 		}
// 		std::cout << std::endl;
// #endif

		// make link
		{
			std::vector<Line2> desLines;
			std::vector<Line2> lks;
			bool ff;
			bool fp;
			size_t k;
			for (size_t i = 0; i < lines.size(); i++)
			{
				lks.clear();
				ff = false;
				fp = false;
				for (size_t j = 0; j < lines.size(); j++)
				{
					if (i == j)
						continue;
					
					if (lines[i].s == lines[j].s || lines[i].s == lines[j].e)
					{
						ff = true;
						lks.emplace_back(lines[j]);
					}
					else if (lines[i].e == lines[j].s || lines[i].e == lines[j].e)
					{
						fp = true;
						lks.emplace_back(lines[j]);
					}
				}
				if (lks.size() > 0 && ff && fp)
					link.emplace_back((stLineLink){lines[i], lks});		
				else
					desLines.emplace_back(lines[i]);
			}

			for (size_t i = 0; i < desLines.size(); i++)
				for (size_t j = 0; j < link.size(); j++)
				{
					k = 0;
					while (k < link[j].lines.size())
						if (link[j].lines[k] == desLines[i])
							link[j].lines.erase(link[j].lines.cbegin() + k);
						else
							k++;
				}
			
			k = 0;
			while (k < link.size())
				if (link[k].lines.size() < 2)
					link.erase(link.cbegin() + k);
				else
					k++;
		}

		if (link.size() <= 0)
			return std::vector<Vec2>(0);

		// make linkedLine
		{
			std::vector<Vec2> linked;
			for (size_t i = 0; i < link.size(); i++)
			{
				linked.clear();
				linked.emplace_back(link[i].me.s);
				linked.emplace_back(link[i].me.e);
				for (size_t j = 0; j < link[i].lines.size(); j++)
					if (linked.back() == link[i].lines[j].s)
					{
						linked.emplace_back(link[i].lines[j].e);
						break;
					}
					else if (linked.back() == link[i].lines[j].e)
					{
						linked.emplace_back(link[i].lines[j].e);
						break;
					}
				linkedLine.emplace_back(linked);
			}
		}

		// trail
		{
			bool f;
			poly.emplace_back(linkedLine[0][0]);
			while (true)
			{
				f = false;
				for (size_t i = 0; i < linkedLine.size(); i++)
				{
					if (poly[0] == linkedLine[i][0])
					{
						poly.insert(poly.begin(), linkedLine[i][1]);
						linkedLine.erase(linkedLine.cbegin() + i);
					}
					else if (poly[0] == linkedLine[i][1])
					{
						poly.insert(poly.begin(), linkedLine[i][0]);
						linkedLine.erase(linkedLine.cbegin() + i);
					}
					else if (poly.back() == linkedLine[i][0])
					{
						poly.emplace_back(linkedLine[i][1]);
						linkedLine.erase(linkedLine.cbegin() + i);
					}
					else if (poly.back() == linkedLine[i][1])
					{
						poly.emplace_back(linkedLine[i][0]);
						linkedLine.erase(linkedLine.cbegin() + i);
					}
					else
						continue;
					f = true;
					break;
				}
				
				if (!f)
					break;
				if (linkedLine.size() <= 0)
					break;
				if (poly[0] == poly.back() && poly.size() >= 3)
					break;
			}

			if (poly[0] == poly.back())
				poly.pop_back();
			else
				poly.clear();
		}

		return poly;
	}

	static bool isGridOverLine(std::vector<Vec2>& s, Line2& l)
	{
		std::vector<Vec2> cps;
		size_t ii;
		Line2 ls;
		Vec2 cp;
		bool f;
		for (size_t i = 0; i < s.size(); i++)
		{
			ii = fmod(i + 1, s.size());
			ls = Line2(s[i], s[ii]);
			if (!ls.isCross(l))
				continue;
			cp = ls.getCross(l);
			f = true;
			for (size_t j = 0; j < cps.size(); j++)
				if (cps[j] == cp)
				{
					f = false;
					break;
				}
			if (f)
			{
				cps.emplace_back(cp);
				if (cps.size() >= 2)
					break;
			}
		}
		return cps.size() >= 2;
	}

	static std::vector<Line2> getRangeLines(std::vector<Line2>& segs, Vec2 p, Vec2 wh)
	{
		std::vector<Line2> o;
		std::vector<Vec2> shape = std::vector<Vec2>({
			p,
			Vec2(p.x + wh.x, p.y),
			p + wh,
			Vec2(p.x, p.y + wh.y)
			});
		for (size_t i = 0; i < segs.size(); i++)
			if (isGridOverLine(shape, segs[i]))
				o.emplace_back(segs[i]);
		return o;
	}
};
