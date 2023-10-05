#include "Segs2Polys.hpp"
#include "Segs2Polys.cpp"

#ifdef _DEBUG
#include <iomanip>
#include <exception>
#include <stdexcept>

#include <functional>
#include <unordered_map>

#include "AStar.hpp"

#include <stack>
#endif

#ifdef _DEBUG
// std::vector<Line2> segs = std::vector<Line2>({
// 	// Line2(Vec2(1.0, 1.0), Vec2(1.0, 3.0)),
// 	// Line2(Vec2(1.0, 1.0), Vec2(3.0, 1.0)),
// 	// Line2(Vec2(1.0, 3.0), Vec2(2.0, 3.0)),
// 	// Line2(Vec2(2.0, 2.0), Vec2(2.0, 3.0)),
// 	// Line2(Vec2(2.0, 2.0), Vec2(3.0, 2.0)),
// 	// Line2(Vec2(2.0, 3.0), Vec2(4.0, 3.0)),
// 	// Line2(Vec2(3.0, 1.0), Vec2(3.0, 2.0)),
// 	// Line2(Vec2(3.0, 1.0), Vec2(5.0, 1.0)),
// 	// Line2(Vec2(3.0, 2.0), Vec2(4.0, 2.0)),
// 	// Line2(Vec2(4.0, 2.0), Vec2(4.0, 3.0)),
// 	// Line2(Vec2(4.0, 3.0), Vec2(5.0, 3.0)),
// 	// Line2(Vec2(5.0, 1.0), Vec2(5.0, 3.0)),

// 	// Line2(Vec2(1.0, 1.0), Vec2(5.0, 1.0)),
// 	// Line2(Vec2(5.0, 1.0), Vec2(5.0, 3.0)),
// 	// Line2(Vec2(5.0, 3.0), Vec2(4.0, 3.0)),
// 	// Line2(Vec2(4.0, 3.0), Vec2(4.0, 2.0)),
// 	// Line2(Vec2(4.0, 2.0), Vec2(3.0, 2.0)),
// 	// Line2(Vec2(2.0, 2.0), Vec2(2.0, 3.0)),
// 	// Line2(Vec2(2.0, 3.0), Vec2(1.0, 3.0)),
// 	// Line2(Vec2(1.0, 3.0), Vec2(1.0, 1.0)),
// 	// Line2(Vec2(3.0, 1.0), Vec2(3.0, 2.0)),
	
// 	Line2(Vec2(1.0, 1.0, 1 ), Vec2(3.0, 1.0, 1 )),
// 	Line2(Vec2(1.0, 1.0, 2 ), Vec2(1.0, 2.0, 2 )),
// 	Line2(Vec2(1.0, 1.0, 3 ), Vec2(3.0, 2.0, 3 )),
// 	Line2(Vec2(3.0, 1.0, 4 ), Vec2(3.0, 2.0, 4 )),
// 	Line2(Vec2(3.0, 1.0, 5 ), Vec2(5.0, 1.0, 5 )),
// 	Line2(Vec2(5.0, 1.0, 6 ), Vec2(5.0, 3.0, 6 )),
// 	Line2(Vec2(5.0, 3.0, 7 ), Vec2(3.0, 2.0, 7 )),
// 	Line2(Vec2(5.0, 3.0, 8 ), Vec2(4.0, 4.0, 8 )),
// 	Line2(Vec2(4.0, 4.0, 9 ), Vec2(3.0, 4.0, 9 )),
// 	Line2(Vec2(3.0, 4.0, 10), Vec2(3.0, 2.0, 10)),
// 	Line2(Vec2(3.0, 4.0, 11), Vec2(1.0, 4.0, 11)),
// 	Line2(Vec2(1.0, 4.0, 12), Vec2(3.0, 2.0, 12)),
// 	Line2(Vec2(1.0, 4.0, 13), Vec2(1.0, 2.0, 13)),
// 	Line2(Vec2(1.0, 2.0, 14), Vec2(3.0, 2.0, 14)),
// 	Line2(Vec2(3.0, 1.0, 15), Vec2(5.0, 3.0, 15)),

// 	//Line2(Vec2(1.0, 1.0, 1) + Vec2(10), Vec2(3.0, 1.0, 1) + Vec2(10)),
// 	//Line2(Vec2(1.0, 1.0, 2) + Vec2(10), Vec2(1.0, 2.0, 2) + Vec2(10)),
// 	//Line2(Vec2(1.0, 1.0, 3) + Vec2(10) , Vec2(3.0, 2.0, 3) + Vec2(10)),
// 	//Line2(Vec2(3.0, 1.0, 4) + Vec2(10) , Vec2(3.0, 2.0, 4) + Vec2(10)),
// 	//Line2(Vec2(3.0, 1.0, 5) + Vec2(10) , Vec2(5.0, 1.0, 5) + Vec2(10)),
// 	//Line2(Vec2(5.0, 1.0, 6) + Vec2(10) , Vec2(5.0, 3.0, 6) + Vec2(10)),
// 	//Line2(Vec2(5.0, 3.0, 7) + Vec2(10) , Vec2(3.0, 2.0, 7) + Vec2(10)),
// 	//Line2(Vec2(5.0, 3.0, 8) + Vec2(10) , Vec2(4.0, 4.0, 8) + Vec2(10)),
// 	//Line2(Vec2(4.0, 4.0, 9) + Vec2(10) , Vec2(3.0, 4.0, 9) + Vec2(10)),
// 	//Line2(Vec2(3.0, 4.0, 10) + Vec2(10), Vec2(3.0, 2.0, 10) + Vec2(10)),
// 	//Line2(Vec2(3.0, 4.0, 11) + Vec2(10), Vec2(1.0, 4.0, 11) + Vec2(10)),
// 	//Line2(Vec2(1.0, 4.0, 12) + Vec2(10), Vec2(3.0, 2.0, 12) + Vec2(10)),
// 	//Line2(Vec2(1.0, 4.0, 13) + Vec2(10), Vec2(1.0, 2.0, 13) + Vec2(10)),
// 	//Line2(Vec2(1.0, 2.0, 14) + Vec2(10), Vec2(3.0, 2.0, 14) + Vec2(10)),
// 	//Line2(Vec2(3.0, 1.0, 15) + Vec2(10), Vec2(5.0, 3.0, 15) + Vec2(10)),

// 	//Line2(Vec2(1.0, 1.0, 1) + Vec2(0, 10), Vec2(5.0, 1.0, 1) + Vec2(0, 10)),
// 	//Line2(Vec2(5.0, 1.0, 1) + Vec2(0, 10), Vec2(5.0, 3.0, 1) + Vec2(0, 10)),
// 	//Line2(Vec2(5.0, 3.0, 1) + Vec2(0, 10), Vec2(4.0, 4.0, 1) + Vec2(0, 10)),
// 	//Line2(Vec2(4.0, 4.0, 1) + Vec2(0, 10), Vec2(1.0, 4.0, 1) + Vec2(0, 10)),
// 	//Line2(Vec2(1.0, 4.0, 1) + Vec2(0, 10), Vec2(1.0, 1.0, 1) + Vec2(0, 10)),
// 	//Line2(Vec2(1.0, 1.0, 1) + Vec2(0, 10), Vec2(5.0, 3.0, 1) + Vec2(0, 10)),
// 	//Line2(Vec2(3.0, 1.0, 1) + Vec2(0, 10), Vec2(5.0, 3.0, 1) + Vec2(0, 10)),
// 	//Line2(Vec2(1.0, 2.0, 1) + Vec2(0, 10), Vec2(3.0, 2.0, 1) + Vec2(0, 10)),
// 	//Line2(Vec2(1.0, 4.0, 1) + Vec2(0, 10), Vec2(3.0, 2.0, 1) + Vec2(0, 10)),
// 	//Line2(Vec2(3.0, 1.0, 1) + Vec2(0, 10), Vec2(3.0, 4.0, 1) + Vec2(0, 10)),


// 	// Line2(Vec2(1.0, 1.0), Vec2(2.0, 1.0)),
// 	// Line2(Vec2(2.0, 1.0), Vec2(5.0, 1.0)),
// 	// Line2(Vec2(5.0, 1.0), Vec2(5.0, 4.0)),
// 	// Line2(Vec2(5.0, 4.0), Vec2(5.0, 5.0)),
// 	// Line2(Vec2(5.0, 5.0), Vec2(4.0, 5.0)),
// 	// Line2(Vec2(4.0, 5.0), Vec2(1.0, 5.0)),
// 	// Line2(Vec2(1.0, 5.0), Vec2(1.0, 2.0)),
// 	// Line2(Vec2(1.0, 2.0), Vec2(1.0, 1.0)),
// 	// Line2(Vec2(2.0, 1.0), Vec2(5.0, 4.0)),
// 	// Line2(Vec2(1.0, 2.0), Vec2(4.0, 5.0)),


// 	// Line2(Vec2(1.0, 1.0), Vec2(2.0, 1.0)),
// 	// Line2(Vec2(2.0, 1.0), Vec2(2.0, 1.0)),
// 	// Line2(Vec2(2.0, 1.0), Vec2(3.0, 1.0)),
// 	// Line2(Vec2(3.0, 1.0), Vec2(4.0, 1.0)),
// 	// Line2(Vec2(4.0, 1.0), Vec2(5.0, 1.0)),
// 	// Line2(Vec2(5.0, 1.0), Vec2(5.0, 5.0)),
// 	// Line2(Vec2(5.0, 5.0), Vec2(1.0, 5.0)),
// 	// Line2(Vec2(1.0, 5.0), Vec2(1.0, 4.0)),
// 	// Line2(Vec2(1.0, 4.0), Vec2(1.0, 3.0)),
// 	// Line2(Vec2(1.0, 3.0), Vec2(1.0, 2.0)),
// 	// Line2(Vec2(1.0, 2.0), Vec2(1.0, 1.0)),
// 	// Line2(Vec2(1.0, 1.0), Vec2(5.0, 5.0)),
// 	// Line2(Vec2(2.0, 1.0), Vec2(5.0, 5.0)),
// 	// Line2(Vec2(3.0, 1.0), Vec2(5.0, 5.0)),
// 	// Line2(Vec2(4.0, 1.0), Vec2(5.0, 5.0)),
// 	// Line2(Vec2(1.0, 4.0), Vec2(5.0, 5.0)),
// 	// Line2(Vec2(1.0, 3.0), Vec2(5.0, 5.0)),
// 	// Line2(Vec2(1.0, 2.0), Vec2(5.0, 5.0)),


// 	//Line2(Vec2(0.0,		0.0),		Vec2(6000.0,	0.0)),
// 	//Line2(Vec2(6000.0,	0.0),		Vec2(24000.0,	0.0)),
// 	//Line2(Vec2(24000.0, 0.0),		Vec2(24000.0,	6000.0)),
// 	//Line2(Vec2(24000.0, 6000.0),	Vec2(18000.0,	11500.0)),
// 	//Line2(Vec2(18000.0, 11500.0),	Vec2(6000.0,	11500.0)),
// 	//Line2(Vec2(6000.0,	11500.0),	Vec2(0.0,		11500.0)),
// 	//Line2(Vec2(0.0,		11500.0),	Vec2(0.0,		6000.0)),
// 	//Line2(Vec2(0.0,		6000.0),	Vec2(0.0,		0.0)),
// 	//Line2(Vec2(6000.0,	0.0),		Vec2(6000.0,	6000.0)),
// 	//Line2(Vec2(6000.0,	6000.0),	Vec2(6000.0,	11500.0)),
// 	//Line2(Vec2(6000.0,	6000.0),	Vec2(0.0,		6000.0)),

// 	//Line2(Vec2(1.0, 1.0), Vec2(5.0, 1.0)),
// 	//Line2(Vec2(5.0, 1.0), Vec2(5.0, 5.0)),
// 	//Line2(Vec2(5.0, 5.0), Vec2(1.0, 5.0)),
// 	//Line2(Vec2(1.0, 5.0), Vec2(1.0, 1.0)),
// 	//Line2(Vec2(2.0, 1.0), Vec2(5.0, 4.0)),
// 	//Line2(Vec2(1.0, 2.0), Vec2(4.0, 5.0)),

// 	// c1
// 	// Line2(Vec2(44515, 20000, -1), Vec2(52900, 20000, 0)),
// 	// Line2(Vec2(50600, 20000, -1), Vec2(50600, 22900, 0)),
// 	// Line2(Vec2(44515, 13000, -1), Vec2(50315, 13000, -1)),
// 	// Line2(Vec2(50315, 13000, -1), Vec2(50315, 10000, 0)),
// 	// Line2(Vec2(50315, 13000, -1), Vec2(56115, 13000, 0)),
// 	// Line2(Vec2(52900, 10570, -1), Vec2(52900, 13000, 0)),
// 	// Line2(Vec2(44515, 16200, -1), Vec2(47515, 16200, -1)),
// 	// Line2(Vec2(47515, 16200, -1), Vec2(47515, 20000, 0)),
// 	// Line2(Vec2(47515, 16200, -1), Vec2(47515, 13000, 0)),
// 	// Line2(Vec2(44515, 15200, -1), Vec2(47515, 15200, 0)),
// 	// Line2(Vec2(44515, 14000, -1), Vec2(47515, 14000, 0)),
// 	// Line2(Vec2(45975, 14000, -1), Vec2(45975, 15200, 0)),
// 	// Line2(Vec2(44515, 10000, 0), Vec2(44515, 22900, 0)),
// 	// Line2(Vec2(44515, 22900, 0), Vec2(52900, 22900, 0)),
// 	// Line2(Vec2(52900, 22900, 0), Vec2(52900, 20000, 0)),
// 	// Line2(Vec2(52900, 20000, 0), Vec2(56900, 20000, 0)),

// 	// c2
// 	// Line2(Vec2(44515, 20000, -1), Vec2(52900, 20000, 0)),
// 	// Line2(Vec2(50600, 20000, -1), Vec2(50600, 22900, 0)),
// 	// Line2(Vec2(58555, 27896, -1), Vec2(60269, 36104, 0)),
// 	// Line2(Vec2(56960, 34446, -1), Vec2(59799, 33853, 0)),
// 	// Line2(Vec2(62200, 21500, -1), Vec2(57424, 22499, 0)),
// 	// Line2(Vec2(59392, 31905, -1), Vec2(69180, 29860, 0)),
// 	// Line2(Vec2(68158, 34747, -1), Vec2(63656, 35688, -1)),
// 	// Line2(Vec2(63656, 35688, -1), Vec2(62720, 31210, 0)),
// 	// Line2(Vec2(62453, 39223, -1), Vec2(62044, 37266, -1)),
// 	// Line2(Vec2(62044, 37266, -1), Vec2(60576, 37573, 0)),
// 	// Line2(Vec2(60860, 31598, -1), Vec2(61796, 36076, -1)),
// 	// Line2(Vec2(61796, 36076, -1), Vec2(63656, 35688, 0)),
// 	// Line2(Vec2(61336, 33874, -1), Vec2(63196, 33485, 0)),
// 	// Line2(Vec2(65711, 35258, -1), Vec2(66369, 38405, 0)),
// 	// Line2(Vec2(56115, 10570, -1), Vec2(56115, 17100, -1)),
// 	// Line2(Vec2(56115, 17100, -1), Vec2(62200, 17100, 0)),
// 	// Line2(Vec2(44515, 13000, -1), Vec2(50315, 13000, -1)),
// 	// Line2(Vec2(50315, 13000, -1), Vec2(50315, 10000, 0)),
// 	// Line2(Vec2(50315, 13000, -1), Vec2(56115, 13000, 0)),
// 	// Line2(Vec2(52900, 10570, -1), Vec2(52900, 13000, 0)),
// 	// Line2(Vec2(44515, 16200, -1), Vec2(47515, 16200, -1)),
// 	// Line2(Vec2(47515, 16200, -1), Vec2(47515, 20000, 0)),
// 	// Line2(Vec2(47515, 16200, -1), Vec2(47515, 13000, 0)),
// 	// Line2(Vec2(59300, 17100, -1), Vec2(59300, 18500, -1)),
// 	// Line2(Vec2(59300, 18500, -1), Vec2(62200, 18500, 0)),
// 	// Line2(Vec2(44515, 15200, -1), Vec2(47515, 15200, 0)),
// 	// Line2(Vec2(44515, 14000, -1), Vec2(47515, 14000, 0)),
// 	// Line2(Vec2(45975, 14000, -1), Vec2(45975, 15200, 0)),
// 	// Line2(Vec2(44515, 10000, 0), Vec2(44515, 22900, 0)),
// 	// Line2(Vec2(44515, 22900, 0), Vec2(52900, 22900, 0)),
// 	// Line2(Vec2(52900, 22900, 0), Vec2(52900, 20000, 0)),
// 	// Line2(Vec2(52900, 20000, 0), Vec2(56900, 20000, 0)),
// 	// Line2(Vec2(56900, 20000, 0), Vec2(58555, 27896, 0)),
// 	// Line2(Vec2(58555, 27896, 0), Vec2(55716, 28489, 0)),
// 	// Line2(Vec2(55716, 28489, 0), Vec2(57430, 36697, 0)),
// 	// Line2(Vec2(57430, 36697, 0), Vec2(60269, 36104, 0)),
// 	// Line2(Vec2(60269, 36104, 0), Vec2(60985, 39530, 0)),
// 	// Line2(Vec2(60985, 39530, 0), Vec2(68816, 37894, 0)),
// 	// Line2(Vec2(68816, 37894, 0), Vec2(68158, 34747, 0)),
// 	// Line2(Vec2(68158, 34747, 0), Vec2(70116, 34338, 0)),
// 	// Line2(Vec2(70116, 34338, 0), Vec2(67215, 20453, 0)),
// 	// Line2(Vec2(67215, 20453, 0), Vec2(62200, 21500, 0)),
// 	// Line2(Vec2(62200, 21500, 0), Vec2(62200, 10000, 0)),
// 	// Line2(Vec2(62200, 10000, 0), Vec2(56115, 10000, 0)),
// 	// Line2(Vec2(56115, 10000, 0), Vec2(56115, 10570, 0)),
// 	// Line2(Vec2(56115, 10570, 0), Vec2(50885, 10570, 0)),
// 	// Line2(Vec2(50885, 10570, 0), Vec2(50885, 10000, 0)),
// 	// Line2(Vec2(50885, 10000, 0), Vec2(44515, 10000, 0)),

// 	// c2
// 	// Line2(Vec2(44515, 20000, -1), Vec2(52900, 20000, 0)),
// 	// Line2(Vec2(50600, 20000, -1), Vec2(50600, 22900, 0)),
// 	// Line2(Vec2(58555, 27896, -1), Vec2(60269, 36104, 0)),
// 	// Line2(Vec2(56960, 34446, -1), Vec2(59799, 33853, 0)),
// 	// Line2(Vec2(57424, 22499, 0), Vec2(62200, 21500, -1)),
// 	// Line2(Vec2(59392, 31905, -1), Vec2(69180, 29860, 0)),
// 	// Line2(Vec2(63656, 35688, -1), Vec2(68158, 34747, -1)),
// 	// Line2(Vec2(62720, 31210, 0), Vec2(63656, 35688, -1)),
// 	// Line2(Vec2(62044, 37266, -1), Vec2(62453, 39223, -1)),
// 	// Line2(Vec2(60576, 37573, 0), Vec2(62044, 37266, -1)),
// 	// Line2(Vec2(60860, 31598, -1), Vec2(61796, 36076, -1)),
// 	// Line2(Vec2(61796, 36076, -1), Vec2(63656, 35688, 0)),
// 	// Line2(Vec2(61336, 33874, -1), Vec2(63196, 33485, 0)),
// 	// Line2(Vec2(65711, 35258, -1), Vec2(66369, 38405, 0)),
// 	// Line2(Vec2(56115, 10570, -1), Vec2(56115, 17100, -1)),
// 	// Line2(Vec2(56115, 17100, -1), Vec2(62200, 17100, 0)),
// 	// Line2(Vec2(44515, 13000, -1), Vec2(50315, 13000, -1)),
// 	// Line2(Vec2(50315, 10000, 0), Vec2(50315, 13000, -1)),
// 	// Line2(Vec2(50315, 13000, -1), Vec2(56115, 13000, 0)),
// 	// Line2(Vec2(52900, 10570, -1), Vec2(52900, 13000, 0)),
// 	// Line2(Vec2(44515, 16200, -1), Vec2(47515, 16200, -1)),
// 	// Line2(Vec2(47515, 16200, -1), Vec2(47515, 20000, 0)),
// 	// Line2(Vec2(47515, 13000, 0), Vec2(47515, 16200, -1)),
// 	// Line2(Vec2(59300, 17100, -1), Vec2(59300, 18500, -1)),
// 	// Line2(Vec2(59300, 18500, -1), Vec2(62200, 18500, 0)),
// 	// Line2(Vec2(44515, 15200, -1), Vec2(47515, 15200, 0)),
// 	// Line2(Vec2(44515, 14000, -1), Vec2(47515, 14000, 0)),
// 	// Line2(Vec2(45975, 14000, -1), Vec2(45975, 15200, 0)),
// 	// Line2(Vec2(44515, 10000, 0), Vec2(44515, 22900, 0)),
// 	// Line2(Vec2(44515, 22900, 0), Vec2(52900, 22900, 0)),
// 	// Line2(Vec2(52900, 20000, 0), Vec2(52900, 22900, 0)),
// 	// Line2(Vec2(52900, 20000, 0), Vec2(56900, 20000, 0)),
// 	// Line2(Vec2(56900, 20000, 0), Vec2(58555, 27896, 0)),
// 	// Line2(Vec2(55716, 28489, 0), Vec2(58555, 27896, 0)),
// 	// Line2(Vec2(55716, 28489, 0), Vec2(57430, 36697, 0)),
// 	// Line2(Vec2(57430, 36697, 0), Vec2(60269, 36104, 0)),
// 	// Line2(Vec2(60269, 36104, 0), Vec2(60985, 39530, 0)),
// 	// Line2(Vec2(60985, 39530, 0), Vec2(68816, 37894, 0)),
// 	// Line2(Vec2(68158, 34747, 0), Vec2(68816, 37894, 0)),
// 	// Line2(Vec2(68158, 34747, 0), Vec2(70116, 34338, 0)),
// 	// Line2(Vec2(67215, 20453, 0), Vec2(70116, 34338, 0)),
// 	// Line2(Vec2(62200, 21500, 0), Vec2(67215, 20453, 0)),
// 	// Line2(Vec2(62200, 10000, 0), Vec2(62200, 21500, 0)),
// 	// Line2(Vec2(56115, 10000, 0), Vec2(62200, 10000, 0)),
// 	// Line2(Vec2(56115, 10000, 0), Vec2(56115, 10570, 0)),
// 	// Line2(Vec2(50885, 10570, 0), Vec2(56115, 10570, 0)),
// 	// Line2(Vec2(50885, 10000, 0), Vec2(50885, 10570, 0)),
// 	// Line2(Vec2(44515, 10000, 0), Vec2(50885, 10000, 0)),

// 	// T test no slash
// 	// Line2(Vec2(6, 1), Vec2(5, 1)),
// 	// Line2(Vec2(5, 1), Vec2(5, 4)),
// 	// Line2(Vec2(5, 4), Vec2(1, 4)),
// 	// Line2(Vec2(1, 4), Vec2(1, 5)),
// 	// Line2(Vec2(1, 5), Vec2(9, 5)),
// 	// Line2(Vec2(9, 5), Vec2(9, 4)),
// 	// Line2(Vec2(9, 4), Vec2(6, 4)),
// 	// Line2(Vec2(6, 4), Vec2(6, 1)),

// 	// t test no slash
// 	// Line2(Vec2(1, 1), Vec2(4, 1)),
// 	// Line2(Vec2(4, 1), Vec2(4, 4)),
// 	// Line2(Vec2(4, 4), Vec2(5, 4)),
// 	// Line2(Vec2(5, 4), Vec2(5, 1)),
// 	// Line2(Vec2(5, 1), Vec2(7, 1)),
// 	// Line2(Vec2(7, 1), Vec2(7, 5)),
// 	// Line2(Vec2(7, 5), Vec2(1, 5)),
// 	// Line2(Vec2(1, 5), Vec2(1, 1)),

// });

// void test()
// {
// #if DEBUG_TIMER
// 	clock_t t1;
// 	clock_t t2;
// #endif

// #if DEBUG_TIMER
// 	t1 = clock();
// #endif

// 	// segs = Segs2Polys::extendMarginIntersection(segs, 0.2);
// 	// segs = Segs2Polys::extendMarginIntersection(segs, 1.0);

// 	// segs = SimpleSplitSegs::extendSegs(segs, 1.0);
// 	segs = SimpleSplitSegs::splitSegs(segs);

// 	// segs = SimpleSplitSegs::splitSegs(segs);
// 	// segs = Segs2Polys::extendMarginIntersection(segs, 1.0);


// #if DEBUG_TIMER
// 	t2 = clock();
// 	std::cout << "SimpleSplitSegs::splitSegs	" << t2 - t1 << "ms" << std::endl;
// #endif

// #if DEBUG_LOG
// 	std::cout << std::fixed;

// 	for (size_t i = 0; i < segs.size(); i++)
// 	{
// 		segs[i].debug(0, 0, false);
// 		std::cout << "," << std::endl;
// 	}

// 	for (size_t i = 0; i < segs.size(); i++)
// 	{
// 		segs[i].s *= 1;
// 		segs[i].e *= 1;
// 	}

// #endif

// #if DEBUG_TIMER
// 	t1 = clock();
// #endif


// 	std::vector<std::vector<Cell>> cells = Segs2Polys::createGrid(segs);


// #if DEBUG_TIMER
// 	t2 = clock();
// 	std::cout << "Segs2Polys::createGrid		" << t2 - t1 << "ms" << std::endl;
// #endif

// #if DEBUG_LOG
// 	std::cout << std::endl << "cells" << std::endl;
// 	std::cout << "[" << std::endl;
// 	for (const std::vector<Cell>& c1 : cells)
// 	{
// 		std::cout << "    [" << std::endl;
// 		for (const Cell& c2 : c1)
// 		{
// 			std::cout << "        {" << std::endl;
// 			std::cout << "            \"swh\":" << "(" << c2.swh.x << ", " << c2.swh.y << ")," << std::endl;
// 			std::cout << "            \"ewh\":" << "(" << c2.ewh.x << ", " << c2.ewh.y << ")," << std::endl;
// 			std::cout << "            \"shape\":{" << std::endl;
// 			std::cout << "                \"hull\":[" << std::endl;
// 			for (size_t i = 0; i < c2.shape.hull.size(); i++)
// 			std::cout << "                    (" << c2.shape.hull[i].x << ", " << c2.shape.hull[i].y << ")," << std::endl;
// 			std::cout << "                ]," << std::endl;
// 			std::cout << "                \"lines\":{" << std::endl;
// 			for (size_t i = 0; i < c2.shape.lines.size(); i++)
// 			std::cout << "                    \"" << i << "\":((" << c2.shape.lines[i].s.x << ", " << c2.shape.lines[i].s.y << "),(" << c2.shape.lines[i].e.x << ", " << c2.shape.lines[i].e.y << "))," << std::endl;
// 			std::cout << "                }," << std::endl;
// 			std::cout << "                \"rays, sp\":{" << std::endl;
// 			for (size_t i = 0; i < c2.shape.rays.size(); i++)
// 			{
// 				long e = 10000;
// 				long long xx = c2.shape.rays[i].x * e;
// 				long long yy = c2.shape.rays[i].y * e;
// 				double x = (double)xx / e;
// 				double y = (double)yy / e;

// 				//double x = c2.shape.rays[i].x;
// 				//double y = c2.shape.rays[i].y;

// 			//std::cout << "                    (" << x << ", " << y << "):[";
// 			std::cout << "                    (" << std::to_string(x) << ", " << std::to_string(y) << "):[";
// 				for (size_t j = 0; j < c2.shape.sp[i].size(); j++)
// 					std::cout << c2.shape.sp[i][j] << ",";
// 				std::cout << "]," << std::endl;
// 			}
// 			std::cout << "                }," << std::endl;
// 			std::cout << "            }," << std::endl;
// 			std::cout << "        }," << std::endl;
// 		}
// 		std::cout << "    ]," << std::endl;
// 	}
// 	std::cout << "]" << std::endl;
// 	std::cout << std::endl << std::endl;
// #endif

// #if DEBUG_TIMER
// 	t1 = clock();
// #endif


// 	std::vector<stCellLink> links = Segs2Polys::getLinkRaycast(cells);


// #if DEBUG_TIMER
// 	t2 = clock();
// 	std::cout << "Segs2Polys::getLinkRaycast	" << t2 - t1 << "ms" << std::endl;
// #endif

// #if DEBUG_LOG
// 	// std::cout << "links" << std::endl;
// 	// for (stCellLink& r1 : links)
// 	// 	r1.debug();
// 	// std::cout << std::endl << std::endl;

// 	std::cout << "links-parse" << std::endl;
// 	std::cout << "[" << std::endl;
// 	for (size_t i = 0; i < links.size(); i++)
// 	{
// 		std::cout << "    (";
// 		cells[links[i].cell11][links[i].cell12].shape.rays[links[i].cellLink1].debug(0, false);
// 		std::cout << ", ";
// 		cells[links[i].cell21][links[i].cell22].shape.rays[links[i].cellLink2].debug(0, false);
// 		std::cout << "),";
// 		std::cout << std::endl;
// 	}

// 	// for (stCellLink& r1 : links)
// 	// {
// 	// 	std::cout << "    (";
// 	// 	cells[r1.cell11][r1.cell12].shape.rays[r1.cellLink1].debug(0, false);
// 	// 	std::cout << ", ";
// 	// 	cells[r1.cell21][r1.cell22].shape.rays[r1.cellLink2].debug(0, false);
// 	// 	std::cout << "),";
// 	// 	std::cout << std::endl;
// 	// }
// 	std::cout << "]" << std::endl << std::endl;

// #endif

// #if DEBUG_TIMER
// 	t1 = clock();
// #endif


// 	std::vector<std::vector<Vec2>> polys = Segs2Polys::trailCells(cells, links);
// 	// std::vector<std::vector<Vec2>> polys = Segs2Polys::trailCells1(cells, links);


// #if DEBUG_TIMER
// 	t2 = clock();
// 	std::cout << "Segs2Polys::trailCells		" << t2 - t1 << "ms" << std::endl;
// #endif

// #if DEBUG_LOG
// 	std::cout << "polys" << std::endl;
// 	std::cout << "[" << std::endl;
// 	for (size_t i = 0; i < polys.size(); i++)
// 	{
// 		std::cout << "    (";
// 		for (Vec2& r1 : polys[i])
// 		{
// 			r1.debug(0, false);
// 			std::cout << ", ";
// 		}
// 		std::cout << ")," << std::endl;
// 	}
// 	std::cout << "]" << std::endl << std::endl;
// #endif

// #if DEBUG_TIMER
// 	t1 = clock();
// #endif


// 	polys = Segs2Polys::mergePolysLines(polys);


// #if DEBUG_TIMER
// 	t2 = clock();
// 	std::cout << "Segs2Polys::mergePolysLines	" << t2 - t1 << "ms" << std::endl;
// #endif

// #if DEBUG_LOG
// 	std::cout << "polys" << std::endl;
// 	std::cout << "[" << std::endl;
// 	for (size_t i = 0; i < polys.size(); i++)
// 	{
// 		std::cout << "    (";
// 		for (Vec2& r1 : polys[i])
// 		{
// 			r1.debug(0, false);
// 			std::cout << ", ";
// 		}
// 		std::cout << ")," << std::endl;
// 	}
// 	std::cout << "]" << std::endl << std::endl;
// #endif
// }

// void test1()
// {
// #if DEBUG_TIMER_TOTAL
// 	clock_t t1;
// 	clock_t t2;
// #endif

// #if DEBUG_TIMER_TOTAL
// 	t1 = clock();
// #endif

// 	int n = 100000;
// 	for (int i = 0; i < n; i++)
// 		test();

// #if DEBUG_TIMER_TOTAL
// 	t2 = clock();
// 	std::cout << "total							" << t2 - t1 << "ms" << std::endl;
// 	std::cout << "lines\t" << segs.size() * n << std::endl;
// #endif
// }

int main(int argc, char* argv[])
{
	{
		std::vector<Vec2> hull = std::vector<Vec2>({
			Vec2(10000,10000, 1),
			Vec2(16000,10000, 2),
			Vec2(34000,10000, 3),
			Vec2(34000,16000, 4),
			Vec2(28000,21500, 5),
			Vec2(16000,21500, 6),
			Vec2(10000,21500, 7),
			Vec2(10000,16000, 8),
		});

		std::vector<Line2> segs = std::vector<Line2>({
			Line2(Vec2(16000,10000, 2), Vec2(16000,16000, 9)),
			Line2(Vec2(16000,16000, 9), Vec2(16000,21500, 6)),
			Line2(Vec2(10000,16000, 8), Vec2(16000,16000, 9)),
		});

		std::unordered_map<Vec2, std::vector<Vec2>> adj;

		std::vector<Vec2> vlines;
		Vec2 vs;
		Vec2 ve;

		std::unordered_map<Vec2, std::vector<Vec2>>::iterator it1;
		std::vector<Vec2> rangevec;
		std::vector<Vec2> brangevec;

		size_t ii;

clock_t t0;
t0 = clock();

		for (size_t i = 0; i < hull.size(); i++)
		{
			ii = fmod(i + 1, hull.size());
			adj[hull[i]].emplace_back(hull[ii]);
			adj[hull[ii]].emplace_back(hull[i]);
		}

		for (size_t i = 0; i < segs.size(); i++)
		{
			adj[segs[i].s].emplace_back(segs[i].e);
			adj[segs[i].e].emplace_back(segs[i].s);
		}

		for (size_t i = 0; i < hull.size(); i++)
			vlines.emplace_back(hull[i]);
		for (size_t i = 0; i < segs.size(); i++)
		{
			vlines.emplace_back(segs[i].s);
			vlines.emplace_back(segs[i].e);
		}

		std::sort(vlines.begin(), vlines.end(), Vec2::lessx);

		vlines.erase(
			std::unique(vlines.begin(), vlines.end(), Vec2::equalx),
			vlines.end());

for (size_t i = 0; i < vlines.size(); i++)
	std::cout << i << "\t" << vlines[i].x << std::endl;
std::cout << std::endl;

		for (size_t i = 1; i < vlines.size(); i++)
		{
			vs = vlines[i - 1];
			ve = vlines[i];
			rangevec.clear();

std::cout << vs.x << " - " << ve.x << std::endl;

			for (it1 = adj.begin(); it1 != adj.end(); it1++)
			{
				if (vs.x <= it1->first.x && it1->first.x <= ve.x)
					rangevec.emplace_back(it1->first);
			}

for (size_t i = 0; i < rangevec.size(); i++)
{
	std::cout << rangevec[i].ToString() << std::endl;
}
std::cout << std::endl;

			for (size_t i = 0; i < rangevec.size(); i++)
			{
				std::cout << rangevec[i].ToString() << std::endl;
			}

		}

std::cout << "time\t" << clock() - t0 << "ms" << std::endl;
	}

	return 0;

// 	{
// 		std::vector<Vec2> hull = std::vector<Vec2>({
// 			// Vec2(10000,10000, 1),
// 			// Vec2(16000,10000, 2),
// 			// Vec2(34000,10000, 3),
// 			// Vec2(34000,16000, 4),
// 			// Vec2(28000,21500, 5),
// 			// Vec2(16000,21500, 6),
// 			// Vec2(10000,21500, 7),
// 			// Vec2(10000,16000, 8),

// 		});

// 		std::vector<Line2> segs = std::vector<Line2>({
// 			// Line2(Vec2(16000,10000, 2), Vec2(16000,16000, 9)),
// 			// Line2(Vec2(16000,16000, 9), Vec2(16000,21500, 6)),
// 			// Line2(Vec2(10000,16000, 8), Vec2(16000,16000, 9)),

// 		});

// 		std::unordered_map<Vec2, std::vector<Vec2>> adj;

// 		size_t ii;

// 		for (size_t i = 0; i < hull.size(); i++)
// 		{
// 			ii = fmod(i + 1, hull.size());
// 			adj[hull[i]].emplace_back(hull[ii]);
// 			adj[hull[ii]].emplace_back(hull[i]);
// 		}

// 		for (size_t i = 0; i < segs.size(); i++)
// 		{
// 			adj[segs[i].s].emplace_back(segs[i].e);
// 			adj[segs[i].e].emplace_back(segs[i].s);
// 		}

// 		std::vector<Vec2> ret;

// 		std::unordered_map<Vec2, std::vector<Vec2>>::iterator it1;
// 		std::unordered_map<Vec2, std::vector<Vec2>>::iterator it2;

// 		Vec2 v;

// 		Vec2 source;
// 		Vec2 target;

// clock_t t0;
// clock_t t1;
// t0 = clock();
// 		// ret = AStar::CalcAstar<Vec2>(adj, Vec2::distance(), hull[0], hull[1]);
// 		// ret = AStar::CalcAstar<Vec2>(adj, Vec2::distance(), hull[0], hull[4]);

// // 		it = adj.begin();
// // 		source = it->first;
// // 		target = it->second.back();
// // 		it->second.pop_back();

// // 		ret = AStar::CalcAstar<Vec2>(adj, Vec2::distance(), source, target);
// // std::cout << "time\t" << clock() - t0 << "ms" << std::endl;

// // 		std::cout << "\t[" << std::endl;
// // 		for (size_t i = 0; i < ret.size() - 1; i++)
// // 			std::cout << "\t\t" << ret[i].ToString()
// // 					  << ", " << ret[fmod(i + 1, ret.size())].ToString()
// // 					  << ", " << std::endl;
// // 		std::cout << "\t]," << std::endl;

// // 		for (size_t i = 0; i < ret.size(); i++)
// // 			adj.erase(ret[i]);

// 		while (!adj.empty())
// 		{
// // for (it1 = adj.begin(); it1 != adj.end(); ++it1)
// // {
// // 	std::cout << it1->first.ToString() << "\t" << it1->first.id << std::endl;
// // 	for (size_t i = 0; i < it1->second.size(); i++)
// // 	{
// // 		std::cout << "\t" << it1->second[i].ToString() << "\t" << it1->second[i].id << std::endl;
// // 	}
// // }

// 			it1 = adj.end();
// 			for (it2 = adj.begin(); it2 != adj.end(); it2++)
// 			{
// 				if (it2->second.size() <= 1)
// 					continue;
// 				it1 = it2;
// 				break;
// 			}

// 			if (it1 == adj.end())
// 			{
// 				std::cout << "# none first" << std::endl;
// 				break;
// 			}

// 			source = it1->first;
// 			target = it1->second.back();
// 			it1->second.pop_back();




// // std::cout << "source\t" << source.ToString() << std::endl;
// // std::cout << "target\t" << target.ToString() << std::endl;

// t1 = clock();
// 			ret = AStar::CalcAstar<Vec2>(adj, Vec2::distance(), source, target);
// if (!ret.empty())
// 	std::cout << "# time\t" << clock() - t1 << "ms" << std::endl;




// 			// if (ret.empty())
// 			// 	break;




// if (!ret.empty())
// {
// 	// std::cout << "# " << ret.size() << std::endl;
// 	std::cout << "\t[" << std::endl;
// 	for (size_t i = 0; i < ret.size(); i++)
// 	{
// 		std::cout << "\t\t" << ret[i].ToString();
// 		std::cout << ", " << ret[fmod(i + 1, ret.size())].ToString();
// 		std::cout << ", " << std::endl;
// 	}
// 	std::cout << "\t]," << std::endl;
// }





// 			it1 = adj.find(source);
// 			for (size_t i = 0; i < ret.size(); i++)
// 			{
// 				it1->second.erase(
// 					std::remove(it1->second.begin(), 
// 								it1->second.end(), 
// 								ret[i]), 
// 					it1->second.end()
// 				);
// 			}

// 			// it2 = adj.begin();
// 			// while (it2 != adj.end())
// 			// {
// 			// 	if (it2->second.size() <= 1)
// 			// 	{
// 			// 		adj.erase(it2);
// 			// 		it2 = adj.begin();
// 			// 		continue;
// 			// 	}
// 			// 	it2++;
// 			// }




// // for (it1 = adj.begin(); it1 != adj.end(); ++it1)
// // {
// // 	std::cout << it1->first.ToString() << "\t" << it1->first.id << std::endl;
// // 	for (size_t i = 0; i < it1->second.size(); i++)
// // 	{
// // 		std::cout << "\t" << it1->second[i].ToString() << "\t" << it1->second[i].id << std::endl;
// // 	}
// // }


// // std::cout << "# ------------------------------------------------" << std::endl;
// 		}
// std::cout << "# time\t" << clock() - t0 << "ms" << std::endl;

// 	}

// 	return 0;

	// {
		// std::vector<Vec2> hull = std::vector<Vec2>({
		// 	// Vec2(10000,10000),
		// 	// Vec2(16000,10000),
		// 	// Vec2(34000,10000),
		// 	// Vec2(34000,16000),
		// 	// Vec2(28000,21500),
		// 	// Vec2(16000,21500),
		// 	// Vec2(10000,21500),
		// 	// Vec2(10000,16000),

		// });

		// std::vector<Line2> segs = std::vector<Line2>({
		// 	// Line2(Vec2(16000,10000), Vec2(16000,16000)),
		// 	// Line2(Vec2(16000,16000), Vec2(16000,21500)),
		// 	// Line2(Vec2(10000,16000), Vec2(16000,16000)),

		// });

	// 	size_t ep;

	// 	clock_t t0;
	// 	t0 = clock();

	// 	Clear();

	// 	for (size_t i = 0; i < segs.size(); i++)
	// 		AddSeg(segs[i].s.x, segs[i].s.y, 0, segs[i].e.x, segs[i].e.y, 0);
	// 	std::cout << "segCnt\t" << GetSegCnt() << std::endl;

	// 	AddPoly();
	// 	for (size_t i = 0; i < hull.size(); i++)
	// 		AddPolyVec(0, hull[i].x, hull[i].y, 0);
	// 	std::cout << "hullCnt\t" << GetPolyCnt() << std::endl;

	// 	CalcEarClip(0, 1);
	// 	ep = GetPolyCnt();
	// 	std::cout << "earCnt\t" << ep << std::endl;

	// 	for (size_t i = 1; i < GetPolyCnt(); i++)
	// 	{
	// 		std::cout << "# " << i - 1 << std::endl; 
	// 		for (size_t j = 0; j < GetPolyVecCnt(i); j++)
	// 			std::cout << "\t" << gPolys[i][j].ToString() << "," << std::endl;
	// 		std::cout << std::endl;
	// 	}

	// 	CalcTrianglesSplitLines(1, ep, 0, segs.size());
	// 	std::cout << "triCnt\t" << GetPolyCnt() << std::endl;

	// 	for (size_t i = ep; i < GetPolyCnt(); i++)
	// 	{
	// 		std::cout << "# " << i - 6 << std::endl; 
	// 		for (size_t j = 0; j < GetPolyVecCnt(i); j++)
	// 			std::cout << "\t" << gPolys[i][j].ToString() << "," << std::endl;
	// 		std::cout << std::endl;
	// 	}

	// 	CalcParseTrisSegs(ep, GetPolyCnt(), 0, segs.size());


	// 	std::cout << "time\t" << clock() - t0 << "ms" << std::endl;
	// }

	// return 0;

// 	{
// 		std::vector<Vec2> hull = std::vector<Vec2>({
// 			// Vec2(10000,10000),
// 			// Vec2(16000,10000),
// 			// Vec2(34000,10000),
// 			// Vec2(34000,16000),
// 			// Vec2(28000,21500),
// 			// Vec2(16000,21500),
// 			// Vec2(10000,21500),
// 			// Vec2(10000,16000),
// 		});

// 		std::vector<Line2> segs = std::vector<Line2>({
// 			// Line2(Vec2(16000,10000), Vec2(16000,16000)),
// 			// Line2(Vec2(16000,16000), Vec2(16000,21500)),
// 			// Line2(Vec2(10000,16000), Vec2(16000,16000)),

// 		});

// 		std::vector<std::vector<Vec2>> tris;
// 		std::vector<std::vector<std::vector<Vec2>>> ret;
// 		std::vector<std::vector<Vec2>> poly;
// 		std::vector<std::vector<std::vector<Vec2>>> polys = std::vector<std::vector<std::vector<Vec2>>>();
		
// clock_t t0;
// clock_t t1;
// t0 = clock();
// t1 = clock();

// 		TriangleUtil::EarClip(hull, tris);
// std::cout << "tris\t" << tris.size() << std::endl;
// 		TriangleUtil::SplitTrisSegs(tris, segs, tris);
// std::cout << "tris\t" << tris.size() << std::endl;
// 		ret = SplitTrisSegs::CalcParseTrisSegs(tris, segs);
// std::cout << "ret \t" << ret.size() << std::endl;

// std::cout << "time\t" << clock() - t1 << "ms" << std::endl;

// 		for (size_t i = 0; i < tris.size(); i++)
// 		{
// 			std::cout << "# " << i << std::endl; 
// 			for (size_t j = 0; j < tris[i].size(); j++)
// 				std::cout << "\t" << tris[i][j].ToString() << "," << std::endl;
// 		}

// std::cout << "# -----------------------------------------------------------------" << std::endl;
	
// 		for (size_t i = 0; i < ret.size(); i++)
// 		{
// 			std::cout << "# ------------------------------" << std::endl;
// 			for (size_t j = 0; j < ret[i].size(); j++)
// 			{
// 				std::cout << "# " << i << ", " << j << std::endl; 
// 				for (size_t k = 0; k < ret[i][j].size(); k++)
// 					std::cout << "\t" << ret[i][j][k].ToString() << "," << std::endl;
// 			}
// 		}

// std::cout << "# -----------------------------------------------------------------" << std::endl;

// t1 = clock();

// 		for (size_t i = 0; i < ret.size(); i++)
// 		{
// 			poly = TriangleUtil::Tri2Poly(ret[i]);
// 			polys.emplace_back(poly);
// 		}

// std::cout << "time\t" << clock() - t1 << "ms" << std::endl;

// 		for (size_t i = 0; i < polys.size(); i++)
// 		{
// 			std::cout << "# ------------------------------" << std::endl;
// 			for (size_t j = 0; j < polys[i].size(); j++)
// 				for (size_t k = 0; k < polys[i][j].size(); k++)
// 					std::cout << polys[i][j][k].ToString() << std::endl;
// 		}

// std::cout << "time\t" << clock() - t0 << "ms" << std::endl;
// 	}

// 	return 0;

// 	{
// 		std::vector<Vec2> hull = std::vector<Vec2>({
// 			// Vec2(1, 1),
// 			// Vec2(5, 1),
// 			// Vec2(5, 5),
// 			// Vec2(4, 5),
// 			// Vec2(4, 4),
// 			// Vec2(2, 4),
// 			// Vec2(2, 5),
// 			// Vec2(1, 5),

// 			Vec2(1, 1),
// 			Vec2(6, 1),
// 			Vec2(6, 5),
// 			Vec2(1, 5),
// 		});

// 		std::vector<std::vector<Vec2>> tris = std::vector<std::vector<Vec2>>();

// 		std::vector<Line2> segs = std::vector<Line2>({
// 			// Line2(Vec2(1, 3), Vec2(5, 3)),
// 			// Line2(Vec2(4, 1), Vec2(4, 4)),
// 			// Line2(Vec2(3, 2), Vec2(3, 3)),
// 			// Line2(Vec2(4, 2), Vec2(3, 2)),

// 			Line2(Vec2(1, 3), Vec2(6, 3)),
// 			Line2(Vec2(5, 1), Vec2(5, 5)),
// 			Line2(Vec2(4, 3), Vec2(4, 4)),
// 			Line2(Vec2(5, 4), Vec2(4, 4)),
// 		});

// 		std::vector<std::vector<Vec2>> ret = std::vector<std::vector<Vec2>>();

// 		std::vector<Vec2> cof = std::vector<Vec2>();

// 		Vec2 c;

// 		size_t kk;
// 		size_t ll;

// 		Line2 l1;
// 		Line2 l2;

// 		bool f;

// 		std::vector<std::pair<size_t, size_t>> link = std::vector<std::pair<size_t, size_t>>();

// 		clock_t t1;

// t1 = clock();

// 		TriangleUtil::EarClip(hull, tris);

// std::cout << "TriangleUtil::EarClip         " << clock() - t1 << "ms" << std::endl;

// 		for (size_t i = 0; i < tris.size(); i++)
// 		{
// 			for (size_t j = 0; j < tris[i].size(); j++)
// 				std::cout << "\t" << std::setprecision(6) << tris[i][j].ToString() << "," << std::endl;
// 			std::cout << std::endl;
// 		}
		
// 		std::cout << "-------------------------------------" << std::endl;

// t1 = clock();

// 		TriangleUtil::SplitTrisSegs(tris, segs, ret);

// std::cout << "TriangleUtil::SplitTrisSegs   " << clock() - t1 << "ms" << std::endl;

// 		for (size_t i = 0; i < ret.size(); i++)
// 		{
// 			std::cout << "# " << i << std::endl;
// 			for (size_t j = 0; j < ret[i].size(); j++)
// 				std::cout << "\t" << std::setprecision(6) << ret[i][j].ToString() << "," << std::endl;
// 			std::cout << std::endl;
// 		}

// 		std::cout << "-------------------------------------" << std::endl;

// t1 = clock();

// 		for (size_t i = 0; i < ret.size(); i++)
// 		{
// 			c = Vec2::zero;
// 			for (size_t j = 0; j < ret[i].size(); j++)
// 				c += ret[i][j];
// 			c /= ret[i].size();
// 			cof.emplace_back(c);
// 		}

// 		for (size_t i = 0; i < ret.size(); i++)
// 		{
// 			for (size_t j = 0; j < ret.size(); j++)
// 			{
// 				if (i == j)
// 					continue;

// 				f = false;
// 				for (size_t k = 0; k < link.size(); k++)
// 					if ((i == link[k].first && j == link[k].second) ||
// 					    (i == link[k].second && j == link[k].first))
// 					{
// 						f = true;
// 						break;
// 					}
// 				if (f)
// 					continue;

// 				f = false;
// 				for (size_t k = 0; k < ret[i].size(); k++)
// 				{
// 					kk = fmod(k + 1, ret[i].size());
// 					l1 = Line2(ret[i][k], ret[i][kk]);
// 					for (size_t l = 0; l < ret[j].size(); l++)
// 					{
// 						ll = fmod(l + 1, ret[j].size());
// 						l2 = Line2(ret[j][l], ret[j][ll]);
// 						if (l1 == l2)
// 						{
// 							f = true;
// 							break;
// 						}
// 					}
// 					if (f)
// 						break;
// 				}
// 				if (!f)
// 					continue;

// 				l1 = Line2(cof[i], cof[j]);

// 				f = false;
// 				for (size_t k = 0; k < segs.size(); k++)
// 				{
// 					if (l1.isCross(segs[k]))
// 					{
// 						f = true;
// 						break;
// 					}
// 				}

// 				if (!f)
// 					link.emplace_back(i, j);
// 			}
// 		}

// std::cout << "calc1                         " << clock() - t1 << "ms" << std::endl;
		
// 		for (size_t i = 0; i < link.size(); i++)
// 		{
// 			std::cout << "# " << link[i].first << ", " << link[i].second << std::endl;
// 			std::cout << "\t[" << std::endl;
// 			std::cout << "\t\t" << cof[link[i].first].ToString() << "," << std::endl; 
// 			std::cout << "\t\t" << cof[link[i].second].ToString() << "," << std::endl; 
// 			std::cout << "\t]," << std::endl;
// 		}


// 		std::cout << "-------------------------------------" << std::endl;



// 		std::cout << "-------------------------------------" << std::endl;

// 		std::cout << "[" << std::endl;
// 		for (size_t i = 0; i < link.size(); i++)
// 			std::cout << "\t[" << link[i].first << ", " << link[i].second << "]," << std::endl;
// 		std::cout << "]" << std::endl;

// 		std::cout << "-------------------------------------" << std::endl;

// t1 = clock();

// 		std::vector<size_t> r = std::vector<size_t>();
// 		std::vector<std::vector<size_t>> rs = std::vector<std::vector<size_t>>();
// 		std::vector<std::vector<size_t>> adj;
// 		std::vector<bool> visited;
// 		std::stack<size_t> stack = std::stack<size_t>();
// 		size_t cur;

// 		adj.resize(ret.size());
// 		for (size_t i = 0; i < link.size(); i++)
// 		{
// 			adj[link[i].first].emplace_back(link[i].second);
// 			adj[link[i].second].emplace_back(link[i].first);
// 		}
// 		visited = std::vector<bool>(link.size(), false);

// 		while (true)
// 		{
// 			r.clear();

// 			cur = -1;
// 			for (size_t i = 0; i < visited.size(); i++)
// 				if (!visited[i])
// 				{
// 					cur = i;
// 					break;
// 				}
// 			if (cur == -1)
// 				break;

// 			stack.push(cur);
// 			while (!stack.empty())
// 			{
// 				cur = stack.top();
// 				stack.pop();

// 				if (visited[cur])
// 					continue;
				
// 				r.emplace_back(cur);
// 				visited[cur] = true;

// 				for (size_t i = 0; i < adj[cur].size(); i++)
// 				{
// 					if (visited[adj[cur][i]])
// 						continue;
// 					stack.push(adj[cur][i]);
// 				}
// 			}
// 			rs.emplace_back(r);
// 		}

// std::cout << "calc2                         " << clock() - t1 << "ms" << std::endl;

// 		for (size_t i = 0; i < rs.size(); i++)
// 		{
// 			for (size_t j = 0; j < rs[i].size(); j++)
// 				std::cout << rs[i][j] << ", ";
// 			std::cout << std::endl;
// 		}

// 		for (size_t i = 0; i < rs.size(); i++)
// 		{
// 			std::cout << "# --------------------------------------" << std::endl;
// 			for (size_t j = 0; j < rs[i].size(); j++)
// 			{
// 				std::cout << "# " << rs[i][j] << std::endl;
// 				for (size_t k = 0; k < ret[rs[i][j]].size(); k++)
// 					std::cout << "\t" << ret[rs[i][j]][k].ToString() << "," << std::endl;
// 			}
// 		}

// 	}

// 	return 0;

	// {
	// 	std::vector<std::vector<Vec2>> tris = std::vector<std::vector<Vec2>>({
	// 		std::vector<Vec2>({
	// 			Vec2(10000, 22000),
	// 			Vec2(10000, 10000),
	// 			Vec2(22000, 10000),
	// 		}),
	// 		std::vector<Vec2>({
	// 			Vec2(10000, 22000),
	// 			Vec2(22000, 10000),
	// 			Vec2(22000, 22000),
	// 		}),
	// 	});

	// 	// Line2 line = Line2(Vec2(10000, 16000), Vec2(22000, 16000));
	// 	Line2 line = Line2(Vec2(12000, 16000), Vec2(20000, 16000));

	// 	for (size_t i = 0; i < tris.size(); i++)
	// 		gPolys.emplace_back(tris[i]);
	// 	gSegs.emplace_back(line);

	// 	std::vector<std::vector<Vec2>> r = std::vector<std::vector<Vec2>>();

	// 	CalcTrianglesSplitLines(0, tris.size(), 0, 1);

	// 	r = std::vector<std::vector<Vec2>>(
	// 		gPolys.begin() + (size_t)tris.size(),
	// 		gPolys.end()
	// 	);

	// 	// TriangleUtil::SplitTrisSeg(tris, line, r);
	// 	for (size_t i = 0; i < r.size(); i++)
	// 	{
	// 		for (size_t j = 0; j < r[i].size(); j++)
	// 		{
	// 			std::cout << "(";
	// 			r[i][j].debug(0, false);
	// 			std::cout << ", ";
	// 			r[i][fmod(j + 1, r[i].size())].debug(0, false);
	// 			std::cout << "),\n";
	// 		}
	// 		std::cout << std::endl;
	// 	}
	// }

	// return 0;

	// {
	// 	std::vector<Vec2> poly = std::vector<Vec2>({
	// 		Vec2(1, 1),
	// 		Vec2(7, 1),
	// 		Vec2(7, 5),
	// 		Vec2(1, 5),
	// 	});

	// 	Line2 line = Line2(Vec2(0, 3), Vec2(8, 3));
	// 	std::vector<std::vector<Vec2>> tris = std::vector<std::vector<Vec2>>();
	// 	std::vector<std::vector<Vec2>> r = std::vector<std::vector<Vec2>>();

	// 	TriangleUtil::EarClip(poly, tris);
	// 	TriangleUtil::SplitTrisSeg(tris, line, r);
	// 	for (size_t i = 0; i < r.size(); i++)
	// 	{
	// 		for (size_t j = 0; j < r[i].size(); j++)
	// 		{
	// 			std::cout << "(";
	// 			r[i][j].debug(0, false);
	// 			std::cout << ", ";
	// 			r[i][fmod(j + 1, r[i].size())].debug(0, false);
	// 			std::cout << "),\n";
	// 		}
	// 		std::cout << std::endl;
	// 	}

	// }

	// return 0;

	// {
	// 	std::vector<Vec2> poly = std::vector<Vec2>({
	// 	Vec2(44100.000000, 21900.000000),
	// 	Vec2(44100.000000, 23400.000000),
	// 	Vec2(52802.000000, 23400.000000),
	// 	Vec2(56695.000000, 63690.000000),
	// 	Vec2(56147.000000, 63743.000000),
	// 	Vec2(56329.000000, 65634.000000),
	// 	Vec2(56877.000000, 65581.000000),
	// 	Vec2(57505.000000, 72101.000000),
	// 	Vec2(59446.000000, 71913.000000),
	// 	Vec2(59297.000000, 70371.000000),
	// 	Vec2(59144.000000, 68779.000000),
	// 	Vec2(58689.000000, 64051.000000),
	// 	Vec2(58535.000000, 62458.000000),
	// 	Vec2(58094.000000, 57879.000000),
	// 	Vec2(57941.000000, 56287.000000),
	// 	Vec2(57500.000000, 51708.000000),
	// 	Vec2(57342.000000, 50065.000000),
	// 	Vec2(56901.000000, 45486.000000),
	// 	Vec2(56743.000000, 43844.000000),
	// 	Vec2(56283.000000, 39066.000000),
	// 	Vec2(56125.000000, 37424.000000),
	// 	Vec2(55919.000000, 35284.000000),
	// 	Vec2(57809.000000, 35101.000000),
	// 	Vec2(57215.000000, 28930.000000),
	// 	Vec2(54827.000000, 29160.000000),
	// 	Vec2(54314.000000, 23400.000000),
	// 	Vec2(61200.000000, 23400.000000),
	// 	Vec2(61200.000000, 23600.000000),
	// 	Vec2(66550.000000, 23600.000000),
	// 	Vec2(66550.000000, 21425.000000),
	// 	Vec2(53100.000000, 21425.000000),
	// 	Vec2(53100.000000, 21900.000000),
	// 	Vec2(49770.000000, 21900.000000),
	// 	Vec2(49770.000000, 21000.000000),
	// 	Vec2(47100.000000, 21000.000000),
	// 	Vec2(47100.000000, 21900.000000),
	// 	});

	// 	std::vector<std::vector<Vec2>> hole = std::vector<std::vector<Vec2>>({
	// 		// std::vector<Vec2>({
	// 		// 	Vec2(16000,32000 ),
	// 		// 	Vec2(16000,16000 ),
	// 		// 	Vec2(36000,16000 ),
	// 		// 	Vec2(36000,32050 ),
	// 		// }),
	// 		// std::vector<Vec2>({
	// 		// 	Vec2(44000,32000 ),
	// 		// 	Vec2(44000,16000 ),
	// 		// 	Vec2(64000,16000 ),
	// 		// 	Vec2(64000,32000 ),
	// 		// }),
	// 	});

	// 	std::vector<std::vector<Vec2>> r = std::vector<std::vector<Vec2>>();
	// 	// std::cout << TriangleUtil::EarClip(poly, r, 5000) << std::endl;
	// 	std::cout << TriangleUtil::EarClipHoles(poly, hole, r, 2) << std::endl;
	// 	std::cout << "[" << std::endl;
	// 	for (size_t i = 0; i < r.size(); i++)
	// 	{
	// 		for (size_t j = 0; j < r[i].size(); j++)
	// 		{
	// 			size_t jj = fmod(j + 1, r[i].size());
	// 			std::cout << "    (";
	// 			r[i][j].debug(0, false);
	// 			std::cout << ", ";
	// 			r[i][jj].debug(0, false);
	// 			std::cout << ")," << std::endl;
	// 		}
	// 		std::cout << std::endl;
	// 	}
	// 	std::cout << "]" << std::endl;
	// }

	// return 0;

	// {
	// 	std::vector<Vec2> tri = std::vector<Vec2>({
	// 		Vec2(2, 1),
	// 		Vec2(5, 1),
	// 		Vec2(3, 4),
	// 	});
	// 	Line2 seg = Line2(Vec2(2, 3), Vec2(4, 0));
	// 	std::vector<std::vector<Vec2>> r = std::vector<std::vector<Vec2>>();

	// 	std::cout << true << "\t" << TriangleUtil::SplitTriSeg(tri, seg, r) << std::endl;
	// 	std::cout << "[" << std::endl;
	// 	for (size_t i = 0; i < r.size(); i++)
	// 	{
	// 		for (size_t j = 0; j < r[i].size(); j++)
	// 		{
	// 			size_t jj = fmod(j + 1, r[i].size());
	// 			std::cout << "    (";
	// 			r[i][j].debug(0, false);
	// 			std::cout << ", ";
	// 			r[i][jj].debug(0, false);
	// 			std::cout << ")," << std::endl;
	// 		}
	// 		std::cout << std::endl;
	// 	}
	// 	std::cout << "]" << std::endl;
	// }

	// return 0;

	// {
	// 	std::vector<Vec2> v = std::vector<Vec2>({
	// 		Vec2(1,1),
	// 		Vec2(6,1),
	// 		Vec2(6,4),
	// 		Vec2(5,4),
	// 		Vec2(5,2),
	// 		Vec2(3,2),
	// 		Vec2(3,4),
	// 		Vec2(1,4),

	// 		// Vec2(1,4),
	// 		// Vec2(1,1),
	// 		// Vec2(3,2),

	// 		// Vec2(1,4),
	// 		// Vec2(3.5,3.5),
	// 		// Vec2(3,2),

	// 		// Vec2(5,2),
	// 		// Vec2(3,2),
	// 		// Vec2(1,1),
	// 	});
	// 	// Line2 seg = Line2(Vec2(0, 3), Vec2(7, 2));
	// 	Line2 seg = Line2(Vec2(5,5), Vec2(0,0));
	// 	std::vector<Line2> segs = std::vector<Line2>({
	// 		// Line2(Vec2(3,6), Vec2(1,-1)),
	// 		// Line2(Vec2(3,6), Vec2(7,0)),

	// 		Line2(Vec2(5,5), Vec2(0,0)),
	// 		Line2(Vec2(5,5), Vec2(7,0)),

	// 		// Line2(Vec2(5,5), Vec2(0,0)),
	// 		// Line2(Vec2(4,4.5), Vec2(0,3)),
	// 	});
	// 	std::vector<std::vector<Vec2>> r = std::vector<std::vector<Vec2>>();
	// 	std::vector<std::vector<Vec2>> rr = std::vector<std::vector<Vec2>>();
	// 	clock_t t;

	// 	t = clock();
	// 	std::cout << true << "\t" << TriangleUtil::EarClip(v, r) << "\ttime:" << clock() - t << std::endl;

	// 	std::cout << "[" << std::endl;
	// 	for (size_t i = 0; i < r.size(); i++)
	// 	{
	// 		for (size_t j = 0; j < r[i].size(); j++)
	// 		{
	// 			size_t jj = fmod(j + 1, r[i].size());
	// 			std::cout << "    (";
	// 			r[i][j].debug(0, false);
	// 			std::cout << ", ";
	// 			r[i][jj].debug(0, false);
	// 			std::cout << ")," << std::endl;
	// 		}
	// 		std::cout << std::endl;
	// 	}
	// 	std::cout << "]" << std::endl << std::endl;
		
	// 	t = clock();
	// 	std::cout << true << "\t" << TriangleUtil::SplitTrisSeg(r, seg, rr) << "\ttime:" << clock() - t << std::endl;
	// 	std::cout << "rr:" << rr.size() << std::endl;
	// 	std::cout << "[" << std::endl;
	// 	for (size_t i = 0; i < rr.size(); i++)
	// 	{
	// 		for (size_t j = 0; j < rr[i].size(); j++)
	// 		{
	// 			size_t jj = fmod(j + 1, rr[i].size());
	// 			std::cout << "    (";
	// 			rr[i][j].debug(0, false);
	// 			std::cout << ", ";
	// 			rr[i][jj].debug(0, false);
	// 			std::cout << ")," << std::endl;
	// 		}
	// 		std::cout << std::endl;
	// 	}
	// 	seg.debug(0, 0, false);
	// 	std::cout << ", " << std::endl;
	// 	std::cout << "]" << std::endl << std::endl;
		
	// 	t = clock();
	// 	std::cout << true << "\t" << TriangleUtil::SplitTrisSegs(r, segs, rr) << "\ttime:" << clock() - t << std::endl;
	// 	std::cout << "rr:" << rr.size() << std::endl;
	// 	std::cout << "[" << std::endl;
	// 	for (size_t i = 0; i < rr.size(); i++)
	// 	{
	// 		for (size_t j = 0; j < rr[i].size(); j++)
	// 		{
	// 			size_t jj = fmod(j + 1, rr[i].size());
	// 			std::cout << "    (";
	// 			rr[i][j].debug(0, false);
	// 			std::cout << ", ";
	// 			rr[i][jj].debug(0, false);
	// 			std::cout << ")," << std::endl;
	// 		}
	// 		std::cout << std::endl;
	// 	}
	// 	for (size_t i = 0; i < segs.size(); i++)
	// 	{
	// 		segs[i].debug(0, 0, false);
	// 		std::cout << ", " << std::endl;
	// 	}
	// 	std::cout << "]" << std::endl << std::endl;
	// }

	// return 0;


	// {
	// 	std::vector<Line2> s = std::vector<Line2>({
	// 		Line2(Vec2(4, 4), Vec2(2, 4)),
	// 		Line2(Vec2(2, 6), Vec2(2, 5)),
	// 		Line2(Vec2(4, 6), Vec2(4, 5)),
	// 		Line2(Vec2(5, 7), Vec2(1, 7)),
	// 	});
	// 	s = Segs2Polys::extendMarginIntersection(s, 1);
	// 	for (size_t i = 0; i < s.size(); i++)
	// 	{
	// 		s[i].debug(0, 0, false);
	// 		std::cout << "," << std::endl;	
	// 	}
	// }

	// return 0;

	// {
	// 	test();
	// }

	// return 0;

	// {
	// 	Clear();

	// 	std::vector<Vec2> hull = std::vector<Vec2>({
	// 		Vec2(10000, 10000),
	// 		Vec2(34000, 10000),
	// 		Vec2(34000, 16000),
	// 		Vec2(28000, 21500),
	// 		Vec2(10000, 21500),
	// 		});

	// 	std::vector<std::vector<Vec2>> holes = std::vector<std::vector<Vec2>>({
	// 		{
	// 			Vec2(14000,12000),
	// 			Vec2(28000,12000),
	// 			Vec2(28000,18000),
	// 			Vec2(24000,18000),
	// 			Vec2(22000,14000),
	// 			Vec2(18000,14000),
	// 			Vec2(16000,18000),

	// 		},
	// 	});

	// 	Clear();
	// 	AddPoly();
	// 	for (size_t i = 0; i < hull.size(); i++)
	// 		AddPolyVec(0, hull[i].x, hull[i].y, 0);

	// 	size_t holeidxno = holes.size();
	// 	long holeidx[1];
	// 	for (size_t i = 0; i < holes.size(); i++)
	// 	{
	// 		AddPoly();
	// 		holeidx[i] = i + 1;
	// 		for (size_t j = 0; j < holes[i].size(); j++)
	// 			AddPolyVec(i + 1, holes[i][j].x, holes[i][j].y, i + 1);
	// 	}

	// 	std::cout << "CalcEarClipHoles\t" << CalcEarClipHoles(0, holeidxno, &(holeidx[0]), 5000) << std::endl;
	// 	size_t pns = holes.size() + 1;
	// 	size_t pne = GetPolyCnt() - pns;
	// 	size_t s;
	// 	double x1, y1, x2, y2;
	// 	long id1, id2;
	// 	size_t jj;
		
	// 	std::cout << fmod(11, 10) << std::endl;
	// 	std::cout << fmod(-1, 10) << std::endl;


	// 	std::cout << "pns:" << pns << std::endl;
	// 	std::cout << "pne:" << pne << std::endl;
	// 	std::cout << "dif:" << pne - pns << std::endl;
	// 	std::cout << "GetPolyCnt:" << GetPolyCnt() << std::endl;
	// 	for (size_t i = pns; i < pne; i++)
	// 	{
	// 		s = GetPolyVecCnt(i);
	// 		std::cout << s << std::endl;
	// 		for (size_t j = 0; j < s; j++)
	// 		{
	// 			jj = fmod(j + 1, s);
	// 			std::cout << j << "\t" << jj << "\t" << s << std::endl;
	// 			GetPolyVec(i, j, &x1, &y1, &id1);
	// 			GetPolyVec(i, jj, &x2, &y2, &id2);
	// 			std::cout << "("
	// 				<< "(" << x1 << "," << y1 << "),"
	// 				<< "(" << x2 << "," << y2 << ")"
	// 				<< ")," << std::endl;
	// 		}
	// 		std::cout << std::endl;
	// 	}
	// }

	// return 0;

	// {
	// 	std::vector<Vec2> hull = std::vector<Vec2>({
	// 		Vec2(10000, 10000),
	// 		Vec2(34000, 10000),
	// 		Vec2(34000, 16000),
	// 		Vec2(28000, 21500),
	// 		Vec2(10000, 21500),
	// 	});

	// 	std::vector<std::vector<Vec2>> holes = std::vector<std::vector<Vec2>>({
	// 		{
	// 			Vec2(14000,12000),
	// 			Vec2(28000,12000),
	// 			Vec2(28000,18000),
	// 			Vec2(24000,18000),
	// 			Vec2(22000,14000),
	// 			Vec2(18000,14000),
	// 			Vec2(16000,18000),

	// 		},
	// 	});

	// 	size_t ii;
	// 	for (size_t i = 0; i < hull.size(); i++)
	// 	{
	// 		ii = Mathf::repeat(i + 1, hull.size());
	// 		std::cout << "("
	// 			<< "(" << hull[i].x << "," << hull[i].y << "),"
	// 			<< "(" << hull[ii].x << "," << hull[ii].y << ")"
	// 			<< ")," << std::endl;
	// 	}
		
	// 	std::cout << "-------------------------------------------" << std::endl;

	// 	size_t jj;
	// 	for (size_t i = 0; i < holes.size(); i++)
	// 	{
	// 		for (size_t j = 0; j < holes[i].size(); j++)
	// 		{
	// 			jj = Mathf::repeat(j + 1, holes[i].size());
	// 			std::cout << "("
	// 				<< "(" << holes[i][j].x << "," << holes[i][j].y << "),"
	// 				<< "(" << holes[i][jj].x << "," << holes[i][jj].y << ")"
	// 				<< ")," << std::endl;
	// 		}
	// 		std::cout << std::endl;
	// 	}
		
	// 	std::cout << "-------------------------------------------" << std::endl;

	// 	std::vector<std::vector<Vec2>> ret;

	// 	TriangleUtil::EarClipHoles(hull, holes, ret, 5000);
	// 	for (size_t i = 0; i < ret.size(); i++)
	// 	{
	// 		for (size_t j = 0; j < ret[i].size(); j++)
	// 		{
	// 			jj = Mathf::repeat(j + 1, ret[i].size());
	// 			std::cout << "("
	// 				<< "(" << ret[i][j].x << "," << ret[i][j].y << "),"
	// 				<< "(" << ret[i][jj].x << "," << ret[i][jj].y << ")"
	// 				<< ")," << std::endl;
	// 		}
	// 		std::cout << std::endl;
	// 	}
	// }

	// return 0;

	////{
	////	std::cout << "DBL_MAX\t" << DBL_MAX << std::endl;
	////	std::cout << "infinity\t" << std::numeric_limits<double>::infinity() << std::endl;
	////	std::cout << true << "\t" << (DBL_MAX == std::numeric_limits<double>::infinity()) << std::endl;
	////}

	//return 0;

	// {
	// 	std::vector<Vec2> vs = std::vector<Vec2>({
	// 		Vec2(10000, 10000),
	// 		Vec2(34000, 10000),
	// 		Vec2(34000, 16000),
	// 		Vec2(28000, 21500),
	// 		Vec2(10000, 21500),
	// 	});
		
	// 	double x;
	// 	double y;
	// 	long id;

	// 	Clear();
	// 	AddPoly();
	// 	for (size_t i = 0; i < vs.size(); i++)
	// 		AddPolyVec(0, vs[i].x, vs[i].y, 0);
	// 	std::cout << "b:" << gPolys.size() << std::endl;
	// 	CalcEarClip(0, 50);
	// 	std::cout << "a:" << gPolys.size() << std::endl;
	// 	std::cout << "aa:" << GetPolyCnt() << std::endl;
	// 	for (size_t i = 1; i < GetPolyCnt(); i++)
	// 	{
	// 		std::cout << "aav:" << GetPolyVecCnt(i) << std::endl;
	// 		for (size_t j = 0; j < GetPolyVecCnt(i); j++)
	// 		{
	// 			GetPolyVec(i, j, &x, &y, &id);
	// 			std::cout << "(" << x << "," << y << ")\t" << id << std::endl;
	// 		}
	// 		std::cout << std::endl;
	// 	}
	// }

	// return 0;

	// {
	// 	std::vector<Vec2> vs = std::vector<Vec2>({ 
	// 		Vec2(10000, 10000),
	// 		Vec2(34000, 10000),
	// 		Vec2(34000, 16000),
	// 		Vec2(28000, 21500),
	// 		Vec2(10000, 21500),
	// 	});

	// 	std::vector<std::vector<Vec2>> ret;

	// 	std::cout << TriangleUtil::EarClip(vs, ret, 1000) << std::endl;
	// 	for (size_t i = 0; i < ret.size(); i++)
	// 	{
	// 		for (size_t j = 0; j < ret.size(); j++)
	// 			ret[i][j].debug();
	// 		std::cout << std::endl;
	// 	}
	// }

	// return 0;

	// {
	// 	std::vector<Vec2> vs;
	// 	std::vector<std::vector<Vec2>> ret;

	// 	std::vector<std::vector<Cell>> cells = Segs2Polys::createGrid(segs);
	// 	std::vector<stCellLink> links = Segs2Polys::getLinkRaycast(cells);
	// 	std::vector<std::vector<Vec2>> polys = Segs2Polys::trailCells(cells, links);
	// 	vs = polys[0];
	// 	for (size_t i = 0; i < vs.size(); i++)
	// 		vs[i].debug();

	// 	std::cout << "true:" << true << "\t" << TriangleUtil::EarClip(vs, ret) << std::endl;
	// 	for (size_t i = 0; i < ret.size(); i++)
	// 	{
	// 		for (size_t j = 0; j < ret[i].size(); j++)
	// 			ret[i][j].debug();
	// 		std::cout << std::endl;
	// 	}

	// }

	// return 0;

	// {
	// 	std::cout << Mathf::repeat(1, (size_t)0) << std::endl;
	// }

	// return 0;

	// {
	// 	double r = 5;
	// 	std::vector<Line2> ls;

	// 	//ls = Segs2Polys::arc2segs(Vec2(0.2, 0.2), Vec2(1, 0), Vec2(1, 1), 0, -r);
	// 	//for (auto l : ls) { l.debug(0, 0, false);	std::cout << "," << std::endl; }
	// 	//std::cout << std::endl;

	// 	//ls = Segs2Polys::arc2segs(Vec2(0.7, 0.7), Vec2(0, 1), Vec2(0, 0), 0, -r);
	// 	//for (auto l : ls) { l.debug(0, 0, false);	std::cout << "," << std::endl; }
	// 	//std::cout << std::endl;
	// 	//
	// 	//ls = Segs2Polys::arc2segs(Vec2(1, 1), Vec2(3, 1), Vec2(2, 2), 0, r);
	// 	//for (auto l : ls) { l.debug(0, 0, false);	std::cout << "," << std::endl; }
	// 	//std::cout << std::endl;
		
	// 	//ls = Segs2Polys::arc2segs(Vec2(1, 1), Vec2(3, 1), Vec2(2, 4), 0, r);
	// 	//for (auto l : ls) { l.debug(0, 0, false);	std::cout << "," << std::endl; }
	// 	//std::cout << std::endl;
	// 	//
	// 	//ls = Segs2Polys::arc2segs(Vec2(1, 1), Vec2(3, 1), Vec2(2, 10), 0, r);
	// 	//for (auto l : ls) { l.debug(0, 0, false);	std::cout << "," << std::endl; }
	// 	//std::cout << std::endl;
	// 	//
	// 	//ls = Segs2Polys::arc2segs(Vec2(3, 2), Vec2(2, 0.6), Vec2(1.5, 2), 0, r);
	// 	//for (auto l : ls) { l.debug(0, 0, false);	std::cout << "," << std::endl; }
	// 	//std::cout << std::endl;
	// 	//
	// 	//ls = Segs2Polys::arc2segs(Vec2(2 + 1, 0.6), Vec2(3 + 1, 2), Vec2(1.5 + 1, 2), 0, r);
	// 	//for (auto l : ls) { l.debug(0, 0, false);	std::cout << "," << std::endl; }
	// 	//std::cout << std::endl;
	// 	//
	// 	//ls = Segs2Polys::arc2segs(Vec2(1, 3), Vec2(3, 3), Vec2(2, 20), 0, r);
	// 	//for (auto l : ls) { l.debug(0, 0, false);	std::cout << "," << std::endl; }
	// 	//std::cout << std::endl;

	// 	//ls = Segs2Polys::arc2segs(Vec2(-1, 1), Vec2(1, 1), Vec2(0, 0), 0, r);
	// 	//for (auto l : ls) { l.debug(0, 0, false);	std::cout << "," << std::endl; }
	// 	//std::cout << std::endl;

	// 	//ls = Segs2Polys::arc2segs(Vec2(-1, 1), Vec2(1, 1), Vec2(0, 0), 0, -r);
	// 	//for (auto l : ls) { l.debug(0, 0, false);	std::cout << "," << std::endl; }
	// 	//std::cout << std::endl;

	// 	//ls = Segs2Polys::arc2segs(Vec2(1, 1), Vec2(-1, 1), Vec2(0, 0), 0, r);
	// 	//for (auto l : ls) { l.debug(0, 0, false);	std::cout << "," << std::endl; }
	// 	//std::cout << std::endl;

	// 	//ls = Segs2Polys::arc2segs(Vec2(1, 1), Vec2(-1, 1), Vec2(0, 0), 0, -r);
	// 	//for (auto l : ls) { l.debug(0, 0, false);	std::cout << "," << std::endl; }
	// 	//std::cout << std::endl;

	// 	//ls = Segs2Polys::arc2segs(Vec2(1, 1), Vec2(-1, 1), Vec2(0, 0), 0, -r);
	// 	//for (auto l : ls) { l.debug(0, 0, false);	std::cout << "," << std::endl; }
	// 	//std::cout << std::endl;

	// 	bool ret;
	// 	size_t en;
	// 	Vec2 s;
	// 	Vec2 e;
	// 	Vec2 c;
	// 	long id;

	// 	std::vector<Vec2> vs;

	// 	ls.emplace_back(Line2(Vec2(1, 0), Vec2(1, 1), false));
	// 	for (auto l : Segs2Polys::arc2segs(Vec2(1, 1), Vec2(-1, 1), Vec2(0, 0), 100, -r))
	// 		ls.emplace_back(l);
	// 	ls.emplace_back(Line2(Vec2(-1, 1), Vec2(-1, 0), false));
	// 	//for (auto l : ls) { l.debug(0, 0, false);	std::cout << "," << std::endl; }
	// 	//ret = Segs2Polys::segs2arc(ls, 1, -r, 1, 3, &en, &s, &e, &c, &id);
	// 	std::cout << "ls  : " << ls.size();
	// 	for (auto l : ls)
	// 		vs.emplace_back(l.s);
	// 	vs.emplace_back(ls.back().e);
	// 	for (auto v : vs) { v.debug(0, false);	std::cout << "," << std::endl; }

	// 	std::cout << "vs  : " << vs.size();

	// 	ret = Segs2Polys::poly2arc(vs, 1, -r, 1, 3, &en, &s, &e, &c, &id);

	// 	std::cout << std::endl;
	// 	std::cout << "true: " << true << std::endl;
	// 	std::cout << "ret : " << ret << std::endl;
	// 	std::cout << "en  : " << en << std::endl;
	// 	std::cout << "s   : ";
	// 	s.debug();
	// 	std::cout << "e   : ";
	// 	e.debug();
	// 	std::cout << "c   : ";
	// 	c.debug();
	// 	std::cout << "id  : " << id << std::endl;
	// 	std::cout << std::endl;

	// 	ls = Segs2Polys::arc2segs(s, e, Vec2(0, 0), 0, -r);
	// 	for (auto l : ls) { l.debug(0, 0, false);	std::cout << "," << std::endl; }
	// }

	// return 0;

	// {
	// 	Line2 l1 = Line2(Vec2(1, 3), Vec2(3, 3));
	// 	Line2 l2 = Line2(Vec2(2, 1), Vec2(3, 2));
	// 	std::cout << Line2::isCrossInf(l1, l2) << std::endl;
	// 	Vec2 c = Line2::getCrossInf(l1, l2);
	// 	c.debug();
	// 	std::cout << l1.distancePointLine(c) << std::endl;

	// 	std::vector<Line2> l = std::vector<Line2>({
	// 		Line2(Vec2(1, 3), Vec2(3, 3)),
	// 		Line2(Vec2(2, 1), Vec2(3, 2))
	// 	});

	// 	l = Segs2Polys::extendMarginIntersection(l, 1);
	// 	std::cout << l.size() << std::endl;

	// 	for (size_t i = 0; i < l.size(); i++)
	// 		l[i].debug();
	// }

	// return 0;

	// {
	// 	test();
	// }

	// return 0;

	// {
	// 	if (argc != 2 + 1)
	// 		return 1;

	// 	std::cout << segs[0].ToString() << std::endl;

	// 	printf("rfile\t%s\n", argv[1]);
	// 	printf("wfile\t%s\n\n", argv[2]);

	// 	std::string rfile = argv[1];
	// 	std::string wfile = argv[2];

	// 	// std::vector<std::string> r = Csv2Line2::readfile(rfile);
	// 	// for (std::string s : r)
	// 	// 	std::cout << s << std::endl;
	// }

	return 0;
}
#endif
