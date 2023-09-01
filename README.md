# named-tuple
Single header implementation a named-tuple in C++

Tuples have always been addressible via index and type -- the obvious gap here is accessing by name. This (small) header-only library provides just that!

### Usage

```cpp
auto make_config() {
    using Configuration = named_tuple<
      named_val<std::string, "install-path">,
      named_val<std::string, "build-path">,
      named_val<int, "retry-limit">,
      named_val<bool, "system-wide">
    >;

    Configuration default_config{"/usr/local/", "./build", 1, true};
    return default_config;
}

void use_config(auto&& config) {
    // named_get supports multiple values and will construct an intermediate named_tuple
    auto build_options = named_get<"build-path", "retry-limit">;
    auto install_options = named_get<"install-path", "system-wide">;
}
```

### Components

- `named_tuple`: The tuple class - `sizeof(named_tuple) == sizeof(tuple)`
- `named_val`: descriptor used to indicate the type and name
- `named_get<"name">`: accessor for retrieving a value stored in the `named_tuple`
- `named_get<names...>`: accessor for splicing a `named_tuple``. You can even permute!
- `named_tie<names...`: std::tie() equivalent for `named_tuple``

### Limitations

* Deduction guides are not really feasible due to their limitation of needing all template arguments specified.
* I have not extensively tested the reference semantics and behavior. I welcome contributions to this library.

### Compiler support

Requires a small subset of C++20 features.

Tested on:

- Linux
  * g++ 13.0
  * clang 16.0
- macOS
  * g++ 13.0
  * clang 16.0
  * AppleClang 14.0

----------

[![CC0](http://i.creativecommons.org/p/zero/1.0/88x31.png)]("http://creativecommons.org/publicdomain/zero/1.0/")

To the extent possible under law, [Will Killian](https://github.com/willkill07) has waived all copyright and related or neighboring rights to the `expected` library. This work is published from: the United States of America.
