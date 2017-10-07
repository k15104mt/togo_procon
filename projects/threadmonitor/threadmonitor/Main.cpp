#include<vector>
#include<thread>
#include<mutex>
#include<utility>

#define NO_S3D_USING
#include<Siv3D.hpp>

#include"utility.hpp"
#include"piece.hpp"
#include"geometry.hpp"
#include"color.hpp"


std::vector<Piece> data;
std::vector<std::vector<Point>> framePoint;


class Solver {
public:
  Solver() :geometry(framePoint), isPut({ 0 }) {};

  int start = 0;
  int active = 0;
  std::vector<putData> already_put;
  Geometry geometry;
  std::array<int, 100> isPut;

private:

};


std::mutex startmtx;
int startflag = 0;

std::mutex mtx[8];
std::vector<Solver> solver;

std::array<std::vector<putData>, 8> copyap;

std::mutex areamtx[8];
std::array<std::vector<std::vector<Point>>, 8> copyarea;

int checkHit(std::vector<putData> &, putData &, Geometry &);
int isAlong(int,Geometry&, putData&);
int solve(int);

void func(int num) {
  while (1) {
	startmtx.lock();
	int tmp = startflag;
	startmtx.unlock();

	if (tmp) {
	  break;
	}

	//1秒ごとにスタートしているかどうか調べる
	Sleep(1000);
  }

  if (solver[num].active) {
	solve(num);
  }

}

void monitor() {
  s3d::Window::SetTitle(L"solver");
  s3d::Window::Resize(1800,800);

  //初期設定

  //**************** GUI 1 ******************
  s3d::GUI gui(s3d::GUIStyle::Default);

  gui.addln(s3d::GUIText::Create(L"データ　　　　　　　　　　                                         　　　　　　　　　　　　　　　　ヒント"));

  gui.add(L"input", s3d::GUITextArea::Create(10, 30));
  gui.add(L"inputClear", s3d::GUIButton::Create(L"X"));
  gui.add(s3d::GUIText::Create(L"     "));

  s3d::TextReader reader(L"out.txt");
  s3d::String fileinput = reader.readAll();
  gui.textArea(L"input").setText(fileinput);

  gui.add(L"hint", s3d::GUITextArea::Create(10, 30));
  gui.addln(L"hintClear", s3d::GUIButton::Create(L"X"));


  gui.add(s3d::GUIText::Create(L"使用PC数"));
  gui.add(L"useNum", s3d::GUIRadioButton::Create({ L"1", L"2", L"3" }, 0));
  gui.add(s3d::GUIText::Create(L"PC番号"));
  gui.add(L"pcNum", s3d::GUIRadioButton::Create({ L"0", L"1", L"2" }, 0));
  gui.add(s3d::GUIText::Create(L"  スレッド数"));

  gui.add(s3d::GUIText::Create(L"0:"));
  gui.add(L"th0", s3d::GUITextField::Create(2));
  gui.textField(L"th0").setText(L"8");
  gui.add(s3d::GUIText::Create(L"  1:"));
  gui.add(L"th1", s3d::GUITextField::Create(2));
  gui.textField(L"th1").setText(L"8");
  gui.add(s3d::GUIText::Create(L"  2:"));
  gui.addln(L"th2", s3d::GUITextField::Create(2));
  gui.textField(L"th2").setText(L"4");

  gui.add(s3d::GUIText::Create(L"  　　　　　　　　　　　　　　　　　　　　　　　　　　　"));
  gui.addln(L"apply", s3d::GUIButton::Create(L"適用"));

  for (int i = 0; i < 8; ++i) {
	gui.add(s3d::GUIText::Create(s3d::ToString(i)));
	if (i == 0) {
	  gui.add(L"range" + s3d::ToString(i), s3d::GUITextField::Create(s3d::none));
	}
	else if ((i + 1) % 4 == 0) {
	  gui.addln(L"range" + s3d::ToString(i), s3d::GUITextField::Create(s3d::none, false));
	}
	else {
	  gui.add(L"range" + s3d::ToString(i), s3d::GUITextField::Create(s3d::none, false));
	}
  }

  gui.add(L"start", s3d::GUIButton::Create(L"スタート"));


  //**************** GUI 2 ******************
  s3d::GUI gui2(s3d::GUIStyle::Default);
  gui2.setPos(0, 700);
  gui2.add(L"update", s3d::GUIButton::Create(L"更新"));
  gui2.add(L"back", s3d::GUIButton::Create(L"戻る"));
  gui2.add(L"copy", s3d::GUIButton::Create(L"回答データをコピー", false));
  gui2.show(false);


  //*************** 回答表示 ****************

  s3d::Font font30(30);

  //***************  data  ******************
  int useNum = 1, pcNum = 0, thNum = 1;
  int status[8] = { 0 };

  while (s3d::System::Update()) {

	if (gui.button(L"inputClear").pushed) {
	  gui.textArea(L"input").setText(L"");
	}

	if (gui.button(L"hintClear").pushed) {
	  gui.textArea(L"hint").setText(L"");
	}

	if (gui.button(L"apply").pushed) {
	  for (int i = 0; i < 3; ++i) {
		if (gui.radioButton(L"useNum").checked(i)) {
		  useNum = (i + 1);
		}
	  }

	  for (int i = 0; i < 3; ++i) {
		if (gui.radioButton(L"pcNum").checked(i)) {
		  pcNum = i;
		}
	  }

	  thNum = s3d::Parse<int>(gui.textField(L"th" + s3d::ToString(pcNum)).text);
	  for(int i=0;i<8;++i){
		if (i < thNum) {
		  gui.textField(L"range" + s3d::ToString(i)).enabled = true;
		}
		else {
		  gui.textField(L"range" + s3d::ToString(i)).setText(L"");
		  gui.textField(L"range" + s3d::ToString(i)).enabled = false;
		}
	  }


	  std::vector<std::string> inputdata = split(gui.textArea(L"input").text.narrow(), '\n');
	  int piece = 0;
	  for (int i = 1; i < static_cast<int>(inputdata.size()); ++i) {
		std::vector<std::string> tmp = split(inputdata[i], ':');
		piece += std::stoi(tmp[0]);
	  }

	  int sumthread = 0;
	  for (int i = 0; i < useNum; ++i) {
		sumthread += s3d::Parse<int>(gui.textField(L"th" + s3d::ToString(i)).text);
	  }

	  int div = piece / sumthread;
	  int rem = piece % sumthread;

	  std::vector<std::vector<int>> start;

	  int sum=0;
	  for (int i = 0; i < useNum; ++i) {
		std::vector<int> tmp;
		for (int j = 0; j < s3d::Parse<int>(gui.textField(L"th" + s3d::ToString(i)).text); ++j) {
		  if (rem) {
			tmp.push_back(sum);
			sum += div + 1;
			rem--;
		  }
		  else {
			tmp.push_back(sum);
			sum += div;
		  }
		}
		start.push_back(tmp);
	  }

	  for (int i = 0; i < thNum; ++i) {
		if (i == thNum - 1 && useNum == (pcNum + 1)) {
		  gui.textField(L"range" + s3d::ToString(i)).setText(s3d::ToString(start[pcNum][i]) + L"～" + s3d::ToString(piece - 1));
		}
		else if(i == thNum - 1){
		  gui.textField(L"range" + s3d::ToString(i)).setText(s3d::ToString(start[pcNum][i]) + L"～" + s3d::ToString(start[pcNum+1][0] -1));
		}
		else {
		  gui.textField(L"range" + s3d::ToString(i)).setText(s3d::ToString(start[pcNum][i]) + L"～" + s3d::ToString(start[pcNum][i+1] -1));
		}
	  }


	}

	if (gui.button(L"start").pushed) {
	  //----------------適用と同じことする--------------------
	  for (int i = 0; i < 3; ++i) {
		if (gui.radioButton(L"useNum").checked(i)) {
		  useNum = (i + 1);
		}
	  }

	  for (int i = 0; i < 3; ++i) {
		if (gui.radioButton(L"pcNum").checked(i)) {
		  pcNum = i;
		}
	  }

	  thNum = s3d::Parse<int>(gui.textField(L"th" + s3d::ToString(pcNum)).text);
	  for (int i = 0; i<8; ++i) {
		if (i < thNum) {
		  gui.textField(L"range" + s3d::ToString(i)).enabled = true;
		}
		else {
		  gui.textField(L"range" + s3d::ToString(i)).setText(L"");
		  gui.textField(L"range" + s3d::ToString(i)).enabled = false;
		}
	  }


	  std::vector<std::string> inputdata = split(gui.textArea(L"input").text.narrow(), '\n');
	  int piece = 0;
	  for (int i = 1; i < static_cast<int>(inputdata.size()); ++i) {
		std::vector<std::string> tmp = split(inputdata[i], ':');
		piece += std::stoi(tmp[0]);
	  }

	  int sumthread = 0;
	  for (int i = 0; i < useNum; ++i) {
		sumthread += s3d::Parse<int>(gui.textField(L"th" + s3d::ToString(i)).text);
	  }

	  int div = piece / sumthread;
	  int rem = piece % sumthread;

	  std::vector<std::vector<int>> start;

	  int sum = 0;
	  for (int i = 0; i < useNum; ++i) {
		std::vector<int> tmp;
		for (int j = 0; j < s3d::Parse<int>(gui.textField(L"th" + s3d::ToString(i)).text); ++j) {
		  if (rem) {
			tmp.push_back(sum);
			sum += div + 1;
			rem--;
		  }
		  else {
			tmp.push_back(sum);
			sum += div;
		  }
		}
		start.push_back(tmp);
	  }

	  for (int i = 0; i < thNum; ++i) {
		if (i == thNum - 1 && useNum == (pcNum + 1)) {
		  gui.textField(L"range" + s3d::ToString(i)).setText(s3d::ToString(start[pcNum][i]) + L"～" + s3d::ToString(piece - 1));
		}
		else if (i == thNum - 1) {
		  gui.textField(L"range" + s3d::ToString(i)).setText(s3d::ToString(start[pcNum][i]) + L"～" + s3d::ToString(start[pcNum + 1][0] - 1));
		}
		else {
		  gui.textField(L"range" + s3d::ToString(i)).setText(s3d::ToString(start[pcNum][i]) + L"～" + s3d::ToString(start[pcNum][i + 1] - 1));
		}
	  }
	  //------------------------------------------------------


	  gui.show(false);
	  gui2.show();
	  s3d::Window::Resize(1800, 800);

	  //------------------------------------------------------
	  std::vector<std::string> input = split(gui.textArea(L"input").text.narrow(), '\n');

	  std::vector<std::vector<Point>> piecePoint;
	  framePoint.resize(1);

	  for (int i = 1; i < static_cast<int>(input.size()); ++i) {
		//QRコードi番目の文字列

		//:でピースごとに区切る
		auto pieceStr = split(input[i], ':');
		for (int j = 1; j < static_cast<int>(pieceStr.size()); ++j) {
		  // で頂点ごとに区切る
		  auto numStr = split(pieceStr[j], ' ');

		  if (i == static_cast<int>(input.size() - 1) && j == static_cast<int>(pieceStr.size() - 1)) {
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
	  //------------------------------------------------------

	  solver.resize(8);


	  //int range = data.size() / (thNum*useNum);
	  for (int i = 0; i < thNum; ++i) {
		solver[i].active = 1;
		solver[i].start = start[pcNum][i];
	  }

	  //ヒントの処理
	  std::vector<std::string>  hintdata = split(gui.textArea(L"hint").text.narrow(), '\n');
	  std::vector<putData> hint;

	  for (int i = 1; i < static_cast<int>(hintdata.size()); ++i) {
		std::vector<std::string> pieceStr = split(hintdata[i],':');
		for (int j = 1; j < static_cast<int>(pieceStr.size()); ++j) {
		  std::vector<std::string> numStr = split(pieceStr[j], ' ');
		  std::vector<Point> pieceTemp;

		  int miny=200, minx=200;
		  for (int k = 1; k < static_cast<int>(numStr.size()); k += 2) {
			Point poinTtmp(stoi(numStr[k]),stoi(numStr[k+1]));
			if (minx > stoi(numStr[k]))minx = stoi(numStr[k]);
			if (miny > stoi(numStr[k + 1]))miny = stoi(numStr[k + 1]);

			pieceTemp.push_back(poinTtmp);
		  }
		  Piece makePiece(pieceTemp);
		  for (int k = 0; k < data.size(); ++k) {
			int pointNum = shapeEquals(makePiece.getPoint()[0], data[k].getPoint());
			if (pointNum != -1) {
			  hint.push_back(putData(k, pointNum, 0,Point(minx,miny)));
			}
		  }
		}
	  }

	  for (int i = 0; i < thNum; ++i) {
		for (int j = 0; j < static_cast<int>(hint.size()); ++j) {
		  solver[i].geometry.getPutPoint(data,solver[i].already_put,LEFT,mtx[i]);
		  solver[i].already_put.push_back(hint[j]);
		  solver[i].isPut[hint[j].piece_num] = 1;
		}
		solver[i].geometry.getPutPoint(data, solver[i].already_put, LEFT, mtx[i]);
	  }


	  startmtx.lock();
	  startflag = 1;
	  startmtx.unlock();
	}

	if (startflag) {
	  static int select = -1;
	  static int updatecount = 0;
	  updatecount++;
	  if (updatecount > 60 || gui2.button(L"update").pushed) {
		for (int i = 0; i < 8; ++i) {
		  if (solver[i].active) {
			mtx[i].lock();
			copyap[i] = solver[i].already_put;
			mtx[i].unlock();
			copyarea[i] = solver[i].geometry.areaPoint;
		  }
		}
		updatecount = 0;
	  }

	  if (gui2.button(L"back").pushed) {
		select = -1;
	  }



	  if (select == -1) {
		for (int i = 0; i < 8; ++i) {
		  if (s3d::Rect(400 * (i % 4) + 10 * ((i % 4) + 1), 256 * (i / 4) + 40 * ((i / 4) + 1), 400, 256).leftClicked && solver[i].active) {
			select = i;
		  }
		}

		if (gui2.button(L"back").enabled) {
		  gui2.button(L"back").enabled = false;
		}
		if (gui2.button(L"copy").enabled) {
		  gui2.button(L"copy").enabled = false;
		}



		for (int i = 0; i < 8; ++i) {
		  s3d::Graphics2D::SetTransform(
			s3d::Mat3x2::Scale(4, s3d::Float2(0, 0))*
			s3d::Mat3x2::Translate(400 * (i % 4) + 10 * ((i % 4) + 1), 256 * (i / 4) + 40 * ((i / 4) + 1))
		  );

		  s3d::Rect(0, 0, 100, 64).draw(s3d::Color(60, 60, 60));

		  if (solver[i].active) {
			for (int j = 0; j < static_cast<int>(framePoint.size()); ++j) {
			  for (int k = 0; k < static_cast<int>(framePoint[j].size()); ++k) {
				s3d::Line(framePoint[j][k].x, framePoint[j][k].y, framePoint[j][(k + 1) % framePoint[j].size()].x, framePoint[j][(k + 1) % framePoint[j].size()].y).draw(0.25, s3d::Color(0, 0, 255));
			  }
			}

			for (int j = 0; j < static_cast<int>(copyap[i].size()); ++j) {
			  for (int k = 0; k < static_cast<int>(data[copyap[i][j].piece_num].getPoint()[copyap[i][j].point_num].size()); ++k) {
				if (j == static_cast<int>(copyap[i].size() - 1)) {
				  s3d::Line(data[copyap[i][j].piece_num].getPoint()[copyap[i][j].point_num][k].x + copyap[i][j].base_point.x,
					data[copyap[i][j].piece_num].getPoint()[copyap[i][j].point_num][k].y + copyap[i][j].base_point.y,
					data[copyap[i][j].piece_num].getPoint()[copyap[i][j].point_num][(k + 1) % data[copyap[i][j].piece_num].getPoint()[copyap[i][j].point_num].size()].x + copyap[i][j].base_point.x,
					data[copyap[i][j].piece_num].getPoint()[copyap[i][j].point_num][(k + 1) % data[copyap[i][j].piece_num].getPoint()[copyap[i][j].point_num].size()].y + copyap[i][j].base_point.y).draw(0.25, s3d::Color(255, 0, 0));
				}
				else {
				  s3d::Line(data[copyap[i][j].piece_num].getPoint()[copyap[i][j].point_num][k].x + copyap[i][j].base_point.x,
					data[copyap[i][j].piece_num].getPoint()[copyap[i][j].point_num][k].y + copyap[i][j].base_point.y,
					data[copyap[i][j].piece_num].getPoint()[copyap[i][j].point_num][(k + 1) % data[copyap[i][j].piece_num].getPoint()[copyap[i][j].point_num].size()].x + copyap[i][j].base_point.x,
					data[copyap[i][j].piece_num].getPoint()[copyap[i][j].point_num][(k + 1) % data[copyap[i][j].piece_num].getPoint()[copyap[i][j].point_num].size()].y + copyap[i][j].base_point.y).draw(0.25, s3d::Color(0, 255, 0));
				}


			  }
			}

		  }
		  else {
			s3d::Graphics2D::SetTransform(
			  s3d::Mat3x2::Scale(1, s3d::Float2(0, 0))*
			  s3d::Mat3x2::Translate(400 * (i % 4) + 10 * ((i % 4) + 1), 256 * (i / 4) + 40 * ((i / 4) + 1))
			);
			font30(L"none").draw(100, 100);

		  }
		}


	  }
	  else {
		if (!gui2.button(L"copy").enabled) {
		  gui2.button(L"copy").enabled = true;
		}
		if (!gui2.button(L"back").enabled) {
		  gui2.button(L"back").enabled = true;
		}

		if (gui2.button(L"copy").pushed) {
		  s3d::String clipboard;
		  for (auto i : copyap[select]) {
			clipboard += (s3d::ToString(i.piece_num) + L" " + s3d::ToString(i.point_num) + L" (" + s3d::ToString(i.base_point.x) + L"," + s3d::ToString(i.base_point.y) + L")\r\n");
		  }
		  s3d::Clipboard::SetText(clipboard);
		}

		static int mOverPiece = -1;
		s3d::Graphics2D::SetTransform(
		  s3d::Mat3x2::Scale(10, s3d::Float2(0, 0))*
		  s3d::Mat3x2::Translate(10, 10)
		);

		s3d::Rect(0, 0, 100, 64).draw(s3d::Color(60, 60, 60));

		std::vector<s3d::Polygon> forCollision;

		for (int i = 0; i < static_cast<int>(framePoint.size()); ++i) {
		  for (int j = 0; j < static_cast<int>(framePoint[i].size()); ++j) {
			s3d::Line(framePoint[i][j].x, framePoint[i][j].y, framePoint[i][(j + 1) % framePoint[i].size()].x, framePoint[i][(j + 1) % framePoint[i].size()].y).draw(0.1, s3d::Color(0, 0, 255));
		  }
		}


		for (int i = 0; i < static_cast<int>(copyarea[select].size()); ++i) {
		  for (int j = 0; j < static_cast<int>(copyarea[select][i].size()); ++j) {
			s3d::Line(copyarea[select][i][j].x,
			  copyarea[select][i][j].y,
			  copyarea[select][i][(j + 1) % copyarea[select][i].size()].x,
			  copyarea[select][i][(j + 1) % copyarea[select][i].size()].y).draw(0.25, s3d::Color(255, 255, 255));
		  }

		}

		for (int i = 0; i < static_cast<int>(copyap[select].size()); ++i) {
		  std::vector<s3d::Vec2> tmp;
		  for (int j = 0; j < static_cast<int>(data[copyap[select][i].piece_num].getPoint()[copyap[select][i].point_num].size()); ++j) {
			if (i == static_cast<int>(copyap[select].size() - 1)) {
			  s3d::Line(data[copyap[select][i].piece_num].getPoint()[copyap[select][i].point_num][j].x + copyap[select][i].base_point.x,
				data[copyap[select][i].piece_num].getPoint()[copyap[select][i].point_num][j].y + copyap[select][i].base_point.y,
				data[copyap[select][i].piece_num].getPoint()[copyap[select][i].point_num][(j + 1) % data[copyap[select][i].piece_num].getPoint()[copyap[select][i].point_num].size()].x + copyap[select][i].base_point.x,
				data[copyap[select][i].piece_num].getPoint()[copyap[select][i].point_num][(j + 1) % data[copyap[select][i].piece_num].getPoint()[copyap[select][i].point_num].size()].y + copyap[select][i].base_point.y).draw(0.1, s3d::Color(255, 0, 0));
			}
			else {
			  s3d::Line(data[copyap[select][i].piece_num].getPoint()[copyap[select][i].point_num][j].x + copyap[select][i].base_point.x,
				data[copyap[select][i].piece_num].getPoint()[copyap[select][i].point_num][j].y + copyap[select][i].base_point.y,
				data[copyap[select][i].piece_num].getPoint()[copyap[select][i].point_num][(j + 1) % data[copyap[select][i].piece_num].getPoint()[copyap[select][i].point_num].size()].x + copyap[select][i].base_point.x,
				data[copyap[select][i].piece_num].getPoint()[copyap[select][i].point_num][(j + 1) % data[copyap[select][i].piece_num].getPoint()[copyap[select][i].point_num].size()].y + copyap[select][i].base_point.y).draw(0.1, s3d::Color(0, 255, 0));
			}
			tmp.push_back(s3d::Vec2(data[copyap[select][i].piece_num].getPoint()[copyap[select][i].point_num][j].x + copyap[select][i].base_point.x,
			  data[copyap[select][i].piece_num].getPoint()[copyap[select][i].point_num][j].y + copyap[select][i].base_point.y));
		  }
		  forCollision.push_back(s3d::Polygon(tmp));
		}
		mOverPiece = -1;
		for (int i = 0; i < static_cast<int>(forCollision.size()); ++i) {
		  s3d::Polygon tmp(forCollision[i]);
		  tmp.scale(10);
		  if (tmp.mouseOver) {
			tmp.scale(0.1);
			mOverPiece = i;
			forCollision[i].draw(s3d::Color(0, 255, 0));
		  }
		}

		//reset
		s3d::Graphics2D::SetTransform(
		  s3d::Mat3x2::Scale(1, s3d::Float2(0, 0))*
		  s3d::Mat3x2::Translate(0, 0)
		);

		if (mOverPiece != -1) {
		  font30(copyap[select][mOverPiece].piece_num).draw(s3d::Mouse::Pos());
		}

	  }

	  //reset
	  s3d::Graphics2D::SetTransform(
		s3d::Mat3x2::Scale(1, s3d::Float2(0, 0))*
		s3d::Mat3x2::Translate(0, 0)
	  );


	}

  }
}


void Main() {

  std::thread m(monitor);

  std::vector<std::thread> th;

  for (int i = 0; i < 8; ++i) {
	th.push_back(std::thread(func, i));
  }

  m.join();
  for (int i = 0; i < 8; ++i) {
	th[i].join();
  }

}


int checkHit(std::vector<putData> &already_put, putData &put, Geometry &geometry) {

  std::vector<Point> cp1(data[put.piece_num].getPoint()[put.point_num]);
  //移動
  move(cp1, put.base_point);




  //旧当たり判定
  for (int i = 0; i < static_cast<int>(already_put.size()); ++i) {
	std::vector<Point> cp2(data[already_put[i].piece_num].getPoint()[already_put[i].point_num]);
	//移動
	move(cp2, already_put[i].base_point);

	if (collisionPiece(cp1, cp2)) {
	  return 1;
	}
  }

  int flag = 0;
  for (int i = 0; i < static_cast<int>(framePoint.size()); i++) {
	if (!collisionFrame(framePoint[i], cp1)) {
	  flag = 1;
	}
  }

  if (!flag) {
	//printf("frame");
	return 1;
  }
  //旧当たり判定ここまで


  /*
  //新当たり判定（開発中）
  if (collisionNotPutArea(geometry.areaPoint, cp1)) {
	return 1;
  }
  //新当たり判定ここまで
  */

  return 0;
}

int isAlong(int num, Geometry &geometry, putData &putdata) {
  std::pair<int, int> areaNum;
  areaNum = solver[num].geometry.getAreaNum();

  areamtx[num].lock();
  int size = solver[num].geometry.areaPoint[areaNum.first].size();
  Vector f = solver[num].geometry.areaPoint[areaNum.first][(areaNum.second + 1) % size] - solver[num].geometry.areaPoint[areaNum.first][areaNum.second];
  Vector b = solver[num].geometry.areaPoint[areaNum.first][(areaNum.second - 1 + size) % size] - solver[num].geometry.areaPoint[areaNum.first][areaNum.second];
  areamtx[num].unlock();

  for (int i = 0; i < static_cast<int>(data[putdata.piece_num].getPoint()[putdata.point_num].size()); ++i) {
	//頂点上にあるかどうか
	if (solver[num].geometry.areaPoint[areaNum.first][areaNum.second] == (data[putdata.piece_num].getPoint()[putdata.point_num][i]+putdata.base_point)) {
	  Vector pf = data[putdata.piece_num].getPoint()[putdata.point_num][(i+1)% data[putdata.piece_num].getPoint()[putdata.point_num].size()] -
		data[putdata.piece_num].getPoint()[putdata.point_num][i];
	  Vector pb = data[putdata.piece_num].getPoint()[putdata.point_num][(i - 1 + data[putdata.piece_num].getPoint()[putdata.point_num].size()) % data[putdata.piece_num].getPoint()[putdata.point_num].size()] -
		data[putdata.piece_num].getPoint()[putdata.point_num][i];
	  if (cross(f, pf) == 0 || cross(b, pb) == 0) {
		return 1;
	  }
	}
  }

  return 0;
}

int solve(int num) {
  if (data.size() == solver[num].already_put.size()) {
	//全部置いたってこと
	return 1;
  }

  //全ピース見ていこうな
  Point tmp = solver[num].geometry.getPutPoint(data, solver[num].already_put, LEFT, areamtx[num]);
  for (int i = 0; i < static_cast<int>(data.size()); ++i) {//ピースの数
	int ii;
	ii = (i + solver[num].start) % data.size();

	//今のピースがすでに置かれているかどうか
	if (solver[num].isPut[ii] == 0) {
	  for (int j = 0; j < static_cast<int>(data[ii].getPoint().size()); ++j) {//回転の組み合わせの数

																			  // printf("(%2d,%2d,%2d) --> (%3d,%3d) result -->", ii, j, k,tmp.x,tmp.y);
		Point putPoint(tmp.x - data[ii].getShapeEdge(j, LEFT).x, tmp.y - data[ii].getShapeEdge(j, LEFT).y);
		putData put(ii, j, 0, putPoint);


		if (isAlong(num, solver[num].geometry, put) && !checkHit(solver[num].already_put, put, solver[num].geometry)) {

		  //もし当たり判定がokなら
		  //setColor(F_CYAN | F_INTENSITY);
		  //printf("       put\n");
		  //setColor();

		  mtx[num].lock();
		  solver[num].already_put.push_back(put);
		  mtx[num].unlock();

		  solver[num].isPut[ii] = 1;

		  if (solve(num)) {
			//もしreturn 1なら解き終わったってこと
			return 1;
		  }
		}
		else {
		  //setColor(F_RED | F_INTENSITY);
		  //printf_s("Hit!!!!\n");
		  //setColor();
		}
	  }
	}
  }

  //ここまで来たってことはダメだったってことだからpopしてバック
  if (solver[num].already_put.size()) {
	//setColor(F_ORANGE | F_INTENSITY);
	//printf("back  depth = %10d\n", already_put.size());
	//setColor();

	solver[num].isPut[solver[num].already_put[solver[num].already_put.size() - 1].piece_num] = 0;
	solver[num].geometry.cancelPut();

	mtx[num].lock();
	solver[num].already_put.pop_back();
	mtx[num].unlock();
  }
  else {
	//setColor(F_ORANGE | F_INTENSITY);
	//printf("back  depth = %10d\n",0);
	//setColor();
  }

  return 0;
}

