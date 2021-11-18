// SPDX-License-Identifier: CC0-1.0
#include "catch.hpp"
#include <experimental/expected.hpp>
#include <memory>
#include <string>

using std::experimental::expected;
using std::experimental::unexpect;
using std::experimental::unexpected;

expected<int, std::string> getInt3(int val) { return val; }

expected<int, std::string> getInt2(int val) { return val; }

expected<int, std::string> getInt1() { return getInt2(5).and_then(getInt3); }

TEST_CASE("Issue 1", "[issues.1]") { getInt1(); }

expected<int, int> operation1() { return 42; }

expected<std::string, int> operation2(int const) { return "Bananas"; }

TEST_CASE("Issue 17", "[issues.17]") {
  auto const intermediate_result = operation1();

  intermediate_result.and_then(operation2);
}

struct a {};
struct b : a {};

auto doit() -> expected<std::unique_ptr<b>, int> { return unexpected(0); }

TEST_CASE("Issue 23", "[issues.23]") {
  expected<std::unique_ptr<a>, int> msg = doit();
  CHECK_FALSE(msg.has_value());
}

TEST_CASE("Issue 26", "[issues.26]") {
  expected<a, int> exp = expected<b, int>(unexpect, 0);
  CHECK_FALSE(exp.has_value());
}

struct foo {
  foo() = default;
  foo(foo &) = delete;
  foo(foo &&){};
};

TEST_CASE("Issue 29", "[issues.29]") {
  std::vector<foo> v;
  v.emplace_back();
  expected<std::vector<foo>, int> ov = std::move(v);
  CHECK(ov->size() == 1);
}

expected<int, std::string> error() {
  return unexpected(std::string("error1 "));
}
std::string maperror(std::string s) { return s + "maperror "; }

TEST_CASE("Issue 30", "[issues.30]") { error().map_error(maperror); }

struct i31 {
  int i;
};
TEST_CASE("Issue 31", "[issues.31]") {
  const expected<i31, int> a = i31{42};
  CHECK(a->i == 42);

  expected<void, std::string> result;
  expected<void, std::string> result2 = result;
  result2 = result;
}

TEST_CASE("Issue 33", "[issues.33]") {
  expected<void, int> res{unexpect, 0};
  CHECK_FALSE(res);
  res = res.map_error([](int) { return 42; });
  CHECK(res.error() == 42);
}

expected<void, std::string> voidWork() { return {}; }
expected<int, std::string> work2() { return 42; }
void errorhandling(std::string) {}

TEST_CASE("Issue 34", "[issues.34]") {
  expected<int, std::string> result = voidWork().and_then(work2);
  result.map_error([&](std::string result) { errorhandling(result); });
}

struct non_copyable {
  non_copyable(non_copyable &&) = default;
  non_copyable(non_copyable const &) = delete;
  non_copyable() = default;
};

TEST_CASE("Issue 42", "[issues.42]") {
  expected<non_copyable, int>{}.map([](non_copyable) {});
}

TEST_CASE("Issue 43", "[issues.43]") {
  auto result = expected<void, std::string>{};
  result = unexpected(std::string{"foo"});
}

using MaybeDataPtr = expected<int, std::unique_ptr<int>>;

MaybeDataPtr test(int i) noexcept { return i; }

MaybeDataPtr test2(int i) noexcept { return i; }

TEST_CASE("Issue 49", "[issues.49]") { auto m = test(10).and_then(test2); }

expected<int, std::unique_ptr<std::string>> func() { return 1; }

TEST_CASE("Issue 61", "[issues.61]") { CHECK(func().value() == 1); }
