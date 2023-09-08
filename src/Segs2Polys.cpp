#include "Segs2Polys.hpp"

static std::vector<Line2> gSegs = std::vector<Line2>();
static std::vector<std::vector<Vec2>> gPolys = std::vector<std::vector<Vec2>>();

EXPORT INT WINAPI Clear()
{
	gSegs.clear();
	gPolys.clear();
	return 0;
}

EXPORT INT WINAPI ClearSegs()
{
	gSegs.clear();
	return 0;
}

EXPORT INT WINAPI ClearPolys()
{
	gPolys.clear();
	return 0;
}

EXPORT INT WINAPI ClearRangeSegs(LONG s, LONG e)
{
	INT o = 0;
	try
	{
		gSegs.erase(gSegs.begin() + (size_t)s, gSegs.begin() + (size_t)e);
	}
	catch (...)
	{
		o = -1;
	}
	return o;
}

EXPORT INT WINAPI ClearRangePolys(LONG s, LONG e)
{
	INT o = 0;
	try
	{
		gPolys.erase(gPolys.begin() + (size_t)s, gPolys.begin() + (size_t)e);
	}
	catch (...)
	{
		o = -1;
	}
	return o;
}

EXPORT INT WINAPI ClearRangePolyVec(LONG pn, LONG s, LONG e)
{
	INT o = 0;
	try
	{
		gPolys[pn].erase(gPolys[(size_t)pn].begin() + (size_t)s, gPolys[(size_t)pn].begin() + (size_t)e);
	}
	catch (...)
	{
		o = -1;
	}
	return o;
}

//
// segment
//

EXPORT INT WINAPI AddSeg(DOUBLE x1, DOUBLE y1, LONG id1, DOUBLE x2, DOUBLE y2, LONG id2)
{
	gSegs.emplace_back(Line2(Vec2((double)x1, (double)y1, id1), Vec2((double)x2, (double)y2, id2)));
	return 0;
}

// cnavi default 5deg
EXPORT INT WINAPI AddSegArc(DOUBLE x1, DOUBLE y1, DOUBLE x2, DOUBLE y2, DOUBLE cx, DOUBLE cy, LONG id, DOUBLE r)
{
	INT o = 0;
	std::vector<Line2> ls;
	try
	{
		ls = Segs2Polys::arc2segs(Vec2((double)x1, (double)y1),
							      Vec2((double)x2, (double)y2),
			                      Vec2((double)cx, (double)cy),
								  (long)id, (double)r);
		for (size_t i = 0; i < ls.size(); i++)
			gSegs.emplace_back(ls[i]);
	}
	catch (...)
	{
		o = -1;
	}
	return o;
}

EXPORT LONG WINAPI GetSegCnt()
{
	return (LONG)gSegs.size();
}

EXPORT INT WINAPI GetSeg(LONG sn, DOUBLE* x1, DOUBLE* y1, LONG* id1, DOUBLE* x2, DOUBLE* y2, LONG* id2)
{
	INT o = 0;
	try
	{
		Line2& l = gSegs[sn];
		*x1 = (DOUBLE)l.s.x;
		*y1 = (DOUBLE)l.s.y;
		*id1 = (LONG)l.s.id;
		*x2 = (DOUBLE)l.e.x;
		*y2 = (DOUBLE)l.e.y;
		*id2 = (LONG)l.e.id;
	}
	catch (...)
	{
		o = -1;
	}
	return o;
}

EXPORT INT WINAPI GetSegS(LONG sn, DOUBLE* x, DOUBLE* y, LONG* id)
{
	INT o = 0;
	try
	{
		Line2& l = gSegs[sn];
		*x = (DOUBLE)l.s.x;
		*y = (DOUBLE)l.s.y;
		*id = (LONG)l.s.id;
	}
	catch (...)
	{
		o = -1;
	}
	return o;
}

EXPORT INT WINAPI GetSegE(LONG sn, DOUBLE* x, DOUBLE* y, LONG* id)
{
	INT o = 0;
	try
	{
		Line2& l = gSegs[sn];
		*x = (DOUBLE)l.e.x;
		*y = (DOUBLE)l.e.y;
		*id = (LONG)l.e.id;
	}
	catch (...)
	{
		o = -1;
	}
	return o;
}

EXPORT INT WINAPI GetSegArc(LONG sn, DOUBLE r, DOUBLE rmargin, LONG minchain, LONG* en, DOUBLE* x1, DOUBLE* y1, DOUBLE* x2, DOUBLE* y2, DOUBLE* cx, DOUBLE* cy, LONG* id)
{
	INT o = 0;
	std::vector<Line2> ls;
	size_t ren;
	Vec2 rs;
	Vec2 re;
	Vec2 rc;
	long rid;
	try
	{
		o = (INT)Segs2Polys::segs2arc(gSegs, (size_t)sn,
			                          (double)r, (double)rmargin, (size_t)minchain,
			                          &ren, &rs, &re, &rc, &rid);
		*en = (LONG)ren;
		*x1 = (DOUBLE)rs.x;
		*y1 = (DOUBLE)rs.y;
		*x2 = (DOUBLE)re.x;
		*y2 = (DOUBLE)re.y;
		*cx = (DOUBLE)rc.x;
		*cy = (DOUBLE)rc.y;
		*id = (LONG)rid;
	}
	catch (...)
	{
		o = -1;
	}
	return o;
}

// 最後に付け足すだけ
// 開始点のみを追加する
EXPORT INT WINAPI CnvSegs2Poly()
{
	INT o = 0;
	try
	{
		size_t ps = gPolys.size();
		for (size_t i = 0; i < gSegs.size(); i++)
			gPolys[ps].emplace_back(gSegs[i].s);
	}
	catch (...)
	{
		o = -1;
	}
	return o;
}

//
// polygon
//

EXPORT INT WINAPI AddPoly()
{
	INT o = 0;
	try
	{
		gPolys.emplace_back(std::vector<Vec2>());
	}
	catch (...)
	{
		o = -1;
	}
	return o;
}

EXPORT INT WINAPI AddPolyVec(LONG pn, DOUBLE x, DOUBLE y, LONG id)
{
	INT o = 0;
	try
	{
		gPolys[pn].emplace_back(Vec2((double)x, (double)y, (long)id));
	}
	catch (...)
	{
		o = -1;
	}
	return o;
}

// cnavi default 5deg
EXPORT INT WINAPI AddPolyArc(LONG pn, DOUBLE x1, DOUBLE y1, DOUBLE x2, DOUBLE y2, DOUBLE cx, DOUBLE cy, LONG id, DOUBLE r)
{
	INT o = 0;
	std::vector<Vec2> vs;
	try
	{
		vs = Segs2Polys::arc2vecs(Vec2((double)x1, (double)y1),
							      Vec2((double)x2, (double)y2),
			                      Vec2((double)cx, (double)cy),
								  (long)id, (double)r);
		for (size_t i = 0; i < vs.size(); i++)
			gPolys[pn].emplace_back(vs[i]);
	}
	catch (...)
	{
		o = -1;
	}
	return o;
}

EXPORT LONG WINAPI GetPolyCnt()
{
	return (LONG)gPolys.size();
}

EXPORT LONG WINAPI GetPolyVecCnt(LONG pn)
{
	LONG o;
	try
	{
		o = (LONG)gPolys[pn].size();
	}
	catch (...)
	{
		o = 0;
	}
	return o;
}

EXPORT INT WINAPI GetPolyVec(LONG pn, LONG vn, DOUBLE* x, DOUBLE* y, LONG* id)
{
	INT o = 0;
	try
	{
		Vec2& v = gPolys[pn][vn];
		*x = (DOUBLE)v.x;
		*y = (DOUBLE)v.y;
		*id = (LONG)v.id;
	}
	catch (...)
	{
		o = -1;
	}
	return o;
}

EXPORT INT WINAPI GetPolyArc(LONG pn, LONG vn, DOUBLE r, DOUBLE rmargin, LONG minchain, LONG* en, DOUBLE* x1, DOUBLE* y1, DOUBLE* x2, DOUBLE* y2, DOUBLE* cx, DOUBLE* cy, LONG* id)
{
	INT o = 0;
	std::vector<Line2> ls;
	size_t ren;
	Vec2 rs;
	Vec2 re;
	Vec2 rc;
	long rid;
	try
	{
		o = (INT)Segs2Polys::poly2arc(gPolys[pn], (size_t)vn,
			                          (double)r, (double)rmargin, (size_t)minchain,
			                          &ren, &rs, &re, &rc, &rid);
		*en = (LONG)ren;
		*x1 = (DOUBLE)rs.x;
		*y1 = (DOUBLE)rs.y;
		*x2 = (DOUBLE)re.x;
		*y2 = (DOUBLE)re.y;
		*cx = (DOUBLE)rc.x;
		*cy = (DOUBLE)rc.y;
		*id = (LONG)rid;
	}
	catch (...)
	{
		o = -1;
	}
	return o;
}

// 最後に付け足すだけ
EXPORT INT WINAPI CnvPoly2Segs(LONG pn)
{
	INT o = 0;
	try
	{
		size_t ps = gPolys[pn].size();
		size_t ii;
		for (size_t i = 0; i < ps; i++)
		{
			ii = fmod(i + 1, ps);
			gSegs.emplace_back(Line2(gPolys[pn][i], gPolys[pn][ii]));
		}
	}
	catch (...)
	{
		o = -1;
	}
	return o;
}

//
// Segs2Polys
//

EXPORT INT WINAPI CalcExtendMarginIntersection(DOUBLE dist)
{
	INT o = 0;
	try
	{
		gSegs = Segs2Polys::extendMarginIntersection(gSegs, (double)dist);
	}
	catch (...)
	{
		o = -1;
	}
	return o;
}

EXPORT INT WINAPI CalcSplitLines()
{
	INT o = 0;
	try
	{
		gSegs = SimpleSplitSegs::splitSegs(gSegs);
	}
	catch (...)
	{
		o = -1;
	}
	return o;
}

EXPORT INT WINAPI CalcSegs2Polys()
{
	INT o = 0;
	try
	{
		std::vector<std::vector<Cell>> cells = Segs2Polys::createGrid(gSegs);
		std::vector<stCellLink> links = Segs2Polys::getLinkRaycast(cells);
		gPolys = Segs2Polys::trailCells(cells, links);
	}
	catch (...)
	{
		o = -1;
	}
	return o;
}

EXPORT INT WINAPI CalcMergePolysLines()
{
	INT o = 0;
	try
	{
		gPolys = Segs2Polys::mergePolysLines(gPolys);
	}
	catch (...)
	{
		o = -1;
	}
	return o;
}

//
// Poly2Triangle
//

EXPORT LONG WINAPI CalcEarClip(LONG pn, LONG safety)
{
	LONG o = 0;
	try
	{
		std::vector<std::vector<Vec2>> ret;
		if (TriangleUtil::EarClip(gPolys[pn], ret, (size_t)safety))
		{
			for (size_t i = 0; i < ret.size(); i++)
				gPolys.emplace_back(ret[i]);
			o = ret.size();
		}
		else
			o = -1;
	}
	catch (...)
	{
		o = -1;
	}
	return o;
}

EXPORT LONG WINAPI CalcEarClipHoles(LONG pn, LONG hpn, LONG* hp, LONG safety)
{
	LONG o = 0;
	try
	{
		std::vector<std::vector<Vec2>> ret;
		std::vector<std::vector<Vec2>> holes = std::vector<std::vector<Vec2>>(hpn);
		for (size_t i = 0; i < hpn; i++)
			holes[i] = gPolys[(size_t)*(hp + i)];

		if (TriangleUtil::EarClipHoles(gPolys[pn], holes, ret, (size_t)safety))
		{
			for (size_t i = 0; i < ret.size(); i++)
				gPolys.emplace_back(ret[i]);
			o = ret.size();
		}
		else
			o = -1;
	}
	catch (...)
	{
		o = -1;
	}
	return o;
}

//
// Utility
//

// right = 0
// left  = 1
EXPORT INT WINAPI CalcDirectionRotatePoly(LONG pn, INT r)
{
	INT o = 0;
	try
	{
		Vec2Util::directionRotatePoly(gPolys[pn], (int)r);
	}
	catch (...)
	{
		o = -1;
	}
	return o;
}

// right = 0
// left  = 1
EXPORT INT WINAPI CalcDirectionRotatePolys(INT r)
{
	INT o = 0;
	try
	{
		Segs2Polys::directionRotatePolys(gPolys, (int)r);
	}
	catch (...)
	{
		o = -1;
	}
	return o;
}

EXPORT INT WINAPI CalcTrianglesSplitLines(LONG spn, LONG epn, LONG ssn, LONG esn)
{
	INT o = 0;
	try
	{
		std::vector<std::vector<Vec2>> ret;
		std::vector<std::vector<Vec2>> polys = std::vector<std::vector<Vec2>>(gPolys.begin() + (size_t)spn, gPolys.begin() + (size_t)epn);
		std::vector<Line2> segs = std::vector<Line2>(gSegs.begin() + (size_t)ssn, gSegs.begin() + (size_t)esn);
		if (TriangleUtil::SplitTrisSegs(polys, segs, ret))
			for (size_t i = 0; i < ret.size(); i++)
				gPolys.emplace_back(ret[i]);
		else
			o = -1;
	}
	catch (...)
	{
		o = -1;
	}
	return o;
}

//
// Debug
//

// EXPORT INT WINAPI DebugCellRays()
// {
// 	INT o = 0;
// 	try
// 	{
// 		std::vector<std::vector<Cell>> cells = Segs2Polys::createGrid(gSegs);
// 		gPolys.emplace_back(std::vector<Vec2>());
// 		for (size_t	i = 0; i < cells.size(); i++)
// 			for (size_t	j = 0; j < cells[i].size(); j++)
// 				for (size_t	k = 0; k < cells[i][j].shape.rays.size(); k++)
// 					gPolys.back().emplace_back(cells[i][j].shape.rays[k]);
// 	}
// 	catch (...)
// 	{
// 		o = -1;
// 	}
// 	return o;
// }

// EXPORT INT WINAPI DebugRayLinks()
// {
// 	INT o = 0;
// 	try
// 	{
// 		std::vector<std::vector<Cell>> cells = Segs2Polys::createGrid(gSegs);
// 		std::vector<stCellLink> links = Segs2Polys::getLinkRaycast(cells);
// 		gPolys.emplace_back(std::vector<Vec2>());
// 		for (size_t	i = 0; i < links.size(); i++)
// 		{
// 			gPolys.back().emplace_back(cells[links[i].cell11][links[i].cell12].shape.rays[links[i].cellLink1]);
// 			gPolys.back().emplace_back(cells[links[i].cell21][links[i].cell22].shape.rays[links[i].cellLink2]);
// 		}
// 	}
// 	catch (...)
// 	{
// 		o = -1;
// 	}
// 	return o;
// }
