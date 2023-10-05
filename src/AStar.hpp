#pragma once

#include <stdint.h>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <stack>
#include <queue>
#include <functional>

#if DEBUG_LOG
#include <iostream>
#include <fstream>
#endif

class AStar
{
public:
	AStar() {}

    template <typename T>
    struct stNode
    {
        double csf;
        double pri;
        T node;
        
        bool operator<(const stNode& n) const { return pri > n.pri; }
    };

    template <typename T>
    static std::vector<T> CalcAstar(std::unordered_map<T, std::vector<T>>& adj, std::function<double(T, T)> heuristic, T& source, T& target)
    {
        std::unordered_map<T, double> csf;
        std::priority_queue<stNode<T>> ft;
        std::unordered_map<T, T> cf;
        double pri;

        std::vector<T> path;
        double ncost;
        T cur;
        T* next;

        csf[source] = 0;
        pri = heuristic(source, target);
        ft.push({ 0, pri, source });

        while (!ft.empty()) {
            cur = ft.top().node;
            ft.pop();
            
            if (cur == target) {
                path.clear();
                while (cur != source) {
                    path.emplace_back(cur);
                    cur = cf[cur];
                }
                path.emplace_back(source);
                std::reverse(path.begin(), path.end());
                return path;
            }
            
            for (size_t i = 0; i < adj[cur].size(); i++) {
                next = &adj[cur][i];
                ncost = csf[cur] + heuristic(cur, *next);
                if (!csf.count(*next) || ncost < csf[*next]) {
                    csf[*next] = ncost;
                    pri = ncost + heuristic(*next, target);
                    ft.push({ ncost, pri, *next });
                    cf[*next] = cur;
                }
            }
        }
        
        return std::vector<T>();
    }
};
