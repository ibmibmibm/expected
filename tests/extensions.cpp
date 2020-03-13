// SPDX-License-Identifier: CC0-1.0
#include "catch.hpp"
#include <experimental/expected.hpp>

using std::experimental::expected;
using std::experimental::unexpect;

TEST_CASE("Map extensions", "[extensions.map]") {
  auto mul2 = [](int a) { return a * 2; };
  auto ret_void = [](int) {};

  {
    expected<int, int> e = 21;
    auto ret = e.map(mul2);
    CHECK(ret);
    CHECK(*ret == 42);
  }

  {
    const expected<int, int> e = 21;
    auto ret = e.map(mul2);
    CHECK(ret);
    CHECK(*ret == 42);
  }

  {
    expected<int, int> e = 21;
    auto ret = std::move(e).map(mul2);
    CHECK(ret);
    CHECK(*ret == 42);
  }

  {
    const expected<int, int> e = 21;
    auto ret = std::move(e).map(mul2);
    CHECK(ret);
    CHECK(*ret == 42);
  }

  {
    expected<int, int> e(unexpect, 21);
    auto ret = e.map(mul2);
    CHECK_FALSE(ret);
    CHECK(ret.error() == 21);
  }

  {
    const expected<int, int> e(unexpect, 21);
    auto ret = e.map(mul2);
    CHECK_FALSE(ret);
    CHECK(ret.error() == 21);
  }

  {
    expected<int, int> e(unexpect, 21);
    auto ret = std::move(e).map(mul2);
    CHECK_FALSE(ret);
    CHECK(ret.error() == 21);
  }

  {
    const expected<int, int> e(unexpect, 21);
    auto ret = std::move(e).map(mul2);
    CHECK_FALSE(ret);
    CHECK(ret.error() == 21);
  }

  {
    expected<int, int> e = 21;
    auto ret = e.map(ret_void);
    CHECK(ret);
    STATIC_REQUIRE((std::is_same_v<decltype(ret), expected<void, int>>));
  }

  {
    const expected<int, int> e = 21;
    auto ret = e.map(ret_void);
    CHECK(ret);
    STATIC_REQUIRE((std::is_same_v<decltype(ret), expected<void, int>>));
  }

  {
    expected<int, int> e = 21;
    auto ret = std::move(e).map(ret_void);
    CHECK(ret);
    STATIC_REQUIRE((std::is_same_v<decltype(ret), expected<void, int>>));
  }

  {
    const expected<int, int> e = 21;
    auto ret = std::move(e).map(ret_void);
    CHECK(ret);
    STATIC_REQUIRE((std::is_same_v<decltype(ret), expected<void, int>>));
  }

  {
    expected<int, int> e(unexpect, 21);
    auto ret = e.map(ret_void);
    CHECK_FALSE(ret);
    STATIC_REQUIRE((std::is_same_v<decltype(ret), expected<void, int>>));
  }

  {
    const expected<int, int> e(unexpect, 21);
    auto ret = e.map(ret_void);
    CHECK_FALSE(ret);
    STATIC_REQUIRE((std::is_same_v<decltype(ret), expected<void, int>>));
  }

  {
    expected<int, int> e(unexpect, 21);
    auto ret = std::move(e).map(ret_void);
    CHECK_FALSE(ret);
    STATIC_REQUIRE((std::is_same_v<decltype(ret), expected<void, int>>));
  }

  {
    const expected<int, int> e(unexpect, 21);
    auto ret = std::move(e).map(ret_void);
    CHECK_FALSE(ret);
    STATIC_REQUIRE((std::is_same_v<decltype(ret), expected<void, int>>));
  }

  // mapping functions which return references
  {
    expected<int, int> e(42);
    auto ret = e.map([](int &i) -> int & { return i; });
    CHECK(ret);
    CHECK(ret == 42);
  }
}

TEST_CASE("Map error extensions", "[extensions.map_error]") {
  auto mul2 = [](int a) { return a * 2; };
  auto ret_void = [](int) {};

  {
    expected<int, int> e = 21;
    auto ret = e.map_error(mul2);
    CHECK(ret);
    CHECK(*ret == 21);
  }

  {
    const expected<int, int> e = 21;
    auto ret = e.map_error(mul2);
    CHECK(ret);
    CHECK(*ret == 21);
  }

  {
    expected<int, int> e = 21;
    auto ret = std::move(e).map_error(mul2);
    CHECK(ret);
    CHECK(*ret == 21);
  }

  {
    const expected<int, int> e = 21;
    auto ret = std::move(e).map_error(mul2);
    CHECK(ret);
    CHECK(*ret == 21);
  }

  {
    expected<int, int> e(unexpect, 21);
    auto ret = e.map_error(mul2);
    CHECK_FALSE(ret);
    CHECK(ret.error() == 42);
  }

  {
    const expected<int, int> e(unexpect, 21);
    auto ret = e.map_error(mul2);
    CHECK_FALSE(ret);
    CHECK(ret.error() == 42);
  }

  {
    expected<int, int> e(unexpect, 21);
    auto ret = std::move(e).map_error(mul2);
    CHECK_FALSE(ret);
    CHECK(ret.error() == 42);
  }

  {
    const expected<int, int> e(unexpect, 21);
    auto ret = std::move(e).map_error(mul2);
    CHECK_FALSE(ret);
    CHECK(ret.error() == 42);
  }

  {
    expected<int, int> e = 21;
    auto ret = e.map_error(ret_void);
    CHECK(ret);
  }

  {
    const expected<int, int> e = 21;
    auto ret = e.map_error(ret_void);
    CHECK(ret);
  }

  {
    expected<int, int> e = 21;
    auto ret = std::move(e).map_error(ret_void);
    CHECK(ret);
  }

  {
    const expected<int, int> e = 21;
    auto ret = std::move(e).map_error(ret_void);
    CHECK(ret);
  }

  {
    expected<int, int> e(unexpect, 21);
    auto ret = e.map_error(ret_void);
    CHECK_FALSE(ret);
  }

  {
    const expected<int, int> e(unexpect, 21);
    auto ret = e.map_error(ret_void);
    CHECK_FALSE(ret);
  }

  {
    expected<int, int> e(unexpect, 21);
    auto ret = std::move(e).map_error(ret_void);
    CHECK_FALSE(ret);
  }

  {
    const expected<int, int> e(unexpect, 21);
    auto ret = std::move(e).map_error(ret_void);
    CHECK_FALSE(ret);
  }
}

TEST_CASE("And then extensions", "[extensions.and_then]") {
  auto succeed = [](int) { return expected<int, int>(21 * 2); };
  auto fail = [](int) { return expected<int, int>(unexpect, 17); };

  {
    expected<int, int> e = 21;
    auto ret = e.and_then(succeed);
    CHECK(ret);
    CHECK(*ret == 42);
  }

  {
    const expected<int, int> e = 21;
    auto ret = e.and_then(succeed);
    CHECK(ret);
    CHECK(*ret == 42);
  }

  {
    expected<int, int> e = 21;
    auto ret = std::move(e).and_then(succeed);
    CHECK(ret);
    CHECK(*ret == 42);
  }

  {
    const expected<int, int> e = 21;
    auto ret = std::move(e).and_then(succeed);
    CHECK(ret);
    CHECK(*ret == 42);
  }

  {
    expected<int, int> e = 21;
    auto ret = e.and_then(fail);
    CHECK_FALSE(ret);
    CHECK(ret.error() == 17);
  }

  {
    const expected<int, int> e = 21;
    auto ret = e.and_then(fail);
    CHECK_FALSE(ret);
    CHECK(ret.error() == 17);
  }

  {
    expected<int, int> e = 21;
    auto ret = std::move(e).and_then(fail);
    CHECK_FALSE(ret);
    CHECK(ret.error() == 17);
  }

  {
    const expected<int, int> e = 21;
    auto ret = std::move(e).and_then(fail);
    CHECK_FALSE(ret);
    CHECK(ret.error() == 17);
  }

  {
    expected<int, int> e(unexpect, 21);
    auto ret = e.and_then(succeed);
    CHECK_FALSE(ret);
    CHECK(ret.error() == 21);
  }

  {
    const expected<int, int> e(unexpect, 21);
    auto ret = e.and_then(succeed);
    CHECK_FALSE(ret);
    CHECK(ret.error() == 21);
  }

  {
    expected<int, int> e(unexpect, 21);
    auto ret = std::move(e).and_then(succeed);
    CHECK_FALSE(ret);
    CHECK(ret.error() == 21);
  }

  {
    const expected<int, int> e(unexpect, 21);
    auto ret = std::move(e).and_then(succeed);
    CHECK_FALSE(ret);
    CHECK(ret.error() == 21);
  }

  {
    expected<int, int> e(unexpect, 21);
    auto ret = e.and_then(fail);
    CHECK_FALSE(ret);
    CHECK(ret.error() == 21);
  }

  {
    const expected<int, int> e(unexpect, 21);
    auto ret = e.and_then(fail);
    CHECK_FALSE(ret);
    CHECK(ret.error() == 21);
  }

  {
    expected<int, int> e(unexpect, 21);
    auto ret = std::move(e).and_then(fail);
    CHECK_FALSE(ret);
    CHECK(ret.error() == 21);
  }

  {
    const expected<int, int> e(unexpect, 21);
    auto ret = std::move(e).and_then(fail);
    CHECK_FALSE(ret);
    CHECK(ret.error() == 21);
  }
}

TEST_CASE("or_else", "[extensions.or_else]") {
  using eptr = std::unique_ptr<int>;
  auto succeed = [](int) { return expected<int, int>(21 * 2); };
  auto succeedptr = [](eptr) { return expected<int, eptr>(21 * 2); };
  auto fail = [](int) { return expected<int, int>(unexpect, 17); };
  auto efail = [](eptr e) {
    *e = 17;
    return expected<int, eptr>(unexpect, std::move(e));
  };
  auto failvoid = [](int) {};
  auto failvoidptr = [](const eptr &) { /* don't consume */ };
  auto consumeptr = [](eptr) {};
  auto make_u_int = [](int n) { return std::make_unique<int>(n); };

  {
    expected<int, int> e = 21;
    auto ret = e.or_else(succeed);
    CHECK(ret);
    CHECK(*ret == 21);
  }

  {
    const expected<int, int> e = 21;
    auto ret = e.or_else(succeed);
    CHECK(ret);
    CHECK(*ret == 21);
  }

  {
    expected<int, int> e = 21;
    auto ret = std::move(e).or_else(succeed);
    CHECK(ret);
    CHECK(*ret == 21);
  }

  {
    expected<int, eptr> e = 21;
    auto ret = std::move(e).or_else(succeedptr);
    CHECK(ret);
    CHECK(*ret == 21);
  }

  {
    const expected<int, int> e = 21;
    auto ret = std::move(e).or_else(succeed);
    CHECK(ret);
    CHECK(*ret == 21);
  }

  {
    expected<int, int> e = 21;
    auto ret = e.or_else(fail);
    CHECK(ret);
    CHECK(*ret == 21);
  }

  {
    const expected<int, int> e = 21;
    auto ret = e.or_else(fail);
    CHECK(ret);
    CHECK(*ret == 21);
  }

  {
    expected<int, int> e = 21;
    auto ret = std::move(e).or_else(fail);
    CHECK(ret);
    CHECK(ret == 21);
  }

  {
    expected<int, eptr> e = 21;
    auto ret = std::move(e).or_else(efail);
    CHECK(ret);
    CHECK(ret == 21);
  }

  {
    const expected<int, int> e = 21;
    auto ret = std::move(e).or_else(fail);
    CHECK(ret);
    CHECK(*ret == 21);
  }

  {
    expected<int, int> e(unexpect, 21);
    auto ret = e.or_else(succeed);
    CHECK(ret);
    CHECK(*ret == 42);
  }

  {
    const expected<int, int> e(unexpect, 21);
    auto ret = e.or_else(succeed);
    CHECK(ret);
    CHECK(*ret == 42);
  }

  {
    expected<int, int> e(unexpect, 21);
    auto ret = std::move(e).or_else(succeed);
    CHECK(ret);
    CHECK(*ret == 42);
  }

  {
    expected<int, eptr> e(unexpect, make_u_int(21));
    auto ret = std::move(e).or_else(succeedptr);
    CHECK(ret);
    CHECK(*ret == 42);
  }

  {
    const expected<int, int> e(unexpect, 21);
    auto ret = std::move(e).or_else(succeed);
    CHECK(ret);
    CHECK(*ret == 42);
  }

  {
    expected<int, int> e(unexpect, 21);
    auto ret = e.or_else(fail);
    CHECK_FALSE(ret);
    CHECK(ret.error() == 17);
  }

  {
    expected<int, int> e(unexpect, 21);
    auto ret = e.or_else(failvoid);
    CHECK_FALSE(ret);
    CHECK(ret.error() == 21);
  }

  {
    const expected<int, int> e(unexpect, 21);
    auto ret = e.or_else(fail);
    CHECK_FALSE(ret);
    CHECK(ret.error() == 17);
  }

  {
    const expected<int, int> e(unexpect, 21);
    auto ret = e.or_else(failvoid);
    CHECK_FALSE(ret);
    CHECK(ret.error() == 21);
  }

  {
    expected<int, int> e(unexpect, 21);
    auto ret = std::move(e).or_else(fail);
    CHECK_FALSE(ret);
    CHECK(ret.error() == 17);
  }

  {
    expected<int, int> e(unexpect, 21);
    auto ret = std::move(e).or_else(failvoid);
    CHECK_FALSE(ret);
    CHECK(ret.error() == 21);
  }

  {
    expected<int, eptr> e(unexpect, make_u_int(21));
    auto ret = std::move(e).or_else(failvoidptr);
    CHECK_FALSE(ret);
    CHECK(*ret.error() == 21);
  }

  {
    expected<int, eptr> e(unexpect, make_u_int(21));
    auto ret = std::move(e).or_else(consumeptr);
    CHECK_FALSE(ret);
    CHECK(ret.error() == nullptr);
  }

  {
    const expected<int, int> e(unexpect, 21);
    auto ret = std::move(e).or_else(fail);
    CHECK_FALSE(ret);
    CHECK(ret.error() == 17);
  }

  {
    const expected<int, int> e(unexpect, 21);
    auto ret = std::move(e).or_else(failvoid);
    CHECK_FALSE(ret);
    CHECK(ret.error() == 21);
  }
}
struct S {
  int x;
};

struct F {
  int x;
};

TEST_CASE("14", "[issue.14]") {
  auto res = expected<S, F>{unexpect, F{}};

  res.map_error([](F) {
  });
}

TEST_CASE("32", "[issue.32]") {
  int i = 0;
  expected<void, int> a;
  a.map([&i] { i = 42; });
  CHECK(i == 42);

  auto x = a.map([] { return 42; });
  CHECK(*x == 42);
}
