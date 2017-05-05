#ifndef _UTILITY_HPP_
#define _UTILITY_HPP_

struct Point{
  int x;
  int y;
};

//二つの線分の始点、終点から、線分同士が交差しているかを返す
//線分が平行な場合は交差していないものとなる
int crossLine(Point Line1Start,Point Line1End,Point Line2Start,Point Line2End);

#endif
