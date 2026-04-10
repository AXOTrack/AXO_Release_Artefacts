#include <zephyr/ztest.h>
#include <vector>
#include <algorithm>

#define private public
#include "data/hit_detect.h"

ZTEST_SUITE(tests_hit_detect, NULL, NULL, NULL, NULL, NULL);

ZTEST(tests_hit_detect, hit_detect_tests)
{
	int32_t samples_per_sec = 4000;
	int16_t threshold = 800;
	int16_t max_msec = 2;

	HitDetector* hitdetector = new HitDetector(samples_per_sec, threshold, max_msec);

	zassert_not_null(hitdetector, "memory alloc failed");
	zassert_equal(hitdetector->max_samples, 8, "Max sample is different");

	delete hitdetector;
}

// MISSING for 100% branch coverage static instantiation does not deliver NULL
// here code line 
// are not covered completly.


ZTEST(tests_hit_detect, hit_detect_tests_reset)
{
	int32_t samples_per_sec = 4000;
	int16_t threshold = 800;
	int16_t max_msec = 2;

	HitDetector* hitdetector = new HitDetector(samples_per_sec, threshold, max_msec);

	hitdetector->reset();

	zassert_equal(hitdetector->n_cur, 0, "n_cur is different");
	zassert_equal(hitdetector->n_high, 0, "n_high is different");
	zassert_equal(hitdetector->hit_cnt, 0, "hit_cnt is different");

	delete hitdetector;
}

ZTEST(tests_hit_detect, hit_detect_tests_input)
{
	int32_t samples_per_sec = 4000;
	int16_t threshold = 800;
	int16_t max_msec = 2;

	HitDetector* hitdetectorCrit = new HitDetector(samples_per_sec, threshold, max_msec);
	
	std::vector<int> v = {200, 210, 215, 218, 225, 228, 230};

	for (int n = 0; n < 256; ++n) {
		float z = 299.0;
		if (std::find(v.begin(), v.end(), n) != v.end()) {
		    z = 900.0;
		}
		hitdetectorCrit->input(z);
	}

    auto n_crit = hitdetectorCrit->getNumHits();
	zassert_equal(n_crit, 7, "hit_cnt is different");

	delete hitdetectorCrit;
}

// Test if (is_above && n_high == 0)    4 cases
// is_above 		1  0  1  0
// n_high == 0   	1  1  0  0

// Test if (is_below && n_high && n_cur - n_high <= max_samples)     8 cases
// is_below                           0  0  0  0  1  1  1  1
// n_high != 0                        0  0  1  1  0  0  1  1
// n_cur - n_high <= max_samples      0  1  0  1  0  1  0  1

// #############################################################

// Test if (is_above && n_high == 0)    1/4
// is_above 		1
// n_high == 0   	1
// Test if (is_below && n_high && n_cur - n_high <= max_samples)    2/8
// is_below 		                  0
// n_high != 0   	                  0
// n_cur - n_high <= max_samples      1
ZTEST(tests_hit_detect, hit_detect_tests_input2)
{
	int32_t samples_per_sec = 4000;
	int16_t threshold = 800;
	int16_t max_msec = 2;

	HitDetector* hitdetectorCrit = new HitDetector(samples_per_sec, threshold, max_msec);
	
	hitdetectorCrit->input(900);
	auto n_crit = hitdetectorCrit->getNumHits();
	zassert_equal(hitdetectorCrit->n_high, 1, "n_high not 1");
	zassert_equal(n_crit, 0, "hit_cnt not 0");

	delete hitdetectorCrit;
}

// Test if (is_above && n_high == 0)    2/4
// is_above 		0
// n_high == 0   	1
// Test if (is_below && n_high && n_cur - n_high <= max_samples)    6/8
// is_below 		                  1
// n_high != 0   	                  0
// n_cur - n_high <= max_samples      1
ZTEST(tests_hit_detect, hit_detect_tests_input3)
{
	int32_t samples_per_sec = 4000;
	int16_t threshold = 800;
	int16_t max_msec = 2;

	HitDetector* hitdetectorCrit = new HitDetector(samples_per_sec, threshold, max_msec);
	
	hitdetectorCrit->input(700);
	auto n_crit = hitdetectorCrit->getNumHits();
	zassert_equal(hitdetectorCrit->n_high, 0, "n_high not 0");
	zassert_equal(n_crit, 0, "hit_cnt not 0");

	delete hitdetectorCrit;
}

// Test if (is_above && n_high == 0)    3/4
// is_above 		1
// n_high == 0   	0
// Test if (is_below && n_high && n_cur - n_high <= max_samples)    4/8
// is_below 		                  0
// n_high != 0   	                  1
// n_cur - n_high <= max_samples      1
ZTEST(tests_hit_detect, hit_detect_tests_input4)
{
	int32_t samples_per_sec = 4000;
	int16_t threshold = 800;
	int16_t max_msec = 2;

	HitDetector* hitdetectorCrit = new HitDetector(samples_per_sec, threshold, max_msec);
	
	hitdetectorCrit->n_high = 1;
	hitdetectorCrit->input(900);
	auto n_crit = hitdetectorCrit->getNumHits();
	zassert_equal(hitdetectorCrit->n_high, 1, "n_high not 1");
	zassert_equal(n_crit, 0, "hit_cnt not 0");

	delete hitdetectorCrit;
}

// Test if (is_above && n_high == 0)    4/4
// is_above 		0
// n_high == 0   	0
// Test if (is_below && n_high && n_cur - n_high <= max_samples)    8/8
// is_below 		                  1
// n_high != 0   	                  1
// n_cur - n_high <= max_samples      1
ZTEST(tests_hit_detect, hit_detect_tests_input5)
{
	int32_t samples_per_sec = 4000;
	int16_t threshold = 800;
	int16_t max_msec = 2;

	HitDetector* hitdetectorCrit = new HitDetector(samples_per_sec, threshold, max_msec);
	
	hitdetectorCrit->n_high = 1;
	hitdetectorCrit->input(700);
	auto n_crit = hitdetectorCrit->getNumHits();
	zassert_equal(hitdetectorCrit->n_high, 0, "n_high not 0");
	zassert_equal(n_crit, 1, "hit_cnt not 1");

	delete hitdetectorCrit;
}

// Test if (is_below && n_high && n_cur - n_high <= max_samples)    1/8
// is_below 		                  0
// n_high != 0   	                  0   NOT TESTABLE
// n_cur - n_high <= max_samples      0

// Test if (is_below && n_high && n_cur - n_high <= max_samples)    3/8
// is_below 		                  0
// n_high != 0   	                  1
// n_cur - n_high <= max_samples      0
ZTEST(tests_hit_detect, hit_detect_tests_input6)
{
	int32_t samples_per_sec = 4000;
	int16_t threshold = 800;
	int16_t max_msec = 2;

	HitDetector* hitdetectorCrit = new HitDetector(samples_per_sec, threshold, max_msec);
	
	hitdetectorCrit->n_cur = 10;
	hitdetectorCrit->n_high = 1;
	hitdetectorCrit->input(900);
	auto n_crit = hitdetectorCrit->getNumHits();
	zassert_equal(hitdetectorCrit->n_high, 1, "n_high not 1");
	zassert_equal(n_crit, 0, "hit_cnt not 0");

	delete hitdetectorCrit;
}

// Test if (is_below && n_high && n_cur - n_high <= max_samples)    5/8
// is_below 		                  0
// n_high != 0   	                  1
// n_cur - n_high <= max_samples      1
ZTEST(tests_hit_detect, hit_detect_tests_input7)
{
	int32_t samples_per_sec = 4000;
	int16_t threshold = 800;
	int16_t max_msec = 2;

	HitDetector* hitdetectorCrit = new HitDetector(samples_per_sec, threshold, max_msec);
	
	hitdetectorCrit->n_high = 1;
	hitdetectorCrit->input(900);
	auto n_crit = hitdetectorCrit->getNumHits();
	zassert_equal(hitdetectorCrit->n_high, 1, "n_high not 1");
	zassert_equal(n_crit, 0, "hit_cnt not 0");

	delete hitdetectorCrit;
}

// Test if (is_below && n_high && n_cur - n_high <= max_samples)    7/8
// is_below 		                  1
// n_high != 0   	                  1
// n_cur - n_high <= max_samples      0
ZTEST(tests_hit_detect, hit_detect_tests_input8)
{
	int32_t samples_per_sec = 4000;
	int16_t threshold = 800;
	int16_t max_msec = 2;

	HitDetector* hitdetectorCrit = new HitDetector(samples_per_sec, threshold, max_msec);
	
	hitdetectorCrit->n_cur = 10;
	hitdetectorCrit->n_high = 1;
	hitdetectorCrit->input(700);
	auto n_crit = hitdetectorCrit->getNumHits();
	zassert_equal(hitdetectorCrit->n_high, 1, "n_high not 1");
	zassert_equal(n_crit, 0, "hit_cnt not 0");

	delete hitdetectorCrit;
}


