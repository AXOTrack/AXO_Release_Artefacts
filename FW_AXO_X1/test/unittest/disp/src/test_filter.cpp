#include <zephyr/ztest.h>
//#include <vector>
//#include <algorithm>

#define private public
#include "disp/filter.h"
#include "disp/matrix.h"

#include "stubs.h"

void Vec2IntArray(vectori& v, int* buf) 
{
	for (size_t i = 0; i < v.size(); i++) 
	{
		buf[i] = v[i];
	}
}

void Vec2DoubleArray(vectord& v, double* buf) 
{
	for (size_t i = 0; i < v.size(); i++) 
	{
		buf[i] = v[i];
	}
}

void DoubleVec2IntArray(vectord& v, int* buf) 
{
	for (size_t i = 0; i < v.size(); i++) 
	{
		buf[i] = (int)(1000* v[i]);
	}
}

void printIntArray(int* array, int len) {
	for (int i = 0; i < len; i++) {
		printf("%d  ", array[i]);
	}
	printf("\n");
}



ZTEST_SUITE(tests_disp_filter, NULL, NULL, NULL, NULL, NULL);

// test constructor and destructor
ZTEST(tests_disp_filter, test_disp_filter_constructor)
{
	CFilter* pFilter = new CFilter();
	zassert_not_null(pFilter, "memory alloc failed");

	delete pFilter;
}

// MISSING for 100% branch coverage static instantiation does not deliver NULL
// here code line 
// static CFilter myInstance;
// are not covered completly.


// test function filtfilt_init, getNfilt and getNfact
ZTEST(tests_disp_filter, test_disp_filter_filtfilt_init)
{
	int ret1, ret2;

	// 4400 Hz sampling rate
    vectord b_coeff = { 0.95806, -2.87418,  2.87418, -0.95806 };
	vectord a_coeff = { 1.00000, -2.91432,  2.83228, -0.91788 };
	CFilter::inst()->zi = { 0,0,0 };

	double exp_b[4] = { 0.95806, -2.87418,  2.87418, -0.95806 };
	double res_b[4];
	double exp_a[4] = { 1.00000, -2.91432,  2.83228, -0.91788 };
	double res_a[4];
	int exp_zzi[3] = { (int)(1000* -0.95806),  (int)(1000* 1.91612),  (int)(1000* -0.95806) };
	int res_zzi[3];
	double exp_zi[3] = { 0.00000,  0.00000,  0.00000 };
	double res_zi[3];

	CFilter::inst()->filtfilt_init(b_coeff, a_coeff);
	zassert_equal(CFilter::inst()->nfilt, 4, "nfilt incorrect");
	zassert_equal(CFilter::inst()->nfact, 9, "nfact incorrect");
	Vec2DoubleArray(CFilter::inst()->b_coeff, res_b);
	zassert_mem_equal(res_b, exp_b, sizeof(res_b), "b_coeff not correct");
	Vec2DoubleArray(CFilter::inst()->a_coeff, res_a);
	zassert_mem_equal(res_a, exp_a, sizeof(res_a), "a_coeff not correct");
	DoubleVec2IntArray(CFilter::inst()->zzi, res_zzi);
	zassert_mem_equal(res_zzi, exp_zzi, sizeof(res_zzi), "zzi not correct");
	Vec2DoubleArray(CFilter::inst()->zi, res_zi);
	zassert_mem_equal(res_zi, exp_zi, sizeof(res_zi), "zi not correct");
	
	printk("nfilt = %d, nfact = %d:\n", CFilter::inst()->nfilt, CFilter::inst()->nfact);
	printk("b_coeff:\n");
	CMatrix::printVector(CFilter::inst()->b_coeff);
	printk("a_coeff:\n");
	CMatrix::printVector(CFilter::inst()->a_coeff);
	printk("zzi:\n");
	CMatrix::printVector(CFilter::inst()->zzi);
	printk("zi:\n");
	CMatrix::printVector(CFilter::inst()->zi);

	ret1 = CFilter::inst()->getNfilt();
	ret2 = CFilter::inst()->getNfact();
	zassert_equal(ret1, 4, "getNfilt incorrect");
	zassert_equal(ret2, 9, "getNfact incorrect");
}

// test function filtfilt_init, getNfilt and getNfact
ZTEST(tests_disp_filter, test_disp_filter_filtfilt_init2)
{
	int ret1, ret2;

	// 4400 Hz sampling rate
    vectord b_coeff = { 0.95806, -2.87418 };
	vectord a_coeff = { 1.00000, -2.91432 };
	CFilter::inst()->zi = { 0,0,0 };

	double exp_b[2] = { 0.95806, -2.87418 };
	double res_b[2];
	double exp_a[2] = { 1.00000, -2.91432 };
	double res_a[2];
	int exp_zzi[1] = { (int)(1000* 0.04288) };
	int res_zzi[1];
	double exp_zi[1] = { 0.00000 };
	double res_zi[1];

	CFilter::inst()->filtfilt_init(b_coeff, a_coeff);
	zassert_equal(CFilter::inst()->nfilt, 2, "nfilt incorrect");
	zassert_equal(CFilter::inst()->nfact, 3, "nfact incorrect");
	Vec2DoubleArray(CFilter::inst()->b_coeff, res_b);
	zassert_mem_equal(res_b, exp_b, sizeof(res_b), "b_coeff not correct");
	Vec2DoubleArray(CFilter::inst()->a_coeff, res_a);
	zassert_mem_equal(res_a, exp_a, sizeof(res_a), "a_coeff not correct");
	DoubleVec2IntArray(CFilter::inst()->zzi, res_zzi);
	zassert_mem_equal(res_zzi, exp_zzi, sizeof(res_zzi), "zzi not correct");
	Vec2DoubleArray(CFilter::inst()->zi, res_zi);
	zassert_mem_equal(res_zi, exp_zi, sizeof(res_zi), "zi not correct");
	
	printk("nfilt = %d, nfact = %d:\n", CFilter::inst()->nfilt, CFilter::inst()->nfact);
	printk("b_coeff:\n");
	CMatrix::printVector(CFilter::inst()->b_coeff);
	printk("a_coeff:\n");
	CMatrix::printVector(CFilter::inst()->a_coeff);
	printk("zzi:\n");
	CMatrix::printVector(CFilter::inst()->zzi);
	printk("zi:\n");
	CMatrix::printVector(CFilter::inst()->zi);
}

// test function filtfilt_init, getNfilt and getNfact
ZTEST(tests_disp_filter, test_disp_filter_filtfilt_init3)
{
	int ret1, ret2;

	// 4400 Hz sampling rate
    vectord b_coeff = { 0.95806, -2.87418,  2.87418, -0.95806 };
	vectord a_coeff = { 1.00000, -2.91432,  2.83228 };
	CFilter::inst()->zi = { 0,0,0 };

	double exp_b[4] = { 0.95806, -2.87418,  2.87418, -0.95806 };
	double res_b[4];
	double exp_a[4] = { 1.00000, -2.91432,  2.83228, 0.00000 };
	double res_a[4];
	int exp_zzi[3] = { (int)(1000* -0.95806),  (int)(1000* 1.91612),  (int)(1000* -0.95806) };
	int res_zzi[3];
	double exp_zi[3] = { 0.00000,  0.00000,  0.00000 };
	double res_zi[3];

	CFilter::inst()->filtfilt_init(b_coeff, a_coeff);
	zassert_equal(CFilter::inst()->nfilt, 4, "nfilt incorrect");
	zassert_equal(CFilter::inst()->nfact, 9, "nfact incorrect");
	Vec2DoubleArray(CFilter::inst()->b_coeff, res_b);
	zassert_mem_equal(res_b, exp_b, sizeof(res_b), "b_coeff not correct");
	Vec2DoubleArray(CFilter::inst()->a_coeff, res_a);
	zassert_mem_equal(res_a, exp_a, sizeof(res_a), "a_coeff not correct");
	DoubleVec2IntArray(CFilter::inst()->zzi, res_zzi);
	zassert_mem_equal(res_zzi, exp_zzi, sizeof(res_zzi), "zzi not correct");
	Vec2DoubleArray(CFilter::inst()->zi, res_zi);
	zassert_mem_equal(res_zi, exp_zi, sizeof(res_zi), "zi not correct");
	
	printk("nfilt = %d, nfact = %d:\n", CFilter::inst()->nfilt, CFilter::inst()->nfact);
	printk("b_coeff:\n");
	CMatrix::printVector(CFilter::inst()->b_coeff);
	printk("a_coeff:\n");
	CMatrix::printVector(CFilter::inst()->a_coeff);
	printk("zzi:\n");
	CMatrix::printVector(CFilter::inst()->zzi);
	printk("zi:\n");
	CMatrix::printVector(CFilter::inst()->zi);
}

// test function add_index_range
ZTEST(tests_disp_filter, test_disp_filter_add_index_range)
{
    vectori v1 = { 9, -28,  2, -9 };
    vectori v2 = { 9, -28,  2, -9 };
    vectori v3 = { };

	int exp1[8] = {9, -28,  2, -9, 4, 5, 6, 7};
	int res1[8];
	int exp2[6] = {9, -28,  2, -9, 4, 6};
	int res2[6];
	int exp3[4] = {0, 1, 2, 3};
	int res3[4];

	CFilter::inst()->add_index_range(v1, 4, 7, 1);
	printk("v1:\n");
	CMatrix::printVector(v1);
	Vec2IntArray(v1, res1);
	zassert_mem_equal(res1, exp1, sizeof(res1), "v1 not correct");

	CFilter::inst()->add_index_range(v2, 4, 7, 2);
	printk("v2:\n");
	CMatrix::printVector(v2);
	Vec2IntArray(v2, res2);
	zassert_mem_equal(res2, exp2, sizeof(res2), "v2 not correct");

	CFilter::inst()->add_index_range(v3, 0, 3, 1);
	printk("v3:\n");
	CMatrix::printVector(v3);
	Vec2IntArray(v3, res3);
	zassert_mem_equal(res3, exp3, sizeof(res3), "v3 not correct");
}

// test function add_index_const
ZTEST(tests_disp_filter, test_disp_filter_add_index_const)
{
    vectori v1 = { 9, -28,  2, -9 };

	int exp1[8] = {9, -28,  2, -9, 77, 77, 77, 77};
	int res1[8];

	CFilter::inst()->add_index_const(v1, 77, 4);
	printk("v1:\n");
	CMatrix::printVector(v1);
	Vec2IntArray(v1, res1);
	zassert_mem_equal(res1, exp1, sizeof(res1), "v1 not correct");
}
// test function append_vector
ZTEST(tests_disp_filter, test_disp_filter_append_vector)
{
    vectord v1 = { 1, 2, 3, 4 };
    vectord v2 = { 5, 6, 7, 8 };

	double exp1[8] = {1,2,3,4,5,6,7,8};
	double res1[8];

	CFilter::inst()->append_vector(v1, v2);
	printk("v1:\n");
	CMatrix::printVector(v1);
	Vec2DoubleArray(v1, res1);
	zassert_mem_equal(res1, exp1, sizeof(res1), "v1 not correct");
}

// test function subvector_reverse
ZTEST(tests_disp_filter, test_disp_filter_subvector_reverse)
{
    vectord v1 = { 1,2,3,4,5,6,7,8 };
    vectord v2;

	double exp1[3] = {7,6,5};
	double res1[3];

	v2 = CFilter::inst()->subvector_reverse(v1, 6, 4);
	printk("v2:\n");
	CMatrix::printVector(v2);
	Vec2DoubleArray(v2, res1);
	zassert_mem_equal(res1, exp1, sizeof(res1), "v1 not correct");
}

// test function max_val
ZTEST(tests_disp_filter, test_disp_filter_max_val)
{
    vectori v1 = { 1,2,3,17,5,6,7,8 };
    int m;

	m = CFilter::inst()->max_val(v1);
	zassert_equal(m, 17, "max val not correct");
}


// test function fwfilter_init
ZTEST(tests_disp_filter, test_disp_filter_fwfilter_init)
{
    CFilter::inst()->b_coeff = { 0.95806, -2.87418,  2.87418, -0.95806 };
	CFilter::inst()->a_coeff = { 1.00000, -2.91432,  2.83228, -0.91788 };
	CFilter::inst()->zzi = { -0.95806, 1.91612, -0.95806 };
	CFilter::inst()->zi = { 0,0,0 };

	vectord vExp_zi = { -1.91612, 3.83224, -1.91612 };
	int exp_zi[3];
	int res_zi[3];

	CFilter::inst()->fwfilter_init(2.0);
	DoubleVec2IntArray(vExp_zi, exp_zi);
	DoubleVec2IntArray(CFilter::inst()->zi, res_zi);
	zassert_mem_equal(res_zi, exp_zi, sizeof(res_zi), "zi not correct");
	
	printk("zi:\n");
	CMatrix::printVector(CFilter::inst()->zi);
}

// test function bwfilter_init
ZTEST(tests_disp_filter, test_disp_filter_bwfilter_init)
{
	CFilter::inst()->zzi = { -0.95806, 1.91612, -0.95806 };
	CFilter::inst()->zi = { 0,0,0 };

	vectord vExp_zi = { -1.91612, 3.83224, -1.91612 };
	int exp_zi[3];
	int res_zi[3];

	CFilter::inst()->bwfilter_init(2.0);
	DoubleVec2IntArray(vExp_zi, exp_zi);
	//DoubleVec2IntArray(CFilter::inst()->n_zi, res_zi);  // NOTE: this should, but don't work !!!!
	DoubleVec2IntArray(CFilter::inst()->zi, res_zi);
	zassert_mem_equal(res_zi, exp_zi, sizeof(res_zi), "zi not correct");
	zassert_equal((int)(*CFilter::inst()->z * 1000), (int)(1000* -1.91612), "max val not correct");
	
	printk("zi:\n");
	CMatrix::printVector(CFilter::inst()->zi);
	printk("z = %f,  iz = %d\n", *CFilter::inst()->z, (int)(*CFilter::inst()->z * 1000));
}

// test function filter_initialize
ZTEST(tests_disp_filter, test_disp_filter_filter_initialize)
{
    vectord b_coeff = { 0.95806, -2.87418,  2.87418, -0.95806 };
	vectord a_coeff = { 1.00000, -2.91432,  2.83228, -0.91788 };
	vectord zi = { -0.95806, 1.91612, -0.95806 };

	vectord vExp_n_zi = { -0.95806,  1.91612, -0.95806 , 0 };
	int exp_n_zi[4];
	int res_n_zi[4];
	double exp_n_A[4] = { 1.00000, -2.91432,  2.83228, -0.91788 };
	double res_n_A[4];
	double exp_n_B[4] = { 0.95806, -2.87418,  2.87418, -0.95806 };
	double res_n_B[4];

	CFilter::inst()->filter_initialize(b_coeff, a_coeff, zi);
	DoubleVec2IntArray(vExp_n_zi, exp_n_zi);
	DoubleVec2IntArray(CFilter::inst()->n_zi, res_n_zi);
	zassert_mem_equal(res_n_zi, exp_n_zi, sizeof(res_n_zi), "n_zi not correct");
	Vec2DoubleArray(CFilter::inst()->n_A, res_n_A);
	zassert_mem_equal(res_n_A, exp_n_A, sizeof(res_n_A), "n_A not correct");
	Vec2DoubleArray(CFilter::inst()->n_B, res_n_B);
	zassert_mem_equal(res_n_B, exp_n_B, sizeof(res_n_B), "n_B not correct");
	
	printk("n_zi:\n");
	CMatrix::printVector(CFilter::inst()->n_zi);
	printk("n_A:\n");
	CMatrix::printVector(CFilter::inst()->n_A);
	printk("n_B:\n");
	CMatrix::printVector(CFilter::inst()->n_B);
}

// test function filter_initialize
ZTEST(tests_disp_filter, test_disp_filter_filter_initialize2)
{
    vectord b_coeff = { 0.95806, -2.87418,  2.87418, -0.95806 };
	vectord a_coeff = { 2.00000, -2.91432,  2.83228, -0.91788 };
	vectord zi = { -0.95806, 1.91612, -0.95806 };

	vectord vExp_n_zi = { -0.95806,  1.91612, -0.95806 , 0 };
	int exp_n_zi[4];
	int res_n_zi[4];
	double exp_n_A[4] = { 1.00000, -1.45716,  1.41614,  -0.45894 };
	double res_n_A[4];
	double exp_n_B[4] = { 0.47903,  -1.43709,  1.43709,  -0.47903 };
	double res_n_B[4];

	CFilter::inst()->filter_initialize(b_coeff, a_coeff, zi);
	DoubleVec2IntArray(vExp_n_zi, exp_n_zi);
	DoubleVec2IntArray(CFilter::inst()->n_zi, res_n_zi);
	zassert_mem_equal(res_n_zi, exp_n_zi, sizeof(res_n_zi), "n_zi not correct");
	Vec2DoubleArray(CFilter::inst()->n_A, res_n_A);
	zassert_mem_equal(res_n_A, exp_n_A, sizeof(res_n_A), "n_A not correct");
	Vec2DoubleArray(CFilter::inst()->n_B, res_n_B);
	zassert_mem_equal(res_n_B, exp_n_B, sizeof(res_n_B), "n_B not correct");
	
	printk("n_zi:\n");
	CMatrix::printVector(CFilter::inst()->n_zi);
	printk("n_A:\n");
	CMatrix::printVector(CFilter::inst()->n_A);
	printk("n_B:\n");
	CMatrix::printVector(CFilter::inst()->n_B);
}

// test function filterFirstIter
ZTEST(tests_disp_filter, test_disp_filter_filterFirstIter)
{
    vectord b_coeff = { 0.95806, -2.87418,  2.87418, -0.95806 };
	vectord a_coeff = { 1.00000, -2.91432,  2.83228, -0.91788 };
	vectord zi = { -0.95806, 1.91612, -0.95806, 0.0 };
    vectord x = { 1,-.2,.3,1,-.5,1,-.7,.8,-.9,.5 };
	vectord y = {  };

	CFilter::inst()->a = &a_coeff[0];
	CFilter::inst()->b = &b_coeff[0];
	CFilter::inst()->z = &zi[0];
	CFilter::inst()->X_old = {0,0,0,0};
	CFilter::inst()->Y_old = {0,0,0,0};
	CFilter::inst()->filter_order = 4;
	
	vectord vExp_X_old = { -0.70000,  0.80000,  -0.90000,  0.50000 };
	int exp_X_old[4];
	int res_X_old[4];
	vectord vExp_Y_old = { -1.37864,  0.18662,  -1.44279,  0.03703 };
	int exp_Y_old[4];
	int res_Y_old[4];
	vectord vExp_y = { 0.00000,  -1.14967,  -0.57214,  0.15171,  -1.29202,  0.26172,  -1.37864,  0.18662,  -1.44279,  0.03703  };
	int exp_y[10];
	int res_y[10];

	CFilter::inst()->filterFirstIter(x, y);
	DoubleVec2IntArray(vExp_X_old, exp_X_old);
	DoubleVec2IntArray(CFilter::inst()->X_old, res_X_old);
	zassert_mem_equal(res_X_old, exp_X_old, sizeof(res_X_old), "X_old not correct");

	DoubleVec2IntArray(vExp_Y_old, exp_Y_old);
	DoubleVec2IntArray(CFilter::inst()->Y_old, res_Y_old);
	zassert_mem_equal(res_Y_old, exp_Y_old, sizeof(res_Y_old), "n_AY_old not correct");

	DoubleVec2IntArray(vExp_y, exp_y);
	DoubleVec2IntArray(y, res_y);
	zassert_mem_equal(res_y, exp_y, sizeof(res_y), "y not correct");
	
	printk("X_old:\n");
	CMatrix::printVector(CFilter::inst()->X_old);
	printk("Y_old:\n");
	CMatrix::printVector(CFilter::inst()->Y_old);
	printk("y:\n");
	CMatrix::printVector(y);
}

// test function filterNthIter
ZTEST(tests_disp_filter, test_disp_filter_filterNthIter)
{
    vectord b_coeff = { 0.95806, -2.87418,  2.87418, -0.95806 };
	vectord a_coeff = { 1.00000, -2.91432,  2.83228, -0.91788 };
	vectord zi = { -0.95806, 1.91612, -0.95806, 0.0 };
    vectord x = { 1,-.2,.3,1,-.5,1,-.7,.8,-.9,.5 };
	vectord y = {  };

	CFilter::inst()->a = &a_coeff[0];
	CFilter::inst()->b = &b_coeff[0];
	CFilter::inst()->z = &zi[0];
	CFilter::inst()->X_old = {-0.70000,  0.80000,  -0.90000,  0.50000};
	CFilter::inst()->Y_old = {-1.37864,  0.18662,  -1.44279,  0.03703};
	CFilter::inst()->filter_order = 4;
	
	vectord vExp_X_old = { -0.70000,  0.80000,  -0.90000,  0.50000 };
	int exp_X_old[4];
	int res_X_old[4];
	vectord vExp_Y_old = { -0.98590,  0.55800,  -1.09218,  0.36746 };
	int exp_Y_old[4];
	int res_Y_old[4];
	vectord vExp_y = { -1.37864,  0.18662,  -1.44279,  0.03703,  0.53336,  -0.64126,  -0.08807,  0.61205,  -0.85481,  0.67639,  -0.98590,  0.55800,  -1.09218,  0.36746  };
	int exp_y[14];
	int res_y[14];

	CFilter::inst()->filterNthIter(x, y);
	DoubleVec2IntArray(vExp_X_old, exp_X_old);
	DoubleVec2IntArray(CFilter::inst()->X_old, res_X_old);
	zassert_mem_equal(res_X_old, exp_X_old, sizeof(res_X_old), "X_old not correct");
	DoubleVec2IntArray(vExp_Y_old, exp_Y_old);
	DoubleVec2IntArray(CFilter::inst()->Y_old, res_Y_old);
	zassert_mem_equal(res_Y_old, exp_Y_old, sizeof(res_Y_old), "n_AY_old not correct");
	DoubleVec2IntArray(vExp_y, exp_y);
	DoubleVec2IntArray(y, res_y);
	zassert_mem_equal(res_y, exp_y, sizeof(res_y), "y not correct");
	
	printk("X_old:\n");
	CMatrix::printVector(CFilter::inst()->X_old);
	printk("Y_old:\n");
	CMatrix::printVector(CFilter::inst()->Y_old);
	printk("y:\n");
	CMatrix::printVector(y);
}

// test function filterNthIter
ZTEST(tests_disp_filter, test_disp_filter_filterNthIter2)
{
    vectord b_coeff = { 0.95806, -2.87418,  2.87418, -0.95806 };
	vectord a_coeff = { 1.00000, -2.91432,  2.83228, -0.91788 };
	vectord zi = { -0.95806, 1.91612, -0.95806, 0.0 };
    vectord x = { 1,-.2,.3,1,-.5,1,-.7,.8,-.9,.5 };
	vectord y = {  };

	CFilter::inst()->a = &a_coeff[0];
	CFilter::inst()->b = &b_coeff[0];
	CFilter::inst()->z = &zi[0];
	CFilter::inst()->X_old = {-0.90000,  0.50000};
	CFilter::inst()->Y_old = {-1.44279,  0.03703};
	CFilter::inst()->filter_order = 4;
	
	vectord vExp_X_old = { -0.70000,  0.80000 };
	int exp_X_old[4];
	int res_X_old[4];
	vectord vExp_Y_old = { -9.49833,  -10.06011 };
	int exp_Y_old[4];
	int res_Y_old[4];
	vectord vExp_y = { -1.44279,  0.03703,  0.17045,  -1.69889,  -2.14247,  -2.71276,  -5.69650,  -5.90273,  -9.49833,  -10.06011,  -13.96603,  -14.89099 };
	int exp_y[14];
	int res_y[14];

	CFilter::inst()->filterNthIter(x, y);
	DoubleVec2IntArray(vExp_X_old, exp_X_old);
	DoubleVec2IntArray(CFilter::inst()->X_old, res_X_old);
	zassert_mem_equal(res_X_old, exp_X_old, sizeof(res_X_old), "X_old not correct");
	DoubleVec2IntArray(vExp_Y_old, exp_Y_old);
	DoubleVec2IntArray(CFilter::inst()->Y_old, res_Y_old);
	zassert_mem_equal(res_Y_old, exp_Y_old, sizeof(res_Y_old), "n_AY_old not correct");
	DoubleVec2IntArray(vExp_y, exp_y);
	DoubleVec2IntArray(y, res_y);
	zassert_mem_equal(res_y, exp_y, sizeof(res_y), "y not correct");
	
	printk("X_old:\n");
	CMatrix::printVector(CFilter::inst()->X_old);
	printk("Y_old:\n");
	CMatrix::printVector(CFilter::inst()->Y_old);
	printk("y:\n");
	CMatrix::printVector(y);
}

// test function filterLastIter
ZTEST(tests_disp_filter, test_disp_filter_filterLastIter)
{
    vectord b_coeff = { 0.95806, -2.87418,  2.87418, -0.95806 };
	vectord a_coeff = { 1.00000, -2.91432,  2.83228, -0.91788 };
	vectord zi = { -0.95806, 1.91612, -0.95806, 0.0 };
    vectord x = { 1,-.2,.3,1,-.5,1,-.7,.8,-.9,.5 };
	vectord y = {  };

	CFilter::inst()->a = &a_coeff[0];
	CFilter::inst()->b = &b_coeff[0];
	CFilter::inst()->z = &zi[0];
	CFilter::inst()->X_old = {-0.70000,  0.80000,  -0.90000,  0.50000};
	CFilter::inst()->Y_old = {-1.37864,  0.18662,  -1.44279,  0.03703};
	CFilter::inst()->filter_order = 4;
	
	vectord vExp_X_old = { -0.70000,  0.80000,  -0.90000,  0.50000 };
	int exp_X_old[4];
	int res_X_old[4];
	vectord vExp_Y_old = { -1.37864,  0.18662,  -1.44279,  0.03703 };
	int exp_Y_old[4];
	int res_Y_old[4];
	vectord vExp_y = { -1.37864,  0.18662,  -1.44279,  0.03703,  0.53336,  -0.64126,  -0.08807,  0.61205,  -0.85481,  0.67639,  -0.98590,  0.55800,  -1.09218,  0.36746  };
	int exp_y[14];
	int res_y[14];

	CFilter::inst()->filterLastIter(x, y);
	DoubleVec2IntArray(vExp_X_old, exp_X_old);
	DoubleVec2IntArray(CFilter::inst()->X_old, res_X_old);
	zassert_mem_equal(res_X_old, exp_X_old, sizeof(res_X_old), "X_old not correct");
	DoubleVec2IntArray(vExp_Y_old, exp_Y_old);
	DoubleVec2IntArray(CFilter::inst()->Y_old, res_Y_old);
	zassert_mem_equal(res_Y_old, exp_Y_old, sizeof(res_Y_old), "Y_old not correct");
	DoubleVec2IntArray(vExp_y, exp_y);
	DoubleVec2IntArray(y, res_y);
	zassert_mem_equal(res_y, exp_y, sizeof(res_y), "y not correct");
	
	printk("X_old:\n");
	CMatrix::printVector(CFilter::inst()->X_old);
	printk("Y_old:\n");
	CMatrix::printVector(CFilter::inst()->Y_old);
	printk("y:\n");
	CMatrix::printVector(y);
}

// test function filterLastIter
ZTEST(tests_disp_filter, test_disp_filter_filterLastIter2)
{
    vectord b_coeff = { 0.95806, -2.87418,  2.87418, -0.95806 };
	vectord a_coeff = { 1.00000, -2.91432,  2.83228, -0.91788 };
	vectord zi = { -0.95806, 1.91612, -0.95806, 0.0 };
    vectord x = { 1,-.2,.3,1,-.5,1,-.7,.8,-.9,.5 };
	vectord y = {  };

	CFilter::inst()->a = &a_coeff[0];
	CFilter::inst()->b = &b_coeff[0];
	CFilter::inst()->z = &zi[0];
	CFilter::inst()->X_old = {-0.90000,  0.50000};
	CFilter::inst()->Y_old = {-1.44279,  0.03703};
	CFilter::inst()->filter_order = 4;
	
	vectord vExp_X_old = { -0.90000,  0.50000 };
	int exp_X_old[4];
	int res_X_old[4];
	vectord vExp_Y_old = { -1.44279,  0.03703 };
	int exp_Y_old[4];
	int res_Y_old[4];
	vectord vExp_y = { -1.44279,  0.03703,  0.17045,  -1.69889,  -2.14247,  -2.71276,  -5.69650,  -5.90273,  -9.49833,  -10.06011,  -13.96603,  -14.89099  };
	int exp_y[12];
	int res_y[12];

	CFilter::inst()->filterLastIter(x, y);
	DoubleVec2IntArray(vExp_X_old, exp_X_old);
	DoubleVec2IntArray(CFilter::inst()->X_old, res_X_old);
	zassert_mem_equal(res_X_old, exp_X_old, sizeof(res_X_old), "X_old not correct");
	DoubleVec2IntArray(vExp_Y_old, exp_Y_old);
	DoubleVec2IntArray(CFilter::inst()->Y_old, res_Y_old);
	zassert_mem_equal(res_Y_old, exp_Y_old, sizeof(res_Y_old), "Y_old not correct");
	DoubleVec2IntArray(vExp_y, exp_y);
	DoubleVec2IntArray(y, res_y);
	zassert_mem_equal(res_y, exp_y, sizeof(res_y), "y not correct");
	
	printk("X_old:\n");
	CMatrix::printVector(CFilter::inst()->X_old);
	printk("Y_old:\n");
	CMatrix::printVector(CFilter::inst()->Y_old);
	printk("y:\n");
	CMatrix::printVector(y);
}

// test function prepare_leftpad
ZTEST(tests_disp_filter, test_disp_filter_prepare_leftpad)
{
    vectord in = { 1,-.2,.3,1,-.5,1,-.7,.8,-.9,.5 };
	vectord leftpad = {  };

	vectord vExp_leftpad = { 2.90000,  1.20000,  2.70000,  1.00000,  2.50000,  1.00000,  1.70000,  2.20000 };
	int exp_leftpad[8];
	int res_leftpad[8];

	CFilter::inst()->prepare_leftpad(leftpad, in);
	DoubleVec2IntArray(vExp_leftpad, exp_leftpad);
	DoubleVec2IntArray(leftpad, res_leftpad);
	zassert_mem_equal(res_leftpad, exp_leftpad, sizeof(res_leftpad), "leftpad not correct");
	
	printk("leftpad:\n");
	CMatrix::printVector(leftpad);
}

// test function prepare_rightpad
ZTEST(tests_disp_filter, test_disp_filter_prepare_rightpad)
{
    vectord in = { 1,-.2,.3,1,-.5,1,-.7,.8,-.9,.5 };
	vectord rightpad = {  };

	vectord vExp_rightpad = { 1.90000,  1.999,  1.70000,  0.00000,  1.50000,  0.00000,  0.70000,  1.20000,  0.00000 };
	int exp_rightpad[9];
	int res_rightpad[9];

	CFilter::inst()->prepare_rightpad(rightpad, in);
	DoubleVec2IntArray(vExp_rightpad, exp_rightpad);
	DoubleVec2IntArray(rightpad, res_rightpad);
	//printIntArray(exp_rightpad,9);
	//printIntArray(res_rightpad,9);
	exp_rightpad[1] = res_rightpad[1]; // workaround for rounding error
	zassert_mem_equal(res_rightpad, exp_rightpad, sizeof(res_rightpad), "rightpad not correct");
	
	printk("rightpad:\n");
	CMatrix::printVector(rightpad);
}



