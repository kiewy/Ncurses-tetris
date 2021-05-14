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

START_TEST(test_block_collision_l)
{
	// Make a 6x6 field,
	// with L-block at (1,2):
	// [ 0 0 0 0 0 0 ]
	// [ 0 0 0 6 0 0 ]
	// [ 0 0 0 6 0 0 ]
	// [ 0 0 1 6 6 0 ]
	// [ 0 0 1 0 0 0 ]
	// [ 1 1 1 1 1 1 ]
	int field[36] = {0};

	for (int i = 35; i > 29; --i)
		field[i] = 1;

	field[20] = 1;
	field[26] = 1;

	int block[4][4];
	copyBlock(block, block_L);
	// colcheck returns: 0b(left)(bottom)(right)(top) solid
	// block is at the left edge of the field, and touching a 
	// solid square, so both bottom and left should be true
	int col = colCheck(field, 6, 6, block, 1, 2);

	ck_assert(col == 0b1000);
}
END_TEST

START_TEST(test_block_collision_r)
{
	// Make a 6x6 field,
	// with L-block twice rotated at (1,1):
	// [ 0 0 0 0 0 0 ]
	// [ 0 6 0 0 0 0 ]
	// [ 0 6 0 0 0 0 ]
	// [ 0 6 6 1 0 0 ]
	// [ 0 0 0 1 0 0 ]
	// [ 1 1 1 1 1 1 ]
	int field[36] = {0};

	for (int i = 35; i > 29; --i)
		field[i] = 1;
	field[27] = 1;
	field[21] = 1;

	int block[4][4];
	copyBlock(block, block_L);
	// colcheck returns: 0b(left)(bottom)(right)(top) solid
	int col = colCheck(field, 6, 6, block, 1, 0);
	if (col != 0b0010)
		printf("BLOCK COL R : COL ERR: COL IS %d\n", col);

	ck_assert(col == 0b0010);
}
END_TEST

START_TEST(test_block_collision_bottom)
{
	// Make a 6x6 field:
	// [ 0 0 0 0 0 0 ]
	// [ 0 2 0 0 0 0 ]
	// [ 0 2 0 0 0 0 ]
	// [ 0 2 0 1 0 0 ]
	// [ 0 2 0 1 0 0 ]
	// [ 1 1 1 1 1 1 ]
	int field[36] = {0};

	for (int i = 35; i > 29; --i)
		field[i] = 1;
	field[27] = 1;
	field[21] = 1;

	int block[4][4];
	copyBlock(block, block_I);
	// colcheck returns: 0b(left)(bottom)(right)(top) solid
	int col = colCheck(field, 6, 6, block, 1, 0);

	ck_assert(col == 0b0100);
}
END_TEST

START_TEST(test_block_collision_top)
{
	// Make a 6x6 field:
	// [ 0 2 0 0 0 0 ]
	// [ 0 2 0 0 0 0 ]
	// [ 0 2 0 0 0 0 ]
	// [ 0 2 0 1 0 0 ]
	// [ 0 0 0 1 0 0 ]
	// [ 1 1 1 1 1 1 ]
	int field[36] = {0};

	for (int i = 35; i > 29; --i)
		field[i] = 1;
	field[27] = 1;
	field[21] = 1;

	int block[4][4];
	copyBlock(block, block_I);
	// colcheck returns: 0b(left)(bottom)(right)(top) solid
	int col = colCheck(field, 6, 6, block, 0, 0);

	ck_assert(col == 0b0001);
}
END_TEST

START_TEST(test_block_collision_clipping)
{
	// Make a 6x6 field,
	// with I-block, clipping
	// at bottom:
	// [ 0 0   0 0 0 0 ]
	// [ 0 0   0 0 0 0 ]
	// [ 0 2   0 0 0 0 ]
	// [ 0 2   0 1 0 0 ]
	// [ 0 2   0 1 0 0 ]
	// [ 1 1/2 1 1 1 1 ]
	int field[36] = {0};

	for (int i = 35; i > 29; --i)
		field[i] = 1;
	field[27] = 1;
	field[21] = 1;

	int block[4][4];
	copyBlock(block, block_I);
	// colcheck returns: 0b(left)(bottom)(right)(top) solid
	int col = colCheck(field, 6, 6, block, 2, 1);

	ck_assert(col == 0b1111);
}
END_TEST

START_TEST(test_block_collision_oob_left)
{
	int field[36] = {0};
	for (int i = 35; i > 29; --i)
		field[i] = 1;
	field[27] = 1;
	int block[4][4];
	copyBlock(block, block_L);
	int col = colCheck(field, 6, 6, block, 1, -2);
	ck_assert(col == 0b1111);
}
END_TEST

START_TEST(test_block_collision_oob_right)
{
	int field[36] = {0};
	for (int i = 35; i > 29; --i)
		field[i] = 1;
	field[27] = 1;
	int block[4][4];
	copyBlock(block, block_L);
	int col = colCheck(field, 6, 6, block, 1, 4);
	ck_assert(col == 0b1111);
}
END_TEST

START_TEST(test_block_collision_oob_top)
{
	int field[36] = {0};
	for (int i = 35; i > 29; --i)
		field[i] = 1;
	field[27] = 1;
	int block[4][4];
	copyBlock(block, block_L);
	int col = colCheck(field, 6, 6, block, -1, 4);
	ck_assert(col == 0b1111);
}
END_TEST

START_TEST(test_block_collision_oob_bottom)
{
	int field[36] = {0};
	for (int i = 35; i > 29; --i)
		field[i] = 1;
	field[27] = 1;
	int block[4][4];
	copyBlock(block, block_L);
	int col = colCheck(field, 6, 6, block, 5, 4);
	ck_assert(col == 0b1111);
}
END_TEST
