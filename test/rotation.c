#include <check.h>
#include "test-util.h"

#ifndef BLOCKS
	#include "../src/blocks.h"
	#define BLOCKS
#endif

#ifndef FUNCS
	#include "../src/funcs.h"
	#define FUNCS
#endif

START_TEST(test_L_rotate_once)
{
	// Initialize new T-block
	int newBlock[4][4];
	copyBlock(newBlock, block_T);
	// Rotate block
	rotateBlock(newBlock,3);

	int block_rotated[4][4] = 
	{{0, 3, 0, 0},
	 {0, 3, 3, 0},
	 {0, 3, 0, 0},
	 {0, 0, 0, 0}};

	ck_assert_msg(checkBlock(block_rotated, newBlock) == 1,
			"T BLOCK ROTATION TEST FAILED");
}
END_TEST

START_TEST(test_I_rotate_once)
{
	int block[4][4];
	copyBlock(block, block_I);
	rotateBlock(block, 4);
	int block_rotated[4][4] = {
		{0, 0, 0, 0},
		{2, 2, 2, 2},
		{0, 0, 0, 0},
		{0, 0, 0, 0}};
	ck_assert_msg(checkBlock(block_rotated, block) == 1,
			"I BLOCK ROTATION TEST FAILED");
}
END_TEST
