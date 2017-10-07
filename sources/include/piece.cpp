#include"piece.hpp"
#include<algorithm>
#include<cstdlib>	// fabs()
#define NOMINMAX
#include<color.hpp>
#define PI 3.14159265358979

//std::vector<Point> calculateEdge(std::vector<Point> &areaPoint, std::vector<std::vector<std::pair<int, int>>> &shapeNum);
std::vector<Point> calculateEdge(std::vector<Point> &areaPoint, std::vector<std::vector<int>> &edgeNum);
double calculateAngle(std::vector<Point> &point);
double calculateSide(std::vector<Point> &point);

//どれだけ回転するかと基準点の座標を送り回転後の座標を返す
Point PointRotate(double angle, Point zeroPoint) {
  Point r;
  r.x = static_cast<int>(round(zeroPoint.x*cos(angle) - zeroPoint.y*sin(angle)));
  r.y = static_cast<int>(round(zeroPoint.x*sin(angle) + zeroPoint.y*cos(angle)));
  return r;
}

//頂点の順を逆に(反時計回り→時計回り)
void reverse(std::vector<Point> &data) {
  int	i, tmpx, tmpy, n = data.size();
  for (i = 0; i < n / 2; ++i) {
	tmpx = data[i].x;
	tmpy = data[i].y;
	data[i].x = data[n - i - 1].x;
	data[i].y = data[n - i - 1].y;
	data[n - i - 1].x = tmpx;
	data[n - i - 1].y = tmpy;
  }
}

//保存済み図形(point)に生成した図形(tmp)と等しいものがあるかどうか
int shapeEquals(std::vector<Point> &tmp, std::vector<std::vector<Point>> &point) {
	int num = point[0].size();
  for (int j = 0; j < static_cast<int>(point.size()); j++) {
	for (int k = 0; k < num; k++) {
	  if (tmp[0] == point[j][k]) {
		int count = 0;
		int k_ = k;
		//setColor(F_ORANGE | F_INTENSITY); printf("[%d][%d](%d,%d)\n", j, k_, point[j][k_].x, point[j][k_].y); setColor();
		while (count<num) {
		  if (k_ >= num)k_ = 0;
		  if (tmp[count] == point[j][k_]) {
			//setColor(F_RED | F_INTENSITY); printf("tmp[%d]==point[%d][%d]\n", count, j, k_); setColor();
			count++;
		  }
		  else {
			//setColor(F_RED | F_INTENSITY); printf("合いませんでした\n"); setColor();
			break;
		  }
		  k_++;
		}
		if (count == num) {
		  //printf("合いました！\n");
		  return j;
		}
	  }
	}
	//printf("\n");
  }
  return -1;
}


Piece::Piece(std::vector<Point> &data) {
  int num = data.size();     		//頂点数
  double sigma = 0;					//面積を求める公式におけるシグマ
  std::vector<Point> tmp(num); 		//1回転パターンの一時格納配列
									//printf("num:%d\n", num);

									//ピースの面積を求める
  for (int i = 0; i < num; i++) {
	if (i == num - 1) {
	  sigma += (double)(data[i].x * data[0].y - data[0].x * data[i].y);
	}
	else {
	  sigma += (double)(data[i].x * data[i + 1].y - data[i + 1].x * data[i].y);
	}
  }
  surface = fabs((1.0 / 2.0)*sigma);
  //printf("debug>面積は%lf\n", surface);

  //////////////////////////////回転して座標//////////////////////////////////////////
  for (int i = 0; i<4; i++) {  			//4回回転(90度ずつ)
	Point min;	//ずらすために取得する座標最小値
	//setColor(F_CYAN | F_INTENSITY); printf("\n--回転パターン[%dπ/2]--\n", i); setColor();//debug
	for (int j = 0; j < num; j++) {
	  tmp[j] = PointRotate((1.0 / 2.0)*i*PI, data[j]);
	  if (j == 0) { min.x = tmp[j].x; min.y = tmp[j].y; }
	  min.x = std::min(min.x, tmp[j].x);
	  min.y = std::min(min.y, tmp[j].y);
	}
	move(tmp, Point(-min.x, -min.y));
	//for(int j=0;j<num;j++) printf("(%d,%d)\n", tmp[j].x, tmp[j].y); //debug

	if (i == 0 || shapeEquals(tmp, point)!=-1) {
	  point.push_back(tmp); //一つの回転パターンをpushback
	  shapeEdge.push_back(calculateEdge(tmp,edgeNum));		//端の座標を格納
	}

	//setColor(F_GREEN | F_INTENSITY); printf("\n--反転パターン[%dπ/2]--\n", i); setColor();
	for (int j = 0; j < num; j++) {
	  tmp[j] = tmp[j] = PointRotate((1.0 / 2.0)*i*PI, data[j]);
	  tmp[j].x *= -1;
	  if (j == 0) { min.x = tmp[j].x; min.y = tmp[j].y; }
	  min.x = std::min(min.x, tmp[j].x);
	  min.y = std::min(min.y, tmp[j].y);
	}
	move(tmp, Point(-min.x, -min.y));
	reverse(tmp);
	//for (int j = 0; j<num; j++) printf("(%d,%d)\n", tmp[j].x, tmp[j].y); //debug

	if (shapeEquals(tmp, point)!=-1) {
	  point.push_back(tmp); //一つの反転パターンをpushback
	  shapeEdge.push_back(calculateEdge(tmp,edgeNum));		//端の座標を格納
	}
  }
  minAngle= calculateAngle(point[0]);
 // minSide = calculateSide(point[0]);
}

std::vector<std::vector<Point>> Piece::getPoint() const {
  return point;
}

int Piece::getSize() {
  return static_cast<int>(point[0].size());
}

double Piece::getSurface() {
  return surface;
}

//double Piece::getMinSide() {
//	return minSide;
//}

double Piece::getMinAngle() {
	return minAngle;
}

//std::pair<int, int> Piece::getEdgeNum(int rotateNum,int putMode) {
int Piece::getEdgeNum(int rotateNum, int putMode) {
	return edgeNum[rotateNum][putMode];
}


// 図形の上下左右頂点を格納
//std::vector<Point> calculateEdge(std::vector<Point> &areaPoint, std::vector<std::vector<std::pair<int, int>>> &edgeNum) {
std::vector<Point> calculateEdge(std::vector<Point> &areaPoint, std::vector<std::vector<int>> &edgeNum) {
  int tall;			//直線方程式 y=-x+b のb
  Point up, left, right, down,upperLeft;
  std::vector<Point> tmp;
  /*
  std::vector<std::pair<int, int>> tmpNum;
  std::pair<int, int> upN;
  std::pair<int, int> rightN;
  std::pair<int, int> leftN;
  std::pair<int, int> downN;
  std::pair<int, int> up_leftN;
  */
  std::vector<int> tmpNum(5);


  for (int i = 0; i < (int)areaPoint.size(); i++) {
	//printf("{%d,%d} ", areaPoint[i].x, areaPoint[i].y);

	if (i == 0) {	//暫定
	  tall = areaPoint[i].x + areaPoint[i].y;
	  upperLeft= areaPoint[i];
	  up = left = right = down = upperLeft=areaPoint[i];
	  //upN.first = leftN.first = rightN.first = downN.first = up_leftN.first = edgeNum.size();
	  //upN.second = leftN.second = rightN.second = downN.second = up_leftN.second = edgeNum.size();
	}
	else if (tall > areaPoint[i].x + areaPoint[i].y) {	//最小
	  tall = areaPoint[i].x + areaPoint[i].y;
	  upperLeft = areaPoint[i]; 
	  //up_leftN.second = i;
	  tmpNum[UP_LEFT] = i;
	}
	else if (tall == areaPoint[i].x + areaPoint[i].y &&upperLeft.x > areaPoint[i].x) {	//tallの値が同じならばxが小さい方を優先
		tall = areaPoint[i].x + areaPoint[i].y;
		upperLeft = areaPoint[i];
		//up_leftN.second = i;
		tmpNum[UP_LEFT] = i;
	}
	//上
	if (up.y > areaPoint[i].y || (up.y==areaPoint[i].y&&up.x>areaPoint[i].x )) {
		up = areaPoint[i];
		//upN.second = i;
		tmpNum[UP] = i;
	}
	//下
	if (down.y < areaPoint[i].y || (down.y == areaPoint[i].y&&down.x > areaPoint[i].x)) {
		down = areaPoint[i];
		//downN.second = i;
		tmpNum[DOWN] = i;
	}
	//左
	if (left.x > areaPoint[i].x || (left.x == areaPoint[i].x&&left.y > areaPoint[i].y)) {
		left = areaPoint[i];
		//leftN.second = i;
		tmpNum[LEFT] = i;
	}
	//右
	if (right.x < areaPoint[i].x || (right.x == areaPoint[i].x&&right.y > areaPoint[i].y)) {
		right = areaPoint[i];
		//rightN.second = i;
		tmpNum[RIGHT] = i;
	}
  }

  tmp.push_back(up);
  tmp.push_back(right);
  tmp.push_back(left);
  tmp.push_back(down);
  tmp.push_back(upperLeft);
  /*tmpNum.push_back(upN);
  tmpNum.push_back(rightN);
  tmpNum.push_back(leftN);
  tmpNum.push_back(downN);
  tmpNum.push_back(up_leftN);*/
  edgeNum.push_back(tmpNum);
  //printf("上(%d,%d) 右(%d,%d) 左(%d,%d) 下(%d,%d)\n",up.x,up.y,right.x, right.y, left.x, left.y ,down.x, down.y);
  //printf("暫定左上(%d,%d),tall:%d\n", point.x, point.y, tall);
  return tmp;
}

Point Piece::getShapeEdge(int eleNum, int putMode) {
  return shapeEdge[eleNum][putMode];
}

//ベクトルの長さを計算する
double get_vector_length(Point v) {
	return pow((v.x * v.x) + (v.y * v.y), 0.5);
}

double calculateAngle(std::vector<Point> &point) {
	Point a, b,C,P,Q;	//aux:補助線のベクトル
	double angle,min=360;
	for (int i = 0; i < static_cast<int>(point.size()); i++) {
		//ベクトルにしてる
		if (i == 0) {
			a = point[0] - point[point.size() - 1];
			C = point[point.size() - 1];
		}
		else {
			a = point[i] - point[i - 1];
			C = point[i - 1];
		}

		if (i == point.size() - 1) {
			b = point[0] - point[i];
			Q = point[0];
		}
		else {
			b = point[i + 1] - point[i];
			Q = point[i+1];
		}

		P = point[i];

		//ベクトルAとBの長さを計算する
		double length_A = get_vector_length(a);
		double length_B = get_vector_length(b);
		//内積とベクトル長さを使って角度を求める
		angle =acos( dot(a, b) / (length_A*length_B));
		angle =180- (angle*180.0 / PI);

		if (((P.x - C.x)*(Q.y - C.y) - (P.y - C.y)*(Q.x - C.x)) < 0) {
			angle = 360 - angle;
		}
		//printf("ベクトル(%d,%d)と(%d,%d)の角度は%lf\n", a.x, a.y, b.x, b.y, angle);
		if (angle < min)min = angle;	//最小値取得
		
	}
	
	return min;
}

double calculateSide(std::vector<Point> &point) {
	Point a, b;
	double len,min=100000;

	for (int i = 0; i < static_cast<int>(point.size()); i++) {
		a = point[i];
		if (i == point.size()-1)b = point[0];
		else b = point[i + 1];

		len = sqrt((double)(b.x-a.x)*(b.x - a.x)+(b.y-a.y)*(b.y - a.y));
		if (i == 0 || len<min)min = len;
	}
	return min;
}

//面積を取得
double calculateSurface(std::vector<Point> &data) {
	int num = data.size();     		//頂点数
	double sigma = 0;					//面積を求める公式におけるシグマ

										//面積を求める
	for (int i = 0; i < num; i++) {
		if (i == num - 1) {
			sigma += (double)(data[i].x * data[0].y - data[0].x * data[i].y);
		}
		else {
			sigma += (double)(data[i].x * data[i + 1].y - data[i + 1].x * data[i].y);
		}
	}

	return fabs((1.0 / 2.0)*sigma);
}