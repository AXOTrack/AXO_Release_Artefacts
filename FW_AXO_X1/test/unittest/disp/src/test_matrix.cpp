#include <zephyr/ztest.h>
#include <vector>
//#include <algorithm>

#define private public
#include "disp/matrix.h"

#include "stubs.h"

ZTEST_SUITE(tests_disp_matrix, NULL, NULL, NULL, NULL, NULL);

// test constructor and destructor
ZTEST(tests_disp_matrix, test_disp_matrix_constructor)
{
	CMatrix* pMatrix = new CMatrix();
	zassert_not_null(pMatrix, "memory alloc failed");

	delete pMatrix;
}

// test function printVector
ZTEST(tests_disp_matrix, test_disp_matrix_printVector)
{
	std::vector<double> v = {1.0, 1.2, 3.5};
	CMatrix::printVector(v);
	std::vector<int> vi = {1, 2, 3};
	CMatrix::printVector(vi);
}

// test function printMatrix
ZTEST(tests_disp_matrix, test_disp_matrix_printMatrix)
{
	MatrixT m = {{1.0, 1.2, 3.5},
	             {1.0, 1.2, 3.5},
				 {1.0, 1.2, 3.5}};
	CMatrix::printMatrix(m);
}

// test function substract
ZTEST(tests_disp_matrix, test_disp_matrix_substract)
{
	std::vector<double> v1 = {1.0, 1.2, 3.5};
	std::vector<double> v2 = {1.0, 0.2, 1.4};
	std::vector<double> v3;
	v3 = CMatrix::substract(v1, v2);
	printk("v3 = %f, %f, %f\n", v3[0], v3[1], v3[2]);
	zassert_equal(v3[0], 0.0, "v3[0] incorrect");
	zassert_equal(v3[1], 1.0, "v3[1] incorrect");
	zassert_equal(v3[2], 2.1, "v3[2] incorrect");
}

// test function mult
ZTEST(tests_disp_matrix, test_disp_matrix_mult)
{
	std::vector<double> v = {1.0, 10.0, 100.0};
	std::vector<double> v2;
	MatrixT m = {{1.0, 2.0, 3.0},
	             {2.0, 4.0, 6.0},
				 {3.0, 6.0, 9.0}};
	v2 = CMatrix::mult(m, v);
	zassert_equal(v2[0], 321.0, "v2[0] incorrect");
	zassert_equal(v2[1], 642.0, "v2[1] incorrect");
	zassert_equal(v2[2], 963.0, "v2[2] incorrect");
	CMatrix::printVector(v2);
} 

// test function setVal
ZTEST(tests_disp_matrix, test_disp_matrix_setVal)
{
	MatrixT m = {{1.0, 1.2, 3.5},
	             {1.0, 1.2, 3.5},
				 {1.0, 1.2, 3.5}};
	zassert_equal(m[0][1], 1.2, "m[0][1] incorrect before");
	CMatrix::setVal(m, 0, 1, 7.7);
	zassert_equal(m[0][1], 7.7, "m[0][1] incorrect after");
} 

// test function initVector
ZTEST(tests_disp_matrix, test_disp_matrix_initVector)
{
	std::vector<double> v(3);
	CMatrix::initVector(v, 3.1);
	zassert_equal(v[0], 3.1, "v[0] incorrect");
	zassert_equal(v[1], 3.1, "v[1] incorrect");
	zassert_equal(v[2], 3.1, "v[2] incorrect");
}

// test function initMatrix
ZTEST(tests_disp_matrix, test_disp_matrix_initMatrix)
{
	MatrixT m(3, std::vector<double> (3));
	CMatrix::initMatrix(m, 7.0);
	zassert_equal(m[0][0], 7.0, "m[0][0] incorrect");
	zassert_equal(m[1][0], 7.0, "m[1][0] incorrect");
	zassert_equal(m[0][1], 7.0, "m[0][1] incorrect");
	zassert_equal(m[2][2], 7.0, "m[2][2] incorrect");
	CMatrix::printMatrix(m);
}

// test function getCofactor
ZTEST(tests_disp_matrix, test_disp_matrix_getCofactor)
{
	MatrixT m2;
	MatrixT m = {{6, 8, 3},
	             {4, 7, 3},
				 {1, 2, 1}};
	m2 = CMatrix::getCofactor(m);
	CMatrix::printMatrix(m2);
	zassert_equal(m2[0][0], 1.0, "m[0][0] incorrect");
	zassert_equal(m2[0][1], -1.0, "m[1][0] incorrect");
	zassert_equal(m2[0][2], 1.0, "m[2][0] incorrect");
	zassert_equal(m2[1][0], -2.0, "m[0][1] incorrect");
	zassert_equal(m2[1][1], 3.0, "m[1][1] incorrect");
	zassert_equal(m2[1][2], -4.0, "m[2][1] incorrect");
	zassert_equal(m2[2][0], 3.0, "m[0][2] incorrect");
	zassert_equal(m2[2][1], -6.0, "m[1][2] incorrect");
	zassert_equal(m2[2][2], 10.0, "m[2][2] incorrect");
}

// test function getTranspose
ZTEST(tests_disp_matrix, test_disp_matrix_getTranspose)
{
	MatrixT m2;
	MatrixT m = {{1.0, 1.2, 3.5},
	             {1.0, 1.2, 3.5},
				 {1.0, 1.2, 3.5}};
	m2 = CMatrix::getTranspose(m);
	CMatrix::printMatrix(m2);
	zassert_equal(m2[0][0], 1.0, "m[0][0] incorrect");
	zassert_equal(m2[0][1], 1.0, "m[1][0] incorrect");
	zassert_equal(m2[0][2], 1.0, "m[2][0] incorrect");
	zassert_equal(m2[1][0], 1.2, "m[0][1] incorrect");
	zassert_equal(m2[1][1], 1.2, "m[1][1] incorrect");
	zassert_equal(m2[1][2], 1.2, "m[2][1] incorrect");
	zassert_equal(m2[2][0], 3.5, "m[0][2] incorrect");
	zassert_equal(m2[2][1], 3.5, "m[1][2] incorrect");
	zassert_equal(m2[2][2], 3.5, "m[2][2] incorrect");
}

// test function getInverse
ZTEST(tests_disp_matrix, test_disp_matrix_getInverse)
{
	MatrixT m2;
	MatrixT m = {{6, 8, 3},
	             {4, 7, 3},
				 {1, 2, 1}};
	m2 = CMatrix::getInverse(m);
	CMatrix::printMatrix(m2);
	zassert_equal(m2[0][0], 1.0, "m[0][0] incorrect");
	zassert_equal(m2[0][1], -2.0, "m[1][0] incorrect");
	zassert_equal(m2[0][2], 3.0, "m[2][0] incorrect");
	zassert_equal(m2[1][0], -1.0, "m[0][1] incorrect");
	zassert_equal(m2[1][1], 3.0, "m[1][1] incorrect");
	zassert_equal(m2[1][2], -6.0, "m[2][1] incorrect");
	zassert_equal(m2[2][0], 1.0, "m[0][2] incorrect");
	zassert_equal(m2[2][1], -4.0, "m[1][2] incorrect");
	zassert_equal(m2[2][2], 10.0, "m[2][2] incorrect");
}

// test function getDeterminant
ZTEST(tests_disp_matrix, test_disp_matrix_getDeterminant)
{
	double d;
	MatrixT m1 = {{6, 8, 3},
	             {4, 7, 3},
				 {1, 2, 1}};
	d = CMatrix::getDeterminant(m1);
	printk("%f\n", d);
	zassert_equal(d, 1.0, "det incorrect");

	MatrixT m2 = {{6, 8},
	              {4, 7}};
	d = CMatrix::getDeterminant(m2);
	printk("%f\n", d);
	zassert_equal(d, 10.0, "det incorrect");

	MatrixT m3 = {{6}};
	d = CMatrix::getDeterminant(m3);
	printk("%f\n", d);
	zassert_equal(d, 6.0, "det incorrect");

	MatrixT m4;
	d = CMatrix::getDeterminant(m4);
	printk("%f\n", d);
	zassert_equal(d, 1.0, "det incorrect");
}




/*
	static std::vector<std::vector<double>> getCofactor(const std::vector<std::vector<double>> vect);
	static std::vector<std::vector<double>> getTranspose(const std::vector<std::vector<double>> matrix1);
	static double getDeterminant(const std::vector<std::vector<double>> vect);
	static std::vector<std::vector<double>> getInverse(const std::vector<std::vector<double>> vect);
*/