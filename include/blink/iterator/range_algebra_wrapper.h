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
      range_algebra_wrapper(const range_algebra_wrapper& that) : m_range(that.m_range)
      {}

      range_algebra_wrapper(range_algebra_wrapper&& that) : m_range(std::move(that.m_range))
      {}

      range_algebra_wrapper& operator=(const range_algebra_wrapper& that)
      {
        m_range = that.m_range;
      }

      range_algebra_wrapper& operator=(range_algebra_wrapper&& that)
      {
        m_range = std::move(that.m_range);
      }

      range_algebra_wrapper(Range&& r) : m_range(std::forward<Range>(r))
      {}
      using range_type = typename std::remove_reference<Range>::type;
      using value_type = typename range_type::iterator::value_type;
      using reference = typename range_type::iterator::reference;
      using iterator = typename range_type::iterator;
     
      iterator begin() // Make this const correct
      {
        return m_range.begin();
      }

      iterator end()
      {
        return m_range.end();
      }
      Range& get()
      {
        return m_range;
      }

      Range m_range;
    };

    template<typename Range>
    range_algebra_wrapper<special_decay_t<Range> > range_algebra_val(Range&& r) // makes a copy
    {
      return range_algebra_wrapper<special_decay_t<Range> >(std::forward<Range>(r));
    }

    template<typename Range>
    range_algebra_wrapper<Range&> range_algebra(Range& r) // takes a ref
    {
      return range_algebra_val(std::ref(r));
    }

    template<class T>
    struct is_range_algebra_wrapped
    {
      const static bool value = false;
    };

    template<class T>
    struct is_range_algebra_wrapped<range_algebra_wrapper<T> >
    {
      const static bool value = true;
    };

  }
}

#endif