#include "Segs2Polys.hpp"
#include "Segs2Polys.cpp"

#if TEST
std::vector<Line2> segs = std::vector<Line2>({
	Line2(Vec2(1.0, 1.0), Vec2(1.0, 3.0)),
	Line2(Vec2(1.0, 1.0), Vec2(3.0, 1.0)),
	Line2(Vec2(1.0, 3.0), Vec2(2.0, 3.0)),
	Line2(Vec2(2.0, 2.0), Vec2(2.0, 3.0)),
	Line2(Vec2(2.0, 2.0), Vec2(3.0, 2.0)),
	Line2(Vec2(2.0, 3.0), Vec2(4.0, 3.0)),
	Line2(Vec2(3.0, 1.0), Vec2(3.0, 2.0)),
	Line2(Vec2(3.0, 1.0), Vec2(5.0, 1.0)),
	Line2(Vec2(3.0, 2.0), Vec2(4.0, 2.0)),
	Line2(Vec2(4.0, 2.0), Vec2(4.0, 3.0)),
	Line2(Vec2(4.0, 3.0), Vec2(5.0, 3.0)),
	Line2(Vec2(5.0, 1.0), Vec2(5.0, 3.0)),


	//Line2(Vec2(1.0, 1.0, 1 ), Vec2(3.0, 1.0, 1 )),
	//Line2(Vec2(1.0, 1.0, 2 ), Vec2(1.0, 2.0, 2 )),
	//Line2(Vec2(1.0, 1.0, 3 ), Vec2(3.0, 2.0, 3 )),
	//Line2(Vec2(3.0, 1.0, 4 ), Vec2(3.0, 2.0, 4 )),
	//Line2(Vec2(3.0, 1.0, 5 ), Vec2(5.0, 1.0, 5 )),
	//Line2(Vec2(5.0, 1.0, 6 ), Vec2(5.0, 3.0, 6 )),
	//Line2(Vec2(5.0, 3.0, 7 ), Vec2(3.0, 2.0, 7 )),
	//Line2(Vec2(5.0, 3.0, 8 ), Vec2(4.0, 4.0, 8 )),
	//Line2(Vec2(4.0, 4.0, 9 ), Vec2(3.0, 4.0, 9 )),
	//Line2(Vec2(3.0, 4.0, 10), Vec2(3.0, 2.0, 10)),
	//Line2(Vec2(3.0, 4.0, 11), Vec2(1.0, 4.0, 11)),
	//Line2(Vec2(1.0, 4.0, 12), Vec2(3.0, 2.0, 12)),
	//Line2(Vec2(1.0, 4.0, 13), Vec2(1.0, 2.0, 13)),
	//Line2(Vec2(1.0, 2.0, 14), Vec2(3.0, 2.0, 14)),
	//Line2(Vec2(3.0, 1.0, 15), Vec2(5.0, 3.0, 15)),

	//Line2(Vec2(1.0, 1.0, 1) + Vec2(10), Vec2(3.0, 1.0, 1) + Vec2(10)),
	//Line2(Vec2(1.0, 1.0, 2) + Vec2(10), Vec2(1.0, 2.0, 2) + Vec2(10)),
	//Line2(Vec2(1.0, 1.0, 3) + Vec2(10) , Vec2(3.0, 2.0, 3) + Vec2(10)),
	//Line2(Vec2(3.0, 1.0, 4) + Vec2(10) , Vec2(3.0, 2.0, 4) + Vec2(10)),
	//Line2(Vec2(3.0, 1.0, 5) + Vec2(10) , Vec2(5.0, 1.0, 5) + Vec2(10)),
	//Line2(Vec2(5.0, 1.0, 6) + Vec2(10) , Vec2(5.0, 3.0, 6) + Vec2(10)),
	//Line2(Vec2(5.0, 3.0, 7) + Vec2(10) , Vec2(3.0, 2.0, 7) + Vec2(10)),
	//Line2(Vec2(5.0, 3.0, 8) + Vec2(10) , Vec2(4.0, 4.0, 8) + Vec2(10)),
	//Line2(Vec2(4.0, 4.0, 9) + Vec2(10) , Vec2(3.0, 4.0, 9) + Vec2(10)),
	//Line2(Vec2(3.0, 4.0, 10) + Vec2(10), Vec2(3.0, 2.0, 10) + Vec2(10)),
	//Line2(Vec2(3.0, 4.0, 11) + Vec2(10), Vec2(1.0, 4.0, 11) + Vec2(10)),
	//Line2(Vec2(1.0, 4.0, 12) + Vec2(10), Vec2(3.0, 2.0, 12) + Vec2(10)),
	//Line2(Vec2(1.0, 4.0, 13) + Vec2(10), Vec2(1.0, 2.0, 13) + Vec2(10)),
	//Line2(Vec2(1.0, 2.0, 14) + Vec2(10), Vec2(3.0, 2.0, 14) + Vec2(10)),
	//Line2(Vec2(3.0, 1.0, 15) + Vec2(10), Vec2(5.0, 3.0, 15) + Vec2(10)),

	//Line2(Vec2(1.0, 1.0, 1) + Vec2(0, 10), Vec2(5.0, 1.0, 1) + Vec2(0, 10)),
	//Line2(Vec2(5.0, 1.0, 1) + Vec2(0, 10), Vec2(5.0, 3.0, 1) + Vec2(0, 10)),
	//Line2(Vec2(5.0, 3.0, 1) + Vec2(0, 10), Vec2(4.0, 4.0, 1) + Vec2(0, 10)),
	//Line2(Vec2(4.0, 4.0, 1) + Vec2(0, 10), Vec2(1.0, 4.0, 1) + Vec2(0, 10)),
	//Line2(Vec2(1.0, 4.0, 1) + Vec2(0, 10), Vec2(1.0, 1.0, 1) + Vec2(0, 10)),
	//Line2(Vec2(1.0, 1.0, 1) + Vec2(0, 10), Vec2(5.0, 3.0, 1) + Vec2(0, 10)),
	//Line2(Vec2(3.0, 1.0, 1) + Vec2(0, 10), Vec2(5.0, 3.0, 1) + Vec2(0, 10)),
	//Line2(Vec2(1.0, 2.0, 1) + Vec2(0, 10), Vec2(3.0, 2.0, 1) + Vec2(0, 10)),
	//Line2(Vec2(1.0, 4.0, 1) + Vec2(0, 10), Vec2(3.0, 2.0, 1) + Vec2(0, 10)),
	//Line2(Vec2(3.0, 1.0, 1) + Vec2(0, 10), Vec2(3.0, 4.0, 1) + Vec2(0, 10)),


	//Line2(Vec2(1.0, 1.0), Vec2(2.0, 1.0)),
	//Line2(Vec2(2.0, 1.0), Vec2(5.0, 1.0)),
	//Line2(Vec2(5.0, 1.0), Vec2(5.0, 4.0)),
	//Line2(Vec2(5.0, 4.0), Vec2(5.0, 5.0)),
	//Line2(Vec2(5.0, 5.0), Vec2(4.0, 5.0)),
	//Line2(Vec2(4.0, 5.0), Vec2(1.0, 5.0)),
	//Line2(Vec2(1.0, 5.0), Vec2(1.0, 2.0)),
	//Line2(Vec2(1.0, 2.0), Vec2(1.0, 1.0)),
	//Line2(Vec2(2.0, 1.0), Vec2(5.0, 4.0)),
	//Line2(Vec2(1.0, 2.0), Vec2(4.0, 5.0)),


	//Line2(Vec2(1.0, 1.0), Vec2(2.0, 1.0)),
	//Line2(Vec2(2.0, 1.0), Vec2(2.0, 1.0)),
	//Line2(Vec2(2.0, 1.0), Vec2(3.0, 1.0)),
	//Line2(Vec2(3.0, 1.0), Vec2(4.0, 1.0)),
	//Line2(Vec2(4.0, 1.0), Vec2(5.0, 1.0)),
	//Line2(Vec2(5.0, 1.0), Vec2(5.0, 5.0)),
	//Line2(Vec2(5.0, 5.0), Vec2(1.0, 5.0)),
	//Line2(Vec2(1.0, 5.0), Vec2(1.0, 4.0)),
	//Line2(Vec2(1.0, 4.0), Vec2(1.0, 3.0)),
	//Line2(Vec2(1.0, 3.0), Vec2(1.0, 2.0)),
	//Line2(Vec2(1.0, 2.0), Vec2(1.0, 1.0)),
	//Line2(Vec2(1.0, 1.0), Vec2(5.0, 5.0)),
	//Line2(Vec2(2.0, 1.0), Vec2(5.0, 5.0)),
	//Line2(Vec2(3.0, 1.0), Vec2(5.0, 5.0)),
	//Line2(Vec2(4.0, 1.0), Vec2(5.0, 5.0)),
	//Line2(Vec2(1.0, 4.0), Vec2(5.0, 5.0)),
	//Line2(Vec2(1.0, 3.0), Vec2(5.0, 5.0)),
	//Line2(Vec2(1.0, 2.0), Vec2(5.0, 5.0)),


	//Line2(Vec2(0.0,		0.0),		Vec2(6000.0,	0.0)),
	//Line2(Vec2(6000.0,	0.0),		Vec2(24000.0,	0.0)),
	//Line2(Vec2(24000.0, 0.0),		Vec2(24000.0,	6000.0)),
	//Line2(Vec2(24000.0, 6000.0),	Vec2(18000.0,	11500.0)),
	//Line2(Vec2(18000.0, 11500.0),	Vec2(6000.0,	11500.0)),
	//Line2(Vec2(6000.0,	11500.0),	Vec2(0.0,		11500.0)),
	//Line2(Vec2(0.0,		11500.0),	Vec2(0.0,		6000.0)),
	//Line2(Vec2(0.0,		6000.0),	Vec2(0.0,		0.0)),
	//Line2(Vec2(6000.0,	0.0),		Vec2(6000.0,	6000.0)),
	//Line2(Vec2(6000.0,	6000.0),	Vec2(6000.0,	11500.0)),
	//Line2(Vec2(6000.0,	6000.0),	Vec2(0.0,		6000.0)),

	//Line2(Vec2(1.0, 1.0), Vec2(5.0, 1.0)),
	//Line2(Vec2(5.0, 1.0), Vec2(5.0, 5.0)),
	//Line2(Vec2(5.0, 5.0), Vec2(1.0, 5.0)),
	//Line2(Vec2(1.0, 5.0), Vec2(1.0, 1.0)),
	//Line2(Vec2(2.0, 1.0), Vec2(5.0, 4.0)),
	//Line2(Vec2(1.0, 2.0), Vec2(4.0, 5.0)),

});

void test()
{
#if DEBUG_TIMER
	clock_t t1;
	clock_t t2;
#endif

#if DEBUG_TIMER
	t1 = clock();
#endif


	segs = SimpleSplitSegs::splitSegs(segs);


#if DEBUG_TIMER
	t2 = clock();
	std::cout << "SimpleSplitSegs::splitSegs	" << t2 - t1 << "ms" << std::endl;
#endif

#if DEBUG_LOG
	std::cout << std::fixed;

	for (size_t i = 0; i < segs.size(); i++)
		segs[i].debug(0, 0, true);

	for (size_t i = 0; i < segs.size(); i++)
	{
		segs[i].s *= 1;
		segs[i].e *= 1;
	}

#endif

#if DEBUG_TIMER
	t1 = clock();
#endif


	std::vector<std::vector<Cell>> cells = Segs2Polys::createGrid(segs);


#if DEBUG_TIMER
	t2 = clock();
	std::cout << "Segs2Polys::createGrid		" << t2 - t1 << "ms" << std::endl;
#endif

#if DEBUG_LOG
	std::cout << std::endl << "cells" << std::endl;
	std::cout << "[" << std::endl;
	for (const std::vector<Cell>& c1 : cells)
	{
		std::cout << "    [" << std::endl;
		for (const Cell& c2 : c1)
		{
			std::cout << "        {" << std::endl;
			std::cout << "            \"swh\":" << "(" << c2.swh.x << ", " << c2.swh.y << ")," << std::endl;
			std::cout << "            \"ewh\":" << "(" << c2.ewh.x << ", " << c2.ewh.y << ")," << std::endl;
			std::cout << "            \"shape\":{" << std::endl;
			std::cout << "                \"hull\":[" << std::endl;
			for (size_t i = 0; i < c2.shape.hull.size(); i++)
			std::cout << "                    (" << c2.shape.hull[i].x << ", " << c2.shape.hull[i].y << ")," << std::endl;
			std::cout << "                ]," << std::endl;
			std::cout << "                \"lines\":{" << std::endl;
			for (size_t i = 0; i < c2.shape.lines.size(); i++)
			std::cout << "                    \"" << i << "\":((" << c2.shape.lines[i].s.x << ", " << c2.shape.lines[i].s.y << "),(" << c2.shape.lines[i].e.x << ", " << c2.shape.lines[i].e.y << "))," << std::endl;
			std::cout << "                }," << std::endl;
			std::cout << "                \"rays, sp\":{" << std::endl;
			for (size_t i = 0; i < c2.shape.rays.size(); i++)
			{
				long e = 10000;
				long long xx = c2.shape.rays[i].x * e;
				long long yy = c2.shape.rays[i].y * e;
				double x = (double)xx / e;
				double y = (double)yy / e;

				//double x = c2.shape.rays[i].x;
				//double y = c2.shape.rays[i].y;

			//std::cout << "                    (" << x << ", " << y << "):[";
			std::cout << "                    (" << std::to_string(x) << ", " << std::to_string(y) << "):[";
				for (size_t j = 0; j < c2.shape.sp[i].size(); j++)
					std::cout << c2.shape.sp[i][j] << ",";
				std::cout << "]," << std::endl;
			}
			std::cout << "                }," << std::endl;
			std::cout << "            }," << std::endl;
			std::cout << "        }," << std::endl;
		}
		std::cout << "    ]," << std::endl;
	}
	std::cout << "]" << std::endl;
	std::cout << std::endl << std::endl;
#endif

#if DEBUG_TIMER
	t1 = clock();
#endif


	std::vector<stCellLink> links = Segs2Polys::getLinkRaycast(cells);


#if DEBUG_TIMER
	t2 = clock();
	std::cout << "Segs2Polys::getLinkRaycast	" << t2 - t1 << "ms" << std::endl;
#endif

#if DEBUG_LOG
	std::cout << "links" << std::endl;
	for (stCellLink& r1 : links)
		r1.debug();
	std::cout << std::endl << std::endl;

	std::cout << "links-parse" << std::endl;
	std::cout << "[" << std::endl;
	for (stCellLink& r1 : links)
	{
		std::cout << "    (";
		cells[r1.cell11][r1.cell12].shape.rays[r1.cellLink1].debug(0, false);
		std::cout << ", ";
		cells[r1.cell21][r1.cell22].shape.rays[r1.cellLink2].debug(0, false);
		std::cout << "),";
		std::cout << std::endl;
	}
	std::cout << "]" << std::endl << std::endl;

#endif

#if DEBUG_TIMER
	t1 = clock();
#endif


	std::vector<std::vector<Vec2>> polys = Segs2Polys::trailCells(cells, links);


#if DEBUG_TIMER
	t2 = clock();
	std::cout << "Segs2Polys::trailCells		" << t2 - t1 << "ms" << std::endl;
#endif

#if DEBUG_LOG
	std::cout << "polys" << std::endl;
	std::cout << "[" << std::endl;
	for (size_t i = 0; i < polys.size(); i++)
	{
		std::cout << "    (";
		for (Vec2& r1 : polys[i])
		{
			r1.debug(0, false);
			std::cout << ", ";
		}
		std::cout << ")," << std::endl;
	}
	std::cout << "]" << std::endl << std::endl;
#endif

#if DEBUG_TIMER
	t1 = clock();
#endif


	polys = Segs2Polys::mergePolysLines(polys);


#if DEBUG_TIMER
	t2 = clock();
	std::cout << "Segs2Polys::mergePolysLines	" << t2 - t1 << "ms" << std::endl;
#endif

#if DEBUG_LOG
	std::cout << "polys" << std::endl;
	std::cout << "[" << std::endl;
	for (size_t i = 0; i < polys.size(); i++)
	{
		std::cout << "    (";
		for (Vec2& r1 : polys[i])
		{
			r1.debug(0, false);
			std::cout << ", ";
		}
		std::cout << ")," << std::endl;
	}
	std::cout << "]" << std::endl << std::endl;
#endif
}

void test1()
{
#if DEBUG_TIMER_TOTAL
	clock_t t1;
	clock_t t2;
#endif

#if DEBUG_TIMER_TOTAL
	t1 = clock();
#endif

	int n = 100000;
	for (int i = 0; i < n; i++)
		test();

#if DEBUG_TIMER_TOTAL
	t2 = clock();
	std::cout << "total							" << t2 - t1 << "ms" << std::endl;
	std::cout << "lines\t" << segs.size() * n << std::endl;
#endif
}

int main(int argc, char* argv[])
{
	{
		Clear();

		std::vector<Vec2> hull = std::vector<Vec2>({
			Vec2(10000, 10000),
			Vec2(34000, 10000),
			Vec2(34000, 16000),
			Vec2(28000, 21500),
			Vec2(10000, 21500),
			});

		std::vector<std::vector<Vec2>> holes = std::vector<std::vector<Vec2>>({
			{
				Vec2(14000,12000),
				Vec2(28000,12000),
				Vec2(28000,18000),
				Vec2(24000,18000),
				Vec2(22000,14000),
				Vec2(18000,14000),
				Vec2(16000,18000),

			},
		});

		Clear();
		AddPoly();
		for (size_t i = 0; i < hull.size(); i++)
			AddPolyVec(0, hull[i].x, hull[i].y, 0);

		size_t holeidxno = holes.size();
		long holeidx[1];
		for (size_t i = 0; i < holes.size(); i++)
		{
			AddPoly();
			holeidx[i] = i + 1;
			for (size_t j = 0; j < holes[i].size(); j++)
				AddPolyVec(i + 1, holes[i][j].x, holes[i][j].y, i + 1);
		}

		std::cout << "CalcEarClipHoles\t" << CalcEarClipHoles(0, holeidxno, &(holeidx[0]), 5000) << std::endl;
		size_t pns = holes.size() + 1;
		size_t pne = GetPolyCnt() - pns;
		size_t s;
		double x1, y1, x2, y2;
		long id1, id2;
		size_t jj;
		
		std::cout << fmod(11, 10) << std::endl;
		std::cout << fmod(-1, 10) << std::endl;


		std::cout << "pns:" << pns << std::endl;
		std::cout << "pne:" << pne << std::endl;
		std::cout << "dif:" << pne - pns << std::endl;
		std::cout << "GetPolyCnt:" << GetPolyCnt() << std::endl;
		for (size_t i = pns; i < pne; i++)
		{
			s = GetPolyVecCnt(i);
			std::cout << s << std::endl;
			for (size_t j = 0; j < s; j++)
			{
				jj = fmod(j + 1, s);
				std::cout << j << "\t" << jj << "\t" << s << std::endl;
				GetPolyVec(i, j, &x1, &y1, &id1);
				GetPolyVec(i, jj, &x2, &y2, &id2);
				std::cout << "("
					<< "(" << x1 << "," << y1 << "),"
					<< "(" << x2 << "," << y2 << ")"
					<< ")," << std::endl;
			}
			std::cout << std::endl;
		}
	}

	return 0;

	{
		std::vector<Vec2> hull = std::vector<Vec2>({
			Vec2(10000, 10000),
			Vec2(34000, 10000),
			Vec2(34000, 16000),
			Vec2(28000, 21500),
			Vec2(10000, 21500),
		});

		std::vector<std::vector<Vec2>> holes = std::vector<std::vector<Vec2>>({
			{
				Vec2(14000,12000),
				Vec2(28000,12000),
				Vec2(28000,18000),
				Vec2(24000,18000),
				Vec2(22000,14000),
				Vec2(18000,14000),
				Vec2(16000,18000),

			},
		});

		size_t ii;
		for (size_t i = 0; i < hull.size(); i++)
		{
			ii = Mathf::repeat(i + 1, hull.size());
			std::cout << "("
				<< "(" << hull[i].x << "," << hull[i].y << "),"
				<< "(" << hull[ii].x << "," << hull[ii].y << ")"
				<< ")," << std::endl;
		}
		
		std::cout << "-------------------------------------------" << std::endl;

		size_t jj;
		for (size_t i = 0; i < holes.size(); i++)
		{
			for (size_t j = 0; j < holes[i].size(); j++)
			{
				jj = Mathf::repeat(j + 1, holes[i].size());
				std::cout << "("
					<< "(" << holes[i][j].x << "," << holes[i][j].y << "),"
					<< "(" << holes[i][jj].x << "," << holes[i][jj].y << ")"
					<< ")," << std::endl;
			}
			std::cout << std::endl;
		}
		
		std::cout << "-------------------------------------------" << std::endl;

		std::vector<std::vector<Vec2>> ret;

		EarClipping::EarClipHoles(hull, holes, ret, 5000);
		for (size_t i = 0; i < ret.size(); i++)
		{
			for (size_t j = 0; j < ret[i].size(); j++)
			{
				jj = Mathf::repeat(j + 1, ret[i].size());
				std::cout << "("
					<< "(" << ret[i][j].x << "," << ret[i][j].y << "),"
					<< "(" << ret[i][jj].x << "," << ret[i][jj].y << ")"
					<< ")," << std::endl;
			}
			std::cout << std::endl;
		}
	}
	return 0;

	////{
	////	std::cout << "DBL_MAX\t" << DBL_MAX << std::endl;
	////	std::cout << "infinity\t" << std::numeric_limits<double>::infinity() << std::endl;
	////	std::cout << true << "\t" << (DBL_MAX == std::numeric_limits<double>::infinity()) << std::endl;
	////}

	//return 0;

	{
		std::vector<Vec2> vs = std::vector<Vec2>({
			Vec2(10000, 10000),
			Vec2(34000, 10000),
			Vec2(34000, 16000),
			Vec2(28000, 21500),
			Vec2(10000, 21500),
		});
		
		double x;
		double y;
		long id;

		Clear();
		AddPoly();
		for (size_t i = 0; i < vs.size(); i++)
			AddPolyVec(0, vs[i].x, vs[i].y, 0);
		std::cout << "b:" << gPolys.size() << std::endl;
		CalcEarClip(0, 50);
		std::cout << "a:" << gPolys.size() << std::endl;
		std::cout << "aa:" << GetPolyCnt() << std::endl;
		for (size_t i = 1; i < GetPolyCnt(); i++)
		{
			std::cout << "aav:" << GetPolyVecCnt(i) << std::endl;
			for (size_t j = 0; j < GetPolyVecCnt(i); j++)
			{
				GetPolyVec(i, j, &x, &y, &id);
				std::cout << "(" << x << "," << y << ")\t" << id << std::endl;
			}
			std::cout << std::endl;
		}
	}

	return 0;

	{
		std::vector<Vec2> vs = std::vector<Vec2>({ 
			Vec2(10000, 10000),
			Vec2(34000, 10000),
			Vec2(34000, 16000),
			Vec2(28000, 21500),
			Vec2(10000, 21500),
		});

		std::vector<std::vector<Vec2>> ret;

		std::cout << EarClipping::EarClip(vs, ret, 1000) << std::endl;
		for (size_t i = 0; i < ret.size(); i++)
		{
			for (size_t j = 0; j < ret.size(); j++)
				ret[i][j].debug();
			std::cout << std::endl;
		}
	}

	return 0;

	{
		std::vector<Vec2> vs;
		std::vector<std::vector<Vec2>> ret;

		std::vector<std::vector<Cell>> cells = Segs2Polys::createGrid(segs);
		std::vector<stCellLink> links = Segs2Polys::getLinkRaycast(cells);
		std::vector<std::vector<Vec2>> polys = Segs2Polys::trailCells(cells, links);
		vs = polys[0];
		for (size_t i = 0; i < vs.size(); i++)
			vs[i].debug();

		std::cout << "true:" << true << "\t" << EarClipping::EarClip(vs, ret) << std::endl;
		for (size_t i = 0; i < ret.size(); i++)
		{
			for (size_t j = 0; j < ret[i].size(); j++)
				ret[i][j].debug();
			std::cout << std::endl;
		}

	}

	return 0;

	{
		std::cout << Mathf::repeat(1, (size_t)0) << std::endl;
	}

	return 0;


	{
		double r = 5;
		std::vector<Line2> ls;

		//ls = Segs2Polys::arc2segs(Vec2(0.2, 0.2), Vec2(1, 0), Vec2(1, 1), 0, -r);
		//for (auto l : ls) { l.debug(0, 0, false);	std::cout << "," << std::endl; }
		//std::cout << std::endl;

		//ls = Segs2Polys::arc2segs(Vec2(0.7, 0.7), Vec2(0, 1), Vec2(0, 0), 0, -r);
		//for (auto l : ls) { l.debug(0, 0, false);	std::cout << "," << std::endl; }
		//std::cout << std::endl;
		//
		//ls = Segs2Polys::arc2segs(Vec2(1, 1), Vec2(3, 1), Vec2(2, 2), 0, r);
		//for (auto l : ls) { l.debug(0, 0, false);	std::cout << "," << std::endl; }
		//std::cout << std::endl;
		
		//ls = Segs2Polys::arc2segs(Vec2(1, 1), Vec2(3, 1), Vec2(2, 4), 0, r);
		//for (auto l : ls) { l.debug(0, 0, false);	std::cout << "," << std::endl; }
		//std::cout << std::endl;
		//
		//ls = Segs2Polys::arc2segs(Vec2(1, 1), Vec2(3, 1), Vec2(2, 10), 0, r);
		//for (auto l : ls) { l.debug(0, 0, false);	std::cout << "," << std::endl; }
		//std::cout << std::endl;
		//
		//ls = Segs2Polys::arc2segs(Vec2(3, 2), Vec2(2, 0.6), Vec2(1.5, 2), 0, r);
		//for (auto l : ls) { l.debug(0, 0, false);	std::cout << "," << std::endl; }
		//std::cout << std::endl;
		//
		//ls = Segs2Polys::arc2segs(Vec2(2 + 1, 0.6), Vec2(3 + 1, 2), Vec2(1.5 + 1, 2), 0, r);
		//for (auto l : ls) { l.debug(0, 0, false);	std::cout << "," << std::endl; }
		//std::cout << std::endl;
		//
		//ls = Segs2Polys::arc2segs(Vec2(1, 3), Vec2(3, 3), Vec2(2, 20), 0, r);
		//for (auto l : ls) { l.debug(0, 0, false);	std::cout << "," << std::endl; }
		//std::cout << std::endl;

		//ls = Segs2Polys::arc2segs(Vec2(-1, 1), Vec2(1, 1), Vec2(0, 0), 0, r);
		//for (auto l : ls) { l.debug(0, 0, false);	std::cout << "," << std::endl; }
		//std::cout << std::endl;

		//ls = Segs2Polys::arc2segs(Vec2(-1, 1), Vec2(1, 1), Vec2(0, 0), 0, -r);
		//for (auto l : ls) { l.debug(0, 0, false);	std::cout << "," << std::endl; }
		//std::cout << std::endl;

		//ls = Segs2Polys::arc2segs(Vec2(1, 1), Vec2(-1, 1), Vec2(0, 0), 0, r);
		//for (auto l : ls) { l.debug(0, 0, false);	std::cout << "," << std::endl; }
		//std::cout << std::endl;

		//ls = Segs2Polys::arc2segs(Vec2(1, 1), Vec2(-1, 1), Vec2(0, 0), 0, -r);
		//for (auto l : ls) { l.debug(0, 0, false);	std::cout << "," << std::endl; }
		//std::cout << std::endl;

		//ls = Segs2Polys::arc2segs(Vec2(1, 1), Vec2(-1, 1), Vec2(0, 0), 0, -r);
		//for (auto l : ls) { l.debug(0, 0, false);	std::cout << "," << std::endl; }
		//std::cout << std::endl;

		bool ret;
		size_t en;
		Vec2 s;
		Vec2 e;
		Vec2 c;
		long id;

		std::vector<Vec2> vs;

		ls.emplace_back(Line2(Vec2(1, 0), Vec2(1, 1), false));
		for (auto l : Segs2Polys::arc2segs(Vec2(1, 1), Vec2(-1, 1), Vec2(0, 0), 100, -r))
			ls.emplace_back(l);
		ls.emplace_back(Line2(Vec2(-1, 1), Vec2(-1, 0), false));
		//for (auto l : ls) { l.debug(0, 0, false);	std::cout << "," << std::endl; }
		//ret = Segs2Polys::segs2arc(ls, 1, -r, 1, 3, &en, &s, &e, &c, &id);
		std::cout << "ls  : " << ls.size();
		for (auto l : ls)
			vs.emplace_back(l.s);
		vs.emplace_back(ls.back().e);
		for (auto v : vs) { v.debug(0, false);	std::cout << "," << std::endl; }

		std::cout << "vs  : " << vs.size();

		ret = Segs2Polys::poly2arc(vs, 1, -r, 1, 3, &en, &s, &e, &c, &id);

		std::cout << std::endl;
		std::cout << "true: " << true << std::endl;
		std::cout << "ret : " << ret << std::endl;
		std::cout << "en  : " << en << std::endl;
		std::cout << "s   : ";
		s.debug();
		std::cout << "e   : ";
		e.debug();
		std::cout << "c   : ";
		c.debug();
		std::cout << "id  : " << id << std::endl;
		std::cout << std::endl;

		ls = Segs2Polys::arc2segs(s, e, Vec2(0, 0), 0, -r);
		for (auto l : ls) { l.debug(0, 0, false);	std::cout << "," << std::endl; }
	}
	return 0;

	{
		Line2 l1 = Line2(Vec2(1, 3), Vec2(3, 3));
		Line2 l2 = Line2(Vec2(2, 1), Vec2(3, 2));
		std::cout << Line2::isCrossInf(l1, l2) << std::endl;
		Vec2 c = Line2::getCross(l1, l2);
		c.debug();
		std::cout << l1.distancePointLine(c) << std::endl;

		std::vector<Line2> l = std::vector<Line2>({
			Line2(Vec2(1, 3), Vec2(3, 3)),
			Line2(Vec2(2, 1), Vec2(3, 2))
		});

		l = Segs2Polys::extendMarginIntersection(l, 1);
		std::cout << l.size() << std::endl;

		for (size_t i = 0; i < l.size(); i++)
			l[i].debug();
	}

	return 0;

	{
		test();
	}

	return 0;
	{
		if (argc != 2 + 1)
			return 1;

		std::cout << segs[0].ToString() << std::endl;

		printf("rfile\t%s\n", argv[1]);
		printf("wfile\t%s\n\n", argv[2]);

		std::string rfile = argv[1];
		std::string wfile = argv[2];

		std::vector<std::string> r = Csv2Line2::readfile(rfile);
		for (std::string s : r)
			std::cout << s << std::endl;
	}
	return 0;
}
#endif
