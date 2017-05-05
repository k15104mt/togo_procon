#ifndef _PIECE_HPP_
#define _PIECE_HPP_

#include<vector>
#include"utility.hpp"

class Piece{
  private:
    std::vector<Point> point; //頂点を格納するPoint型動的配列
  public:
    Piece(int pointNum,std::vector<Point> &data);  //引数：一つのピースの頂点数と構造体配列
    Point getPoint(int n);    //引数：取得するピースの頂点数
};


#endif
