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
      using tuple_indices = blink::utility::make_index_sequence < std::tuple_size<ranges_tuple>::value >;
      template<std::size_t I>
      using selected_range = get_type < std::tuple_element<I, ranges_tuple> >;

    public:

      // use get_iterator instead of ::iterator to overcome Visual Studio bug
      using iterator = zip_iterator < get_type < get_iterator<get_type<std::remove_reference<Ranges> > > >... > ;

      template<class... InRanges>
      explicit zip_range(InRanges&&... ranges) : m_ranges(std::forward<InRanges>(ranges)...)
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
        return iterator((std::get<S>(m_ranges).begin())...);
      }

      template<std::size_t ...S> iterator end(blink::utility::index_sequence<S...>)
      {
        return iterator((std::get<S>(m_ranges).end())...);
      }

      ranges_tuple m_ranges;
    };

    template<class...Ranges>
    zip_range<unwrap<get_type<std::remove_reference<Ranges> > >...> make_zip_range(Ranges&&... rgs)
    {
      return zip_range<unwrap<get_type<std::remove_reference<Ranges> > >...>(std::forward<Ranges>(rgs)...);
    }
  }
}
#endif