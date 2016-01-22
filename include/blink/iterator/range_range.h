//
//=======================================================================
// Copyright 2015
// Author: Alex Hagen-Zanker
// University of Surrey
//
// Distributed under the MIT Licence (http://opensource.org/licenses/MIT)
//=======================================================================
//
// To iterate over a range of ranges. This is a kind of runtime zip range. 
//
// Still under development.

#ifndef BLINK_ITERATOR_RANGE_RANGE_H_AHZ
#define BLINK_ITERATOR_RANGE_RANGE_H_AHZ

#include <vector>
#include <type_traits> // remove_reference
#include <tuple>
#include <utility> // forward

namespace blink {
  namespace iterator {

    //Iterate over a range of ranges

    template<typename RangeRange>
    struct range_range_iterator
    {
      using range = typename RangeRange::value_type;
      using range_iterator = typename range::iterator;
      using range_reference = typename range_iterator::reference;
      using iterators = std::vector < range_iterator > ;

      struct reference_level_2
      {
        struct iterator
        {
          iterator(const typename iterators::iterator& iter) : m_iter(iter)
          using reference = range_reference;
          reference operator*() const
          {
            **m_iter;
          }

          iterator& operator++()
          {
            ++m_iter;
            return *this;
          }
          bool operator==(const iterator& that)
          {
            return m_iter == that.m_iter;
          }

          bool operator!=(const iterator& that)
          {
            return m_iter != that.m_iter;
          }
          typename iterators::iterator m_iter;
        };

        iterator begin() const
        {
          return m_begin;
        }
        iterator end() const
        {
          return m_end;
        }

        typename range_reference operator[](int i)
        {
          return **(m_begin + i);
        }

        typename iterators::const_iterator m_begin;
        typename iterators::const_iterator m_end;
      };
      using value_type = reference_level_2;
      using reference = reference_level_2;

      void find_begin(RangeRange& ranges) 
      {
        m_iterators.clear();
        for (auto& range : ranges)
        {
          m_iterators.push_back(range.begin());
        }
      }

      void find_end(RangeRange& ranges)
      {
        m_iterators.clear();
        for (auto& range : ranges)
        {
          m_iterators.push_back(range.end());
        }
      }

      reference_level_2 operator*() const
      {
        reference_level_2 r;
        r.m_begin = m_iterators.begin();
        r.m_end = m_iterators.end();
        return r;
      }

      range_range_iterator& operator++()
      {
        for (int i = 0; i < m_iterators.size(); ++i)
        {
          ++(m_iterators[i]);
        }
        return *this;
      }

      bool operator==(const range_range_iterator& that)
      {
        return m_iterators.size() == 0 || m_iterators[0] == that.m_iterators[0];
      }

      bool operator!=(const range_range_iterator& that)
      {
        return m_iterators.size() != 0 && m_iterators[0] != that.m_iterators[0];
      }

      iterators m_iterators;
    };

    template<typename RangeRange>
    struct range_zip_range
    {
      range_zip_range(RangeRange& range_range) : m_range_range(range_range)
      {}

      using iterator = range_range_iterator < RangeRange > ;

      iterator begin() const
      {
        iterator i;
        i.find_begin(m_range_range);
        return i;
      }

      iterator end() const
      {
        iterator i;
        i.find_end(m_range_range);
        return i;
      }
      RangeRange& m_range_range;
    };

    template<typename RangeRange>
    range_zip_range<RangeRange> make_range_range(RangeRange& rr)
    {
      return range_zip_range<RangeRange>(rr);
    }
  }
}

#endif