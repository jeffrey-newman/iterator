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

#ifndef BLINK_ITERATOR_RANGE_ALGEBRA_PLUS_H_AHZ
#define BLINK_ITERATOR_RANGE_ALGEBRA_PLUS_H_AHZ

#include <blink/iterator/range_algebra_wrapper.h>
#include <blink/iterator/range_algebra_transform.h>

namespace blink
{
  namespace iterator {

    template<class R, class T>
    range_algebra_wrapper<
      range_algebra_transform<std::plus<>
      , special_decay_t<range_algebra_wrapper<R>>
      , special_decay_t<T> > >
      operator+(range_algebra_wrapper<R>&& r, T&& constant)
    {
      return range_algebra_function(std::plus < > {}, std::forward < range_algebra_wrapper<R> >(r),
        std::forward<T>(constant));
    }

    template<class R, class T>
    range_algebra_wrapper<
      range_algebra_transform<std::plus<>, special_decay_t<T>, special_decay_t<range_algebra_wrapper<R>> > >
      operator+(T&& constant, range_algebra_wrapper<R>&& r)
    {
      return range_algebra_function(std::plus < > {},
        std::forward<T>(constant), std::forward < range_algebra_wrapper<R> >(r));
    }

    template<class R1, class R2>
    range_algebra_wrapper <
      range_algebra_transform<std::plus<>
      , special_decay_t<range_algebra_wrapper<R1>>
      , special_decay_t<range_algebra_wrapper<R2>> > >
      operator+(range_algebra_wrapper<R1>&& r1, range_algebra_wrapper<R2>&& r2)
    {
      return range_algebra_function(std::plus < > {}, std::forward < range_algebra_wrapper<R1> >(r1),
        std::forward < range_algebra_wrapper<R2> >(r2) );
    }
  }
}
#endif