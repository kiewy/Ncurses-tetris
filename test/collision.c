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

START_TEST(test_block_collision_lb)
	// Make a 6x6 field,
	// with L-block once rotated at (0,2):
	// [ 0 0 0 0 0 0 ]
	// [ 0 0 0 0 0 0 ]
	// [ 0 0 0 0 0 0 ]
	// [ 6 6 6 0 0 0 ]
	// [ 6 0 0 1 0 0 ]
	// [ 1 1 1 1 1 1 ]
	int field[36];

	for (int i = 35; i > 29; --i)
		field[i] = 1;
	field[27] = 1;
	// Block is an L-block, once rotated
	// [ 0 0 0 0 ]
	// [ 6 6 6 0 ]
	// [ 6 0 0 0 ]
	// [ 0 0 0 0 ]
	int block[4][4];
	copyBlock(block, block_L);
	rotateBlock(block, 3);
	// colcheck returns: 0b(left)(bottom)(right)(top) solid
	// block is at the left edge of the field, and touching a 
	// solid square, so both bottom and left should be true
	int col = colCheck(field, 6, 6, block, 2, 0);

	ck_assert_msg(col == 0b1100,
			"BLOCK COLLISION TEST: TOUCH LEFT BOTTOM FAILED");
END_TEST

START_TEST(test_block_collision_oob_left)
{
	int field[36];
	for (int i = 35; i > 29; --i)
		field[i] = 1;
	field[27] = 1;
	int block[4][4];
	copyBlock(block, block_L);
	rotateBlock(block, 3);
	int col = colCheck(field, 6, 6, block, 4, 0);
	ck_assert_msg(col == 0b1111,
			"BLOCK COLLISION TEST: OUT-OF-BOUNDS LEFT FAILED");
}
END_TEST
