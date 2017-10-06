#ifndef _PIECE_HPP_
#define _PIECE_HPP_
#define UP 1
#define RIGHT 2
#define LEFT 3
#define DOWN 4
#define UP_LEFT 5

#include<vector>
#include"utility.hpp"

struct D_Point {
	double x;
	double y;
};

class Piece {
private:
  std::vector<std::vector<Point>> point;	//頂点座標
  std::vector < std::vector<Point> > shapeEdge;	//各回転状態での上、右、左、下、左上の頂点座標
  double surface;							//ピース面積
  double minAngle;	//ピース内の最小角度
  double minSide;	//ピースの最小の辺の長さ
public:
  Piece(std::vector<Point> &data);
  std::vector<std::vector<Point>> getPoint() const;
  int getSize();
  double getMinAngle();
  double getMinSide();
  double getSurface();
  Point getShapeEdge(int eleNum,int putMode);	//引数1:回転番号,引数2:UPとかLEFT
};

double calculateAngle(std::vector<Point> &point);
double calculateSurface(std::vector<Point> &data);
#endif
