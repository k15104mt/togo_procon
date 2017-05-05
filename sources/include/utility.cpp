#include"utility.hpp"


//二つの線分の始点、終点から、線分同士が交差しているかを返す
//線分が平行な場合は交差していないものとなる
int crossLine(Point Line1Start,Point Line1End,
              Point Line2Start,Point Line2End){
    
  if((((Line1Start.x-Line1End.x)*(Line2Start.y-Line1Start.y)+
  (Line1Start.y-Line1End.y)*(Line1Start.x-Line2Start.x))*
		((Line1Start.x-Line1End.x)*(Line2End.y-Line1Start.y)+
    (Line1Start.y-Line1End.y)*(Line1Start.x-Line2End.x))<0) &&
		(((Line2Start.x-Line2End.x)*(Line1Start.y-Line2Start.y)+
    (Line2Start.y-Line2End.y)*(Line2Start.x-Line1Start.x))*
		((Line2Start.x-Line2End.x)*(Line1End.y-Line2Start.y)+
    (Line2Start.y-Line2End.y)*(Line2Start.x-Line1End.x))<0)){
			return 1;
		}
    
	return 0;
}