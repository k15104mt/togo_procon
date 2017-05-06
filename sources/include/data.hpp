#ifndef _DATA_HPP_
#define _DATA_HPP_


#include<vector>
#include"utility.hpp"

class Data{
protected:
  std::vector<std::vector<Point>> point;
public:
  std::vector<std::vector<Point>> getPoint();
  virtual int checkHit(Data &)=0;
};

#endif
