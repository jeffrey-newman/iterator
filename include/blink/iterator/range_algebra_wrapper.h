//
//=======================================================================
// Copyright 2016
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

      template < typename InRange, typename = enable_if_t<
        std::is_same<special_decay_t<InRange>, Range>::value> >
        range_algebra_wrapper(InRange&& r) : m_range(std::forward<InRange>(r))
      {}

      template<class SomeConstant>
      range_algebra_wrapper& operator=(const SomeConstant& c)
      {
        auto j = begin();
        auto j_end = end();
        for (; j != j_end; ++j)
        {
          *j = c;
        }
        return *this;
      }

      template<class SomeRange>
      range_algebra_wrapper& operator=(const range_algebra_wrapper<SomeRange>& r)
      {
        range_algebra_wrapper<SomeRange>& rnc = const_cast<range_algebra_wrapper<SomeRange>&>(r);
        auto i = rnc.begin();
        auto j = begin();
        auto j_end = end();
        for (; j != j_end; ++i, ++j)
        {
          *j = *i;
        }
        return *this;
      }
     
       range_algebra_wrapper& operator++()
      {
        auto j = begin();
        auto j_end = end();
        for (; j != j_end; ++j)
        {
          ++(*j);
        }
        return *this;
      }

       range_algebra_wrapper& operator--()
      {
        auto i = r.begin();
        auto j = begin();
        auto j_end = end();
        for (; j != j_end; ++i, ++j)
        {
          --(*j);
        }
        return *this;
      }
      template<class T>
      range_algebra_wrapper& operator+=(T&& r)
      {
        *this = (*this + std::forward<T>(r));
        return *this;
      }

      template<class T>
      range_algebra_wrapper& operator-=(T&& r)
      {
        *this = (*this - std::forward<T>(r));
        return *this;
      }

      template<class T>
      range_algebra_wrapper& operator*=(T&& r)
      {
        *this = (*this * std::forward<T>(r));
        return *this;
      }

      template<class T>
      range_algebra_wrapper& operator/=(T&& r)
      {
        *this = (*this / std::forward<T>(r));
        return *this;
      }

     // template<class SomeRange> range_algebra_wrapper& operator&=(SomeRange& r)
     // template<class SomeRange> range_algebra_wrapper& operator|=(SomeRange& r)
     // range_algebra_wrapper& operator<<=(SomeRange& r)
     // range_algebra_wrapper& operator>>=(SomeRange& r)
      
      using range_type = typename std::remove_reference<Range>::type;
      using value_type = typename range_type::iterator::value_type;
      using reference = typename range_type::iterator::reference;
      using iterator = typename range_type::iterator;
     
      iterator begin() 
      {
        return m_range.begin();
      }

      iterator end() 
      {
        return m_range.end();
      }
      const Range& get() const
      {
        return m_range;
      }
      Range& get() 
      {
        return m_range;
      }
      Range m_range;
    };

    template<typename Range>
    range_algebra_wrapper<special_decay_t<Range> > range_algebra(Range&& r) 
    {
      return range_algebra_wrapper<special_decay_t<Range> >(std::forward<Range>(r));
    }

    template<typename Range>
    range_algebra_wrapper<Range&> range_algebra_ref(Range& r) // takes a ref
    {
      return range_algebra(std::ref(r));
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