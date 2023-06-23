// coming soon

//#pragma once
//
//#include <stdint.h>
//#include <vector>
//#include <cmath>
//#include <algorithm>
//
//// my
//#include "Vec2.hpp"
//#include "Line2.hpp"
//
//#if DEBUG_LOG
//#include <iostream>
//#endif
//
//typedef enum
//{
//	T_TRUE,
//	T_FALSE,
//	T_NONE
//} T_BOOL;
//
//class SweepEvent
//{
//public:
//	size_t idx;
//	Vec2 p;
//	T_BOOL isleft;
//
//	SweepEvent(size_t idx, Vec2 p, T_BOOL isleft) : idx(idx), p(p), isleft(isleft) {}
//	SweepEvent(size_t idx, Vec2 p, bool isleft) : idx(idx), p(p)
//	{
//		if (isleft)
//			this->isleft = T_BOOL::T_TRUE;
//		else
//			this->isleft = T_BOOL::T_FALSE;
//	}
//};
//
//class SweepLineAlgorithm
//{
//public:
//	std::vector<Line2> segs;
//	std::vector<SweepEvent> events;
//
//	SweepLineAlgorithm(std::vector<Line2> segs) : segs(segs)
//	{
//		events.clear();
//		for (size_t i = 0; i < segs.size(); i++)
//		{
//			events.emplace_back(SweepEvent(i, segs[i].s, segs[i].s.x <= segs[i].e.x));
//			events.emplace_back(SweepEvent(i, segs[i].e, segs[i].e.x < segs[i].s.x));
//		}
//
//		for (size_t i = 0; i < events.size(); i++)
//			for (size_t j = 0; j < events.size() - i - 1; j++)
//				if (events[j].p.x > events[j + 1].p.x)
//					std::swap(events[j], events[j + 1]);
//				else if (events[j].p.x == events[j + 1].p.x && events[j].p.y > events[j + 1].p.y)
//					std::swap(events[j], events[j + 1]);
//	}
//
//	std::vector<Line2> Sweep()
//	{
//		while (events.size() > 0)
//		{
//		
//		
//		}
//	}
//};
//
//
//
//
