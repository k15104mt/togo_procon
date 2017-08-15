#include"utility.hpp"


//二つの線分の始点、終点から、線分同士が交差しているかを返す
//線分が平行な場合は交差していないものとなる
int cl(Point &Line1Start,Point &Line1End,
              Point &Line2Start,Point &Line2End){
    
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

int crossLine(std::vector<Point> &data1,std::vector<Point> &data2){
  for(int i=0;i<data1.size();++i){
    for(int j=0;j<data2.size();++j){
      if(cl(data1[i],data1[(i+1)%static_cast<int>(data1.size())],
                   data2[j],data2[(j+1)%static_cast<int>(data2.size())])){
        return 1;
      }
    }
  }
	return 0;
}

//data1にdata２の頂点がすべて内包されてい
int inPolygon(std::vector<Point> &data1,std::vector<Point> &data2){
  //data2のi番目の点がdata1のポリゴン内にあるかどうか
  for(int i=0;i<data2.size();++i){
    double x = 0.0;
    for(int j=0;j<data1.size();++j){
      Vector a = data1[(j+1)%data1.size()]-data2[i];
      Vector b = data1[j]-data2[i];
      
      if(dot(a,b) <= 0 && cross(a,b) == 0){
        break;
      }
      
      //角度を積極的に足していこう
      if(cross(a,b)<0) x+=(acos((double)(dot(a,b))/(a.size()*b.size()))*180.0/acos(-1.0));
  		if(cross(a,b)>0) x+=-1.0*(acos((double)(dot(a,b))/(a.size()*b.size()))*180.0/acos(-1.0));	
    }
    
    //天上にもなく、360じゃないときはさっさと終わらせる
    //ここ多分誤差を考慮する感じにしたほうがいい
    if(x!=360){
	  return 0;
    }
  }
  
  return 1;
}


int collisionPiece(std::vector<Point> &data1,std::vector<Point> &data2){
  //線が一つでも交わっていたら当たっている（それはそう）
  if(crossLine(data1,data2)){
    return 1;
  }
  
  if(inPolygon(data1,data2) || inPolygon(data2,data1)){
    return 1;
  }
  return 0;  
}
