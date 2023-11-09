#pragma once

#include <stdint.h>
#include <vector>
#include <cmath>
#include <algorithm>
#include <unordered_set>

// my
#include "Vec2.hpp"
#include "Line2.hpp"
#include "EarClipping.hpp"
#include "Vec2Util.hpp"

#if DEBUG_LOG
#include <iostream>
#endif

class TriangleUtil : public EarClipping
{
public:
	TriangleUtil() {}

    static bool SplitTrisSegs(std::vector<std::vector<Vec2>> tris, std::vector<Line2>& segs, std::vector<std::vector<Vec2>>& result)
    {
        bool o = true;
        result.clear();
		for (size_t i = 0; i < segs.size(); i++)
			o &= SplitTrisSeg(tris, segs[i], tris);
        result = tris;
        return o;
    }

    static bool SplitTrisSeg(std::vector<std::vector<Vec2>> tris, Line2& seg, std::vector<std::vector<Vec2>>& result)
    {
        bool o = true;
        std::vector<std::vector<Vec2>> r;
        result.clear();
		for (size_t i = 0; i < tris.size(); i++)
		{
			o &= SplitTriSeg(tris[i], seg, r);
            for (size_t j = 0; j < r.size(); j++)
                result.emplace_back(r[j]);
		}
        return o;
    }

    static bool SplitTriSeg(std::vector<Vec2>& tri, Line2& seg, std::vector<std::vector<Vec2>>& result)
    {
        std::vector<Vec2> trailer;
        std::vector<size_t> interIdx;
        int_fast32_t inter = 0;

        result.clear();

        {
            Line2 line;
            Vec2 v;
            Vec2 c;
            size_t ii;
            
            for (size_t i = 0; i < tri.size(); i++)
            {
                ii = fmod(i + 1, tri.size());
                line = Line2(tri[i], tri[ii], false);
                v = Vec2(tri[i].x, tri[i].y, tri[i].id, 0);
                if (seg.isCross(line))
                {
                    c = seg.getCross(line);
                    inter++;
                    c.w = inter;
                    if (c != v)
                        trailer.emplace_back(v);
                    interIdx.emplace_back(trailer.size());
                    trailer.emplace_back(c);
                }
                else
                    trailer.emplace_back(v);
            }
        }

        if (inter == 0)
        {
            result.emplace_back(tri);
            return true;
        }
        
        if (isPointInTriangle(tri[0], tri[1], tri[2], seg.s))
        {
            trailer.insert(trailer.begin() + interIdx[0] + 1, {seg.s, trailer[interIdx[0]]});
            EarClip(trailer, result);
            return true;
        }
        if (isPointInTriangle(tri[0], tri[1], tri[2], seg.e))
        {
            trailer.insert(trailer.begin() + interIdx[0] + 1, {seg.e, trailer[interIdx[0]]});
            EarClip(trailer, result);
            return true;
        }

        {
            std::vector<std::vector<Vec2>> r;
            std::vector<std::vector<Vec2>> t;

            trailVec(trailer, interIdx, r);
            for (size_t i = 0; i < r.size(); i++)
            {
                if (r[i].size() == 3)
                    result.emplace_back(r[i]);
                else
                {
                    if (EarClip(r[i], t))
                        for (size_t j = 0; j < t.size(); j++)
                            result.emplace_back(t[j]);
                }
            }
            return true;
        }

        return false;
    }

    static std::vector<std::vector<Vec2>> Tri2Poly(std::vector<std::vector<Vec2>>& tris)
    {
        std::vector<std::vector<Vec2>> o;

        std::vector<Line2> segs;
        std::vector<Line2> segw;

        std::vector<Line2> w;
		std::vector<bool> visited;
        std::vector<Vec2> poly;

        bool f;
        size_t cur;
        size_t ii;

        for (size_t i = 0; i < tris.size(); i++)
            for (size_t j = 0; j < tris[i].size(); j++)
                segw.emplace_back(Line2(tris[i][j],
                                        tris[i][fmod(j + 1, tris[i].size())]));

        for (size_t i = 0; i < segw.size(); i++)
        {
            f = false;
            for (size_t j = 0; j < segw.size(); j++)
            {
                if (i == j)
                    continue;
                if (segw[i] == segw[j])
                {
                    f = true;
                    break;
                }
            }
            if (!f)
            {
                segw[i].w = segs.size();
                segs.emplace_back(segw[i]);
            }
        }

        visited = std::vector<bool>(segs.size(), false);
        while (true)
        {
			cur = -1;
			for (size_t i = 0; i < visited.size(); i++)
				if (!visited[i])
				{
					cur = i;
					break;
				}
			if (cur == -1)
				break;
            
            w.clear();
            w.emplace_back(segs[cur]);
            visited[cur] = true;
            while (true)
            {
                f = false;
                for (size_t i = 0; i < visited.size(); i++)
                {
                    if (visited[i])
                        continue;

                    if (w.back() |= segs[i])
                    {
                        f = true;
                        visited[i] = true;
                        w.emplace_back(segs[i]);
                    }
                }
                if (!f)
                    break;
                if (w.front() |= w.back())
                    break;
            }

            poly.clear();
            for (size_t i = 0; i < w.size(); i++)
            {
                ii = fmod(i + 1, w.size());
                if (w[i].s == w[ii].s)
                    poly.emplace_back(w[i].s);
                else if (w[i].s == w[ii].e)
                    poly.emplace_back(w[i].s);
                else if (w[i].e == w[ii].s)
                    poly.emplace_back(w[i].e);
                else if (w[i].e == w[ii].e)
                    poly.emplace_back(w[i].e);
            }
            if (poly.size() < 3)
                continue;

            o.emplace_back(poly);
        }

        return o;
    }

    static std::vector<std::vector<Vec2>> MergeTrisWithSegsAll(std::vector<std::vector<Vec2>> tris, std::vector<Line2>& segs)
    {
        size_t s;
        do
        {
            s = tris.size();
            tris = MergeTrisWithSegsOne(tris, segs);
        }
        while (s != tris.size());
        return tris;
    }

    static std::vector<std::vector<Vec2>> MergeTrisWithSegsOne(std::vector<std::vector<Vec2>> tris, std::vector<Line2>& segs)
    {
        std::vector<std::vector<Vec2>> o;
        std::unordered_set<size_t> mergedIdx;
        size_t lap1;
        size_t lap2;
        size_t t1;
        size_t t2;

        for (size_t i = 0; i < tris.size(); i++)
            Vec2Util::directionRotatePoly(tris[i]);

        for (size_t i = 0; i < tris.size(); i++)
        {
            if (mergedIdx.find(i) != mergedIdx.end())
                continue;
            for (size_t j = 0; j < tris.size(); j++)
            {
                if (i == j)
                    continue;
                if (mergedIdx.find(j) != mergedIdx.end())
                    continue;

                if (!isTriMerge(tris[i], tris[j], segs, lap1, lap2, t1, t2))
                    continue;

// std::cout << "# tri - 1 - " << i << std::endl;
// for (size_t k = 0; k < tris[i].size(); k++)
//     std::cout << tris[i][k].ToString() << std::endl;
// std::cout << "# tri - 2 - " << j << std::endl;
// for (size_t k = 0; k < tris[j].size(); k++)
//     std::cout << tris[j][k].ToString() << std::endl;

                o.emplace_back(MergeTri2(tris[i], tris[j], lap1, lap2, t1, t2));

// std::cout << "lap1 : " << lap1 << std::endl;
// std::cout << "lap2 : " << lap2 << std::endl;
// std::cout << "t1 :   " << t1 << std::endl;
// std::cout << "t2 :   " << t2 << std::endl;
// std::cout << "# tri - 3 - " << o.size() - 1 << std::endl;
// for (size_t k = 0; k < o.back().size(); k++)
//     std::cout << o.back()[k].ToString() << std::endl;
// std::cout << std::endl;

                mergedIdx.insert(i);
                mergedIdx.insert(j);
            }
        }

        // for (size_t i = 0; i < tris.size(); i++)
        // {
        //     if (mergedIdx.find(i) != mergedIdx.end())
        //         continue;
        //     o.emplace_back(tris[i]);
        // }

        return o;
    }

	// static bool isPointInTriangle(Vec2& a, Vec2& b, Vec2& c, Vec2& p, const bool isInclude = false )
	// {
	// 	double c1;
	// 	double c2;
	// 	double c3;
	// 	c1 = Vec2::cross(a - b, b - p);
	// 	c2 = Vec2::cross(b - c, c - p);
	// 	c3 = Vec2::cross(c - a, a - p);
    //     if (!isInclude)
	// 	    return (c1 > 0 && c2 > 0 && c3 > 0) || (c1 < 0 && c2 < 0 && c3 < 0);
	// 	else
    //         return (c1 >= 0 && c2 >= 0 && c3 >= 0) || (c1 <= 0 && c2 <= 0 && c3 <= 0);
	// }

    // static bool TriOnPoint(std::vector<Vec2>& tri, Vec2& p, bool incBorder = false)
    // {
    //     bool b1;
    //     bool b2;
    //     bool b3;
    //     if (incBorder)
    //     {
    //         b1 = Vec2::cross3(p, tri[0], tri[1]) <= 0;
    //         b2 = Vec2::cross3(p, tri[1], tri[2]) <= 0;
    //         b3 = Vec2::cross3(p, tri[2], tri[0]) <= 0;
    //     }
    //     else
    //     {
    //         b1 = Vec2::cross3(p, tri[0], tri[1]) < 0;
    //         b2 = Vec2::cross3(p, tri[1], tri[2]) < 0;
    //         b3 = Vec2::cross3(p, tri[2], tri[0]) < 0;
    //     }
    //     return b1 == b2 && b2 == b3;
    // }

// private:
    // normalized tri
    static std::vector<Vec2> MergeTri2(std::vector<Vec2>& tri1, std::vector<Vec2>& tri2, size_t& lap1, size_t& lap2, size_t& t1, size_t& t2)
    {
        size_t tm = fmod(t1 + 2, tri1.size());
        size_t tp = fmod(t1 + 1, tri1.size());
        return std::vector<Vec2>({tri1[t1], tri2[t2], tm != lap1 ? tri1[tm] : tri1[tp]});
    }

    // normalized tri
    static bool isTriMerge(std::vector<Vec2>& tri1, std::vector<Vec2>& tri2, std::vector<Line2>& segs, size_t& lap1, size_t& lap2, size_t& t1, size_t& t2)
    {
        Line2 l;
        if (!GetTriOverlapSegIdx(tri1, tri2, lap1, lap2))
            return false;
        if (!GetTriTIdx(tri1, tri2, lap1, lap2, t1, t2))
            return false;
        l = Line2(tri1[lap1], tri1[fmod(lap1 + 1, tri1.size())]);
        for (size_t i = 0; i < segs.size(); i++)
            switch (Line2::isLineOverlap(l, segs[i]))
            {
                case 0:
                case 1:
                case 2:
                    return false;
            }
        return true;
    }

    // normalized tri
    static bool GetTriTIdx(std::vector<Vec2>& tri1, std::vector<Vec2>& tri2, size_t& lap1, size_t& lap2, size_t& t1, size_t& t2)
    {
// std::cout << std::endl;
        size_t pi11 = fmod(lap1 + 2, tri1.size());
        size_t pi12 = fmod(lap1 + 1, tri1.size());
        size_t pi21 = fmod(lap2 + 2, tri2.size());
        size_t pi22 = fmod(lap2 + 1, tri2.size());
        if (tri1[lap1] != tri2[pi22])
            return false;
        if (tri1[pi12] != tri2[lap2])
            return false;
// std::cout << "lap1 : " << tri1[lap1].ToString() << std::endl;
// std::cout << "pi11 : " << tri1[pi11].ToString() << std::endl;
// std::cout << "pi12 : " << tri1[pi12].ToString() << std::endl;
// std::cout << "lap2 : " << tri2[lap2].ToString() << std::endl;
// std::cout << "pi21 : " << tri2[pi21].ToString() << std::endl;
// std::cout << "pi22 : " << tri2[pi22].ToString() << std::endl;
// std::cout << tri1[pi11].ToString() << "\t" << tri2[pi21].ToString() << "\t" << tri1[lap1].ToString() << std::endl;
// std::cout << tri1[pi11].ToString() << "\t" << tri2[pi21].ToString() << "\t" << tri1[pi12].ToString() << std::endl;
// std::cout << "00\t" << std::to_string(Vec2::cross3(tri1[pi11], tri2[pi21], tri1[lap1])) << std::endl;
// std::cout << "\t" << tri1[pi11].ToString() << "\t" << tri2[pi21].ToString() << "\t" << tri1[lap1].ToString() << std::endl;
// std::cout << "01\t" << std::to_string(Vec2::cross3(tri1[pi11], tri2[pi21], tri2[lap2])) << std::endl;
// std::cout << "\t" << tri1[pi11].ToString() << "\t" << tri2[pi21].ToString() << "\t" << tri2[lap2].ToString() << std::endl;
        if (std::abs(Vec2::cross3(tri1[pi11], tri2[pi21], tri1[lap1])) < Vec2::kEpsilond)
        {
            t1 = pi11;
            t2 = pi21;
// std::cout << "00\t" << tri1[t1].ToString() << " - " << tri2[t2].ToString() << std::endl;
            return true;
        }
        if (std::abs(Vec2::cross3(tri1[pi11], tri2[pi21], tri2[lap2])) < Vec2::kEpsilond)
        {
            t1 = pi11;
            t2 = pi21;
// std::cout << "01\t" << tri1[t1].ToString() << " - " << tri2[t2].ToString() << std::endl;
            return true;
        }
        return false;
    }

    static bool GetTriOverlapSegIdx(std::vector<Vec2>& tri1, std::vector<Vec2>& tri2, size_t& lap1, size_t& lap2)
    {
        Line2 l;
        for (size_t i = 0; i < tri1.size(); i++)
        {
            l = Line2(tri1[i], tri1[fmod(i + 1, tri1.size())]);
            for (size_t j = 0; j < tri2.size(); j++)
                if (l == Line2(tri2[j], tri2[fmod(j + 1, tri2.size())]))
                {
                    lap1 = i;
                    lap2 = j;
                    return true;
                }                
        }
        return false;
    }

    static void trailVec(std::vector<Vec2>& trailer, std::vector<size_t>& interIdx, std::vector<std::vector<Vec2>>& result)
    {
        std::vector<Vec2> w;
        size_t jj;

        result.clear();
        for (size_t i = 0; i < interIdx.size(); i++)
        {
            w.clear();
            for (size_t j = 0; j < trailer.size(); j++)
            {
                jj = fmod(j + interIdx[i], trailer.size());
                w.emplace_back(trailer[jj]);
                if (trailer[jj].w > 0 && interIdx[i] != jj)
                    break;
            }
            if (w.front() != w.back() or w.front().w != w.back().w)
                result.emplace_back(w);
        }
    }
};
