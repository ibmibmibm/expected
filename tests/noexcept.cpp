// SPDX-License-Identifier: CC0-1.0
#include "catch.hpp"
#include <experimental/expected.hpp>

using std::experimental::expected;
using std::experimental::unexpect;
using std::experimental::unexpect_t;
using std::experimental::unexpected;

TEST_CASE("no throw", "[noexcept]") {
  using T = expected<int, int>;
  CHECK(std::is_nothrow_default_constructible_v<T>);
  CHECK(std::is_nothrow_constructible_v<T, const int &>);
  CHECK(std::is_nothrow_constructible_v<T, int &&>);
  CHECK(std::is_nothrow_constructible_v<T, std::in_place_t, const int &>);
  CHECK(std::is_nothrow_constructible_v<T, std::in_place_t, int &&>);
  CHECK(std::is_nothrow_constructible_v<T, unexpect_t, const int &>);
  CHECK(std::is_nothrow_constructible_v<T, unexpect_t, int &&>);
  CHECK(std::is_nothrow_assignable_v<T &, const int &>);
  CHECK(std::is_nothrow_assignable_v<T &, int &&>);
  CHECK(std::is_nothrow_assignable_v<T &, const unexpected<int> &>);
  CHECK(std::is_nothrow_assignable_v<T &, unexpected<int> &&>);
  CHECK(std::is_nothrow_destructible_v<T>);
  CHECK(std::is_nothrow_swappable_v<T>);
}

TEST_CASE("throw all", "[noexcept]") {
  struct throw_all {
    throw_all() noexcept(false) { throw 0; }
    throw_all(const throw_all &) noexcept(false) { throw 0; }
    throw_all(throw_all &&) noexcept(false) { throw 0; }
    ~throw_all() noexcept(false) { throw 0; }
    throw_all &operator=(const throw_all &) noexcept(false) { throw 0; }
    throw_all &operator=(throw_all &&) noexcept(false) { throw 0; }
  };
  using T = expected<throw_all, int>;
  CHECK_FALSE(std::is_nothrow_default_constructible_v<T>);
  CHECK_FALSE(std::is_nothrow_constructible_v<T, const throw_all &>);
  CHECK_FALSE(std::is_nothrow_constructible_v<T, throw_all &&>);
  CHECK(
      !std::is_nothrow_constructible_v<T, std::in_place_t, const throw_all &>);
  CHECK_FALSE(std::is_nothrow_constructible_v<T, std::in_place_t, throw_all &&>);
  {
    const int &y = 10;
    T x(unexpect, y);
  }

  // because destructor throw
  CHECK_FALSE(std::is_nothrow_constructible_v<T, unexpect_t, const int &>);
  CHECK_FALSE(std::is_nothrow_constructible_v<T, unexpect_t, int &&>);

  CHECK_FALSE(std::is_nothrow_assignable_v<T &, const throw_all &>);
  CHECK_FALSE(std::is_nothrow_assignable_v<T &, throw_all &&>);
  CHECK_FALSE(std::is_nothrow_assignable_v<T &, const unexpected<int> &>);
  CHECK_FALSE(std::is_nothrow_assignable_v<T &, unexpected<int> &&>);
  CHECK_FALSE(std::is_nothrow_destructible_v<T>);
  CHECK_FALSE(std::is_nothrow_swappable_v<T>);
}

TEST_CASE("throw copy", "[noexcept]") {
  struct throw_copy {
    throw_copy() noexcept {}
    throw_copy(const throw_copy &) noexcept(false) { throw 0; }
    throw_copy(throw_copy &&) noexcept {}
    ~throw_copy() noexcept {}
    throw_copy &operator=(const throw_copy &) noexcept(false) { throw 0; }
    throw_copy &operator=(throw_copy &&) noexcept { return *this; }
  };
  using T = expected<throw_copy, int>;
  CHECK(std::is_nothrow_default_constructible_v<T>);
  CHECK_FALSE(std::is_nothrow_constructible_v<T, const throw_copy &>);
  CHECK(std::is_nothrow_constructible_v<T, throw_copy &&>);
  CHECK(
      !std::is_nothrow_constructible_v<T, std::in_place_t, const throw_copy &>);
  CHECK(std::is_nothrow_constructible_v<T, std::in_place_t, throw_copy &&>);
  CHECK(std::is_nothrow_constructible_v<T, unexpect_t, const int &>);
  CHECK(std::is_nothrow_constructible_v<T, unexpect_t, int &&>);
  CHECK_FALSE(std::is_nothrow_assignable_v<T &, const throw_copy &>);
  CHECK(std::is_nothrow_assignable_v<T &, throw_copy &&>);
  CHECK(std::is_nothrow_assignable_v<T &, const unexpected<int> &>);
  CHECK(std::is_nothrow_assignable_v<T &, unexpected<int> &&>);
  CHECK(std::is_nothrow_destructible_v<T>);
  CHECK(std::is_nothrow_swappable_v<T>);
}

TEST_CASE("throw move", "[noexcept]") {
  struct throw_move {
    throw_move() noexcept {}
    throw_move(const throw_move &) noexcept {}
    throw_move(throw_move &&) noexcept(false) { throw 0; }
    ~throw_move() noexcept {}
    throw_move &operator=(const throw_move &) noexcept { return *this; }
    throw_move &operator=(throw_move &&) noexcept(false) { throw 0; }
  };
  using T = expected<throw_move, int>;
  CHECK(std::is_nothrow_default_constructible_v<T>);
  CHECK(std::is_nothrow_constructible_v<T, const throw_move &>);
  CHECK_FALSE(std::is_nothrow_constructible_v<T, throw_move &&>);
  CHECK(
      std::is_nothrow_constructible_v<T, std::in_place_t, const throw_move &>);
  CHECK_FALSE(std::is_nothrow_constructible_v<T, std::in_place_t, throw_move &&>);
  CHECK(std::is_nothrow_constructible_v<T, unexpect_t, const int &>);
  CHECK(std::is_nothrow_constructible_v<T, unexpect_t, int &&>);
  CHECK(std::is_nothrow_assignable_v<T &, const throw_move &>);
  CHECK_FALSE(std::is_nothrow_assignable_v<T &, throw_move &&>);
  CHECK(std::is_nothrow_assignable_v<T &, const unexpected<int> &>);
  CHECK(std::is_nothrow_assignable_v<T &, unexpected<int> &&>);
  CHECK(std::is_nothrow_destructible_v<T>);
  CHECK_FALSE(std::is_nothrow_swappable_v<T>);
}

TEST_CASE("throw copy and move", "[noexcept]") {
  struct throw_copy_move {
    throw_copy_move() noexcept {}
    throw_copy_move(const throw_copy_move &) noexcept(false) { throw 0; }
    throw_copy_move(throw_copy_move &&) noexcept(false) { throw 0; }
    ~throw_copy_move() noexcept {}
    throw_copy_move &operator=(const throw_copy_move &) noexcept(false) {
      throw 0;
    }
    throw_copy_move &operator=(throw_copy_move &&) noexcept(false) { throw 0; }
  };
  using T = expected<throw_copy_move, int>;
  CHECK(std::is_nothrow_default_constructible_v<T>);
  CHECK_FALSE(std::is_nothrow_constructible_v<T, const throw_copy_move &>);
  CHECK_FALSE(std::is_nothrow_constructible_v<T, throw_copy_move &&>);
  CHECK_FALSE(std::is_nothrow_constructible_v<T, std::in_place_t,
                                           const throw_copy_move &>);
  CHECK(
      !std::is_nothrow_constructible_v<T, std::in_place_t, throw_copy_move &&>);
  CHECK(std::is_nothrow_constructible_v<T, unexpect_t, const int &>);
  CHECK(std::is_nothrow_constructible_v<T, unexpect_t, int &&>);
  CHECK_FALSE(std::is_nothrow_assignable_v<T &, const throw_copy_move &>);
  CHECK_FALSE(std::is_nothrow_assignable_v<T &, throw_copy_move &&>);
  CHECK(std::is_nothrow_assignable_v<T &, const unexpected<int> &>);
  CHECK(std::is_nothrow_assignable_v<T &, unexpected<int> &&>);
  CHECK(std::is_nothrow_destructible_v<T>);
  CHECK_FALSE(std::is_nothrow_swappable_v<T>);
}
