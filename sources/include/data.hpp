#ifndef _DATA_HPP_
#define _DATA_HPP_

#include<vector>
//#include"utility.hpp"std::vector<std::vector<int>>

class Data{
private:
   std::vector<int> point;
public:
  Data(){point.resize(5);}
  std::vector<int> getAllPoint();
  //virtual int checkHit(Data &)=0;
};

#endif
