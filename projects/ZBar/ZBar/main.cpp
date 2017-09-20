#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <zbar.h>
#include <iostream>
#include <fstream>


int main(){
  cv::VideoCapture cap(0);

  if (!cap.isOpened()){
	std::cout << "カメラが使えなかった（哲学）" << std::endl;
	return -1;
  }

  zbar::ImageScanner scanner;
  scanner.set_config(zbar::ZBAR_NONE, zbar::ZBAR_CFG_ENABLE, 1);

  double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH);
  double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

  cv::namedWindow("Camera", CV_WINDOW_AUTOSIZE);

  std::vector<std::string> data;

  while (1){
	cv::Mat frame;

	if (!cap.read(frame)){
	  std::cout << "カメラから画像読み込めなかった" << std::endl;
	  break;
	}

	cv::Mat grey;
	cv::cvtColor(frame, grey, CV_BGR2GRAY);
	uchar *raw = (uchar *)grey.data;

	int width = frame.cols;
	int height = frame.rows;

	zbar::Image image(width, height, "Y800", raw, width * height);
	//スキャン
	int barcodeNum = scanner.scan(image);


	//QRコードの数だけ繰り返す
	int inputNum = 0;
	for (zbar::Image::SymbolIterator symbol = image.symbol_begin();symbol != image.symbol_end();++symbol){
	  
	  data.push_back(symbol->get_data());

	  std::cout << "input_" << inputNum+1 << "\"" << *(data.end()-1)  << "\"" << std::endl;


	  std::vector<cv::Point> vp;
	  int n = symbol->get_location_size();
	  for (int i = 0; i<n; i++) {
		vp.push_back(cv::Point(symbol->get_location_x(i), symbol->get_location_y(i)));
	  }
	  cv::RotatedRect r = minAreaRect(vp);
	  cv::Point2f pts[4];
	  r.points(pts);
	  for  (int i = 0; i<4; i++) {
		line(frame, pts[i], pts[(i + 1) % 4], cv::Scalar(255, 0, 0), 3);
	  }  

	  inputNum++;
	}

	if (barcodeNum) {
	  std::cout << "読み込んだバーコードの合計数 = " << data.size() << std::endl;
	  std::cout << "まだ読み込む(y/n)" << std::endl;
	  char a;
	  std::cin >> a;

	  if (a == 'n' || a == 'N') {
		std::ofstream ost("out.txt");
		ost << data.size();
		ost << "\n";
		for (auto i : data) {
		  ost << i;
		  ost << "\n";
		}

		break;
	  }
	  else {
		if (!cap.read(frame)) {
		  std::cout << "カメラから画像読み込めなかった" << std::endl;
		  break;
		}
	  }
	}

	imshow("Camera", frame);//ウィンドウに表示

	//escで終了
	if (cv::waitKey(30) == 27){
	  std::cout << "終了" << std::endl;
	  break;
	}

  }
  return 0;

}