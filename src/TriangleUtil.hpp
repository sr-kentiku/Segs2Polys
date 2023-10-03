#pragma once

#include <stdint.h>
#include <vector>
#include <cmath>
#include <algorithm>

// my
#include "Vec2.hpp"
#include "Line2.hpp"
#include "EarClipping.hpp"

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
			o &= TriangleUtil::SplitTrisSeg(tris, segs[i], tris);
        result = tris;
        return o;
    }

    static bool SplitTrisSeg(std::vector<std::vector<Vec2>> tris, Line2& seg, std::vector<std::vector<Vec2>>& result)
    {
        bool o = true;
        std::vector<std::vector<Vec2>> r = std::vector<std::vector<Vec2>>();

        result.clear();
		for (size_t i = 0; i < tris.size(); i++)
		{
			o &= TriangleUtil::SplitTriSeg(tris[i], seg, r);
            for (size_t j = 0; j < r.size(); j++)
                result.emplace_back(r[j]);
		}
        return o;
    }

    static bool SplitTriSeg(std::vector<Vec2>& tri, Line2& seg, std::vector<std::vector<Vec2>>& result)
    {
        std::vector<Vec2> trailer = std::vector<Vec2>();
        std::vector<size_t> interIdx = std::vector<size_t>();
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
            bool f;
            trailer.insert(trailer.begin() + interIdx[0] + 1, { seg.s, trailer[interIdx[0]] });
            EarClip(trailer, result);
            return true;
        }
        if (isPointInTriangle(tri[0], tri[1], tri[2], seg.e))
        {
            bool f;
            trailer.insert(trailer.begin() + interIdx[0] + 1, { seg.e, trailer[interIdx[0]] });
            EarClip(trailer, result);
            return true;
        }

        {
            std::vector<std::vector<Vec2>> r = std::vector<std::vector<Vec2>>();
            std::vector<std::vector<Vec2>> t = std::vector<std::vector<Vec2>>();

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
        std::vector<std::vector<Vec2>> o = std::vector<std::vector<Vec2>>();

        std::vector<Line2> segs = std::vector<Line2>();
        std::vector<Line2> segw = std::vector<Line2>();

        std::vector<Line2> w = std::vector<Line2>();
		std::vector<bool> visited;
        std::vector<Vec2> poly = std::vector<Vec2>();

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

private:
    static void trailVec(std::vector<Vec2>& trailer, std::vector<size_t>& interIdx, std::vector<std::vector<Vec2>>& result)
    {
        std::vector<Vec2> w = std::vector<Vec2>();
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
