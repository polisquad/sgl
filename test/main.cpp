#include "public/test_containers.h"
#include "public/test_math.h"

/// @brief Global allocator
Malloc * gMalloc = nullptr;

/**
 * @brief Run all unit tests
 * 
 * @return test results
 */
int main(int argc, char ** argv)
{
	// Context initialization
	Memory::createGMalloc();

	// Containers initialization
	initContainers();

	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}