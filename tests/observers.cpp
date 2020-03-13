// SPDX-License-Identifier: CC0-1.0
#include "catch.hpp"
#include <experimental/expected.hpp>

using std::experimental::expected;
using std::experimental::unexpect;
using std::experimental::bad_expected_access;

struct move_detector {
  move_detector() = default;
  move_detector(move_detector &&rhs) { rhs.been_moved = true; }
  bool been_moved = false;
};

TEST_CASE("Observers", "[observers]") {
  expected<int, int> o1 = 42;
  expected<int, int> o2{unexpect, 0};
  const expected<int, int> o3 = 42;

  CHECK(*o1 == 42);
  CHECK(*o1 == o1.value());
  CHECK(o2.value_or(42) == 42);
  CHECK(o2.error() == 0);
  CHECK(o3.value() == 42);
  CHECK_THROWS_AS(o2.value(), bad_expected_access<int>);
  CHECK(std::is_same_v<decltype(o1.value()), int &>);
  CHECK(std::is_same_v<decltype(o3.value()), const int &>);
  CHECK(std::is_same_v<decltype(std::move(o1).value()), int &&>);
  CHECK(std::is_same_v<decltype(std::move(o3).value()), const int &&>);

  expected<move_detector, int> o4{std::in_place};
  move_detector o5 = std::move(o4).value();
  CHECK(o4->been_moved);
  CHECK_FALSE(o5.been_moved);
}
