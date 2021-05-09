#include <check.h>
#include "../src/blocks.h"
#include "../src/funcs.h"

int checkBlock(int a[4][4], int b[4][4]) {
	for (int i = 0; i < 16; ++i) {
		if (a[i%4][i/4] != b[i%4][i/4])
			return 0;
	}
	return 1;
}

START_TEST(test_block_rotate)
{
	// Initialize new T-block
	int newBlock[4][4];
	copyBlock(newBlock, block_T);
	// Rotate block
	rotateBlock(newBlock,3);
	
	int block_rotated[4][4] = 
	{{0, 0, 0, 0},
	 {0, 0, 0, 0},
	 {0, 0, 0, 0},
	 {0, 0, 0, 0}};

	ck_assert_msg(checkBlock(block_rotated, newBlock) == 1,
			"Blocks aren't equal!!");
}
END_TEST

Suite * block_suite(void)
{
	Suite *s;

	TCase *tc_core;
	s = suite_create("Blocks");
	
	tc_core = tcase_create("Core");

	tcase_add_test(tc_core, test_block_rotate);
	suite_add_tcase(s, tc_core);

	return (s);
}

int main() {
	int number_failed;
	Suite *s;
	SRunner *sr;

	s = block_suite();
	sr = srunner_create(s);

	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? 0 : -1;
}

