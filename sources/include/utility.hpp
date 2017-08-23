#ifndef _UTILITY_HPP_
#define _UTILITY_HPP_

#include<vector>
#include<cmath>

class Point{
public:
	int x,y;
	Point() {};
	Point(int x,int y):x(x),y(y){};

	Point operator + (Point &obj){return Point(x+obj.x,y+obj.y);};
	Point operator - (Point &obj){return Point(x-obj.x,y-obj.y);};
	Point & operator = (Point &obj){x = obj.x; y = obj.y; return *this;};
	Point & operator += (Point &obj) { x += obj.x; y += obj.y; return *this; };
	bool operator == (Point &obj){return x==obj.x && y==obj.y;};

	double size() { return sqrt(x*x + y*y); };
	void print() { printf("(%d,%d)", x, y); };
	void println() { printf("(%d,%d)\n", x, y); };
};

//設置情報
class putData {
public:
  putData(int, int, int, Point&);

  int piece_num;//ピース
  int point_num;//回転
  int vertex_num;//頂点
  Point base_point;//原点がどの位置か

};

//ベクトルも同じ型みたいなものだし多少はね
using Vector = Point;

//内積ィ！！
int dot(Vector&,Vector&);
//外積ィ！！
int cross(Vector&,Vector&);

void move(std::vector<Point>&, Point);

//すべての辺を見て交わっているものがあった場合は１を返す
int crossLine(std::vector<Point>&,std::vector<Point>&);

//第一引数のポリゴンに第二引数のすべての頂点が内包されている場合または
//第二引数のポリゴンに第一引数のすべての頂点が内包されている場合に1を返す
//返上にある場合も内側に入っているものとする
int inPolygon(std::vector<Point>&,std::vector<Point>&);

int collisionPiece(std::vector<Point>&,std::vector<Point>&);

int collisionFrame(std::vector<Point>&,std::vector<Point>&);

#endif
