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
        
        if (TriOnPoint(tri, seg.s))
        {
            bool f;
            trailer.insert(trailer.begin() + interIdx[0] + 1, { seg.s, trailer[interIdx[0]] });
            EarClip(trailer, result);
            return true;
        }
        if (TriOnPoint(tri, seg.e))
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

    static bool TriOnPoint(std::vector<Vec2>& tri, Vec2& p, bool incBorder = false)
    {
        bool b1;
        bool b2;
        bool b3;
        if (incBorder)
        {
            b1 = Vec2::cross3(p, tri[0], tri[1]) <= 0;
            b2 = Vec2::cross3(p, tri[1], tri[2]) <= 0;
            b3 = Vec2::cross3(p, tri[2], tri[0]) <= 0;
        }
        else
        {
            b1 = Vec2::cross3(p, tri[0], tri[1]) < 0;
            b2 = Vec2::cross3(p, tri[1], tri[2]) < 0;
            b3 = Vec2::cross3(p, tri[2], tri[0]) < 0;
        }
        return b1 == b2 && b2 == b3;
    }

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
