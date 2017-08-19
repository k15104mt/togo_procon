#include"piece.hpp"
#define PI 3.14159265358979

//どれだけ回転するかと基準点の座標を送り回転後の座標を返す
Point PointRotate(double angle, Point zeroPoint) {
	Point r;
	r.x = round(zeroPoint.x*cos(angle) - zeroPoint.y*sin(angle));
	r.y = round(zeroPoint.x*sin(angle) + zeroPoint.y*cos(angle));
	return r;
}

Piece::Piece(std::vector<Point> &data){
  int num = data.size();     //頂点数
  std::vector<Point> tmp(num); //1回転パターンの一時格納配列

  for (int i = 0; i<4; i++){  //4回回転(90度ずつ)
    //printf("--回転パターン[%dπ/2]--\n", i); //debug
    for (int j = 0; j < num; j++) {
      tmp[j] = PointRotate((1.0 / 2.0)*i*PI, data[j]);
			//printf("%d,%d\n",tmp[j].x, tmp[j].y); //debug
    }
    point.push_back(tmp); //一つの回転パターンをpushback
  }
}

std::vector<std::vector<Point>> Piece::getPoint(){
  return point;
}

int Piece::getSize() {
  return static_cast<int>(point[0].size());
}
