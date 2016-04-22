//
//=======================================================================
// Copyright 2015
// Author: Alex Hagen-Zanker
// University of Surrey
//
// Distributed under the MIT Licence (http://opensource.org/licenses/MIT)
//=======================================================================
//
// Implements a zip range, it is probably not fully correct in providing all
// required range and iterator traits, but since these are still in flux
// anyway I am not bothering about those.

#ifndef BLINK_ITERATOR_ZIP_RANGE_H_AHZ
#define BLINK_ITERATOR_ZIP_RANGE_H_AHZ

#include <blink\utility\index_sequence.h>
#include <blink\iterator\zip_iterator.h>
#include <blink\iterator\detail.h>

#include <type_traits> // remove_reference
#include <tuple>
#include <utility> // forward

namespace blink {
  namespace iterator {
    template<class...Ranges>
    class zip_range
    {
    private:
     
      using ranges_tuple = std::tuple <Ranges...>;
      using tuple_indices = blink::utility::make_index_sequence 
        < std::tuple_size<ranges_tuple>::value >;
      
      template<std::size_t I>
      using selected_range = tuple_element_t<I, ranges_tuple>;

    public:
      // use get_iterator instead of ::iterator to overcome Visual Studio bug
      using iterator = zip_iterator < 
        get_iterator_t< remove_reference_t<Ranges> >... > ;

      template < typename... InRanges, typename = enable_if_t<
        std::is_same
        < std::tuple<special_decay_t<InRanges>... >
        , std::tuple<Ranges... > >::value> >
      zip_range(InRanges&&... ranges) 
        : m_ranges(std::forward<InRanges>(ranges)...)
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
      template< template<std::size_t...> class Pack, std::size_t ...S> 
      iterator begin(Pack<S...>)
      {
        return make_zip_iterator(std::get<S>(m_ranges).begin()...);
      }

      template< template<std::size_t...> class Pack, std::size_t ...S>
      iterator end(Pack<S...>)
      {
        return make_zip_iterator(std::get<S>(m_ranges).end()...);
      }

      ranges_tuple m_ranges;
    };

    template<class...Ranges>
    zip_range<special_decay_t<Ranges> ...> make_zip_range(Ranges&&... rgs)
    {
      return zip_range<special_decay_t<Ranges>...>(std::forward<Ranges>(rgs)...);
    }
  }
}
#endif