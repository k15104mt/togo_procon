#include"piece.hpp"
#include"utility.hpp"

#include<cstdio>
#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include<stack>
#include<array>

#define NUM 4

int roop = 0;

void func(std::array<int, NUM> &array, std::vector<int> &vector) {
  //上で呼び出されたときに選択されている数字は選択しないようする（要改善）
  for (int i = 0; i<static_cast<int>(array.size()); i++) {
	if (([&]() {int n = 1; for (auto j : vector) { if (i == j) { n = 0; } }return n; }())) {
	  vector.push_back(i);
	  func(array, vector);
	}
  }
  if (array.size() == vector.size()) {
	printf("%8d:", roop);
	for (auto i : vector) {
	  std::cout << array[i] << " ";
	}
	std::cout << std::endl;

	roop++;
  }
  vector.pop_back();
}

class put_data {
public:
  put_data(int,int,Point&);

  int piece_num;
  int point_num;
  Point base_point;

};

put_data::put_data(int piece_num, int point_num, Point &base_point) {
  this->piece_num = piece_num;
  this->point_num = point_num;
  this->base_point = base_point;
}


//再帰
int solve(std::vector<Piece> &data,std::stack<int> &not_put){
  //置かれずに飛ばされたピースがあるか
  if (!not_put.empty()) {
	std::stack<int> tmp;
	//飛ばされたピースを置く
	for (int i = 0; i < static_cast<int>(not_put.size()); ++i) {
	  

	}
  }

  //全要素見ていこうな
  for (int i = 0; i < static_cast<int>(data.size()); ++i) {


  }

}

//文字列分割の関数
std::vector<std::string> split(std::string, char);

int main() {
  //QRコードの数
  int qrNum;
  std::cin >> qrNum;
  std::cin.ignore();

  //QRコードの数だけ入力を繰り返す
  std::vector<std::string> input;
  for (int i = 0; i < qrNum; ++i) {
	std::string temp;
	std::getline(std::cin, temp);
	input.push_back(temp);
  }


  std::vector<std::vector<Point>> piecePoint;
  std::vector<Point> framePoint;

  for (int i = 0; i < input.size(); ++i) {
	//QRコードi番目の文字列

	//:でピースごとに区切る
	auto pieceStr = split(input[i], ':');
	for (int j = 1; j < pieceStr.size(); ++j) {
	  // で頂点ごとに区切る
	  auto numStr = split(pieceStr[j], ' ');

	  if (i == input.size() - 1 && j == pieceStr.size() - 1) {
		//最後のQRの最後のデータの場合はframePointに格納する
		for (int k = 1; k < numStr.size(); k += 2) {
		  Point pointTemp(stoi(numStr[k]), stoi(numStr[k + 1]));
		  framePoint.push_back(pointTemp);
		}
	  }
	  else {
		//角頂点の座標をpieceTemoに数字に変換して格納して、piecePointに追加する
		std::vector<Point> pieceTemp;
		for (int k = 1; k < numStr.size(); k += 2) {
		  Point pointTemp(stoi(numStr[k]), stoi(numStr[k + 1]));
		  pieceTemp.push_back(pointTemp);
		}
		piecePoint.push_back(pieceTemp);
	  }

	}
  }



  /*確認用
  for (int i = 0; i < piecePoint.size(); ++i) {
  std::cout << "piece[" << i << "]" << "(頂点数:" << piecePoint[i].size() << "):";
  for (auto j : piecePoint[i]) {
  std::cout << "(" << j.x << "," << j.y << ") ";
  }
  std::cout << std::endl;
  }


  std::cout << "frame（頂点数:" << framePoint.size() << "):";
  for (auto i : framePoint) {
  std::cout << "(" << i.x << "," << i.y << ") ";
  }
  std::cout << std::endl;
  */


  //再帰テスト用a
  //std::array<int, NUM> array = { 0,1,2,3 };
  //std::vector<int> vector
  //func(array, vector);

  return 0;
}


std::vector<std::string> split(std::string input, char spilit_character) {
  std::stringstream stream(input);

  std::vector<std::string> result;
  std::string temp;
  while (std::getline(stream, temp, spilit_character)) {
	result.push_back(temp);
  }

  return result;
}