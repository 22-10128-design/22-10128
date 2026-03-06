#pragma once
#include <concepts>
#include <vector>
#include <string>
#include <functional>
#include <tuple>
#include <iostream>
#include "lab2.hpp"


template <typename a, typename b>
  requires EqImplementation<a> && EqImplementation<b>
struct Eq<std::tuple<a,b>>
  { static bool eq(std::tuple<a,b> x, std::tuple<a,b> y)
    { return Eq<a>::eq(std::get<0>(x), std::get<0>(y)) && Eq<b>::eq(std::get<1>(x), std::get<1>(y)); }
    static bool neq(std::tuple<a,b> x, std::tuple<a,b> y)
    { return !eq(x,y); }
  };

namespace extras
{


template <typename a, typename b, typename f_a_b_b>
  requires std::invocable<f_a_b_b, a, b>
    && std::convertible_to<f_a_b_b,std::function<b(a, b)>>
b reduceRight(f_a_b_b f, b init, List<a> const& xs )
  {
    if (xs.isEmpty())
      return init;
    b res = reduceRight(f, init, xs.tail());
    return f(xs.head(), res);
  }

template<typename a>
List<a> reverse(List<a> const& xs)
  { return reduceLeft(
      [](List<a> acc, a x) { return acc.cons(x); },
      List<a>(),
      xs
    );
  }

template <typename t>
List<t> snoc(const List<t> &xs, t x)
  {
    List<t> reversed = reverse(xs);
    List<t> result = reversed.cons(x);
    return reverse(result);
  }

template <typename t>
int length(List<t> const& xs)
  {
    return reduceLeft([](int acc, t x) { return acc + 1; }, 0, xs);
  }


template <typename t>
  List<t> from_vector(const std::vector<t> &v)
  {
    List<t> l;
    for (int i = v.size() - 1; i >= 0; --i)
      l = l.cons(v[i]);
    return l;
  }

}
