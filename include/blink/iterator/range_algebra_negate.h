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

    template<class R>
    range_algebra_wrapper<
      transform_range<std::logical_not<>, range_algebra_wrapper<R>& > >
      operator-(range_algebra_wrapper<R>& r)
    {
        return range_algebra(make_transform_range(std::negate<>{},
          std::ref(r)));
    }

    template<class R>
    range_algebra_wrapper<
      transform_range<std::logical_not<>, range_algebra_wrapper<R> > >
      operator-(range_algebra_wrapper<R>&& r)
    {
        return range_algebra(make_transform_range(std::negate<>{},
          std::move(r)));
    }
  }
}
#endif