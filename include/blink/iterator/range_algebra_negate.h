//
//=======================================================================
// Copyright 2016
// Author: Alex Hagen-Zanker
// University of Surrey
//
// Distributed under the MIT Licence (http://opensource.org/licenses/MIT)
//=======================================================================
//
// Implement the unary operator- overload for range_algebra_wrappers

#ifndef BLINK_ITERATOR_RANGE_ALGEBRA_NEGATE_H_AHZ
#define BLINK_ITERATOR_RANGE_ALGEBRA_NEGATE_H_AHZ

#include <blink/iterator/range_algebra_wrapper.h>
#include <blink/iterator/transform_range.h>

namespace blink
{
  namespace iterator {
    struct negate
    {
      template<class T>
      auto operator()(T&& v) const->decltype(-std::forward<T>(v) )
      {
        return -v;
      }
    };


    template<class R>
    struct negate_helper
    {
      using r = typename std::remove_reference<R>::type;
      using v = typename r::value_type;
      using result_type = decltype(-std::declval<v>() );
      using range = transform_range <negate, r> ;
    };

    template<class R>
    range_algebra_wrapper<typename negate_helper<
      range_algebra_wrapper<R> >::range>
      operator-(const range_algebra_wrapper<R>& r)
    {
      return range_algebra_val(negate_helper<range_algebra_wrapper<R> >
        ::range(negate{}, r));
    }
  }
}
#endif