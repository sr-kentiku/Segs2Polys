#pragma once

#include <stack>
#include <vector>
#include <omp.h>

// my
#include "Vec2.hpp"
#include "Line2.hpp"
#include "TriangleUtil.hpp"

#ifdef _DEBUG
#include <iostream>
#include <iomanip>
#endif

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

clock_t t0;
t0 = clock();
        links = MakeLinks(tris, segs);
std::cout << clock() - t0 << "," << std::endl;
t0 = clock();
        triIdxs = TrailLinks(links, tris.size());
std::cout << clock() - t0 << "," << std::endl;

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
		visited = std::vector<bool>(triSize, false);

		for (size_t i = 0; i < links.size(); i++)
		{
			adj[links[i].first].emplace_back(links[i].second);
			adj[links[i].second].emplace_back(links[i].first);
		}

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
		
clock_t t0;
clock_t t1;
t0 = clock();

        for (size_t i = 0; i < tris.size(); i++)
		{
			c = Vec2::zero;
			for (size_t j = 0; j < tris[i].size(); j++)
				c += tris[i][j];
			c /= tris[i].size();
			com.emplace_back(c);
		}
		
std::cout << "\t" << clock() - t0 << std::endl;
t0 = clock();

		#pragma omp parallel for
		for (size_t i = 0; i < tris.size(); i++)
		{
// t1 = clock();

			#pragma omp parallel for private(kk, l1, l2, f)
			for (size_t j = 0; j < tris.size(); j++)
			{
				if (i == j)
					continue;

				f = false;
				#pragma omp parallel for
				for (size_t k = 0; k < o.size(); k++)
				{
					if (f)
						continue;
					if ((i == o[k].first && j == o[k].second) ||
					    (i == o[k].second && j == o[k].first))
					{
						#pragma omp critical
						{
							f = true;
						}
						// break;
					}
				}
				if (f)
					continue;

				f = false;
				#pragma omp parallel for
				for (size_t k = 0; k < tris[i].size(); k++)
				{
					if (f)
						continue;
					kk = fmod(k + 1, tris[i].size());
					l1 = Line2(tris[i][k], tris[i][kk]);
					for (size_t l = 0; l < tris[j].size(); l++)
					{
						ll = fmod(l + 1, tris[j].size());
						l2 = Line2(tris[j][l], tris[j][ll]);
						if (l1 == l2)
						{
							#pragma omp critical
							{
								f = true;
							}
							// break;
						}
					}
					// if (f)
					// 	break;
				}
				if (!f)
					continue;

				l1 = Line2(com[i], com[j]);

				f = false;
				#pragma omp parallel for
				for (size_t k = 0; k < segs.size(); k++)
				{
					if (f)
						continue;
					if (l1.isCross(segs[k]))
					{
						#pragma omp critical
						{
							f = true;
						}
						// break;
					}
				}

				if (!f)
				{
					#pragma omp critical
					{
						o.emplace_back(i, j);
					}
				}
			}
// std::cout << "\t\t" << clock() - t1 << "\t" << i << "\t" << omp_get_thread_num() << std::endl;
		}
std::cout << "\t" << clock() - t0 << std::endl;

        return o;
    }
};
