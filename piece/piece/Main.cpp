
#define NO_S3D_USING
# include <Siv3D.hpp>
#include<Windows.h>
#include<cstdio>
#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include<stack>
#include<array>
#include"utility.hpp"
#include"piece.hpp"
#include"color.hpp"

std::vector<std::string> split(std::string input, char spilit_character) {
	std::stringstream stream(input);

	std::vector<std::string> result;
	std::string temp;
	while (std::getline(stream, temp, spilit_character)) {
		result.push_back(temp);
	}

	return result;
}

void Main()
{
	AllocConsole();
	FILE* out = 0; freopen_s(&out, "CON", "w", stdout);
	FILE* in = 0; freopen_s(&in, "CON", "r", stdin);

	s3d::Window::Resize(1120, 630);

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


	for (int i = 0; i < static_cast<int>(input.size()); ++i) {
		//QRコードi番目の文字列

		//:でピースごとに区切る
		auto pieceStr = split(input[i], ':');
		for (int j = 1; j < static_cast<int>(pieceStr.size()); ++j) {
			// で頂点ごとに区切る
			auto numStr = split(pieceStr[j], ' ');

			if (i == input.size() - 1 && j == pieceStr.size() - 1) {
				//最後のQRの最後のデータの場合はframePointに格納する
				for (int k = 1; k < static_cast<int>(numStr.size()); k += 2) {
					Point pointTemp(stoi(numStr[k]), stoi(numStr[k + 1]));
					framePoint.push_back(pointTemp);
				}
			}
			else {
				//角頂点の座標をpieceTemoに数字に変換して格納して、piecePointに追加する
				std::vector<Point> pieceTemp;
				for (int k = 1; k < static_cast<int>(numStr.size()); k += 2) {
					Point pointTemp(stoi(numStr[k]), stoi(numStr[k + 1]));
					pieceTemp.push_back(pointTemp);
				}
				piecePoint.push_back(pieceTemp);
			}

		}
	}

	std::vector<Piece> data;

	for (int i = 0; i < static_cast<int>(piecePoint.size()); i++) {
		data.push_back(piecePoint[i]);
	}

	int i=0, j=0,k;
	s3d::Font font(30);
	while (s3d::System::Update()){
		if (i < data.size()-1 && s3d::Input::KeyRight.clicked) {
			i++;
			j = 0;
		}else if (i > 0 && s3d::Input::KeyLeft.clicked) {
			i--;
			j = 0;
		}
		if (j < data[i].getPoint().size() - 1 && s3d::Input::KeyDown.clicked) {
			j++;
		}
		else if (j > 0 && s3d::Input::KeyUp.clicked) {
			j--;
		}
		font(L"ピース番号:",i+1,L"/", data.size()).draw(0, 0);
		font(L"角度:",(360/ data[i].getPoint().size())*j,L"°").draw(0, 50);
		font(L"頂点の数:",data[i].getPoint()[j].size()).draw(0, 100);
		for (k = 0; k < data[i].getPoint()[j].size()-1;k++) {
			s3d::Line(data[i].getPoint()[j][k].x *10 + 540, data[i].getPoint()[j][k].y * 10 + 100, data[i].getPoint()[j][k+1].x * 10 + 540, data[i].getPoint()[j][k+1].y * 10 + 100).draw();
			font(L"座標",k+1,L":(",data[i].getPoint()[j][k].x,L",", data[i].getPoint()[j][k].y, L")").draw((k / 8) * 270, (k % 8) * 50 + 150);
		}
		s3d::Line(data[i].getPoint()[j][k].x * 10 + 540, data[i].getPoint()[j][k].y * 10 + 100, data[i].getPoint()[j][0].x * 10 + 540, data[i].getPoint()[j][0].y * 10 + 100).draw();
		font(L"座標", k + 1, L":(", data[i].getPoint()[j][k].x, L",", data[i].getPoint()[j][k].y,L")").draw((k / 8) * 270, (k % 8) * 50 + 150);

	}
}
