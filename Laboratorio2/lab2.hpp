#pragma once
#include <concepts>
#include <vector>
#include <string>
#include <functional>
#include <tuple>

template <typename t>
struct Eq;

template <typename t>
concept EqImplementation = requires (t x, t y)
  {
    { x == y } -> std::same_as<bool>;
    { x != y } -> std::same_as<bool>;
  };

template<>
struct Eq<int>
  { static bool eq(int x, int y) { return x == y; }
    static bool neq(int x, int y) { return x != y; }
  };

template <>
struct Eq<double>
  { static bool eq(double x, double y) { return x == y; }
    static bool neq(double x, double y) { return x != y; }
  };

template<>
struct Eq<std::string>
  { static bool eq(std::string x, std::string y) { return x == y; }
    static bool neq(std::string x, std::string y) { return x != y; }
  };

template<>
struct Eq<bool>
  { static bool eq(bool x, bool y) { return x == y; }
    static bool neq(bool x, bool y) { return x != y; }
  };


template <typename t>
struct Show;

template <typename t>
concept ShowImplementation = requires (t x)
  {
    { Show<t>::show(x) } -> std::same_as<std::string>;
  };

template<>
struct Show<int>
  { static std::string show(int x) { return std::to_string(x); } };

template <>
struct Show<double>
  { static std::string show(double x) { return std::to_string(x); } };

template<>
struct Show<std::string>
  { static std::string show(std::string x) { return x; } };

template<>
struct Show<bool>
  { static std::string show(bool x) { return x ? "true" : "false"; } };


template <typename t>
class List
  {
    private:
      std::vector<t> _inner;
      List<t> clone() const
      { List<t> xs;
        xs._inner = this->_inner;
        return xs;
      }
    public:
      List() {}
      List(List<t> const& other) : _inner(other._inner)  {}
      List<t> cons (t x) const
      { List<t> xs(*this);
        xs._inner.push_back(x);
        return xs;
      }
      List<t> tail() const
      { List<t> xs(*this);
        if (xs._inner.size() > 0)
          xs._inner.pop_back();
        return xs;
      }
      t head() const
      { return _inner.back(); }
      bool isEmpty() const
      { return _inner.empty(); }
  };

template<typename a, typename b>
  requires ShowImplementation<a> && ShowImplementation<b>
struct Show<std::tuple<a, b>>
  { static std::string show(std::tuple<a, b> t)
    { return "(" + Show<a>::show(std::get<0>(t)) + ", " + Show<b>::show(std::get<1>(t)) + ")"; }
  };


template <typename t>
  requires ShowImplementation<t>
struct Show<List<t>>
  { static std::string show(List<t> xs)
    { std::string s = "[";
      while(!xs.isEmpty())
        { s += Show<t>::show(xs.head());
          xs = xs.tail();
          if (!xs.isEmpty())
            s += ", ";
        }
      s += "]";
      return s;
    } };

template <typename t>
  requires EqImplementation<t>
struct Eq<List<t>>
  { static bool eq(List<t> xs, List<t> ys)
    { if (xs.isEmpty() && ys.isEmpty())
        return true;
      if (xs.isEmpty() || ys.isEmpty())
        return false;
      if (!Eq<t>::eq(xs.head(), ys.head()))
        return false;
      return eq(xs.tail(), ys.tail());
    } };


template <typename t>
  requires ShowImplementation<t>
std::string show(const t& x) {
    return Show<t>::show(x);
}

template <typename t>
  requires EqImplementation<t>
bool operator==(const List<t>& xs, const List<t>& ys) {
    return Eq<List<t>>::eq(xs, ys);
}

template <typename t>
  requires EqImplementation<t>
bool operator!=(const List<t>& xs, const List<t>& ys) {
    return !Eq<List<t>>::eq(xs, ys);
}

template <typename a, typename b, typename f_b_a_b>
  requires std::invocable<f_b_a_b, b, a>
    && std::convertible_to<f_b_a_b,std::function<b(b, a)>>
b reduceLeft(f_b_a_b f, b init, List<a> const& xs )
  {
    b acc = init;
    List<a> curr = xs;
    while (!curr.isEmpty()) {
      acc = f(acc, curr.head());
      curr = curr.tail();
    }
    return acc;
  }

template <typename a, typename b, typename f_b_a_b>
  requires std::invocable<f_b_a_b, b, a>
    && std::convertible_to<f_b_a_b,std::function<b(b, a)>>
b reduceLeftRec(f_b_a_b f, b init, List<a> const& xs )
  {
    if (xs.isEmpty()) return init;
    return reduceLeftRec(f, f(init, xs.head()), xs.tail());
  }

template <typename a, typename b, typename f_a_b>
  requires std::invocable<f_a_b, a>
    && std::convertible_to<f_a_b,std::function<b(a)>>
List<b> map(f_a_b f, List<a> const& xs )
  {
    List<a> rev;
    List<a> curr = xs;
    while (!curr.isEmpty()) {
      rev = rev.cons(curr.head());
      curr = curr.tail();
    }
    List<b> res;
    while (!rev.isEmpty()) {
      res = res.cons(f(rev.head()));
      rev = rev.tail();
    }
    return res;
  }

template <typename a, typename b, typename f_a_b>
  requires std::invocable<f_a_b, a>
    && std::convertible_to<f_a_b,std::function<b(a)>>
List<b> mapRec(f_a_b f, List<a> const& xs )
  {
    if (xs.isEmpty()) return List<b>();
    return mapRec<a, b>(f, xs.tail()).cons(f(xs.head()));
  }

template <typename a, typename b, typename f_a_b>
  requires std::invocable<f_a_b, a>
    && std::convertible_to<f_a_b,std::function<b(a)>>
List<b> mapReduce(f_a_b f, List<a> const& xs )
  {
    auto rev = reduceLeft<a, List<a>>(
      [](List<a> acc, a x) { return acc.cons(x); }, List<a>(), xs);
    return reduceLeft<a, List<b>>(
      [f](List<b> acc, a x) { return acc.cons(f(x)); }, List<b>(), rev);
  }

template <typename t>
List<t> concat(List<List<t>> const& xss)
  {
    auto revOuter = reduceLeft<List<t>, List<List<t>>>(
      [](List<List<t>> acc, List<t> l) { return acc.cons(l); }, List<List<t>>(), xss);
    return reduceLeft<List<t>, List<t>>(
      [](List<t> acc, List<t> l) {
        auto revInner = reduceLeft<t, List<t>>(
          [](List<t> innerAcc, t x) { return innerAcc.cons(x); }, List<t>(), l);
        return reduceLeft<t, List<t>>(
          [](List<t> innerAcc2, t x) { return innerAcc2.cons(x); }, acc, revInner);
      }, List<t>(), revOuter);
  }

template <typename a, typename f_a_b>
  requires std::invocable<f_a_b, a>
    && std::convertible_to<f_a_b,std::function<bool(a)>>
List<a> filter(f_a_b f, List<a> const& xs )
  {
    List<a> rev;
    List<a> curr = xs;
    while (!curr.isEmpty()) {
      rev = rev.cons(curr.head());
      curr = curr.tail();
    }
    List<a> res;
    while (!rev.isEmpty()) {
      if (f(rev.head())) res = res.cons(rev.head());
      rev = rev.tail();
    }
    return res;
  }

template <typename a, typename f_a_b>
  requires std::invocable<f_a_b, a>
    && std::convertible_to<f_a_b,std::function<bool(a)>>
List<a> filterRec(f_a_b f, List<a> const& xs )
  {
    if (xs.isEmpty()) return List<a>();
    if (f(xs.head())) return filterRec(f, xs.tail()).cons(xs.head());
    return filterRec(f, xs.tail());
  }

template <typename a, typename f_a_b>
  requires std::invocable<f_a_b, a>
    && std::convertible_to<f_a_b,std::function<bool(a)>>
List<a> filterMap(f_a_b f, List<a> const& xs )
  {
    List<a> rev;
    map<a, bool>([&rev](a x){ rev = rev.cons(x); return true; }, xs);
    List<a> res;
    map<a, bool>([&res, f](a x){
      if (f(x)) res = res.cons(x);
      return true;
    }, rev);
    return res;
  }

template <typename a, typename f_a_b>
  requires std::invocable<f_a_b, a>
    && std::convertible_to<f_a_b,std::function<bool(a)>>
List<a> filterReduce(f_a_b f, List<a> const& xs )
  {
    auto rev = reduceLeft<a, List<a>>(
      [](List<a> acc, a x) { return acc.cons(x); }, List<a>(), xs);
    return reduceLeft<a, List<a>>(
      [f](List<a> acc, a x) {
        if (f(x)) return acc.cons(x);
        return acc;
      }, List<a>(), rev);
  }

template <typename t>
  requires EqImplementation<t>
List<List<t>> groupEqual(List<t> const& xs)
  {
    auto revGroups = reduceLeft<t, List<List<t>>>(
      [](List<List<t>> acc, t x) {
        if (acc.isEmpty()) return acc.cons(List<t>().cons(x));
        List<t> lastGroup = acc.head();
        if (lastGroup.head() == x) {
          return acc.tail().cons(lastGroup.cons(x));
        } else {
          return acc.cons(List<t>().cons(x));
        }
      }, List<List<t>>(), xs);

    return reduceLeft<List<t>, List<List<t>>>(
      [](List<List<t>> acc, List<t> g) { return acc.cons(g); }, List<List<t>>(), revGroups);
  }

template <typename t>
  requires EqImplementation<t>
List<std::tuple<t, int>> encode(List<t> const& xs)
  {
    List<List<t>> grouped = groupEqual(xs);
    List<std::tuple<t, int>> rev;
    while (!grouped.isEmpty()) {
      List<t> g = grouped.head();
      t val = g.head();
      int count = 0;
      while (!g.isEmpty()) { count++; g = g.tail(); }
      rev = rev.cons(std::make_tuple(val, count));
      grouped = grouped.tail();
    }
    List<std::tuple<t, int>> res;
    while (!rev.isEmpty()) {
      res = res.cons(rev.head());
      rev = rev.tail();
    }
    return res;
  }

template <typename t>
  requires EqImplementation<t>
List<t> decode(List<std::tuple<t, int>> const& xss)
  {
    List<std::tuple<t, int>> rev;
    List<std::tuple<t, int>> curr = xss;
    while (!curr.isEmpty()) {
      rev = rev.cons(curr.head());
      curr = curr.tail();
    }
    List<t> res;
    while (!rev.isEmpty()) {
      auto pair = rev.head();
      for (int i = 0; i < std::get<1>(pair); i++) {
        res = res.cons(std::get<0>(pair));
      }
      rev = rev.tail();
    }
    return res;
  }

template <typename a, typename b>
  List<std::tuple<a,b>> zip(List<a> const& xs, List<b> const& ys)
  {
    List<a> revX; List<b> revY;
    List<a> cX = xs; List<b> cY = ys;
    while (!cX.isEmpty() && !cY.isEmpty()) {
      revX = revX.cons(cX.head());
      revY = revY.cons(cY.head());
      cX = cX.tail(); cY = cY.tail();
    }
    List<std::tuple<a, b>> res;
    while (!revX.isEmpty() && !revY.isEmpty()) {
      res = res.cons(std::make_tuple(revX.head(), revY.head()));
      revX = revX.tail(); revY = revY.tail();
    }
    return res;
  }

template <typename t>
  List<t> dropEveryN(int n, List<t> const& xs)
  {
    List<int> idxReversed;
    List<t> cX = xs;
    int count = 1;
    while (!cX.isEmpty()) {
      idxReversed = idxReversed.cons(count++);
      cX = cX.tail();
    }
    List<int> fwdIdx;
    while (!idxReversed.isEmpty()) {
      fwdIdx = fwdIdx.cons(idxReversed.head());
      idxReversed = idxReversed.tail();
    }
    auto zipped = zip(xs, fwdIdx);

    List<std::tuple<t, int>> revZipped;
    while (!zipped.isEmpty()) {
      revZipped = revZipped.cons(zipped.head());
      zipped = zipped.tail();
    }
    List<t> res;
    while (!revZipped.isEmpty()) {
      auto tup = revZipped.head();
      if (std::get<1>(tup) % n != 0) {
        res = res.cons(std::get<0>(tup));
      }
      revZipped = revZipped.tail();
    }
    return res;
  }


template <typename t, typename f_a_b>
  requires std::invocable<f_a_b, t, t>
    && std::convertible_to<f_a_b,std::function<bool(t, t)>>
  List<t> sortBy(List<t> const& xs, f_a_b f)
  {
    List<t> res;
    List<t> curr = xs;
    while (!curr.isEmpty()) {
      t val = curr.head();
      List<t> temp;
      while (!res.isEmpty() && f(res.head(), val)) {
        temp = temp.cons(res.head());
        res = res.tail();
      }
      res = res.cons(val);
      while (!temp.isEmpty()) {
        res = res.cons(temp.head());
        temp = temp.tail();
      }
      curr = curr.tail();
    }
    List<t> finalRes;
    while (!res.isEmpty()) {
      finalRes = finalRes.cons(res.head());
      res = res.tail();
    }
    return finalRes;
  }

template <typename t>
  List<t> rotateByN(List<t> const& xs, int n)
  {
    int size = 0;
    List<t> cX = xs;
    while (!cX.isEmpty()) { size++; cX = cX.tail(); }
    if (size == 0) return xs;

    n = n % size;
    if (n < 0) n += size;

    List<int> idx;
    for (int i = size - 1; i >= 0; i--) { idx = idx.cons(i); }

    auto zipped = zip(xs, idx);
    auto comp = [n, size](std::tuple<t, int> a, std::tuple<t, int> b) {
      int rotA = (std::get<1>(a) - n + size) % size;
      int rotB = (std::get<1>(b) - n + size) % size;
      return rotA <= rotB;
    };

    auto sorted = sortBy<std::tuple<t, int>>(zipped, comp);
    List<std::tuple<t, int>> revSorted;
    while (!sorted.isEmpty()) {
      revSorted = revSorted.cons(sorted.head());
      sorted = sorted.tail();
    }
    List<t> res;
    while (!revSorted.isEmpty()) {
      res = res.cons(std::get<0>(revSorted.head()));
      revSorted = revSorted.tail();
    }
    return res;
  }