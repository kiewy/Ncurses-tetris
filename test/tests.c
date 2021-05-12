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

#include "block-copy.c"
#include "rotation.c"
#include "collision.c"

//#define VERBOSITY CK_VERBOSE
#define VERBOSITY CK_NORMAL

Suite* block_copy_suite(void)
{
	// Create and allocate suite
	Suite *s;
	s = suite_create("Block-copy");
	// Setup cases
	TCase *tc_Z_copy = tcase_create("Z-copy");
	// Add tests to cases
	tcase_add_test(tc_Z_copy, test_block_copy);
	// Add cases to suite
	suite_add_tcase(s, tc_Z_copy);

	return s;
}

Suite* block_rotation_suite(void)
{
	// Create suite
	Suite *s;
	s = suite_create("Block-rotation");
	// Create cases
	TCase *tc_T_rotation = tcase_create("T-rotate"),
		  *tc_I_rotation = tcase_create("I-rotate");

	tcase_add_test(tc_T_rotation, test_T_rotate_once);
	tcase_add_test(tc_I_rotation, test_I_rotate_once);

	// Add cases to suite
	suite_add_tcase(s, tc_T_rotation);
	suite_add_tcase(s, tc_I_rotation);
	return(s);
}

Suite* block_collision_suite(void)
{
	// Create suite
	Suite *s;
	s = suite_create("collision");
	// Create cases
	TCase *tc_collision_l = tcase_create("collision-touch-left"),
		  *tc_collision_r = tcase_create("collision-touch-right"),
		  *tc_collision_top = tcase_create("collision-touch-top"),
		  *tc_collision_bottom = tcase_create("collision-touch-bottom"),
		  *tc_collision_oob_l = tcase_create("collision-oob-left"),
		  *tc_collision_oob_r = tcase_create("collision-oob-right"),
		  *tc_collision_oob_top = tcase_create("collision-oob-top"),
		  *tc_collision_oob_bottom = tcase_create("collision-oob-bottom"),
		  *tc_collision_clipping = tcase_create("collision-clipping");

	// Add tests to cases
	tcase_add_test(tc_collision_l, test_block_collision_l);
	tcase_add_test(tc_collision_r, test_block_collision_r);
	tcase_add_test(tc_collision_top, test_block_collision_top);
	tcase_add_test(tc_collision_bottom, test_block_collision_bottom);
	tcase_add_test(tc_collision_oob_l, test_block_collision_oob_left);
	tcase_add_test(tc_collision_oob_r, test_block_collision_oob_right);
	tcase_add_test(tc_collision_oob_top, test_block_collision_oob_top);
	tcase_add_test(tc_collision_oob_bottom, test_block_collision_oob_bottom);
	tcase_add_test(tc_collision_clipping, test_block_collision_clipping);

	// Add cases to suite
	suite_add_tcase(s, tc_collision_l);
	suite_add_tcase(s, tc_collision_r);
	suite_add_tcase(s, tc_collision_top);
	suite_add_tcase(s, tc_collision_bottom);
	suite_add_tcase(s, tc_collision_oob_l);
	suite_add_tcase(s, tc_collision_oob_r);
	suite_add_tcase(s, tc_collision_oob_top);
	suite_add_tcase(s, tc_collision_oob_bottom);
	suite_add_tcase(s, tc_collision_clipping);

	return(s);
}

int main() {
	int number_failed;
	SRunner *sr;

	// Block copy suite
	Suite *blkcs;
	blkcs = block_copy_suite();
	sr = srunner_create(blkcs);
	srunner_run_all(sr, VERBOSITY);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);

	// Block rotation suite
	Suite *brs;
	brs = block_rotation_suite();
	sr = srunner_create(brs);
	srunner_run_all(sr, VERBOSITY);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);

	// Block collision suite
	Suite *bcs;
	bcs = block_collision_suite();
	sr = srunner_create(bcs);
	srunner_run_all(sr, VERBOSITY);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);

	return (number_failed == 0) ? 0 : -1;
}
