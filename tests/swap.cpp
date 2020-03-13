// SPDX-License-Identifier: CC0-1.0
#include "catch.hpp"
#include <experimental/expected.hpp>

using std::experimental::expected;
using std::experimental::unexpect;
using std::experimental::unexpected;

struct no_throw {
  no_throw(std::string i) : i(i) {}
  std::string i;
};
struct canthrow_move {
  canthrow_move(std::string i) : i(i) {}
  canthrow_move(canthrow_move const &) = default;
  canthrow_move(canthrow_move &&other) noexcept(false) : i(other.i) {}
  canthrow_move &operator=(canthrow_move &&) = default;
  std::string i;
};

template <bool should_throw = false> struct willthrow_move {
  willthrow_move(std::string i) : i(i) {}
  willthrow_move(willthrow_move const &) = default;
  willthrow_move(willthrow_move &&other) : i(other.i) {
    if (should_throw)
      throw 0;
  }
  willthrow_move &operator=(willthrow_move &&) = default;
  std::string i;
};
static_assert(std::is_swappable_v<no_throw>, "");

template <class T1, class T2> void swap_test() {
  std::string s1 = "abcdefghijklmnopqrstuvwxyz";
  std::string s2 = "zyxwvutsrqponmlkjihgfedcba";

  using std::swap;
  {
    expected<T1, T2> a{s1};
    expected<T1, T2> b{s2};
    swap(a, b);
    CHECK(a->i == s2);
    CHECK(b->i == s1);
  }

  {
    expected<T1, T2> a{s1};
    expected<T1, T2> b{unexpected<T2>(s2)};
    swap(a, b);
    CHECK(a.error().i == s2);
    CHECK(b->i == s1);
  }

  {
    expected<T1, T2> a{unexpected<T2>(s1)};
    expected<T1, T2> b{s2};
    swap(a, b);
    CHECK(a->i == s2);
    CHECK(b.error().i == s1);
  }

  {
    expected<T1, T2> a{unexpected<T2>(s1)};
    expected<T1, T2> b{unexpected<T2>(s2)};
    swap(a, b);
    CHECK(a.error().i == s2);
    CHECK(b.error().i == s1);
  }
}

TEST_CASE("swap") {
  swap_test<no_throw, no_throw>();
  swap_test<no_throw, canthrow_move>();
  swap_test<canthrow_move, no_throw>();
  // swap_test<canthrow_move, canthrow_move>();

  std::string s1 = "abcdefghijklmnopqrstuvwxyz";
  std::string s2 = "zyxwvutsrqponmlkjihgfedcbaxxx";
  expected<no_throw, willthrow_move<true>> a{s1};
  expected<no_throw, willthrow_move<true>> b{unexpect, s2};

  CHECK_THROWS(swap(a, b));

  CHECK(a->i == s1);
  CHECK(b.error().i == s2);
}

TEST_CASE("swap compile test") {
  CHECK(std::is_swappable_v<expected<no_throw, no_throw>>);
  CHECK(std::is_swappable_v<expected<no_throw, canthrow_move>>);
  CHECK(std::is_swappable_v<expected<canthrow_move, no_throw>>);
  CHECK_FALSE(std::is_swappable_v<expected<canthrow_move, canthrow_move>>);
}
