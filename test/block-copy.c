#include <check.h>
#include "test-util.h"
#include <stdio.h>

#ifndef BLOCKS
	#include "../src/blocks.h"
	#define BLOCKS
#endif

#ifndef FUNCS
	#include "../src/funcs.h"
	#define FUNCS
#endif

START_TEST(test_block_copy)
{
	int block[4][4];

	copyBlock(block, block_Z);

	int block_ref[4][4] = {
		{0, 5, 5, 0},
		{5, 5, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
	};

	int blocksEqual = checkBlock(block, block_ref);

	//ck_assert_msg(blocksEqual <= 0,
			//"BLOCK COPY TEST FAILED");
	ck_assert(blocksEqual <= 0);
}
END_TEST
