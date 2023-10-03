#pragma once

#include <stack>
#include <vector>

// my
#include "Vec2.hpp"
#include "Line2.hpp"
#include "TriangleUtil.hpp"

class SplitTrisSegs
{
public:
    SplitTrisSegs() {}

    static std::vector<std::vector<std::vector<Vec2>>> CalcParseTrisSegs(std::vector<std::vector<Vec2>>& tris, std::vector<Line2>& segs)
    {
        std::vector<std::vector<std::vector<Vec2>>> o = std::vector<std::vector<std::vector<Vec2>>>();

        std::vector<std::vector<Vec2>> tri = std::vector<std::vector<Vec2>>();
        std::vector<std::pair<size_t, size_t>> links;
        std::vector<std::vector<size_t>> triIdxs;

        links = MakeLinks(tris, segs);
        triIdxs = TrailLinks(links, tris.size());

        for (size_t i = 0; i < triIdxs.size(); i++)
        {
            if (triIdxs[i].empty())
                continue;
            tri.clear();
            for (size_t j = 0; j < triIdxs[i].size(); j++)
                tri.emplace_back(tris[triIdxs[i][j]]);
            o.emplace_back(tri);
        }

        return o;
    }

private:
    static std::vector<std::vector<size_t>> TrailLinks(std::vector<std::pair<size_t, size_t>>& links, size_t triSize)
    {
		std::vector<std::vector<size_t>> o = std::vector<std::vector<size_t>>();
		
        std::vector<size_t> r = std::vector<size_t>();
        std::vector<std::vector<size_t>> adj;
		std::vector<bool> visited;
		std::stack<size_t> stack = std::stack<size_t>();
		size_t cur;

		adj.resize(triSize);
		for (size_t i = 0; i < links.size(); i++)
		{
			adj[links[i].first].emplace_back(links[i].second);
			adj[links[i].second].emplace_back(links[i].first);
		}
		visited = std::vector<bool>(triSize, false);

		while (true)
		{
			r.clear();

			cur = -1;
			for (size_t i = 0; i < visited.size(); i++)
				if (!visited[i])
				{
					cur = i;
					break;
				}
			if (cur == -1)
				break;

			stack.push(cur);
			while (!stack.empty())
			{
				cur = stack.top();
				stack.pop();

				if (visited[cur])
					continue;
				
				r.emplace_back(cur);
				visited[cur] = true;

				for (size_t i = 0; i < adj[cur].size(); i++)
				{
					if (visited[adj[cur][i]])
						continue;
					stack.push(adj[cur][i]);
				}
			}
			o.emplace_back(r);
		}

        return o;
    }

    static std::vector<std::pair<size_t, size_t>> MakeLinks(std::vector<std::vector<Vec2>>& tris, std::vector<Line2>& segs)
    {
        std::vector<std::pair<size_t, size_t>> o = std::vector<std::pair<size_t, size_t>>();

        std::vector<Vec2> com = std::vector<Vec2>();

        size_t kk;
        size_t ll;

		Line2 l1;
		Line2 l2;
        
        Vec2 c;
        bool f;
		
        for (size_t i = 0; i < tris.size(); i++)
		{
			c = Vec2::zero;
			for (size_t j = 0; j < tris[i].size(); j++)
				c += tris[i][j];
			c /= tris[i].size();
			com.emplace_back(c);
		}

		for (size_t i = 0; i < tris.size(); i++)
		{
			for (size_t j = 0; j < tris.size(); j++)
			{
				if (i == j)
					continue;

				f = false;
				for (size_t k = 0; k < o.size(); k++)
					if ((i == o[k].first && j == o[k].second) ||
					    (i == o[k].second && j == o[k].first))
					{
						f = true;
						break;
					}
				if (f)
					continue;

				f = false;
				for (size_t k = 0; k < tris[i].size(); k++)
				{
					kk = fmod(k + 1, tris[i].size());
					l1 = Line2(tris[i][k], tris[i][kk]);
					for (size_t l = 0; l < tris[j].size(); l++)
					{
						ll = fmod(l + 1, tris[j].size());
						l2 = Line2(tris[j][l], tris[j][ll]);
						if (l1 == l2)
						{
							f = true;
							break;
						}
					}
					if (f)
						break;
				}
				if (!f)
					continue;

				l1 = Line2(com[i], com[j]);

				f = false;
				for (size_t k = 0; k < segs.size(); k++)
				{
					if (l1.isCross(segs[k]))
					{
						f = true;
						break;
					}
				}

				if (!f)
					o.emplace_back(i, j);
			}
		}

        return o;
    }
};
