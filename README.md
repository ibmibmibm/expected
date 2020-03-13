# expected-17
Single header implementation of `std::expected` in c++17 with functional-style extensions.
Inspired from [tl::expected](https://github.com/TartanLlama/expected)

Clang + GCC: [![Linux Build Status](https://travis-ci.org/ibmibmibm/expected.svg?branch=master)](https://travis-ci.org/github/ibmibmibm/expected)

[`std::expected`](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p0323r9.html) is proposed as the preferred way to represent object which will either have an expected value, or an unexpected value giving information about why something failed. Unfortunately, chaining together many computations which may fail can be verbose, as error-checking code will be mixed in with the actual programming logic. This implementation provides a number of utilities to make coding with `expected` cleaner.

For example, instead of writing this code:

```cpp
std::expected<image,fail_reason> get_cute_cat (const image& img) {
    auto cropped = crop_to_cat(img);
    if (!cropped) {
      return cropped;
    }

    auto with_tie = add_bow_tie(*cropped);
    if (!with_tie) {
      return with_tie;
    }

    auto with_sparkles = make_eyes_sparkle(*with_tie);
    if (!with_sparkles) {
       return with_sparkles;
    }

    return add_rainbow(make_smaller(*with_sparkles));
}
```

You can do this:

```cpp
std::expected<image,fail_reason> get_cute_cat (const image& img) {
    return crop_to_cat(img)
           .and_then(add_bow_tie)
           .and_then(make_eyes_sparkle)
           .map(make_smaller)
           .map(add_rainbow);
}
```

The interface is the same as `std::expected` as proposed in [p0323r9](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p0323r9.html), but the following member functions are also defined. Explicit types are for clarity.

- `map`: carries out some operation on the stored object if there is one.
  * `std::expected<std::size_t,std::error_code> s = exp_string.map(&std::string::size);`
- `map_error`: carries out some operation on the unexpected object if there is one.
  * `my_error_code translate_error (std::error_code);`
  * `std::expected<int,my_error_code> s = exp_int.map_error(translate_error);`
- `and_then`: like `map`, but for operations which return a `tl::expected`.
  * `std::expected<ast, fail_reason> parse (const std::string& s);`
  * `std::expected<ast, fail_reason> exp_ast = exp_string.and_then(parse);`
- `or_else`: calls some function if there is no value stored.
  * `exp.or_else([] { throw std::runtime_error{"oh no"}; });`

### Compiler support

Tested on:

- Linux
  * g++ 9
  * g++ 8
  * g++ 7
  * clang++ 9
  * clang++ 8
  * clang++ 7

### Dependencies

Requires [Catch](https://github.com/philsquared/Catch) for testing. This is bundled in the test directory.

### Acknowledgements

If you're looking c++11 or c++14 version, please visit [tl::expected](https://github.com/TartanLlama/expected).

----------

[![CC0](http://i.creativecommons.org/p/zero/1.0/88x31.png)]("http://creativecommons.org/publicdomain/zero/1.0/")

To the extent possible under law, [Shen-Ta Hsieh](https://github.com/ibmibmibm) has waived all copyright and related or neighboring rights to the `expected` library.
