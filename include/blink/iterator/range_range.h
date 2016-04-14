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

#include <blink/iterator/detail.h>

#include <vector>
#include <type_traits> // remove_reference
#include <tuple>
#include <utility> // forward

namespace blink {
  namespace iterator {

    //Zip-iterate over a range of ranges

    template<typename RangeRange>
    struct range_range_iterator
    {
      using range_range_type = remove_reference_t<RangeRange>;
      using range = typename range_range_type::value_type;
      using range_iterator = typename range::iterator;
      using range_reference = typename range_iterator::reference;
      using range_value = typename range_iterator::value_type;
      using iterators = std::vector < range_iterator >;

      range_range_iterator()
      {}

      range_range_iterator(const range_range_iterator& that) : m_iterators(that.m_iterators)
      {}

      range_range_iterator(range_range_iterator&& that) : m_iterators(std::move(that.m_iterators))
      {}

      struct reference_level_2
      {
        
        struct iterator : public  
          boost::iterator_facade <iterator, range_value,
          boost::random_access_traversal_tag,
          range_reference>
        {
          iterator(const typename iterators::const_iterator& iter) : m_iter(iter)
          {}

        private:
          friend boost::iterator_core_access;
         
          range_reference dereference() const
          {
            return **m_iter;
          }

          void increment()
          {
            ++m_iter;
          }

          void decrement()
          {
            --m_iter;
          }

          void advance(std::ptrdiff_t n)
          {
            m_iter += n;
          }

          template<class OtherIterator >
          std::ptrdiff_t distance_to(const OtherIterator& that) const
          {
            return std::distance(m_iter, that.m_iter);
          }

          template<class OtherIterator >
          bool equal(const OtherIterator& that) const
          {
            return m_iter == that.m_iter;
          }

          typename iterators::const_iterator m_iter;
        };
        
        iterator begin() const
        {
          return iterator(m_begin);
        }
        iterator end() const
        {
          return iterator(m_end);
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
        for (auto&& range : ranges)
        {
           m_iterators.emplace_back(range.begin());
        }
      }

      void find_end(RangeRange& ranges)
      {
        m_iterators.clear();
        for (auto&& range : ranges)
        {
          m_iterators.emplace_back(range.end());
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
      template<typename InRangeRange>
      range_zip_range(InRangeRange&& range_range) : m_range_range(
        std::forward<InRangeRange>(range_range) )
      {}

      using iterator = range_range_iterator < remove_reference_t<RangeRange> > ;

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
      RangeRange m_range_range;
    };

    template<typename RangeRange>
    range_zip_range<special_decay_t<RangeRange> > make_range_zip_range(
      RangeRange&& rr)
    {
      return range_zip_range<special_decay_t<RangeRange> >(
        std::forward<RangeRange>(rr));
    }
  }
}

#endif