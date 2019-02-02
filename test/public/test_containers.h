#include <gtest/gtest.h>

#include "hal/platform_memory.h"
#include "containers/array.h"
#include "containers/linked_list.h"
#include "containers/queue.h"
#include "containers/string.h"
#include "containers/map.h"
#include "containers/containers.h"

/**
 * @note All tests are run using the default allocator.
 * To change the default allocator, change the global
 * variable @ref gMalloc
 */

/////////////////////////////////////////////////
// Global containers
/////////////////////////////////////////////////

/// @{
static Array<uint64> * array;
static Queue<uint64> * queue;
/// @}

/////////////////////////////////////////////////
// Array tests
/////////////////////////////////////////////////

TEST(Containers, arr_test)
{
	// Create array
	Array<uint64> array;

	// Push
	for (uint64 i = 0; i < 1024 * 256; ++i) array.push(i);
	EXPECT_EQ(100, array[100]);
	EXPECT_EQ(1000, array[1000]);
	EXPECT_EQ(10000, array[10000]);

	// Some removes
	for (uint64 i = 0; i < 1024 * 128; ++i) array.removeAt(array.getCount() - 1);
	EXPECT_EQ(1024 * 128, array.getCount());
	EXPECT_EQ(100, array[100]);
	EXPECT_EQ(1000, array[1000]);
	EXPECT_EQ(10000, array[10000]);

	// Insert at 0
	/* for (uint64 i = 0; i < 1024; ++i) array.insert(i);
	for (uint64 i = 1023; i <= 0; --i) EXPECT_EQ(1023 - i, array.operator[](i));

	// Append self
	array.append(*array);
	for (uint64 i = 0; i < 256; ++i) EXPECT_EQ(array.operator[](i), array.operator[](i + 1024));
	EXPECT_EQ(2048, array.getCount());

	// Remove filter
	array.filter([](uint64 elem) -> bool { return elem & 0x1; });
	for (const auto elem : *array) EXPECT_TRUE(elem & 0x1); */
}

/////////////////////////////////////////////////
// String test
/////////////////////////////////////////////////

TEST(Containers, str_construct)		{ String str("sneppy"); EXPECT_TRUE(strncmp(*str, "sneppy", 6) == 0); }
TEST(Containers, str_append_cstr)	{ String str("sneppy"); str += "rulez"; EXPECT_TRUE(strncmp(*str, "sneppyrulez", 11) == 0); }
TEST(Containers, str_append_str)	{ String str("sneppy"); str += String("rulez"); EXPECT_TRUE(strncmp(*str, "sneppyrulez", 11) == 0); }
TEST(Containers, str_comparison)
{

	// Create a few strings
	String a("sneppy"), b("Gu"), c("lpraat"), d("sNePPY");

	// Test comparison functions
	EXPECT_EQ(a.compare(b), 's' - 'G');
	EXPECT_EQ(a.compare(c), 's' - 'l');
	EXPECT_EQ(a.compare(d), 'n' - 'N');
	
	EXPECT_EQ(a.comparei(b), 's' - 'g');
	EXPECT_EQ(a.comparei(d), 0);

	// Test operators
	EXPECT_TRUE(a == a);
	EXPECT_TRUE(a != d);
	EXPECT_TRUE(a > b);
	EXPECT_TRUE(a >= b);
	EXPECT_TRUE(b < c);
	EXPECT_TRUE(b <= c);
}

/////////////////////////////////////////////////
// LinkedList and queue test
/////////////////////////////////////////////////

TEST(Containers, ll_test)
{
	LinkedList<uint64> ll;

	// Push
	for (uint64 i = 0; i < 1024; ++i) ll.push(i);
	
	{
		auto it = ll.begin();
		for (uint64 i = 0; i < 1024; ++i, ++it) EXPECT_EQ(1023 - i, *it);
		for (uint64 i = 0; i < 1024; ++i, ++it) EXPECT_EQ(i, *it);
	}

	// Insert
	for (uint64 i = 0; i < 1024; ++i) ll.pushFront(i);

	{
		auto it = ll.begin();
		for (uint64 i = 0; i < 1024; ++i, ++it) EXPECT_EQ(1023 - i, *it);
	}
}

//////////////////////////////////////////////////
// Map test
//////////////////////////////////////////////////