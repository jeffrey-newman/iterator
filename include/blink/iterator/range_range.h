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
    // TODO: this is not being used at teh moment, but should come in handy for 
    // distance weighted moving windows.

    template<typename RangeRange>
    struct range_range_iterator
    {

      using range = typename RangeRange::value_type;
      using value_type = typename range::value_type;
      using range_iterator = typename range::iterator;
      using range_reference = typename range_iterator::reference;

      using iterators = std::vector < range_iterator > ;
      struct reference_proxy
      {
        void operator=(const value_type& v) const
        {
          **m_iter = v;
        }

        void operator=(const reference_proxy& that) const
        {
          **m_iter = static_cast<value_type>(**that.m_iter);
        }


        // conversion to make the iterator readable
        operator value_type() const
        {
          return **m_iter;;
        }

        reference_proxy(const range_iterator* iter) :m_iter(iter)
        {}

        const range_iterator* m_iter;
      };

      using reference = std::vector < reference_proxy > ;


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


      range_reference get(int i)
      {
        return *(m_iterators[i]);

      }
      reference operator*()
      {
        reference result;
        for (int i = 0; i < m_iterators.size(); ++i)
        {
          result.push_back(reference_proxy(&m_iterators[i]));
        }
        return result;
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

      iterator begin()
      {
        iterator i;
        i.find_begin(m_range_range);
        return i;
      }

      iterator end()
      {
        iterator i;
        i.find_end(m_range_range);
        return i;
      }

      RangeRange& m_range_range;

    };

    template<typename RangeRange>
    range_zip_range<RangeRange> make_range_zip_range(RangeRange& rr)
    {
      return range_zip_range<RangeRange>(rr);
    }
  }
}

#endif