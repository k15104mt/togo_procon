#define NO_S3D_USING
#include <Siv3D.hpp>

#include"utility.hpp"
#include"piece.hpp"

#define QRMAX 5

void Main(){

  std::vector<Piece> data;
  std::vector<std::vector<Point>> framePoint;
  std::vector<putData> putdata;

  s3d::Window::SetTitle(L"回答表示");
  s3d::Window::Resize(1350, 700);

  s3d::GUI gui(s3d::GUIStyle::Default);
  gui.setPos(1040,0);

  //-----QR-----
  gui.add(s3d::GUIText::Create(L"QRコードの数"));
  gui.addln(L"qNum", s3d::GUITextField::Create(s3d::none));
  for (int i = 0; i < QRMAX; ++i) {
	gui.add(s3d::GUIText::Create(s3d::ToString(i+1)));
	gui.add(L"q" + s3d::ToString(i), s3d::GUITextField::Create(s3d::none));
	gui.addln(L"q" + s3d::ToString(i) + L"c", s3d::GUIButton::Create(L"X"));
  }
  gui.addln(L"qApply", s3d::GUIButton::Create(L"適用"));
  //---Answer---
  gui.add(L"answer", s3d::GUITextArea::Create(10,10));
  gui.addln(L"ac", s3d::GUIButton::Create(L"X"));
  gui.addln(L"aApply", s3d::GUIButton::Create(L"適用"));

  s3d::Font pfont(20);

  while (s3d::System::Update()){
	//qrのクリア
	for (int i = 0; i < QRMAX; ++i) {
	  if (gui.button(L"q" + s3d::ToString(i) + L"c").pushed) {
		data.clear();
		framePoint.clear();
		gui.textField(L"q" + s3d::ToString(i)).setText(L"");
	  }
	}
	//acのクリア
	if (gui.button(L"ac").pushed) {
	  putdata.clear();
	  gui.textArea(L"answer").setText(L"");
	}

	//QR 適用
	if (gui.button(L"qApply").pushed) {
	  data.clear();
	  framePoint.clear();

	  int qrNum = [=]() {int a = s3d::Parse<int>(gui.textField(L"qNum").text); if (a > QRMAX) { return QRMAX; }return a; }();

	  //QRコードの数だけ入力を繰り返す
	  std::vector<std::string> input;
	  for (int i = 0; i < qrNum; ++i) {
		std::string tmp;
		tmp = gui.textField(L"q" + s3d::ToString(i)).text.narrow();
		input.push_back(tmp);
	  }

	  framePoint.resize(1);

	  std::vector<std::vector<Point>> piecePoint;
	  for (int i = 0; i < static_cast<int>(input.size()); ++i) {
		//QRコードi番目の文字列

		//:でピースごとに区切る
		auto pieceStr = split(input[i], ':');
		for (int j = 1; j < static_cast<int>(pieceStr.size()); ++j) {
		  // で頂点ごとに区切る
		  auto numStr = split(pieceStr[j], ' ');

		  if (i == static_cast<int>(input.size()) - 1 && j == static_cast<int>(pieceStr.size()) - 1) {
			//最後のQRの最後のデータの場合はframePointに格納する
			for (int k = 1; k < static_cast<int>(numStr.size()); k += 2) {
			  Point pointTemp(stoi(numStr[k]), stoi(numStr[k + 1]));
			  framePoint[0].push_back(pointTemp);
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

	  for (int i = 0; i < static_cast<int>(piecePoint.size()); i++) {
		data.push_back(piecePoint[i]);
	  }
	}

	//Answer 適用
	if (gui.button(L"aApply").pushed) {
	  putdata.clear();
	  std::vector<std::string> input = split(gui.textArea(L"answer").text.narrow(),'\n');
	  for (int i = 0; i < static_cast<int>(input.size()); ++i) {
		int tmp_piece, tmp_rota;
		Point tmp_pos;
		sscanf_s(input[i].c_str(), "%d %d (%d,%d)",&tmp_piece,&tmp_rota,&tmp_pos.x, &tmp_pos.y);
		putData tmp(tmp_piece,tmp_rota,0,tmp_pos);
		putdata.push_back(tmp);
	  }
	}
	
	if (framePoint.size()) {
	  s3d::Graphics2D::SetTransform(
		s3d::Mat3x2::Rotate(0)*
		s3d::Mat3x2::Translate(10, 10)
	  );

	  for (int i = 0; i < static_cast<int>(framePoint[0].size()); ++i) {
		s3d::Line(framePoint[0][i].x*10, framePoint[0][i].y*10, framePoint[0][(i+1)%framePoint[0].size()].x*10, framePoint[0][(i + 1) % framePoint[0].size()].y*10).draw(s3d::Color(0,0,255));
	  }

	  s3d::Graphics2D::SetTransform(
		s3d::Mat3x2::Rotate(0)*
		s3d::Mat3x2::Translate(0, 0)
	  );
	}

	if (putdata.size() && data.size()) {
	  std::vector<s3d::Polygon> po;
	  for (int i = 0; i < static_cast<int>(putdata.size()); ++i) {
		s3d::Graphics2D::SetTransform(
		  s3d::Mat3x2::Rotate(0)*
		  s3d::Mat3x2::Translate(putdata[i].base_point.x * 10 + 10, putdata[i].base_point.y * 10 + 10)
		);

		std::vector<s3d::Vec2> tmp;
		for (int j = 0; j < static_cast<int>(data[putdata[i].piece_num].getPoint()[putdata[i].point_num].size()); ++j) {
		  s3d::Line(data[putdata[i].piece_num].getPoint()[putdata[i].point_num][j].x*10, data[putdata[i].piece_num].getPoint()[putdata[i].point_num][j].y*10,
			data[putdata[i].piece_num].getPoint()[putdata[i].point_num][(j + 1) % data[putdata[i].piece_num].getPoint()[putdata[i].point_num].size()].x*10,
			data[putdata[i].piece_num].getPoint()[putdata[i].point_num][(j + 1) % data[putdata[i].piece_num].getPoint()[putdata[i].point_num].size()].y*10).draw(s3d::Color(0, 255, 0));
		  
		  
		  tmp.push_back(s3d::Vec2(data[putdata[i].piece_num].getPoint()[putdata[i].point_num][j].x * 10 + putdata[i].base_point.x * 10 + 10
			, data[putdata[i].piece_num].getPoint()[putdata[i].point_num][j].y * 10 + putdata[i].base_point.y * 10 + 10));
		}
		po.push_back(s3d::Polygon(tmp));
	  }

	  s3d::Graphics2D::SetTransform(
		s3d::Mat3x2::Rotate(0)*
		s3d::Mat3x2::Translate(0, 0)
	  );

	  for (int i = 0; i < static_cast<int>(po.size()); ++i) {
		if (po[i].mouseOver) {
		  po[i].draw(s3d::Color(0, 255, 0));
		  pfont(i).draw(s3d::Mouse::Pos().x, s3d::Mouse::Pos().y, s3d::Color(255, 0, 0));
		}
	  }
	}

	
  }
}
