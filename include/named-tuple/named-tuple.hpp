#ifndef NAMED_TUPLE_HPP_
#define NAMED_TUPLE_HPP_

#include <algorithm>
#include <array>
#include <iterator>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>

namespace detail {

/// fixed_string can be used in a NTTP (non-type template parameter) context
/// It provides a constructor from a string literal so implicit construction
/// can occur within a template e.g. val<"Hello">
///
/// \brief a NTTP-friendly string holder
template<size_t N>
struct fixed_string : public std::array<char, N - 1>
{
  using parent = std::array<char, N - 1>;

  constexpr
  fixed_string(const char (&str)[N]) noexcept
  {
    std::copy_n(std::begin(str), N - 1, this->begin());
  }

  template<size_t M>
  [[nodiscard]] constexpr bool
  operator==(fixed_string<M> const&) noexcept
    requires(M != N)
  {
    return false;
  }

  template<size_t M>
  [[nodiscard]] constexpr bool
  operator!=(fixed_string<M> const&) noexcept
    requires(M != N)
  {
    return true;
  }

  explicit operator std::string_view () const noexcept {
    return std::string_view{this->data(), this->size()};
  }
};

template<typename NamedTuple>
constexpr static bool keys_unique = [] () constexpr {
  using T = std::remove_reference_t<NamedTuple>;
  constexpr size_t const N{std::tuple_size_v<typename T::type>};
  return []<size_t... Is, size_t... Js>(std::index_sequence<Is...>,
                                        std::index_sequence<Js...>) {
    return (... && []<size_t I>(std::integral_constant<size_t, I>) {
      return (... && (Js <= I || get<Js>(T::names) != get<I>(T::names)));
    }(std::integral_constant<size_t, Is>{}));
  }(std::make_index_sequence<N>{}, std::make_index_sequence<N>{});
}();

template<typename NamedTuple, fixed_string Key>
constexpr static bool keys_contain = [] {
  using T = std::remove_reference_t<NamedTuple>;
  return []<size_t... Is>(std::index_sequence<Is...>) {
    return (... || (Key == get<Is>(T::names)));
  }(std::make_index_sequence<std::tuple_size_v<typename T::type>>{});
}();

template<typename NamedTuple, fixed_string Key>
constexpr static size_t named_index_of = [] {
  using T = std::remove_reference_t<NamedTuple>;
  return []<size_t... Is>(std::index_sequence<Is...>) {
    // yield the index when the key matches (else zero)
    return (... + ((Key == get<Is>(T::names)) ? Is : 0lu));
  }(std::make_index_sequence<std::tuple_size_v<typename T::type>>{});
}();

} // namespace detail

template<typename T, detail::fixed_string Name>
struct named_val
{
  using type = T;
  static constexpr auto name = Name;
};

template<typename... Vs>
struct named_tuple : public std::tuple<typename Vs::type...>
{
  constexpr static const std::tuple names{Vs::name...};
  using type = std::tuple<typename Vs::type...>;
  static_assert(detail::keys_unique<named_tuple>, "keys must be unique");
  using type::type;
};

template<detail::fixed_string... Names, typename... Vals>
auto
named_tie(Vals&&... vals)
{
  return named_tuple<named_val<Vals&, Names>...>{
    std::forward<std::add_lvalue_reference_t<Vals>>(vals)...};
}

template<detail::fixed_string Key, typename NamedTuple>
auto&&
named_get(NamedTuple&& tup)
{
  static_assert(detail::keys_contain<NamedTuple, Key>, "key not found");
  return get<detail::named_index_of<NamedTuple, Key>>(tup);
}

template<detail::fixed_string Key, typename NamedTuple>
auto const&&
named_get(NamedTuple const&& tup)
{
  static_assert(detail::keys_contain<NamedTuple, Key>, "key not found");
  return get<detail::named_index_of<NamedTuple, Key>>(tup);
}

template<detail::fixed_string Key, typename NamedTuple>
auto&
named_get(NamedTuple& tup)
{
  static_assert(detail::keys_contain<NamedTuple, Key>, "key not found");
  return get<detail::named_index_of<NamedTuple, Key>>(tup);
}

template<detail::fixed_string Key, typename NamedTuple>
auto const&
named_get(NamedTuple const& tup)
{
  static_assert(detail::keys_contain<NamedTuple, Key>, "key not found");
  return get<detail::named_index_of<NamedTuple, Key>>(tup);
}

template<detail::fixed_string... Ks, typename NamedTuple>
auto
named_get(NamedTuple&& tup)
  requires(sizeof...(Ks) > 1)
{
  static_assert((detail::keys_contain<NamedTuple, Ks> && ...),
                "key(s) not found");
  return named_tie<Ks...>(named_get<Ks>(std::forward<NamedTuple>(tup))...);
}

template<detail::fixed_string... Ks, typename NamedTuple>
auto
named_get(NamedTuple const&& tup)
  requires(sizeof...(Ks) > 1)
{
  static_assert((detail::keys_contain<NamedTuple, Ks> && ...),
                "key(s) not found");
  return named_tie<Ks...>(named_get<Ks>(std::forward<NamedTuple>(tup))...);
}

template<detail::fixed_string... Ks, typename NamedTuple>
auto
named_get(NamedTuple& tup)
  requires(sizeof...(Ks) > 1)
{
  static_assert((detail::keys_contain<NamedTuple, Ks> && ...),
                "key(s) not found");
  return named_tie<Ks...>(named_get<Ks>(std::forward<NamedTuple>(tup))...);
}

template<detail::fixed_string... Ks, typename NamedTuple>
auto
named_get(NamedTuple const& tup)
  requires(sizeof...(Ks) > 1)
{
  static_assert((detail::keys_contain<NamedTuple, Ks> && ...),
                "key(s) not found");
  return named_tie<Ks...>(named_get<Ks>(std::forward<NamedTuple>(tup))...);
}

#endif
