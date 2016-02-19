//
//=======================================================================
// Copyright 206
// Author: Alex Hagen-Zanker
// University of Surrey
//
// Distributed under the MIT Licence (http://opensource.org/licenses/MIT)
//=======================================================================
//
// range_algebra_wrapper is a light wrapper around ranges that signals operators to use 
// operators in lazy element-by-element fashion.

#ifndef BLINK_ITERATOR_RANGE_ALGEBRA_WRAPPER_H_AHZ
#define BLINK_ITERATOR_RANGE_ALGEBRA_WRAPPER_H_AHZ

#include <blink/iterator/transform_range.h>

namespace blink
{
  namespace iterator {

    template<class Range> // Either a Range or a reference to a Range
    class range_algebra_wrapper
    {
    public:
      range_algebra_wrapper(Range& r) : m_range(r)
      {}
      using range_type = typename std::remove_reference<Range>::type;
      using value_type = typename range_type::iterator::value_type;
      using iterator = typename range_type::iterator;

      iterator begin() // Make this const correct
      {
        return m_range.begin();
      }

      iterator end()
      {
        return m_range.end();
      }
      Range m_range;
    };

    template<typename Range>
    range_algebra_wrapper<Range> range_algebra_val(Range& r) // makes a copy
    {
      return range_algebra_wrapper<Range>(r);
    }

    template<typename Range>
    range_algebra_wrapper<Range&> range_algebra(Range& r) // takes a ref
    {
      return range_algebra_wrapper<Range&>(r);
    }
  }
}

#endif