#include <gtest/gtest.h>

#include "hal/platform_memory.h"
#include "containers/array.h"
#include "containers/linked_list.h"
#include "containers/queue.h"
#include "containers/string.h"
#include "containers/tree_map.h"
#include "containers/containers.h"

/**
 * @note All tests are run using the default allocator.
 * To change the default allocator, change the global
 * variable @ref gMalloc
 */

/////////////////////////////////////////////////
// Global containers                           //
/////////////////////////////////////////////////

/// @{
static Array<uint64> * array;
static Queue<uint64> * queue;
/// @}

/// @brief Initialize containers
void initContainers()
{
	array = new Array<uint64>;
	queue = new Queue<uint64>;	
}

/////////////////////////////////////////////////
// Array tests                                 //
/////////////////////////////////////////////////

TEST(Containers, arr_push)			{ EXPECT_EQ(0, array->push(1)); }
TEST(Containers, arr_random_access)	{ EXPECT_EQ(1, array->operator[](0)); }
TEST(Containers, arr_insert)		{ array->insert(0, 0); EXPECT_EQ(0, array->operator[](0)); }
TEST(Containers, arr_oob_access)	{ array->operator()(7) = 7; EXPECT_EQ(7, array->operator[](7)); }
TEST(Containers, arr_get_count)		{ EXPECT_EQ(8, array->getCount()); }
TEST(Containers, arr_reset)			{ array->reset(); EXPECT_EQ(0, array->getCount()); }
TEST(Containers, arr_get_size)		{ array->reset(); EXPECT_EQ(2, array->getSize()); }
TEST(Containers, arr_append_arr)	{ Array<uint64> a; a.push(2); array->append(a); EXPECT_EQ(2, array->operator[](0)); }
TEST(Containers, arr_remove_at)		{ array->push(3); array->removeAt(0); EXPECT_EQ(3, array->operator[](0)); }
TEST(Containers, arr_it)			{ array->reset(); array->push(0); array->push(0); for (uint64 elem : *array) EXPECT_EQ(0, elem); }
TEST(Containers, arr_stress_test)
{
	// Reset array
	array->reset();

	// Push
	for (uint64 i = 0; i < 1024 * 256; ++i) array->push(i);
	EXPECT_EQ(100, array->operator[](100));
	EXPECT_EQ(1000, array->operator[](1000));
	EXPECT_EQ(10000, array->operator[](10000));

	// Some removes
	for (uint64 i = 0; i < 1024 * 128; ++i) array->removeAt(array->getCount() - 1);
	EXPECT_EQ(1024 * 128, array->getCount());
	EXPECT_EQ(100, array->operator[](100));
	EXPECT_EQ(1000, array->operator[](1000));
	EXPECT_EQ(10000, array->operator[](10000));	

	// Reset
	array->reset();

	// Insert at 0
	for (uint64 i = 0; i < 1024; ++i) array->insert(i);
	for (uint64 i = 1023; i <= 0; --i) EXPECT_EQ(1023 - i, array->operator[](i));

	// Append self
	array->append(*array);
	for (uint64 i = 0; i < 256; ++i) EXPECT_EQ(array->operator[](i), array->operator[](i + 1024));
	EXPECT_EQ(2048, array->getCount());

	// Remove filter
	array->filter([](uint64 elem) -> bool { return elem & 0x1; });
	for (const auto elem : *array) EXPECT_TRUE(elem & 0x1);
}

/////////////////////////////////////////////////
// String test                                 //
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
// LinkedList and queue test                   //
/////////////////////////////////////////////////

TEST(Containers, ll_insert)	{ LinkedList<uint64> ll; ll.insert(13), ll.insert(11); EXPECT_EQ(11, ll[0]); }
TEST(Containers, ll_push)	{ LinkedList<uint64> ll; ll.push(13), ll.push(11); EXPECT_EQ(13, ll[0]); }
TEST(Containers, ll_remove)	{ LinkedList<uint64> ll; ll.push(13), ll.push(11), ll.remove(); EXPECT_EQ(11, ll[0]); }
TEST(Containers, ll_pop)	{ LinkedList<uint64> ll; ll.push(13), ll.push(11), ll.pop(); EXPECT_EQ(13, ll[0]); }
TEST(Containers, ll_stress_test)
{
	LinkedList<uint64> ll;

	// Insert
	for (uint64 i = 0; i < 1024; ++i) ll.insert(i);
	{
		auto it = ll.begin();
		for (uint64 i = 0; i < 1024; ++i, ++it) EXPECT_EQ(1023 - i, *it);
	}

	// Push
	for (uint64 i = 0; i < 1024; ++i) ll.push(i);
	{
		auto it = ll.begin();
		for (uint64 i = 0; i < 1024; ++i, ++it) EXPECT_EQ(1023 - i, *it);
		for (uint64 i = 0; i < 1024; ++i, ++it) EXPECT_EQ(i, *it);
	}
}

//////////////////////////////////////////////////
// TreeMap test                                 //
//////////////////////////////////////////////////

TEST(Containers, tmap_push)	{ TreeMap<uint64, String> map; map[8] = "0100", map[9] = "0101", map[10] = "0110", map[11] = "0111"; EXPECT_EQ(String("0101"), map[9]); }
TEST(Containers, tmap_find)	{ TreeMap<uint64, String> map; map[8] = "0100", map[9] = "0101", map[10] = "0110", map[11] = "0111"; auto it = map.find(7); EXPECT_TRUE(it == map.end()); it = map.find(8); EXPECT_TRUE(it != map.end()); }