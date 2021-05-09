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

#include "rotation.c"
#include "collision.c"

Suite* block_rotation_suite(void)
{
	// Create suite
	Suite *s;
	s = suite_create("Blocks");
	// Create cases
	TCase *tc_T_rotation, *tc_I_rotation;

	tc_T_rotation = tcase_create("L-rotate");
	tcase_add_test(tc_T_rotation, test_L_rotate_once);

	tc_I_rotation = tcase_create("I-rotate");
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
	TCase *tc_collision_bl, *tc_collision_oob_l;
	tc_collision_bl = tcase_create("collision-bottom-left");
	tcase_add_test(tc_collision_bl, test_block_collision_lb);

	tc_collision_oob_l = tcase_create("collision-oob-left");
	tcase_add_test(tc_collision_oob_l, test_block_collision_oob_left);

	// Add cases to suite
	suite_add_tcase(s, tc_collision_bl);
	suite_add_tcase(s, tc_collision_oob_l);

	return(s);
}

int main() {
	int number_failed;
	SRunner *sr;

	// Block rotation suite
	Suite *brs;
	brs = block_rotation_suite();
	sr = srunner_create(brs);
	srunner_run_all(sr, CK_VERBOSE);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);

	// Block collision suite
	Suite *bcs;
	bcs = block_collision_suite();
	sr = srunner_create(bcs);
	srunner_run_all(sr, CK_VERBOSE);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);

	return (number_failed == 0) ? 0 : -1;
}
