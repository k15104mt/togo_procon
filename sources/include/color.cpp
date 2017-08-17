#include"color.hpp"

void setColor(int fc, int bc){
  if (fc == 0) {
	fc = F_WHITE;
  }
  if (bc == 0) {
	bc = 0x00;
  }
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),fc|bc);
}
