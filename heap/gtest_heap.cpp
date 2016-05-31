#include "gtest/gtest.h"

#include <vector>
#include <string>
#include <algorithm>
#include "heap.h"

using namespace std;

template<class T>
bool checkMinHeapProperty(const MinHeap<T>& heap, int pos) {
	int child = heap.getD() * pos + 1, child0 = child;
	if ( child > heap.size() - 1 ) return true;

	while ( child <  child0 + heap.getD() && child < heap.size() ) {
		if ( heap.prioAt(pos) > heap.prioAt(child) ) return false;
		else {
			if ( !checkMinHeapProperty(heap, child) ) return false;
		}
		child++;
	}
	return true;
}

class D_aryHeapTest : public testing::Test
{
public:
	D_aryHeapTest() {};
	~D_aryHeapTest() {};

	void SetUp() {
		fruit_array = new string[6] {"apple", "orange", "nectarine", "banana", "carrot", "onion"};
		priority1 = new int[6] {1, 2, 3, 4, 5, 6};

		priority2 = new int[6] {3, 2, 6, 5, 1, 4};
		priority_index2 = new int[6] {4, 1, 0, 5, 3, 2};

		priority3 = new int[6] {6, 1, 2, 5, 3, 4};
		priority_index3 = new int[6] {1, 2, 4, 5, 3, 0};
	}

	void TearDown() {
		delete[] fruit_array;
		delete[] priority1;
		delete[] priority2;
		delete[] priority_index2;
		delete[] priority3;
		delete[] priority_index3;
	}
	
	string * fruit_array;
	int fruit_size = 6;

	int * priority1;
	int * priority2;
	int * priority_index2;
	int * priority3;
	int * priority_index3;
	
};

// TEST_F(D_aryHeapTest, EmptyTest) {
// 	MinHeap<int> mh(3);
// 	EXPECT_TRUE(mh.isEmpty());
// }

// TEST_F(D_aryHeapTest, TestD3) {
// 	MinHeap<int> mh(3);
// 	EXPECT_TRUE(mh.isEmpty());

// 	for (int i = 1; i < 16; ++i)
// 	{
// 		mh.add(i, i);
// 	}

// 	for (int i = 1; i < 16; ++i)
// 	{
// 		EXPECT_EQ(mh.peek(), i);
// 		mh.remove();
// 	}
// }

// TEST_F(D_aryHeapTest, TestD3Reverse) {
// 	MinHeap<int> mh(3);
// 	EXPECT_TRUE(mh.isEmpty());

// 	for (int i = 1; i < 16; ++i)
// 	{
// 		mh.add(i, 16 - i);
// 	}

// 	for (int i = 1; i < 16; ++i)
// 	{
// 		EXPECT_EQ(mh.peek(), 16 - i);
// 		mh.remove();
// 	}
// }

// TEST_F(D_aryHeapTest, TestD3ReverseBig) {
// 	MinHeap<int> mh(3);
// 	EXPECT_TRUE(mh.isEmpty());

// 	for (int i = 1; i < 10000; ++i)
// 	{
// 		mh.add(i, 10000 - i);
// 	}

// 	for (int i = 1; i < 10000; ++i)
// 	{
// 		EXPECT_EQ(mh.peek(), 10000 - i);
// 		mh.remove();
// 	}
// }

// TEST_F(D_aryHeapTest, TestD4) {
// 	MinHeap<int> mh(4);
// 	EXPECT_TRUE(mh.isEmpty());

// 	for (int i = 1; i < 16; ++i)
// 	{
// 		mh.add(i, i);
// 	}

// 	for (int i = 1; i < 16; ++i)
// 	{
// 		EXPECT_EQ(mh.peek(), i);
// 		mh.remove();
// 	}
// }

// TEST_F(D_aryHeapTest, TestD4Reverse) {
// 	MinHeap<int> mh(4);
// 	EXPECT_TRUE(mh.isEmpty());

// 	for (int i = 1; i < 16; ++i)
// 	{
// 		mh.add(i, 16 - i);
// 	}

// 	for (int i = 1; i < 16; ++i)
// 	{
// 		EXPECT_EQ(mh.peek(), 16 - i);
// 		mh.remove();
// 	}
// }

// TEST_F(D_aryHeapTest, TestD2StringPriority1) {
// 	MinHeap<string> mh_str(2);

// 	for (int i = 0; i < fruit_size; ++i)
// 	{
// 		mh_str.add(fruit_array[i], priority1[i]);
// 	}

// 	for (int i = 0; i < fruit_size; ++i)
// 	{	
// 		EXPECT_EQ(mh_str.peek(), fruit_array[i]);
// 		mh_str.remove();
// 	}
// }


// TEST_F(D_aryHeapTest, TestD2StringPriority2) {
// 	MinHeap<string> mh_str(2);

// 	for (int i = 0; i < fruit_size; ++i)
// 	{
// 		mh_str.add(fruit_array[i], priority2[i]);
// 	}

// 	for (int i = 0; i < fruit_size; ++i)
// 	{	
// 		EXPECT_EQ(mh_str.peek(), fruit_array[ priority_index2[i] ]);
// 		mh_str.remove();
// 	}
// }

// TEST_F(D_aryHeapTest, TestD3StringPriority2) {
// 	MinHeap<string> mh_str(3);

// 	for (int i = 0; i < fruit_size; ++i)
// 	{
// 		mh_str.add(fruit_array[i], priority2[i]);
// 	}

// 	for (int i = 0; i < fruit_size; ++i)
// 	{	
// 		EXPECT_EQ(mh_str.peek(), fruit_array[ priority_index2[i] ]);
// 		mh_str.remove();
// 	}
// }

// TEST_F(D_aryHeapTest, TestD3StringPriority3) {
// 	MinHeap<string> mh_str(3);

// 	for (int i = 0; i < fruit_size; ++i)
// 	{
// 		mh_str.add(fruit_array[i], priority3[i]);
// 	}

// 	for (int i = 0; i < fruit_size; ++i)
// 	{	
// 		EXPECT_EQ(mh_str.peek(), fruit_array[ priority_index3[i] ]);
// 		mh_str.remove();
// 	}
// }

TEST_F(D_aryHeapTest, TestCheckHeapPropertyAdd300RemoveUntilEmpty) {
	// std::vector<std::pair<int, int> > elements;
	MinHeap<int> mh(2);

	for (int i = 0; i < 800; ++i)
	{
		int random_item = rand();
		int random_priority = rand();
		// elements.push_back(std::pair<int, int>(random_item, random_priority));
		mh.add(random_item, random_priority);

		if ( i % 20 == 0 ) {
			EXPECT_TRUE(checkMinHeapProperty(mh ,0));
		}
	}

	while ( !mh.isEmpty() ) {
		mh.remove();
		if ( mh.size() % 10 == 0 ) {
			int isValidHeap = true;
			EXPECT_TRUE((isValidHeap = checkMinHeapProperty(mh, 0)));
			if ( !isValidHeap ) cout << mh.size() << endl;
		}
	}

}
