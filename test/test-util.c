#include "test-util.h"

int checkBlock(int a[4][4], int b[4][4]) {
  for (int i = 0; i < 16; ++i) {
    if (a[i % 4][i / 4] != b[i % 4][i / 4])
      return 0;
  }
  return 1;
}
