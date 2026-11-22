#include "TCloud.h"

Cloud::Cloud(int _x, int _y) : Point(_x, _y) {
  // Задали высоту и длинну
  width = 256;
  height = 256;
  for (int i = 0; i < mapN*mapM/8; i++)
        bitMap[i] = image2[i];
}

Cloud::~Cloud() {}