// SPDX-License-Identifier: CC0-1.0
#include "catch.hpp"
#include <experimental/expected.hpp>
#include <memory>
#include <tuple>
#include <vector>

using std::experimental::expected;
using std::experimental::unexpected;

namespace {
struct takes_init_and_variadic {
  std::vector<int> v;
  std::tuple<int, int> t;
  template <class... Args>
  takes_init_and_variadic(std::initializer_list<int> l, Args &&... args)
      : v(l), t(std::forward<Args>(args)...) {}
};
} // namespace

TEST_CASE("Emplace", "[emplace]") {
  {
    expected<std::unique_ptr<int>, int> e;
    e.emplace(new int{42});
    CHECK(e);
    CHECK(**e == 42);
  }

  {
    expected<std::vector<int>, int> e;
    e.emplace({0, 1});
    CHECK(e);
    CHECK((*e)[0] == 0);
    CHECK((*e)[1] == 1);
  }

  {
    expected<std::tuple<int, int>, int> e;
    e.emplace(2, 3);
    CHECK(e);
    CHECK(std::get<0>(*e) == 2);
    CHECK(std::get<1>(*e) == 3);
  }

  {
    expected<takes_init_and_variadic, int> e = unexpected(0);
    e.emplace({0, 1}, 2, 3);
    CHECK(e);
    CHECK(e->v[0] == 0);
    CHECK(e->v[1] == 1);
    CHECK(std::get<0>(e->t) == 2);
    CHECK(std::get<1>(e->t) == 3);
  }
}
