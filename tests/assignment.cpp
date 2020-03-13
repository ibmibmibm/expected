// SPDX-License-Identifier: CC0-1.0
#include "catch.hpp"
#include <experimental/expected.hpp>

using std::experimental::expected;
using std::experimental::unexpected;

TEST_CASE("Simple assignment", "[assignment.simple]") {
  expected<int, int> e1 = 42;
  expected<int, int> e2 = 17;
  expected<int, int> e3 = 21;
  expected<int, int> e4 = unexpected(42);
  expected<int, int> e5 = unexpected(17);
  expected<int, int> e6 = unexpected(21);

  e1 = e2;
  CHECK(e1);
  CHECK(*e1 == 17);
  CHECK(e2);
  CHECK(*e2 == 17);

  e1 = std::move(e2);
  CHECK(e1);
  CHECK(*e1 == 17);
  CHECK(e2);
  CHECK(*e2 == 17);

  e1 = 42;
  CHECK(e1);
  CHECK(*e1 == 42);

  auto unex = unexpected(12);
  e1 = unex;
  CHECK_FALSE(e1);
  CHECK(e1.error() == 12);

  e1 = unexpected(42);
  CHECK_FALSE(e1);
  CHECK(e1.error() == 42);

  e1 = e3;
  CHECK(e1);
  CHECK(*e1 == 21);

  e4 = e5;
  CHECK(!e4);
  CHECK(e4.error() == 17);

  e4 = std::move(e6);
  CHECK_FALSE(e4);
  CHECK(e4.error() == 21);

  e4 = e1;
  CHECK(e4);
  CHECK(*e4 == 21);
}

TEST_CASE("Assignment deletion", "[assignment.deletion]") {
  struct has_all {
    has_all() noexcept = default;
    has_all(const has_all &) noexcept = default;
    has_all(has_all &&) noexcept = default;
    has_all &operator=(const has_all &) noexcept = default;
  };

  CHECK((std::is_assignable_v<expected<has_all, has_all> &,
                              expected<has_all, has_all>>));

  struct except_move {
    except_move() noexcept = default;
    except_move(const except_move &) noexcept = default;
    except_move(except_move &&) noexcept(false) {}
    except_move &operator=(const except_move &) noexcept = default;
  };
  CHECK_FALSE((std::is_assignable_v<expected<except_move, except_move> &,
                                    expected<except_move, except_move>>));
}

TEST_CASE("Assignment throwing recovery", "[assignment.throw]") {
  struct throw_move {
    int v;
    throw_move(int v) noexcept : v(v) {}
    throw_move(const throw_move &) noexcept = default;
    throw_move(throw_move &&) noexcept(false) { throw 0; }
    throw_move &operator=(const throw_move &) noexcept = default;
    throw_move &operator=(throw_move &&) noexcept(false) { throw 0; }
  };

  {
    expected<throw_move, int> e1 = 1;
    expected<throw_move, int> e2 = 2;
    expected<throw_move, int> e3 = unexpected(3);

    CHECK(e1);
    CHECK(e1->v == 1);
    CHECK(e2);
    CHECK(e2->v == 2);
    CHECK_FALSE(e3);
    CHECK(e3.error() == 3);

    CHECK_NOTHROW(e1 = e2);
    CHECK(e1);
    CHECK(e1->v == 2);
    CHECK(e2);
    CHECK(e2->v == 2);
    CHECK_NOTHROW(e3 = e2);
    CHECK(e3);
    CHECK(e3->v == 2);
    CHECK(e2);
    CHECK(e2->v == 2);
  }

  {
    expected<throw_move, int> e1 = 1;
    expected<throw_move, int> e2 = 2;
    expected<throw_move, int> e3 = unexpected(3);

    CHECK(e1);
    CHECK(e1->v == 1);
    CHECK_FALSE(e3);
    CHECK(e3.error() == 3);

    CHECK_THROWS(e1 = std::move(e2));
    CHECK(e1);
    CHECK(e1->v == 1);
    CHECK(e2);
    CHECK(e2->v == 2);
    CHECK_THROWS(e3 = std::move(e2));
    CHECK_FALSE(e3);
    CHECK(e3.error() == 3);
    CHECK(e2);
    CHECK(e2->v == 2);
  }
}
