#include"color.hpp"

void setColor(int fc, int bc){
  if (fc == -1) {
	fc = F_WHITE;
  }
  if (bc == -1) {
	bc = 0x00;
  }
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),fc|bc);
}
