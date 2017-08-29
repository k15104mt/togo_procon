#include"piece.hpp"
#include<algorithm>
#include<cstdlib>	//// abs() for integer
#define PI 3.14159265358979

//どれだけ回転するかと基準点の座標を送り回転後の座標を返す
Point PointRotate(double angle, Point zeroPoint) {
	Point r;
	r.x = static_cast<int>(round(zeroPoint.x*cos(angle) - zeroPoint.y*sin(angle)));
	r.y = static_cast<int>(round(zeroPoint.x*sin(angle) + zeroPoint.y*cos(angle)));
	return r;
}

//頂点の順を逆に(反時計回り→時計回り)
void reverse(std::vector<Point> &data) {
	int	i, tmpx, tmpy,n=data.size();
	for (i = 0; i < n/ 2; ++i) {
		tmpx = data[i].x;
		tmpy = data[i].y;
		data[i].x = data[n - i - 1].x;
		data[i].y = data[n - i - 1].y;
		data[n - i - 1].x = tmpx;
		data[n - i - 1].y = tmpy;
	}
}

Piece::Piece(std::vector<Point> &data){
  int num = data.size();     				//頂点数
  std::vector<Point> tmp(num); 			//1回転パターンの一時格納配列
  
  for (int i = 0; i<4; i++){  			//4回回転(90度ずつ)
	Point min;	//ずらすために取得する座標最小値
	//printf("--回転パターン[%dπ/2]--\n", i); //debug
	for (int j = 0; j < num; j++) {
	  tmp[j] = PointRotate((1.0 / 2.0)*i*PI, data[j]);
	  if (j == 0) { min.x = tmp[j].x; min.y = tmp[j].y; }
	  min.x = std::min(min.x, tmp[j].x);
	  min.y = std::min(min.y, tmp[j].y);
	  //printf("(%d,%d)\n", tmp[j].x, tmp[j].y); //debug
	}
	move(tmp, Point(-min.x,-min.y));
	point.push_back(tmp); //一つの回転パターンをpushback

	//printf("--反転パターン[%dπ/2]--\n", i);
	for (int j = 0; j < num; j++) {
		tmp[j] = tmp[j] = PointRotate((1.0 / 2.0)*i*PI, data[j]);
		tmp[j].x *= -1;
		if (j == 0) { min.x = tmp[j].x; min.y = tmp[j].y; }
		min.x = std::min(min.x, tmp[j].x);
		min.y = std::min(min.y, tmp[j].y);
		//printf("(%d,%d)\n", tmp[j].x, tmp[j].y); //debug
	}
	move(tmp, Point(-min.x, -min.y));
	reverse(tmp);
	point.push_back(tmp); //一つの回転パターンをpushback
  }
}

std::vector<std::vector<Point>> Piece::getPoint(){
  return point;
}

int Piece::getSize() {
  return static_cast<int>(point[0].size());
}
