#include "test_math.h"

#include <gtest/gtest.h>

/**
 * @brief Run all unit tests
 * 
 * @return test results
 */
int main(int argc, char ** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}