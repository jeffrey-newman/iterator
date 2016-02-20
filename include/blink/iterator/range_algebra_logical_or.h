//
//=======================================================================
// Copyright 2016
// Author: Alex Hagen-Zanker
// University of Surrey
//
// Distributed under the MIT Licence (http://opensource.org/licenses/MIT)
//=======================================================================
//
// Implement the operator+ overloads for range_algebra_wrappers

#ifndef BLINK_ITERATOR_RANGE_ALGEBRA_LOGICAL_OR_H_AHZ
#define BLINK_ITERATOR_RANGE_ALGEBRA_LOGICAL_OR_H_AHZ

#include <blink/iterator/range_algebra_wrapper.h>
#include <blink/iterator/transform_range.h>

namespace blink
{
  namespace iterator {
    struct logical_or
    {
      template<class A, class B>
      auto operator()(A&& a, B&& b) const->decltype(std::forward<A>(a) || 
        std::forward<B>(b))
      {
        return a || b;
      }
    };

    template<class Constant>
    struct logical_or_constant_pre
    {
      logical_or_constant_pre(Constant a) : a(a)
      {}

      template<class B>
      auto operator()(B&& b) const -> decltype(a || std::forward<B>(b))
      {
        return a || b;
      }
      const Constant a;
    };

    template<class Constant>
    struct logical_or_constant
    {
      logical_or_constant(Constant b) : b(b)
      {}

      template<class A>
      auto operator()(A&& a) const -> decltype(std::forward<A>(a) || b)
      {
        return a || b;
      }
      const Constant b;
    };

    template<class R1, class R2>
    struct logical_or_helper
    {
      using r1 = typename std::remove_reference<R1>::type;
      using r2 = typename std::remove_reference<R2>::type;
      using v1 = typename r1::value_type;
      using v2 = typename r2::value_type;
      using result_type = decltype(std::declval<v1>() || std::declval<v2>());
      using range = transform_range <logical_or, R1, R2>;
    };

    template<class R, class T>
    struct logical_or_constant_helper
    {
      using r = typename std::remove_reference<R>::type;
      using v1 = typename r::value_type;
      using v2 = typename T;
      using result_type = decltype(std::declval<v1>() || std::declval<v2>());
      using range = transform_range<logical_or_constant<v2>, R>;
    };

    template<class R, class T>
    struct logical_or_constant_pre_helper
    {
      using r = typename std::remove_reference<R>::type;
      using v1 = typename T;
      using v2 = typename r::value_type;
      using result_type = decltype(std::declval<v1>() || std::declval<v2>());
      using range = transform_range<logical_or_constant_pre<v1>, R>;
    };

    template<class R, class T>
    range_algebra_wrapper<typename logical_or_constant_helper<
      range_algebra_wrapper<R>, T>::range>
      operator||(const range_algebra_wrapper<R>& r, const T& constant)
    {
      return range_algebra_val(logical_or_constant_helper<range_algebra_wrapper<R>, T>
        ::range(logical_or_constant<T>(constant), r));
    }

    template<class R, class T>
    range_algebra_wrapper<typename logical_or_constant_pre_helper<
      range_algebra_wrapper<R>, T>::range>
      operator||(const T& constant, const range_algebra_wrapper<R>& r)
    {
      return range_algebra_val(logical_or_constant_pre_helper<range_algebra_wrapper<R>, T>
        ::range(logical_or_constant_pre<T>(constant), r));
    }

    template<class R1, class R2>
    range_algebra_wrapper<typename logical_or_helper<range_algebra_wrapper<R1>, 
      range_algebra_wrapper<R2>>::range>
      operator||(const range_algebra_wrapper<R1>& r1, 
      const range_algebra_wrapper<R2>& r2)
    {
      return range_algebra_val(logical_or_helper<range_algebra_wrapper<R1>, 
        range_algebra_wrapper<R2>>::range(logical_or{}, r1, r2));
    }
  }
}
#endif