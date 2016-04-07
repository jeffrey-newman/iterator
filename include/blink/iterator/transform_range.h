//
//=======================================================================
// Copyright 2015
// Author: Alex Hagen-Zanker
// University of Surrey
//
// Distributed under the MIT Licence (http://opensource.org/licenses/MIT)
//=======================================================================
//
// Implements a transform range, it is zip range that lazily applies a function 
// on the iterated over elements.

#ifndef BLINK_ITERATOR_TRANSFORM_RANGE_H_AHZ
#define BLINK_ITERATOR_TRANSFORM_RANGE_H_AHZ

#include <blink\utility\index_sequence.h>
#include <blink\iterator\transform_iterator.h>
#include <blink\iterator\detail.h>

#include <type_traits> // remove_reference
#include <tuple>
#include <utility> // forward

namespace blink {
  namespace iterator {
    template<class Function, class...Ranges>
    class transform_range
    {
    private:
     
      using ranges_tuple = std::tuple <Ranges...>;
      using tuple_indices = blink::utility::make_index_sequence < std::tuple_size<ranges_tuple>::value >;
      template<std::size_t I>
      using selected_range = tuple_element_t<I, ranges_tuple>;

    public:
      //using value_type = Value;
      using value_type = typename transform_helper<Function, Ranges...>::value_type;
      // use get_iterator_t instead of ::iterator to overcome Visual Studio bug
      using iterator = transform_iterator < 
        Function,
        value_type,
        get_iterator_t<remove_reference_t<Ranges> >... > ;

      // Perhaps this should be deleted?
      transform_range(const transform_range& that)
        : m_ranges(that.m_ranges)
        , m_f(that.m_f)
      {
      }
      
      transform_range(transform_range&& that)
        : m_ranges(std::move(that.m_ranges))
        , m_f(std::move(that.m_f))

      {

      }
      template<class... InRanges>
      explicit transform_range(Function f, InRanges&&... ranges) 
        : 
        m_f(f),
        m_ranges(std::forward<InRanges>(ranges)...)
      { }

      iterator begin()
      {
        return begin(tuple_indices());
      }

      iterator end()
      {
        return end(tuple_indices());
      }

      template<std::size_t I> selected_range<I>& get()
      {
        return std::get<I>(m_ranges);
      }

      template<std::size_t I> const selected_range<I>& get() const
      {
        return std::get<I>(m_ranges);
      }

    private:
      template<std::size_t ...S> iterator begin(blink::utility::index_sequence<S...>)
      {
        return iterator(m_f, (std::get<S>(m_ranges).begin())...);
      }

      template<std::size_t ...S> iterator end(blink::utility::index_sequence<S...>)
      {
        return iterator(m_f, (std::get<S>(m_ranges).end())...);
      }

      ranges_tuple m_ranges;
      Function m_f;
    };

    template<class Function, class...Ranges>
    transform_range<Function, special_decay_t<Ranges>...>
      make_transform_range (Function f, Ranges&&... rgs)
    {
      using return_type = transform_range < Function, special_decay_t<Ranges>... >;
      return return_type(f, std::forward<Ranges>(rgs)...);
    }
  }
}
#endif