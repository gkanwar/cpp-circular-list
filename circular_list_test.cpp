/// circular_list_test.cpp - This file is part of the cpp-circular-list project.
///
/// Author: Gurtej Kanwar (https://github.com/gkanwar)
///
/// To the extent possible under law, the author(s) have dedicated all copyright
/// and related and neighboring rights to this software to the public domain
/// worldwide. This software is distributed without any warranty.  You should
/// have received a copy of the CC0 Public Domain Dedication along with this
/// software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

#include "gtest/gtest.h"
#include "circular_list.h"

using namespace ext_std;
using namespace std;

struct Dummy {
  int val;
};
bool operator==(const Dummy& l, const Dummy& r) {
  return l.val == r.val;
}
bool operator!=(const Dummy& l, const Dummy& r) {
  return !(l == r);
}

TEST(CircularListTest, EmptyListHasZeroSize) {
  circular_list<Dummy> list;
  EXPECT_EQ(list.size(), 0);
  EXPECT_EQ(list.begin(), list.end());
  EXPECT_EQ(list.cbegin(), list.cend());
}

TEST(CircularListTest, ListFromVector) {
  vector<Dummy> vec = { { 0 }, { 1 }, { 2 } };
  circular_list<Dummy> list(vec);
  EXPECT_EQ(list.size(), 3);
  EXPECT_EQ(list.begin()->val, 0);
  EXPECT_EQ(std::next(list.begin(), 1)->val, 1);
  EXPECT_EQ(std::next(list.begin(), 2)->val, 2);
  EXPECT_EQ(std::next(list.begin(), 3)->val, 0);
  EXPECT_EQ(std::next(list.begin(), 4)->val, 1);
  EXPECT_EQ(std::next(list.begin(), 5)->val, 2);
  EXPECT_EQ(std::prev(list.begin(), 1)->val, 2);
  EXPECT_EQ(std::prev(list.begin(), 2)->val, 1);
  EXPECT_EQ(std::prev(list.begin(), 3)->val, 0);
  EXPECT_NE(list.begin(), list.end());
  int tot = 0;
  for (auto d : list) {
    tot += d.val;
  }
  EXPECT_EQ(tot, 3);
}

TEST(CircularListTest, NonEmptyListHasDistinctBeginAndEnd) {
  vector<Dummy> vec = { { 0 }, { 1 }, { 2 } };
  circular_list<Dummy> list(vec);
  EXPECT_EQ(list.size(), 3);
  EXPECT_NE(list.begin(), list.end());
  EXPECT_NE(list.cbegin(), list.cend());
}

TEST(CircularListTest, EraseCorrectlyRemovesNonHead) {
  vector<Dummy> vec = { { 0 }, { 1 }, { 2 } };
  circular_list<Dummy> list(vec);
  EXPECT_EQ(list.size(), 3);
  list.erase(std::next(list.begin(), 1), std::next(list.begin(), 2));
  EXPECT_EQ(list.size(), 2);
  EXPECT_EQ(list.begin()->val, 0);
  EXPECT_EQ(std::next(list.begin())->val, 2);
}

TEST(CircularListTest, EraseCorrectlyRemovesHead) {
  vector<Dummy> vec = { { 0 }, { 1 }, { 2 } };
  circular_list<Dummy> list(vec);
  EXPECT_EQ(list.size(), 3);
  list.erase(list.begin(), std::next(list.begin(), 1));
  EXPECT_EQ(list.size(), 2);
  EXPECT_EQ(list.begin()->val, 1);
  EXPECT_EQ(std::next(list.begin())->val, 2);
}

TEST(CircularListTest, EraseCorrectlyRemovesThroughHead) {
  vector<Dummy> vec = { { 0 }, { 1 }, { 2 } };
  circular_list<Dummy> list(vec);
  EXPECT_EQ(list.size(), 3);
  list.erase(std::prev(list.end()), std::next(list.end(), 1));
  EXPECT_EQ(list.size(), 1);
  EXPECT_EQ(list.begin()->val, 1);
}

TEST(CircularListTest, EraseCanRemoveWholeList) {
  vector<Dummy> vec = { { 0 }, { 1 }, { 2 } };
  circular_list<Dummy> list(vec);
  EXPECT_EQ(list.size(), 3);
  list.erase(list.begin(), list.end());
  EXPECT_EQ(list.size(), 0);
}

TEST(CircularListTest, InsertAtEndSucceeds) {
  vector<Dummy> vec1 = { { 0 }, { 1 }, { 2 } };
  vector<Dummy> vec2 = { { 3 }, { 4 }, { 5 } };
  circular_list<Dummy> list(vec1);
  EXPECT_EQ(list.size(), 3);
  list.insert(list.end(), vec2);
  EXPECT_EQ(list.size(), 6);
  EXPECT_EQ(list, circular_list<Dummy>({ { 0 }, { 1 }, { 2 }, { 3 }, { 4 }, { 5 } }));
}

TEST(CircularListTest, InsertInMiddleSucceeds) {
  vector<Dummy> vec = { { 0 }, { 1 }, { 2 } };
  circular_list<Dummy> list(vec);
  EXPECT_EQ(list.size(), 3);
  list.insert(list.begin()+1, vec);
  EXPECT_EQ(list.size(), 6);
  EXPECT_EQ(list, circular_list<Dummy>({ { 0 }, { 0 }, { 1 }, { 2 }, { 1 }, { 2 } }));
}

TEST(CircularListTest, InsertAtStartSucceeds) {
  vector<Dummy> vec1 = { { 0 }, { 1 }, { 2 } };
  vector<Dummy> vec2 = { { 3 }, { 4 }, { 5 } };
  circular_list<Dummy> list(vec1);
  EXPECT_EQ(list.size(), 3);
  list.insert(list.begin(), vec2);
  EXPECT_EQ(list.size(), 6);
  EXPECT_EQ(list, circular_list<Dummy>({ { 3 }, { 4 }, { 5 }, { 0 }, { 1 }, { 2 } }));
}

TEST(CircularListTest, InsertAndEraseSequenceSucceeds) {
  vector<Dummy> vec1 = { { 0 }, { 1 }, { 2 } };
  vector<Dummy> vec2 = { { 3 }, { 4 }, { 5 } };
  circular_list<Dummy> list(vec1);
  EXPECT_EQ(list.size(), 3);
  list.erase(list.begin()+1, list.begin()+2);
  EXPECT_EQ(list.size(), 2);
  EXPECT_EQ(list, circular_list<Dummy>({ { 0 }, { 2 } }));
  list.insert(list.begin(), vec2);
  EXPECT_EQ(list.size(), 5);
  EXPECT_EQ(list, circular_list<Dummy>({ { 3 }, { 4 }, { 5 }, { 0 }, { 2 } }));
  list.insert(list.begin()+2, vec1);
  EXPECT_EQ(list.size(), 8);
  EXPECT_EQ(list, circular_list<Dummy>({ { 3 }, { 4 }, { 0 }, { 1 }, { 2 }, { 5 }, { 0 }, { 2 } }));
  list.erase(list.end()-1, list.end()+4);
  EXPECT_EQ(list.size(), 3);
  EXPECT_EQ(list, circular_list<Dummy>({ { 2 }, { 5 }, { 0 } }));
  list.insert(list.begin(), vec2);
  list.insert(list.end(), vec1);
  EXPECT_EQ(list.size(), 9);
  EXPECT_EQ(list, circular_list<Dummy>({ { 3 }, { 4 }, { 5 }, { 2 }, { 5 }, { 0 }, { 0 }, { 1 }, { 2 } }));
  list.erase(list.begin()-2, list.begin()+7);
  EXPECT_EQ(list.size(), 0);
}
