// SPDX-License-Identifier: CC0-1.0
#include "catch.hpp"
#include <experimental/expected.hpp>
#include <string>
#include <type_traits>
#include <vector>

using std::experimental::expected;
using std::experimental::unexpect;
using std::experimental::unexpected;

struct takes_init_and_variadic {
  std::vector<int> v;
  std::tuple<int, int> t;
  template <class... Args>
  takes_init_and_variadic(std::initializer_list<int> l, Args &&... args)
      : v(l), t(std::forward<Args>(args)...) {}
};

TEST_CASE("Constructors", "[constructors]") {
  {
    expected<int, int> e;
    CHECK(e);
    CHECK(e == 0);
  }

  {
    expected<int, int> e = unexpected(0);
    CHECK_FALSE(e);
    CHECK(e.error() == 0);
  }

  {
    expected<int, int> e(unexpect, 0);
    CHECK_FALSE(e);
    CHECK(e.error() == 0);
  }

  {
    expected<int, int> e(std::in_place, 42);
    CHECK(e);
    CHECK(e == 42);
  }

  {
    expected<std::vector<int>, int> e(std::in_place, {0, 1});
    CHECK(e);
    CHECK((*e)[0] == 0);
    CHECK((*e)[1] == 1);
  }

  {
    expected<std::tuple<int, int>, int> e(std::in_place, 0, 1);
    CHECK(e);
    CHECK(std::get<0>(*e) == 0);
    CHECK(std::get<1>(*e) == 1);
  }

  {
    expected<takes_init_and_variadic, int> e(std::in_place, {0, 1}, 2, 3);
    CHECK(e);
    CHECK(e->v[0] == 0);
    CHECK(e->v[1] == 1);
    CHECK(std::get<0>(e->t) == 2);
    CHECK(std::get<1>(e->t) == 3);
  }

  {
    expected<int, int> e;
    CHECK(std::is_default_constructible_v<decltype(e)>);
    CHECK(std::is_copy_constructible_v<decltype(e)>);
    CHECK(std::is_move_constructible_v<decltype(e)>);
    CHECK(std::is_copy_assignable_v<decltype(e)>);
    CHECK(std::is_move_assignable_v<decltype(e)>);
    CHECK(std::is_trivially_copy_constructible_v<decltype(e)>);
    CHECK(std::is_trivially_copy_assignable_v<decltype(e)>);
    CHECK(std::is_trivially_move_constructible_v<decltype(e)>);
    CHECK(std::is_trivially_move_assignable_v<decltype(e)>);
  }

  {
    expected<int, std::string> e;
    CHECK(std::is_default_constructible_v<decltype(e)>);
    CHECK(std::is_copy_constructible_v<decltype(e)>);
    CHECK(std::is_move_constructible_v<decltype(e)>);
    CHECK(std::is_copy_assignable_v<decltype(e)>);
    CHECK(std::is_move_assignable_v<decltype(e)>);
    CHECK_FALSE(std::is_trivially_copy_constructible_v<decltype(e)>);
    CHECK_FALSE(std::is_trivially_copy_assignable_v<decltype(e)>);
    CHECK_FALSE(std::is_trivially_move_constructible_v<decltype(e)>);
    CHECK_FALSE(std::is_trivially_move_assignable_v<decltype(e)>);
  }

  {
    expected<std::string, int> e;
    CHECK(std::is_default_constructible_v<decltype(e)>);
    CHECK(std::is_copy_constructible_v<decltype(e)>);
    CHECK(std::is_move_constructible_v<decltype(e)>);
    CHECK(std::is_copy_assignable_v<decltype(e)>);
    CHECK(std::is_move_assignable_v<decltype(e)>);
    CHECK_FALSE(std::is_trivially_copy_constructible_v<decltype(e)>);
    CHECK_FALSE(std::is_trivially_copy_assignable_v<decltype(e)>);
    CHECK_FALSE(std::is_trivially_move_constructible_v<decltype(e)>);
    CHECK_FALSE(std::is_trivially_move_assignable_v<decltype(e)>);
  }

  {
    expected<std::string, std::string> e;
    CHECK(std::is_default_constructible_v<decltype(e)>);
    CHECK(std::is_copy_constructible_v<decltype(e)>);
    CHECK(std::is_move_constructible_v<decltype(e)>);
    CHECK(std::is_copy_assignable_v<decltype(e)>);
    CHECK(std::is_move_assignable_v<decltype(e)>);
    CHECK_FALSE(std::is_trivially_copy_constructible_v<decltype(e)>);
    CHECK_FALSE(std::is_trivially_copy_assignable_v<decltype(e)>);
    CHECK_FALSE(std::is_trivially_move_constructible_v<decltype(e)>);
    CHECK_FALSE(std::is_trivially_move_assignable_v<decltype(e)>);
  }

  {
    expected<void, int> e;
    CHECK(e);
  }

  {
    expected<void, int> e(unexpect, 42);
    CHECK_FALSE(e);
    CHECK(e.error() == 42);
  }
}
