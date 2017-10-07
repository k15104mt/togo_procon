#ifndef _PIECE_HPP_
#define _PIECE_HPP_
#define UP 0
#define RIGHT 1
#define LEFT 2
#define DOWN 3
#define UP_LEFT 4

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
	double surface;	//ピース面積
	double minAngle;	//ピース内の最小角度
	//double minSide;	//ピースの最小の辺の長さ
   // std::vector< std::vector<std::pair<int, int>>> edgeNum;	//各端の点の要素番号(上右左下左上の順に格納),vectorは、各回転要素の中に、各上下左右のペアがある
	std::vector<std::vector<int>> edgeNum;	//各端の点の頂点番号(上右左下左上の順)
public:
	Piece(std::vector<Point> &data);
	std::vector<std::vector<Point>> getPoint() const;
	int getSize();
	double getMinAngle();
	//double getMinSide();
	double getSurface();
	Point getShapeEdge(int eleNum, int putMode);	//引数1:回転番号,引数2:UPとかLEFT
	//std::pair<int,int> getEdgeNum(int rotateNum,int putMode);		//ピースの端の要素番号(引数:回転番号,UPとかLEFT)
	int getEdgeNum(int rotateNum, int putMode);		//ピースの端の要素番号(引数:回転番号,UPとかLEFT)
};

int shapeEquals(std::vector<Point> &tmp, std::vector<std::vector<Point>> &point);
double calculateSide(std::vector<Point> &point);
double calculateAngle(std::vector<Point> &point);
double calculateSurface(std::vector<Point> &data);
#endif
