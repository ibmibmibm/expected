// SPDX-License-Identifier: CC0-1.0
#include "catch.hpp"
#include <array>
#include <experimental/expected.hpp>
using std::experimental::expected;
using std::experimental::unexpect;
using std::experimental::unexpected;

TEST_CASE("Constexpr", "[constexpr]") {
  STATIC_REQUIRE([]() { return *expected<int, int>(5); }() == 5);
  STATIC_REQUIRE([]() { return unexpected<int>(3).value(); }() == 3);
  STATIC_REQUIRE(
      []() { return expected<int, int>(unexpect, 5).value_or(4); }() == 4);
}
