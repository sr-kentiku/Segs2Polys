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

class CellInternal
{
private:
#if !DEBUG_LOG
	// static constexpr double kcon = Vec2::kEpsilon * 100;
	static constexpr double kcon = 0.1;
#else
	// static constexpr double kcon = Vec2::kEpsilon;
	// static constexpr double kcon = Vec2::kEpsilon * 100;
	// static constexpr double kcon = 0.1;
	// static constexpr double kcon = 0.05;
	// static constexpr double kcon = 200;
	static constexpr double kcon = 100;
#endif

public:
	std::vector<Vec2>						hull;
	std::vector<Line2>						lines;
	std::vector<Vec2>						rays;
	std::vector<std::vector<size_t>>		sp;
	
	CellInternal() : hull(std::vector<Vec2>()), lines(std::vector<Line2>()), rays(std::vector<Vec2>()), sp(std::vector<std::vector<size_t>>()) {}
	CellInternal(std::vector<Vec2> hull, std::vector<Line2> lines, Vec2 swh, Vec2 ewh) : hull(hull), lines(lines), rays(std::vector<Vec2>()), sp(std::vector<std::vector<size_t>>())
	{
		if(isAllVH())
			calcVHCentorOfMass(swh, ewh);
		else
			calcNoVHCentorOfMass(swh, ewh);
	}
	
	bool isHitRay(Line2& ray)
	{
		for (size_t i = 0; i < lines.size(); i++)
			if (ray.isCross(lines[i]))
				return true;
		return false;
	}

#if DEBUG_LOG
	void debug(uint_fast8_t t1 = 0, uint_fast8_t t2 = 0, bool r = true) const
	{
		size_t i;

		std::cout << "{" << std::endl;
		std::cout << "hull:" << std::endl;
		for (i = 0; i < hull.size(); i++)
			hull[i].debug(t1, true);
		std::cout << "lines:" << std::endl;
		for (i = 0; i < lines.size(); i++)
		{
			std::cout << i << ":";
			lines[i].debug(t1, t2, true);
		}
		std::cout << "rays, sp:" << std::endl;
		for (i = 0; i < rays.size(); i++)
		{
			rays[i].debug(t1, false);
			std::cout << ":[";
			for (size_t j = 0; j < sp[i].size(); j++)
				std::cout << sp[i][j] << ", ";
			std::cout << "]" << std::endl;
		}
		std::cout << "}";
		if (r)
			std::cout << std::endl;
	}
#endif

private:
	bool isAllVH()
	{
		for (size_t i = 0; i < lines.size(); i++)
			if (!lines[i].isVH())
				return false;
		return true;
	}

	std::vector<Line2> clipInLine()
	{
		std::vector<Line2> o;
		uint_fast8_t inter = 0;
		Vec2 ps;
		Vec2 pe;
		size_t jj;
		Line2 lh;
		Vec2 c;
		for (size_t i = 0; i < lines.size(); i++)
		{
			inter = 0;
			for (size_t j = 0; j < hull.size(); j++)
			{
				jj = fmod(j + 1, hull.size());
				lh = Line2(hull[j], hull[jj]);
				if (!lh.isCross(lines[i]))
					continue;
				c = lh.getCross(lines[i]);
				switch (inter)
				{
					case 0:
						inter = 1;
						ps = c;
						break;
					case 1:
						if (ps == c)
							continue;
						inter = 2;
						pe = c;
						break;
				}
				if (inter == 2)
					break;
			}
			if (inter == 2)
				o.emplace_back(Line2(ps, pe, -1, (long)i, (long)i));
		}
		return o;
	}

	void calcVHCentorOfMass(Vec2& swh, Vec2& ewh)
	{
		std::vector<size_t> idx1;
		for (size_t i = 0; i < lines.size(); i++)
			idx1.emplace_back(i);
		sp.emplace_back(idx1);
		Vec2 cm = ewh - swh;
		cm *= Vec2::five;
		cm += swh;
		rays.emplace_back(cm);
	}

	void calcNoVHCentorOfMass(Vec2& swh, Vec2& ewh)
	{
		std::vector<Line2> clpLines = clipInLine();
		
		// sort
		{
			Vec2 v1;
			Vec2 v2;
			for (size_t i = 0; i < clpLines.size(); i++)
				for (size_t j = 0; j < clpLines.size() - i - 1; j++)
				{
					if (clpLines[j].s.x > clpLines[j + 1].s.x)
						std::swap(clpLines[j], clpLines[j + 1]);
					else if (clpLines[j].s.x == clpLines[j + 1].s.x && clpLines[j].s.y == clpLines[j + 1].s.y)
						std::swap(clpLines[j], clpLines[j + 1]);
				}
		}

		std::vector<Vec2> rays;
		std::vector<Line2*> raysLineSrc;
		std::vector<Vec2*> raysLinef;
		std::vector<Vec2*> raysLineb;

		std::vector<Line2*> vhRaysLineSrc;

		bool ap = true;

		{
			Line2 m;
			Line2 r;
			Line2 tl;
			for (size_t i = 0; i < clpLines.size(); i++)
			{
				m = clpLines[i].midPerpendicular();
				r = m.resize(kcon);
				tl = Line2(
					r.e,
					r.s - r.vec()
				);
				tl.s.w = clpLines[i].w;
				tl.e.w = clpLines[i].w;

				if (!clpLines[i].isVH())
				{
					rays.emplace_back(tl.s);
					rays.emplace_back(tl.e);
					raysLineSrc.emplace_back(&clpLines[i]);

					if (ap)
						if (raysLineSrc.size() > 1)
							ap = (*raysLineSrc[0]).isVH(clpLines[i]);
				}
				else
				{
					vhRaysLineSrc.emplace_back(&clpLines[i]);
				}
			}

			for (size_t i = 0; i < rays.size(); i += 2)
			{
				raysLinef.emplace_back(&(rays[i]));
				raysLineb.emplace_back(&(rays[i + 1]));
			}
		}

		if (raysLineSrc.size() <= 0)
		{

#if DEBUG_LOG
	for (size_t i = 0; i < lines.size(); i++)
	{
		lines[i].debug(0, 0, false);
		std::cout << ",-----------------" << std::endl;
	}
#endif
			calcVHCentorOfMass(swh, ewh);
		}

		// single
		else if (raysLineSrc.size() == 1)
			calcNoVHCentorOfMassSingle(raysLineSrc, raysLinef, raysLineb, vhRaysLineSrc);
		
		// all parallel
		else if (ap)
			calcNoVHCentorOfMassParallel(raysLineSrc, raysLinef, raysLineb, vhRaysLineSrc);
		
		// other
		else
			calcNoVHCentorOfMassMixed(raysLineSrc, raysLinef, raysLineb, vhRaysLineSrc);
	}
	
	// +-------+----------+
	// |      /        ///|
	// | <1 /       / / / |
	// |  /   <2 /  /  /  |
	// |/     / <3/   /   |
	// +   /    /  <4/ >5 |
	// |/     /     /     |
	// +----+------+------+
	void calcNoVHCentorOfMassMixed(std::vector<Line2*>& raysLineSrc,
		                           std::vector<Vec2*>& raysLinef,
		                           std::vector<Vec2*>& raysLineb,
		                           std::vector<Line2*>& vhRaysLineSrc)
	{
		// angle sort
		std::vector<Line2*> lines = std::vector<Line2*>(raysLineSrc.size());
		std::vector<Vec2*> raysf = std::vector<Vec2*>(raysLinef.size());
		std::vector<Vec2*> raysb = std::vector<Vec2*>(raysLineb.size());
		std::vector<double> angles = std::vector<double>(raysLineSrc.size());
		for (size_t i = 0; i < raysLineSrc.size(); i++)
		{
			lines[i] = raysLineSrc[i];
			raysf[i] = raysLinef[i];
			raysb[i] = raysLineb[i];
		}
		for (size_t i = 0; i < lines.size(); i++)
			angles[i] = Vec2::signedAngle(Vec2::right, (*lines[i]).vec());
		for (size_t i = 0; i < angles.size(); i++)
			for (size_t j = 0; j < angles.size() - i - 1; j++)
				if (angles[j] > angles[j + 1])
				{
					std::swap(lines[j], lines[j + 1]);
					std::swap(raysf[j], raysf[j + 1]);
					std::swap(raysb[j], raysb[j + 1]);
					std::swap(angles[j], angles[j + 1]);
				}
				else if (angles[j] == angles[j + 1] && (*lines[j]).s.x > (*lines[j + 1]).s.x)
				{
					std::swap(lines[j], lines[j + 1]);
					std::swap(raysf[j], raysf[j + 1]);
					std::swap(raysb[j], raysb[j + 1]);
					std::swap(angles[j], angles[j + 1]);
				}
				else if (angles[j] == angles[j + 1] && (*lines[j]).s.x == (*lines[j + 1]).s.x && (*lines[j]).s.y > (*lines[j + 1]).s.y)
				{
					std::swap(lines[j], lines[j + 1]);
					std::swap(raysf[j], raysf[j + 1]);
					std::swap(raysb[j], raysb[j + 1]);
					std::swap(angles[j], angles[j + 1]);
				}

		#if DEBUG_LOG
		{
			std::cout << std::endl << std::endl << "calcNoVHCentorOfMassMixed" << std::endl;
			for (size_t i = 0; i < lines.size(); i++)
			{
				std::cout << i << std::endl;
				(*lines[i]).debug();
				std::cout << angles[i] << "\t";
				(*lines[i]).vec().debug();
				std::cout << std::endl;
			}
			std::cout << std::endl;
		}
		#endif

		calcNoVHCentorOfMassParallel(lines, raysf, raysb, vhRaysLineSrc);
	}

	// +---------+--------+
	// |        /        /|
	// |       /        / |
	// |   <1 /     <2 /  |
	// |     /        /   |
	// |    /        /  >3|
	// |   /        /     |
	// +--+--------+------+
	void calcNoVHCentorOfMassParallel(std::vector<Line2*>& raysLineSrc,
		                              std::vector<Vec2*>& raysLinef,
		                              std::vector<Vec2*>& raysLineb,
		                              std::vector<Line2*>& vhRaysLineSrc)
	{
		std::vector<size_t> fl;
		Vec2 vb;

		Vec2 v1;
		Vec2 v2;
		double c1;
		double c2;
		
		// front
		for (size_t i = 0;i < raysLineSrc.size();i++)
		{
			fl.clear();
			fl.emplace_back((size_t)(*raysLineSrc[i]).w);
			if (i > 0)
				fl.emplace_back((size_t)(*raysLineSrc[i - 1]).w);
			vb = (*raysLineSrc[i]).vec();
			if (vb.y < 0)
				vb = (*raysLineSrc[i]).s - (*raysLineSrc[i]).e;
			for (size_t j = 0; j < vhRaysLineSrc.size(); j++)
			{
				if ((*vhRaysLineSrc[j]).w1 == -1)
					continue;
				v1 = (*vhRaysLineSrc[j]).s - (*raysLineSrc[i]).s;
				v2 = (*vhRaysLineSrc[j]).e - (*raysLineSrc[i]).s;
				c1 = vb.cross(v1);
				c2 = vb.cross(v2);
				if (c1 >= 0 and c2 >= 0)
				{
					fl.emplace_back((size_t)(*vhRaysLineSrc[j]).w);
					(*vhRaysLineSrc[j]).w1 = -1;
				}
			}
			this->rays.emplace_back(*raysLinef[i]);
			sp.emplace_back(fl);
		}

		// back
		fl.clear();
		fl.emplace_back((size_t)(*raysLineSrc.back()).w);
		vb = (*raysLineSrc.back()).vec();
		if (vb.y < 0)
			vb = (*raysLineSrc.back()).s - (*raysLineSrc.back()).e;
		for (size_t j = 0; j < vhRaysLineSrc.size(); j++)
		{
			if ((*vhRaysLineSrc[j]).w1 == -1)
				continue;
			v1 = (*vhRaysLineSrc[j]).s - (*raysLineSrc.back()).s;
			v2 = (*vhRaysLineSrc[j]).e - (*raysLineSrc.back()).s;
			c1 = vb.cross(v1);
			c2 = vb.cross(v2);
			if (c1 <= 0 and c2 <= 0)
				fl.emplace_back((size_t)(*vhRaysLineSrc[j]).w);
		}
		this->rays.emplace_back(*raysLineb.back());
		sp.emplace_back(fl);
	}

	// +-----------+
	// |          /|
	// |         / |
	// |     <1 /  |
	// |       /   |
	// |      / >2 |
	// |     /     |
	// +----+------+
	void calcNoVHCentorOfMassSingle(std::vector<Line2*>& raysLineSrc,
							        std::vector<Vec2*>& raysLinef,
							        std::vector<Vec2*>& raysLineb,
							        std::vector<Line2*>& vhRaysLineSrc)
	{
		std::vector<size_t> fl;
		std::vector<size_t> bl;
		Vec2 vb;

		Vec2 v1;
		Vec2 v2;
		double c1;
		double c2;

		fl.emplace_back((size_t)(*raysLineSrc[0]).w);
		bl.emplace_back((size_t)(*raysLineSrc[0]).w);
		vb = (*raysLineSrc[0]).vec();
		for (size_t i = 0; i < vhRaysLineSrc.size(); i++)
		{
			v1 = (*vhRaysLineSrc[i]).s - (*raysLineSrc[0]).s;
			v2 = (*vhRaysLineSrc[i]).e - (*raysLineSrc[0]).s;
			c1 = vb.cross(v1);
			c2 = vb.cross(v2);
			if (c1 >= 0 and c2 >= 0)
				fl.emplace_back((size_t)(*vhRaysLineSrc[i]).w);
			if (c1 <= 0 and c2 <= 0)
				bl.emplace_back((size_t)(*vhRaysLineSrc[i]).w);
		}
		this->rays.emplace_back(*raysLinef[0]);
		this->rays.emplace_back(*raysLineb[0]);
		sp.emplace_back(fl);
		sp.emplace_back(bl);
	}
};

constexpr double CellInternal::kcon;
