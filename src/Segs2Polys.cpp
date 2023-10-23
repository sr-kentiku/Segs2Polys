#include "Segs2Polys.hpp"

static std::vector<Line2> gSegs;
static std::vector<std::vector<Vec2>> gPolys;

EXPORT int WINAPI Clear()
{
	gSegs.clear();
	gPolys.clear();
	return 0;
}

EXPORT int WINAPI ClearSegs()
{
	gSegs.clear();
	return 0;
}

EXPORT int WINAPI ClearPolys()
{
	gPolys.clear();
	return 0;
}

EXPORT int WINAPI ClearRangeSegs(long s, long e)
{
	int o = 0;
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

EXPORT int WINAPI ClearRangePolys(long s, long e)
{
	int o = 0;
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

EXPORT int WINAPI ClearRangePolyVec(long pn, long s, long e)
{
	int o = 0;
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

EXPORT int WINAPI AddSeg(double x1, double y1, long id1, double x2, double y2, long id2)
{
	gSegs.emplace_back(Line2(Vec2(x1, y1, id1), Vec2(x2, y2, id2)));
	return 0;
}

// cnavi default 5deg
EXPORT int WINAPI AddSegArc(double x1, double y1, double x2, double y2, double cx, double cy, long id, double r)
{
	int o = 0;
	std::vector<Line2> ls;
	try
	{
		ls = Segs2Polys::arc2segs(Vec2(x1, y1),
							      Vec2(x2, y2),
			                      Vec2(cx, cy),
								  id, r);
		for (size_t i = 0; i < ls.size(); i++)
			gSegs.emplace_back(ls[i]);
	}
	catch (...)
	{
		o = -1;
	}
	return o;
}

EXPORT long WINAPI GetSegCnt()
{
	return gSegs.size();
}

EXPORT int WINAPI GetSeg(long sn, double* x1, double* y1, long* id1, double* x2, double* y2, long* id2)
{
	int o = 0;
	Line2 l;
	try
	{
		l = gSegs[(size_t)sn];
		*x1 = l.s.x;
		*y1 = l.s.y;
		*id1 = l.s.id;
		*x2 = l.e.x;
		*y2 = l.e.y;
		*id2 = l.e.id;
	}
	catch (...)
	{
		o = -1;
	}
	return o;
}

EXPORT int WINAPI GetSegS(long sn, double* x, double* y, long* id)
{
	int o = 0;
	Line2 l;
	try
	{
		l = gSegs[(size_t)sn];
		*x = l.s.x;
		*y = l.s.y;
		*id = l.s.id;
	}
	catch (...)
	{
		o = -1;
	}
	return o;
}

EXPORT int WINAPI GetSegE(long sn, double* x, double* y, long* id)
{
	int o = 0;
	Line2 l;
	try
	{
		l = gSegs[(size_t)sn];
		*x = l.e.x;
		*y = l.e.y;
		*id = l.e.id;
	}
	catch (...)
	{
		o = -1;
	}
	return o;
}

EXPORT int WINAPI GetSegArc(long sn, double r, double rmargin, long minchain, long* en, double* x1, double* y1, double* x2, double* y2, double* cx, double* cy, long* id)
{
	int o = 0;
	std::vector<Line2> ls;
	size_t ren;
	Vec2 rs;
	Vec2 re;
	Vec2 rc;
	long rid;
	try
	{
		o = (int)Segs2Polys::segs2arc(gSegs, (size_t)sn,
			                          r, rmargin, (size_t)minchain,
			                          &ren, &rs, &re, &rc, &rid);
		*en = ren;
		*x1 = rs.x;
		*y1 = rs.y;
		*x2 = re.x;
		*y2 = re.y;
		*cx = rc.x;
		*cy = rc.y;
		*id = rid;
	}
	catch (...)
	{
		o = -1;
	}
	return o;
}

// 最後に付け足すだけ
// 開始点のみを追加する
EXPORT int WINAPI CnvSegs2Poly()
{
	int o = 0;
	size_t ps;
	try
	{
		ps = gPolys.size();
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

EXPORT int WINAPI AddPoly()
{
	int o = 0;
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

EXPORT int WINAPI AddPolyVec(long pn, double x, double y, long id)
{
	int o = 0;
	try
	{
		gPolys[(size_t)pn].emplace_back(Vec2(x, y, id));
	}
	catch (...)
	{
		o = -1;
	}
	return o;
}

// cnavi default 5deg
EXPORT int WINAPI AddPolyArc(long pn, double x1, double y1, double x2, double y2, double cx, double cy, long id, double r)
{
	int o = 0;
	std::vector<Vec2> vs;
	try
	{
		vs = Segs2Polys::arc2vecs(Vec2(x1, y1),
							      Vec2(x2, y2),
			                      Vec2(cx, cy),
								  id, r);
		for (size_t i = 0; i < vs.size(); i++)
			gPolys[(size_t)pn].emplace_back(vs[i]);
	}
	catch (...)
	{
		o = -1;
	}
	return o;
}

EXPORT long WINAPI GetPolyCnt()
{
	return gPolys.size();
}

EXPORT long WINAPI GetPolyVecCnt(long pn)
{
	long o;
	try
	{
		o = gPolys[(size_t)pn].size();
	}
	catch (...)
	{
		o = 0;
	}
	return o;
}

EXPORT int WINAPI GetPolyVec(long pn, long vn, double* x, double* y, long* id)
{
	int o = 0;
	Vec2 v;
	try
	{
		v = gPolys[(size_t)pn][(size_t)vn];
		*x = v.x;
		*y = v.y;
		*id = v.id;
	}
	catch (...)
	{
		o = -1;
	}
	return o;
}

EXPORT int WINAPI GetPolyArc(long pn, long vn, double r, double rmargin, long minchain, long* en, double* x1, double* y1, double* x2, double* y2, double* cx, double* cy, long* id)
{
	int o = 0;
	std::vector<Line2> ls;
	size_t ren;
	Vec2 rs;
	Vec2 re;
	Vec2 rc;
	long rid;
	try
	{
		o = (int)Segs2Polys::poly2arc(gPolys[(size_t)pn], (size_t)vn,
			                          r, rmargin, (size_t)minchain,
			                          &ren, &rs, &re, &rc, &rid);
		*en = ren;
		*x1 = rs.x;
		*y1 = rs.y;
		*x2 = re.x;
		*y2 = re.y;
		*cx = rc.x;
		*cy = rc.y;
		*id = rid;
	}
	catch (...)
	{
		o = -1;
	}
	return o;
}

// 最後に付け足すだけ
EXPORT int WINAPI CnvPoly2Segs(long pn)
{
	int o = 0;
	size_t ps;
	size_t ii;
	try
	{
		ps = gPolys[(size_t)pn].size();
		for (size_t i = 0; i < ps; i++)
		{
			ii = fmod(i + 1, ps);
			gSegs.emplace_back(Line2(gPolys[(size_t)pn][i], gPolys[(size_t)pn][ii]));
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

EXPORT int WINAPI CalcextendMarginIntersection(double dist)
{
	int o = 0;
	try
	{
		gSegs = Segs2Polys::extendMarginIntersection(gSegs, dist);
	}
	catch (...)
	{
		o = -1;
	}
	return o;
}

EXPORT int WINAPI CalcSplitLines()
{
	int o = 0;
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

EXPORT int WINAPI CalcSegs2Polys()
{
	int o = 0;
	std::vector<std::vector<Cell>> cells;
	std::vector<stCellLink> links;
	try
	{
		cells = Segs2Polys::createGrid(gSegs);
		links = Segs2Polys::getLinkRaycast(cells);
		gPolys = Segs2Polys::trailCells(cells, links);
	}
	catch (...)
	{
		o = -1;
	}
	return o;
}

EXPORT int WINAPI CalcMergePolysLines()
{
	int o = 0;
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

EXPORT long WINAPI CalcEarClip(long pn, long safety)
{
	long o = 0;
	std::vector<std::vector<Vec2>> ret;
	try
	{
		if (TriangleUtil::EarClip(gPolys[(size_t)pn], ret, (size_t)safety))
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

EXPORT long WINAPI CalcEarClipHoles(long pn, long hpn, long* hp, long safety)
{
	long o = 0;
	std::vector<std::vector<Vec2>> ret;
	std::vector<std::vector<Vec2>> holes;
	try
	{
		holes = std::vector<std::vector<Vec2>>(
			gPolys.begin() + (size_t)(*hp),
			gPolys.begin() + (size_t)(*hp) + (size_t)hpn
		);

		if (TriangleUtil::EarClipHoles(gPolys[(size_t)pn], holes, ret, (size_t)safety))
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
EXPORT int WINAPI CalcDirectionRotatePoly(long pn, int r)
{
	int o = 0;
	try
	{
		Vec2Util::directionRotatePoly(gPolys[(size_t)pn], (int)r);
	}
	catch (...)
	{
		o = -1;
	}
	return o;
}

// right = 0
// left  = 1
EXPORT int WINAPI CalcDirectionRotatePolys(int r)
{
	int o = 0;
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

EXPORT int WINAPI CalcTrianglesSplitLines(long spn, long epn, long ssn, long esn)
{
	int o = 0;
	std::vector<std::vector<Vec2>> polys;
	std::vector<Line2> segs;
	std::vector<std::vector<Vec2>> ret;
	try
	{
		polys = std::vector<std::vector<Vec2>>(
			gPolys.begin() + (size_t)spn,
			gPolys.begin() + (size_t)epn
		);
		segs = std::vector<Line2>(
			gSegs.begin() + (size_t)ssn,
			gSegs.begin() + (size_t)esn
		);
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

EXPORT double WINAPI CalcAreaPoly(long pn)
{
	double o = 0;
	try
	{
		o = Vec2Util::AreaTwoPoly(gPolys[(size_t)pn]);
		o *= 0.5;
	}
	catch (...)
	{
		o = 0;
	}
	return o;
}

EXPORT int WINAPI CalcParseTrisSegs(long spn, long epn, long ssn, long esn)
{
	int o = 0;
	std::vector<std::vector<Vec2>> polys;
	std::vector<Line2> segs;
	std::vector<std::vector<std::vector<Vec2>>> ret;
	try
	{
		polys = std::vector<std::vector<Vec2>>(
			gPolys.begin() + (size_t)spn,
			gPolys.begin() + (size_t)epn
		);
		segs = std::vector<Line2>(
			gSegs.begin() + (size_t)ssn,
			gSegs.begin() + (size_t)esn
		);
		ret = SplitTrisSegs::CalcParseTrisSegs(polys, segs);
		for (size_t i = 0; i < ret.size(); i++)
			for (size_t j = 0; j < ret[i].size(); j++)
			{
				for (size_t k = 0; k < ret[i][j].size(); k++)
					ret[i][j][k].id = i;
				gPolys.emplace_back(ret[i][j]);
			}
	}
	catch (...)
	{
		o = -1;
	}
	return o;
}

EXPORT int WINAPI CalcTri2Poly(long spn, long epn)
{
	int o = 0;
	std::vector<std::vector<Vec2>> polys;
	std::vector<std::vector<Vec2>> ret;
	try
	{
		polys = std::vector<std::vector<Vec2>>(
			gPolys.begin() + (size_t)spn,
			gPolys.begin() + (size_t)epn
		);
		ret = TriangleUtil::Tri2Poly(polys);
		for (size_t i = 0; i < ret.size(); i++)
			gPolys.emplace_back(ret[i]);
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

// EXPORT int WINAPI DebugCellRays()
// {
// 	int o = 0;
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

// EXPORT int WINAPI DebugRayLinks()
// {
// 	int o = 0;
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
