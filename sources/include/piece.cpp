#include"piece.hpp"
#include<cstdlib>	//// abs() for integer
#define PI 3.14159265358979

//どれだけ回転するかと基準点の座標を送り回転後の座標を返す
Point PointRotate(double angle, Point zeroPoint) {
	Point r;
	r.x = round(zeroPoint.x*cos(angle) - zeroPoint.y*sin(angle));
	r.y = round(zeroPoint.x*sin(angle) + zeroPoint.y*cos(angle));
	return r;
}

//回転後ずらすためにxとyの最小値を取得
Point MinPoint(std::vector<Point> &data){
	Point min;
	min.x=0;min.y=0;
	for(int i=0;i<data.size();i++){
		if(min.x>std::abs(data[i].x))min.x=data[i].x;
		if(min.y>std::abs(data[i].y))min.y=data[i].y;
	}
	return min;
}

Piece::Piece(std::vector<Point> &data){
  int num = data.size();     				//頂点数
  std::vector<Point> tmp(num); 			//1回転パターンの一時格納配列
	Point min;min.x=0,min.y=0;				//ずらすために取得する座標最小値

	for (int i = 0; i<4; i++){  			//4回回転(90度ずつ)
    //printf("--回転パターン[%dπ/2]--\n", i); //debug
    for (int j = 0; j < num; j++) {
      tmp[j] = PointRotate((1.0 / 2.0)*i*PI, data[j]);
			//printf("前%d,%d\n",tmp[j].x, tmp[j].y); //debug
			if(min.x>tmp[j].x)min.x=tmp[j].x;
			if(min.y>tmp[j].y)min.y=tmp[j].y;
    }

		if(min.x<0){
			for(int j=0;j<num;j++){
				tmp[j].x+=std::abs(min.x);
				//printf("後%d,%d\n",tmp[j].x, tmp[j].y); //debug
			}
		}
		if(min.y<0){
			for(int j=0;j<num;j++){
				tmp[j].y+=std::abs(min.y);
				//printf("後%d,%d\n",tmp[j].x, tmp[j].y); //debug
			}
		}
    point.push_back(tmp); //一つの回転パターンをpushback
		min.x=0; min.y=0;
  }
}

std::vector<std::vector<Point>> Piece::getPoint(){
  return point;
}

int Piece::getSize() {
  return static_cast<int>(point[0].size());
}
