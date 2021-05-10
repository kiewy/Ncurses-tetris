#include "test-util.h"

int checkBlock(int a[4][4], int b[4][4]) {
	int result = 0;

	for (int i = 0; i < 4; ++i) {
		for (int k = 0; k < 4; ++k) {
			if (a[i][k] != b[i][k])
				++result;
		}
	}

	return result;
}
