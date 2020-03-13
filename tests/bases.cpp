// SPDX-License-Identifier: CC0-1.0
#include "catch.hpp"
#include <experimental/expected.hpp>
#include <string>
#include <type_traits>

using std::experimental::expected;
using std::experimental::unexpected;

TEST_CASE("Triviality", "[bases.triviality]") {
  CHECK(std::is_trivially_copy_constructible_v<expected<int, int>>);
  CHECK(std::is_trivially_copy_assignable_v<expected<int, int>>);
  CHECK(std::is_trivially_move_constructible_v<expected<int, int>>);
  CHECK(std::is_trivially_move_assignable_v<expected<int, int>>);
  CHECK(std::is_trivially_destructible_v<expected<int, int>>);

  CHECK(std::is_trivially_copy_constructible_v<expected<void, int>>);
  CHECK(std::is_trivially_move_constructible_v<expected<void, int>>);
  CHECK(std::is_trivially_destructible_v<expected<void, int>>);

  {
    struct T {
      T(const T &) = default;
      T(T &&) = default;
      T &operator=(const T &) = default;
      T &operator=(T &&) = default;
      ~T() = default;
    };
    CHECK(std::is_trivially_copy_constructible_v<expected<T, int>>);
    CHECK(std::is_trivially_copy_assignable_v<expected<T, int>>);
    CHECK(std::is_trivially_move_constructible_v<expected<T, int>>);
    CHECK(std::is_trivially_move_assignable_v<expected<T, int>>);
    CHECK(std::is_trivially_destructible_v<expected<T, int>>);
  }

  {
    struct T {
      T(const T &) {}
      T(T &&){};
      T &operator=(const T &) { return *this; }
      T &operator=(T &&) { return *this; }
      ~T() {}
    };
    CHECK_FALSE(std::is_trivially_copy_constructible_v<expected<T, int>>);
    CHECK_FALSE(std::is_trivially_copy_assignable_v<expected<T, int>>);
    CHECK_FALSE(std::is_trivially_move_constructible_v<expected<T, int>>);
    CHECK_FALSE(std::is_trivially_move_assignable_v<expected<T, int>>);
    CHECK_FALSE(std::is_trivially_destructible_v<expected<T, int>>);
  }
}

TEST_CASE("Deletion", "[bases.deletion]") {
  CHECK(std::is_copy_constructible_v<expected<int, int>>);
  CHECK(std::is_copy_assignable_v<expected<int, int>>);
  CHECK(std::is_move_constructible_v<expected<int, int>>);
  CHECK(std::is_move_assignable_v<expected<int, int>>);
  CHECK(std::is_destructible_v<expected<int, int>>);

  {
    struct T {
      T() = default;
    };
    CHECK(std::is_default_constructible_v<expected<T, int>>);
  }

  {
    struct T {
      T(int) {}
    };
    CHECK_FALSE(std::is_default_constructible_v<expected<T, int>>);
  }

  {
    struct T {
      T(const T &) = default;
      T(T &&) = default;
      T &operator=(const T &) = default;
      T &operator=(T &&) = default;
      ~T() = default;
    };
    CHECK(std::is_copy_constructible_v<expected<T, int>>);
    CHECK(std::is_copy_assignable_v<expected<T, int>>);
    CHECK(std::is_move_constructible_v<expected<T, int>>);
    CHECK(std::is_move_assignable_v<expected<T, int>>);
    CHECK(std::is_destructible_v<expected<T, int>>);
  }

  {
    struct T {
      T(const T &) = delete;
      T(T &&) = delete;
      T &operator=(const T &) = delete;
      T &operator=(T &&) = delete;
    };
    CHECK_FALSE(std::is_copy_constructible_v<expected<T, int>>);
    CHECK_FALSE(std::is_copy_assignable_v<expected<T, int>>);
    CHECK_FALSE(std::is_move_constructible_v<expected<T, int>>);
    CHECK_FALSE(std::is_move_assignable_v<expected<T, int>>);
  }

  {
    struct T {
      T(const T &) = delete;
      T(T &&) = default;
      T &operator=(const T &) = delete;
      T &operator=(T &&) = default;
    };
    CHECK_FALSE(std::is_copy_constructible_v<expected<T, int>>);
    CHECK_FALSE(std::is_copy_assignable_v<expected<T, int>>);
    CHECK(std::is_move_constructible_v<expected<T, int>>);
    CHECK(std::is_move_assignable_v<expected<T, int>>);
  }

  {
    struct T {
      T(const T &) = default;
      T(T &&) = delete;
      T &operator=(const T &) = default;
      T &operator=(T &&) = delete;
    };
    CHECK(std::is_copy_constructible_v<expected<T, int>>);
    CHECK(std::is_copy_assignable_v<expected<T, int>>);
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
}
