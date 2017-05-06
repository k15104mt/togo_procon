#ifndef _PIECE_HPP_
#define _PIECE_HPP_

#include<vector>
#include"data.hpp"
#include"utility.hpp"

class Piece:public Data{
  public:
    Piece(std::vector<Point> &data);
    int checkHit(Data &) override;
  };


#endif
