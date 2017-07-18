#include"piece.hpp"
#include"utility.hpp"

#include<cstdio>
#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include<stack>
#include<array>


std::vector<Point> framePoint;

//当たり判定
int checkHit(std::vector<Piece> &data, std::vector<putData> &already_put, putData &put) {
  std::vector<Point> cp1(data[put.piece_num].getPoint()[put.point_num]);
  //移動
  for (auto i : cp1) {
	i.x += put.base_point.x;
	i.y += put.base_point.y;
  }

  for (int i = 0; i < already_put.size();++i) {
	std::vector<Point> cp2(data[already_put[i].piece_num].getPoint()[already_put[i].point_num]);
	for (auto j : cp2) {
	  j.x += already_put[i].base_point.x;
	  j.y += already_put[i].base_point.y;
	}
	if (collisionPiece(cp1, cp2)) {
	  return 1;
	}
  }

  if (collisionFrame(framePoint, cp1)){
	return 1;
  }
  

  return 0;
}

Point getPutPoint(std::vector<Piece> &data, std::vector<putData> &already_put) {

}

//再帰
int solve(std::vector<Piece> &data, std::vector<putData> &already_put) {
  //全ピース見ていこうな
  for (int i = 0; i < static_cast<int>(data.size()); ++i) {//ピースの数
	//今のピースがすでに置かれているかどうか
	for (int j = 0; j < already_put.size(); ++j) {
	  if (already_put[j].piece_num == i) {
		break;
	  }
	}

	for (int j = 0; j < data[i].getPoint().size(); ++j) {//回転の組み合わせの数
	  for (int k = 0; k < data[i].getPoint()[j].size(); ++k) {//設置頂点
		/*


		ここに設置プログラム


		*/


		 Point tmp;
		 putData put(i, j, k, tmp);
		if (!checkHit(data,already_put,put)) {
		  //もし当たり判定がokなら
		  already_put.push_back(put);
		  if (solve(data, already_put)) {
			//もしreturn 1なら解き終わったってこと
			return 1;
		  }
		}
	  }
	}
  }

  if (data.size() == already_put.size()) {
	//全部置いたってこと
	return 1;
  }
  already_put.pop_back();
  return 0;
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
